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
import threading
import queue
from typing import Tuple, Optional, Any

# For logging, like pwarn(), pinfo(), pok(), perr
from csmcoloredloggerprint import *

# Enable ANSI escape codes
os.system("")

LOG_PATH = "/source/logs/"
CONTEST_PATH = "/source/contests.json"
SETTINGS_PATH = "/source/settings.json"
VERSION_PATH = "/source/version.json"

# Variables
filePath = os.path.dirname(os.path.abspath(__file__))
try:
    with open(filePath+VERSION_PATH, "r", encoding="utf-8") as file:
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
        pinfo("[Compile] Creating temporary folder in host.")
        host_temp_working_dir = filePath + "/tempWorking"
        os.makedirs(host_temp_working_dir, exist_ok=True)

        # Get the compile container
        compilecontainer = client.containers.get("compiler")
        compilecontainer.start()
        pinfo("[Compile] Creating temporary folder in Docker container.")

        # Step 1: Write the source code to a file inside the container
        container_temp_working_dir = "/tmp/tempWorking"
        compilecontainer.exec_run(f"mkdir -p {container_temp_working_dir}")

        pinfo("[Compile] Checking if language is supported based on extension.")
        source_filename = "source_code"
        if lang == "cpp":
            source_filename += ".cpp"
        elif lang == "pascal":
            source_filename += ".pas"
        else:
            perr(f"[Compile] Unsupported language. Supported languages: C++, Pascal, Python (Direct Execution). Target language: {lang}")
            return [-1, "Unsupported language"]

        # Create the file using heredoc - fixed indentation issues
        pinfo("[Compile] Writing file content...")
        cat_command = f"""cat << 'EOF' > {container_temp_working_dir}/{source_filename}
{filecontents.strip()}
EOF"""

        exec_result = compilecontainer.exec_run(['sh', '-c', cat_command])
        # pinfo("[Compile] Executing command: " + cat_command)
        if exec_result.exit_code != 0:
            error_msg = exec_result.output.decode()
            perr(f"[Compile] Failed to create source file:\n{error_msg}")
            return [-5, f"Failed to create source compilation file: {error_msg}"]
        
        # Verify file was created and has content
        verify_result = compilecontainer.exec_run(f"test -s {container_temp_working_dir}/{source_filename}")
        if verify_result.exit_code != 0:
            perr("[Compile] File was not created or is empty")
            return [-5, "File creation failed - file is empty or missing"]
            
        pok("[Compile] Created source file inside container")

        # Rest of compilation logic remains the same...
        if lang == "cpp" or lang == "c++" or lang == "cplusplus":
            compile_command = f"g++ {container_temp_working_dir}/{source_filename} -o {container_temp_working_dir}/a.out"
        elif lang == "pas" or lang == "pascal":
            compile_command = f"fpc {container_temp_working_dir}/{source_filename} -o{container_temp_working_dir}/a.out"

        exec_result = compilecontainer.exec_run(compile_command)
        if exec_result.exit_code != 0:
            perr(f"[Compile] Compilation failed:\n{exec_result.output.decode()}")
            return [-3, exec_result.output.decode()]

        # Copy binary back to host
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
    
def exec_with_timeout(container, cmd: list, timeout: float) -> Tuple[Optional[Any], float, bool]:
    """
    Execute a command in a container with timeout.
    
    Args:
        container: Docker container object
        cmd: Command to execute as list
        timeout: Maximum execution time in seconds
    
    Returns:
        Tuple containing:
        - exec_result: The execution result or None if timed out
        - execution_time: Actual execution time
        - timed_out: Boolean indicating if execution timed out
    """
    result_queue = queue.Queue()
    start_time = time.time()
    
    def target():
        try:
            # Start the execution
            exec_result = container.exec_run(cmd)
            result_queue.put(exec_result)
        except Exception as e:
            result_queue.put(e)
    
    # Start execution in separate thread
    thread = threading.Thread(target=target, daemon=True)
    thread.start()
    
    try:
        # Wait for result or timeout
        exec_result = result_queue.get(timeout=timeout)
        execution_time = time.time() - start_time
        
        if isinstance(exec_result, Exception):
            raise exec_result
            
        return exec_result, execution_time, False
        
    except queue.Empty:
        # Timeout occurred
        execution_time = time.time() - start_time
        
        # Kill the running process in the container
        # Get all running processes
        ps_result = container.exec_run(['ps', '-ef'])
        processes = ps_result.output.decode().split('\n')
        
        for process in processes:
            if any(c in process for c in (cmd if isinstance(cmd, list) else [cmd])):
                try:
                    pid = process.split()[1]
                    container.exec_run(['kill', '-9', pid])
                except Exception as e:
                    pass  # Best effort to kill
                    
        return None, execution_time, True

