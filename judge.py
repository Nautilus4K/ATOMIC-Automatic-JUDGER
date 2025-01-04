# Dockerfile templates structure:
# ATOMIC-Automatic-JUDGER/
# ├──templates/
# |   ├──Dockerfile.python
# |   ├──Dockerfile.compile
# ├──judge.py

import time
import datetime
import docker
import os
import json
import pathlib
import keyboard

# Enable ANSI escape codes
os.system("")

def pinfo(content: str):
    print(f"\x1b[0m[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [INFO] {content}\x1b[0m")

def perr(content: str):
    print(f"\x1b[31m[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [ERROR] {content}\x1b[0m")

def pwarn(content: str):
    print(f"\x1b[33m[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [WARNING] {content}\x1b[0m")

def pok(content: str):
    print(f"\x1b[32m[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [SUCCESS] {content}\x1b[0m")

# Variables
filePath = os.path.dirname(os.path.abspath(__file__))
try:
    with open(filePath+"/source/version.json", "r", encoding="utf-8") as file:
        data = json.loads(file.read())
        version = data["version"]
        branch = data["branch"]
        signature = data["signature"]
        band = data["band"]
        stable = data["stable"]
except:
    perr("No `version.json` found in directory. Please reinstall the software")

versionDisplay = f"ATOMIC Automatic Testing Software\nVersion: {version}\nBranch: {branch}\n{signature}@{band}"

# Actual codes
print(versionDisplay)
print("------------------------")

if not stable:
    pwarn("This build/version of ATOMIC is not stable. Use with caution")

pinfo("Current running directory: " + filePath)

# Connect to docker environment
try:
    client = docker.from_env()
except Exception as e:
    perr(f"Failed to connect to Docker environment: {str(e)}")
    perr("Your Docker environment might be faulty, or you might not have Docker Engine installed and turned on.")
    exit()

# Check if the connection is okay
try:
    client.ping()
    pinfo("Connected to Docker environment.")
except Exception as e:
    perr("Docker environment inactive. Try running ATOMIC again or verify your Docker installation.")
    exit()



# Prepare judging environment
pinfo("Now preparing judging environment (Docker images/containers). Make sure you have Internet access.")
def check_image_exists(image_name):
    client = docker.from_env()  # Connect to the Docker daemon
    try:
        # Try to fetch the image from local storage
        client.images.get(image_name)
        return True
    except docker.errors.NotFound:
        return False
    
def check_container_exists(container_name):
    client = docker.from_env()  # Connect to the Docker daemon
    try:
        # Try to fetch the container by name
        client.containers.get(container_name)
        return True
    except docker.errors.NotFound:
        return False
    
def run_container(image_name, mounted_dir: str):
    # Get the current working directory on the host
    current_directory = filePath + mounted_dir
    
    # Mount the host's current directory into /mnt inside the container
    container = client.containers.run(
        image_name,
        command="/bin/sh -c 'cd /mnt && /bin/sh'",  # Start the shell
        volumes={current_directory: {'bind': '/mnt', 'mode': 'rw'}},  # Mount the current directory to /mnt
        detach=True,  # Run in the background
        working_dir='/mnt'  # Set the working directory inside the container to /mnt (host's current directory)
    )
    
    print(f"Container {container.id} is running with host's current directory mounted.")
    return container

if not check_image_exists("atomic-python"):
    pwarn("No Environment image found, building...")
    pyimage, build_logs = client.images.build(path=filePath+"/templates", dockerfile="Dockerfile.python", tag="atomic-python", rm=True)
    for log in build_logs:
        pinfo(log.get('stream', '').strip())
    pok(f"Image built with ID: {pyimage.id}")
pinfo("Generating new container for Environment")
runcontainer = run_container("atomic-python", "/tempWorking")

