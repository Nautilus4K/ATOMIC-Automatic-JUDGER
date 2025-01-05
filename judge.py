# Dockerfile templates structure:
# ATOMIC-Automatic-JUDGER/
# ├──templates/
# |   ├──Dockerfile.python
# |   ├──Dockerfile.compile
# ├──judge.py

import time
import docker
import os
import json
import pathlib
import keyboard

# For logging, like pwarn(), pinfo(), pok(), perr
from csmcoloredloggerprint import *

# Enable ANSI escape codes
os.system("")

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
        volumes={current_directory: {'bind': '/mnt', 'mode': 'rw'}},  # Mount the current directory to /mnt
        detach=True,  # Run in the background
        working_dir='/mnt'  # Set the working directory inside the container to /mnt (host's current directory)
    )
    
    container.start()
    container.reload()
    pinfo(f"Container {container.id} is running with host's current directory mounted. Current status: {container.status}")
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

        # Create the file using heredoc - no indentation in the multiline string
        pinfo("[Compile] Writing file content...")
        cat_command = f"""cat << 'EOF' > {container_temp_working_dir}/{source_filename}
        {filecontents.strip()}
        EOF""" # EOF right at start, no extra spaces

        # OR even cleaner:
        cat_command = 'cat << \'EOF\' > ' + f'{container_temp_working_dir}/{source_filename}\n{filecontents.strip()}\nEOF'

        exec_result = compilecontainer.exec_run(['sh', '-c', cat_command])
        if exec_result.exit_code != 0:
            perr(f"[Compile] Failed to create source file:\n{exec_result.output.decode()}")
            return [-5, "Failed to create source compilation file"]
        pok("[Compile] Created source file inside container")

        # Step 3: Verify the file exists in the container
        file_contents2_result = compilecontainer.exec_run(f"cat {container_temp_working_dir}/{source_filename}")
        if file_contents2_result.exit_code != 0:
            perr(f"[Compile] Failed to read source file:\n{file_contents2_result.output.decode()}")
            return [-6, "Failed to read source compilation file"]
        pok("[Compile] Source file exists inside container.")

        # Decode and print the content
        # file_contents2 = file_contents2_result.output.decode('utf-8')
        # pinfo(f"Copied over file content:\n{file_contents2}")

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

        pok(f"[Compile] Compilation successful. Binary saved to host at '{host_binary_path}'.")
        return [0, ""]

    except Exception as e:
        perr(f"Error during compilation: {str(e)}")
        return [-4, str(e)]
    
def runAndGet(isPython: bool, filedIn: bool, filedOut: bool, inputstr: str, filedInName: str = "", filedOutName: str = ""):
    # Get information about the execution of the code
    # Returns output and execution time. In case of error, returns error message and execution time
    """
    Runs a program in container and returns (return_code, output, execution_time)
    """
    # Untar the executable file
    runcontainer.exec_run("tar -xvf /mnt/a.out -C /mnt")

    # Construct command with time
    if not filedIn: # Straight input into the executing process
        cmd = f"""cat << 'EOF' | /mnt/a.out 2>&1
{inputstr}
EOF"""
    else: # Input from additional files
        with open(filePath+"/tempWorking/"+filedInName, "w") as file:
            file.write(inputstr)
        cmd = f"/mnt/a.out 2>&1"

    beforeTime = time.time()
    # Run the program
    exec_result = runcontainer.exec_run(['sh', '-c', cmd])
    execution_time = time.time() - beforeTime
    
    # Parse output
    output = exec_result.output.decode()
    lines = output.splitlines()
    error = None

    if not filedOut:
        # Everything else is program output
        program_output = '\n'.join(lines[:-1])
    else:
        # if the program outputs a file. We check that file
        exec_result_2 = runcontainer.exec_run(['sh', '-c', f"cat {filedOutName}"])
        try:
            program_output = exec_result_2.output.decode()
            runcontainer.exec_run(["sh", "-c", f"rm -rf {filedOutName}"])
            if exec_result_2.exit_code != 0:
                perr("Error reading output file of submit. Error: " + program_output)
                execution_folder = runcontainer.exec_run(['sh', '-c', "pwd && ls"]).output.decode()
                perr("Execution folder: " + execution_folder)
                program_output = None
        except UnicodeDecodeError:
            perr("File returned non-utf8 bytes. Marking as wrong...")
            program_output = None
            error = UnicodeDecodeError
        except Exception as e:
            perr("Error reading output file of submit. Error: " + str(e))
            execution_folder = runcontainer.exec_run(['sh', '-c', "pwd && ls"]).output.decode()
            perr("Execution folder: " + execution_folder)
            program_output = None
            error = e

    # Remove input/output files
    os.remove(filePath+"/tempWorking/"+filedInName)
    
    return exec_result.exit_code, program_output, execution_time, error

# Judging function. Used for checking if a submit had done okay.
def judge(filename: str):
    try:
        pinfo("Reloading contests information...")
        with open(filePath+"/source/contests.json", "r", encoding="utf-8") as file:
            contests = json.loads(file.read())
            normalized_contests = {key.lower(): value for key, value in contests.items()}
    except:
        perr("No `contests.json` found in source. Please generate one or reinstall the software.")
        return [-4, None]

    # We test if the file's name is one of the things we need to judge, else just remove the file for storage's sake and return its name into result?
    fileSubmitTarget = pathlib.Path(filename).stem.lower()
    if fileSubmitTarget in normalized_contests:
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
                perr(f"> Failed to read file. Please check the file's integrity. Error: {str(e)}")
                return [-2, None]
            
            comres, exception = compile_code(filecontents, ext.lower()) # Compile results, too
            if comres == 0:
                pok("> Compilation successful. Entering testing step")
                try: 
                    inputFromFile = bool(normalized_contests[fileSubmitTarget]["InputType"] == "file")
                    outputFromFile = bool(normalized_contests[fileSubmitTarget]["OutputType"] == "file")
                        
                    code, output, exectime, exc = runAndGet(
                        False, 
                        inputFromFile,
                        outputFromFile,
                        normalized_contests[fileSubmitTarget]["Tests"][0][0],
                        normalized_contests[fileSubmitTarget]["InputFile"],
                        normalized_contests[fileSubmitTarget]["OutputFile"]
                    )

                    if exc:
                        # If an exception happened
                        perr(f"> Exception happened during execution. Error: {str(exc)}")
                        if exc == UnicodeDecodeError:
                            perr("> File returned non-utf8 bytes.")

                    pok(f"Executed file with exit code {code} in {exectime} seconds. Output: {output}")
                    os.remove(filePath+"/tempWorking/a.out")
                except Exception as e:
                    perr(f"An error occurred during execution: {str(e)}")
                    return [-4, e]
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
            input("Pausing...")
        
        time.sleep(wait_time)
    except KeyboardInterrupt:
        pwarn("Process terminated with Keyboard. Shutting down ATOM/C...")
        running = False
        break

# Remove the container for running tasks
pinfo("Stopping execution container...")
runcontainer.stop()
pinfo("Removing execution container...")
runcontainer.remove()
pinfo("Stopping compilation container...")
compilecontainer.stop()