def runAndGet(filedIn: bool, filedOut: bool, inputstr: str, filedInName: str = "", filedOutName: str = "", timeout: float = 1):
    """
    Runs a program in container and returns (return_code, output, execution_time)
    """
    try:
        # Create working directory and move there
        work_dir = "/mnt/work"
        runcontainer.exec_run(f"mkdir -p {work_dir}")
        
        # Untar the executable file into working directory
        runcontainer.exec_run(f"tar -xvf /mnt/a.out -C {work_dir}")
        
        # Construct command with time
        if not filedIn:  # Straight input into the executing process
            cmd = f"""cd {work_dir} && cat << 'EOF' | ./a.out 2>&1
        {inputstr}
        EOF"""
        else:  # Input from additional files
            with open(filePath + "/tempWorking/" + filedInName, "w") as file:
                file.write(inputstr)
            # Copy input file to container work directory
            runcontainer.exec_run(f"cp /mnt/tempWorking/{filedInName} {work_dir}/")
            cmd = f"cd {work_dir} && ./a.out 2>&1"

        # Run the program with timeout
        exec_result, execution_time, timed_out = exec_with_timeout(
            runcontainer, 
            ['sh', '-c', cmd],
            timeout
        )
        
        # Parse output
        output = exec_result.output.decode()
        lines = output.splitlines()
        error = None
        
        if not filedOut:
            # Everything else is program output
            program_output = '\n'.join(lines[:-1])
        else:
            # Verify output file exists
            file_check = runcontainer.exec_run(
                ['sh', '-c', f"test -f {work_dir}/{filedOutName} && echo 'exists'"]
            )
            if b'exists' not in file_check.output:
                perr(f"Output file {filedOutName} not found in execution directory")
                execution_folder = runcontainer.exec_run(
                    ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                ).output.decode()
                perr("Execution folder contents: " + execution_folder)
                return exec_result.exit_code, None, execution_time, FileNotFoundError
            
            # Read the output file
            exec_result_2 = runcontainer.exec_run(
                ['sh', '-c', f"cat {work_dir}/{filedOutName}"]
            )
            try:
                program_output = exec_result_2.output.decode()
                # Clean up output file
                runcontainer.exec_run(["sh", "-c", f"rm -rf {work_dir}/{filedOutName}"])
                if exec_result_2.exit_code != 0:
                    perr("Error reading output file of submit. Error: " + program_output)
                    execution_folder = runcontainer.exec_run(
                        ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                    ).output.decode()
                    perr("Execution folder: " + execution_folder)
                    program_output = None
            except UnicodeDecodeError:
                perr("File returned non-utf8 bytes. Marking as wrong...")
                program_output = None
                error = UnicodeDecodeError
            except Exception as e:
                perr("Error reading output file of submit. Error: " + str(e))
                execution_folder = runcontainer.exec_run(
                    ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                ).output.decode()
                perr("Execution folder: " + execution_folder)
                program_output = None
                error = e
        
        # Clean up input file if it exists
        if filedIn:
            os.remove(filePath + "/tempWorking/" + filedInName)
        
        # Clean up working directory
        runcontainer.exec_run(["sh", "-c", f"rm -rf {work_dir}"])
        
        return exec_result.exit_code, program_output, execution_time, error, timed_out
        
    except Exception as e:
        perr(f"Unexpected error in runAndGet: {str(e)}")
        return -1, None, 0, e