if not check_image_exists("atomic-compile"):
    pwarn("No compiler image found, building one...")
    pinfo("Building Compile Environment Docker image...")
    try:
        compileimage, build_logs = client.images.build(path=filePath+"/templates", dockerfile="Dockerfile.compile", tag="atomic-compile")
        for log in build_logs:
            pinfo(log.get('stream', '').strip())
            pok(f"Image built with ID: {compileimage.id}")
    except Exception as e:
        perr("Error in building Compile Environment Docker image: " + str(e))
        runcontainer.remove()
        exit()

if not check_container_exists("compiler"):
    pwarn("No compiler container found, creating one...")
    container = client.containers.create("atomic-compile", name="compiler")
    pok(f"Compiler {container.name} build with ID {container.id}")



pinfo("Starting compile container")
try:
    compilecontainer = client.containers.get("compiler")
    compilecontainer.reload()
    if compilecontainer.status != "running":
        pwarn("Container isn't running.")
        compilecontainer.start()
        compilecontainer.reload()
        pok(f"Successfully started compile container. Current status: {compilecontainer.status}")
    else: pinfo("Did not start container because it is already started.")
    # compilecontainer.exec_run("")
except Exception as e:
    perr(f"Failed to start compile container: {e}")
    runcontainer.remove()
    exit()

pinfo("Preparing to enable judging...")


def compile_code(filecontents: str, lang: str):
    try:
        # Debug: Print original content
        # pinfo("[Compile] Original file content:\n" + filecontents)
        pinfo("[Compile] Creating temporary folder in host.")
        # Host directory to save the compiled file
        host_temp_working_dir = filePath + "/tempWorking"
        os.makedirs(host_temp_working_dir, exist_ok=True)  # Ensure the directory exists

        # Get the compile container
        compilecontainer = client.containers.get("compiler")
        compilecontainer.start()
        pinfo("[Compile] Creating temporary folder in Docker container.")

        # Step 1: Write the source code to a file inside the container
        container_temp_working_dir = "/tmp/tempWorking"
        compilecontainer.exec_run(f"/bin/bash -c mkdir -p {container_temp_working_dir}")  # Ensure directory exists

        pinfo("[Compile] Checking if language is supported based on extension.")
        # Determine the source filename based on language
        source_filename = "source_code"
        if lang == "cpp":
            source_filename += ".cpp"
        elif lang == "pascal":
            source_filename += ".pas"
        else:
            perr(f"[Compile] Unsupported language. Supported languages: C++, Pascal, Python (Direct Execution). Target language: {lang}")
            return [-1, "Unsupported language"]

        # Create the file using heredoc
        pinfo("[Compile] Writing file content...")
        # Remove any trailing newlines from filecontents to prevent extra blank lines
        filecontents = filecontents.rstrip('\n')
        cat_command = f"""cat << 'EOF' > {container_temp_working_dir}/{source_filename}
        {filecontents}
        EOF"""  # EOF must be at start of line with no trailing spaces

        exec_result = compilecontainer.exec_run(['sh', '-c', cat_command])
        if exec_result.exit_code != 0:
            perr(f"[Compile] Failed to create source file:\n{exec_result.output.decode()}")
            return [-5, "Failed to create source compilation file"]
        pok(f"[Compile] Created source file inside container. Logs:\n{exec_result.output.decode()}")

        # Step 3: Verify the file exists in the container
        file_contents2_result = compilecontainer.exec_run(f"cat {container_temp_working_dir}/{source_filename}")
        if file_contents2_result.exit_code != 0:
            perr(f"[Compile] Failed to read source file:\n{file_contents2_result.output.decode()}")
            return [-6, "Failed to read source compilation file"]
        pok("[Compile] Source file exists inside container.")

        # Decode and print the content
        file_contents2 = file_contents2_result.output.decode('utf-8')
        pinfo(f"Copied over file content:\n{file_contents2}")

        # Verify that the source file exists in the container
        exec_result = compilecontainer.exec_run(f"ls -l {container_temp_working_dir}/{source_filename}")
        if exec_result.exit_code != 0:
            perr(f"[Compile] Failed to list file details:\n{exec_result.output.decode()}")
            return [-2, "Failed to list file details"]

        # Step 2: Compile the code
        if lang == "cpp" or lang == "c++" or lang == "cplusplus":
            compile_command = f"g++ {container_temp_working_dir}/{source_filename} -o {container_temp_working_dir}/a.out"
        elif lang == "pas" or lang == "pascal":
            compile_command = f"fpc {container_temp_working_dir}/{source_filename} -o{container_temp_working_dir}/a.out"

        exec_result = compilecontainer.exec_run(compile_command)
        if exec_result.exit_code != 0:
            perr(f"[Compile] Compilation failed:\n{exec_result.output.decode()}")
            return [-3, exec_result.output.decode()]

        # Step 3: Copy the compiled binary back to the host
        binary_stream, _ = compilecontainer.get_archive(f"{container_temp_working_dir}/a.out")
        host_binary_path = os.path.join(host_temp_working_dir, "a.out")

        with open(host_binary_path, "wb") as f:
            for chunk in binary_stream:
                f.write(chunk)

        pinfo(f"[Compile] Compilation successful. Binary saved to host at '{host_binary_path}'.")
        return [0, ""]

    except Exception as e:
        perr(f"Error during compilation: {str(e)}")
        return [-4, str(e)]


# Judging function. Used for checking if a submit had done okay.
def judge(filename: str):
    try:
        with open(filePath+"/source/contests.json", "r", encoding="utf-8") as file:
            contests = json.loads(file.read())
            normalized_contests = {item.lower() for item in contests}
    except:
        perr("No `contests.json` found in source. Please generate one or reinstall the software.")
        return [-4, None]

    # We test if the file's name is one of the things we need to judge, else just remove the file for storage's sake and return its name into result?
    if pathlib.Path(filename).stem.lower() in normalized_contests:
        pinfo(f"Judging {filename}...")
        root, ext = os.path.splitext(filename)
        ext = ext[1:]
        if ext == "py" or ext == "pyw" or ext == "python":
            pinfo("> Python file detected. Entering testing step")
            pass
        else:
            pinfo("> Not a Python file. Entering compilation step")
            pinfo("> Fetching filedata")
            try:
                with open(filePath+"/workspace/queue/"+filename) as file:
                    filecontents = file.read()
            except:
                perr(f"Failed to read file. Please check the file's integrity. Error: {str(e)}")
                return [-2, None]
            
            comres, exception = compile_code(filecontents, ext) # Compile results, too
            if comres == 0:
                pinfo("> Compilation successful. Entering testing step")
            else:
                perr(f"> Compilation failed with code {comres}. Please check the code's integrity.")
                perr(f"> Exception:\n {exception}")
                return [-3, exception]

    else:
        pinfo(f"Found submit not in line with any current contests, removing {filename}.")
        os.remove(filePath+"/workspace/queue/"+filename)
    return -1



pinfo("Now testing/judging current queued submissions.")
running = True
while running:
    try:
        with open(filePath+"/source/settings.json", "r", encoding="utf-8") as file:
            wait_time = json.loads(file.read())["wait_time"]
    except Exception as e:
        perr(f"Failed to read settings.json: {str(e)}")
    try:
        dirs = os.listdir(filePath + "/workspace/queue/")
        if len(dirs) > 0:
            # There is files, actual files
            for file in dirs:
                judge(file)
        # Reduce drive I/O stress
        if keyboard.is_pressed("a") and keyboard.is_pressed("x") and keyboard.is_pressed("z"):
            # Keybind: A + X + Z
            running = False
            break
        
        time.sleep(wait_time)
    except KeyboardInterrupt:
        pwarn("Process terminated with Keyboard. Shutting down ATOM/C...")
        running = False
        break

# Remove the container for running tasks
runcontainer.remove()