# Judging function. Used for checking if a submit had done okay.
def judge(filename: str):
    try:
        pinfo("Reloading contests information...")
        with open(filePath+CONTEST_PATH, "r", encoding="utf-8") as file:
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
            logfile = open(filePath+LOG_PATH+filename+".log", "w", encoding="utf-8")
            logfile.write("BÀI: " + filename + "\n")
            pinfo("> Not a Python file. Entering compilation step")
            pinfo("> Fetching filedata")
            try:
                with open(filePath+"/workspace/queue/"+filename) as file:
                    filecontents = file.read()
                    logfile.write("Đã đọc được bài nộp. Đang biên soạn...\n")
            except:
                logfile.write("Không đọc được bài nộp\n")
                logfile.close()
                perr(f"> Failed to read file. Please check the file's integrity. Error: {str(e)}")
                return [-2, None]
            
            comres, exception = compile_code(filecontents, ext.lower()) # Compile results, too
            if comres == 0:
                pok("> Compilation successful. Entering testing step")
                logfile.write("Biên soạn thành công\nĐang kiểm tra...\n")
                for i in range(normalized_contests[fileSubmitTarget]["TestAmount"]):
                    logfile.write("\n------------------------------------\n" + filename + " :: Test " + str(i) + "\n------------------------------------\n")
                    try: 
                        inputFromFile = bool(normalized_contests[fileSubmitTarget]["InputType"] == "file")
                        outputFromFile = bool(normalized_contests[fileSubmitTarget]["OutputType"] == "file")
                        inputstr = normalized_contests[fileSubmitTarget]['Tests'][i][0].strip()
                        code, output, exectime, exc, timedOut = runAndGet(
                            inputFromFile,
                            outputFromFile,
                            inputstr + "\n",
                            normalized_contests[fileSubmitTarget]["InputFile"],
                            normalized_contests[fileSubmitTarget]["OutputFile"]
                        )

                        if exc:
                            # If an exception happened
                            perr(f"> Exception happened during execution. Error: {str(exc)}")
                            if exc == UnicodeDecodeError:
                                perr("> File returned non-utf8 bytes.")

                        pok(f"Executed file with exit code {code} in {exectime} seconds")
                        strippedOutput = output.strip()
                        strippedDestine = normalized_contests[fileSubmitTarget]["Tests"][i][1].strip()
                        logfile.write(f"Đầu vào:\n{inputstr}\n\nĐầu ra chương trình:\n{strippedOutput}\n\nĐầu ra chính xác:\n{strippedDestine}\n\n")
                        if not timedOut:
                            logfile.write(f"Exit code {code} :: {exectime} seconds\n")
                        else:
                            logfile.write(f"Exit code {code} :: {exectime}+ seconds\n")
                        if exectime <= float(normalized_contests[fileSubmitTarget]["TimeLimit"]) and not timedOut:
                            if strippedOutput == strippedDestine:
                                pinfo(f"> Test {i} passed!")
                                logfile.write("Đầu ra chương trình giống đầu ra dự kiến\n")
                            else:
                                pinfo(f"> Test {i} did not pass!")
                                logfile.write("Đầu ra chương trình KHÁC đầu ra dự kiến\n")
                        else:
                            pinfo(f"> Test {i} timed out!")
                            logfile.write("Chương trình chạy quá thời gian\n")
                            
                    except Exception as e:
                        perr(f"An error occurred during execution: {str(e)}")
                        continue
                        
                os.remove(filePath+"/tempWorking/a.out")
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
        with open(filePath+SETTINGS_PATH, "r", encoding="utf-8") as file:
            wait_time = json.loads(file.read())["wait_time"]
    except Exception as e:
        perr(f"Failed to read settings.json: {str(e)}")
    try:
        dirs = os.listdir(filePath + "/workspace/queue/")
        if len(dirs) > 0:
            # There is files, actual files
            for file in dirs:
                judge(file)
                os.remove(filePath+"/workspace/queue/"+file)
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
