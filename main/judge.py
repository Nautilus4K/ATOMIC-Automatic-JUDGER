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
import shutil
import traceback
import signal
import sys

# For logging, like pwarn(), pinfo(), pok(), perr
from csmcoloredloggerprint import *

# Enable ANSI escape codes
os.system("")

LOG_PATH = "/workspace/result/logs/"
CONTEST_PATH = "/source/contests.json"
SETTINGS_PATH = "/source/settings.json"
VERSION_PATH = "/source/version.json"
STATUS_PATH = "/central/status.json"
USERFILE_PATH = "/source/users.json"

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
    pwarn("Phiên bản ATOMIC này không ổn định. Sử dụng một cách thận trọng.")

pinfo("Vị trí chạy chương trình: " + filePath)

# Connect to docker environment
try:
    client = docker.from_env()
except Exception as e:
    perr(f"Thất bại trong quá trình kết nối đến môi trường Docker: {str(e)}")
    perr("Môi trường Docker của bạn có thể bị lỗi hoặc chưa được tải về và bật lên.")
    exit(-127)

# Check if the connection is okay
try:
    client.ping()
    pinfo("Đã kết nối đến môi trường Docker.")
except Exception as e:
    perr("Môi trường Docker không hoạt động. Hãy thử chạy lại hệ thống chấm bài hoặc khởi động lại Docker.")
    exit(-126)

# Get all containers, both running and not running
containers = client.containers.list(all=True)
# pinfo("Currently running containers: ")
# for container in containers:
#     print(" - " + container.name)

pinfo("Đang làm sạch môi trường Docker.")

index = 0
for container in containers:
    index+=1
    pinfo(f"Container ({index}/{len(containers)}): " + container.name)
    if container.status == "running":
        pinfo("Container đang chạy, đang dừng lại...")
        container.stop()
    else:
        pwarn("Container không phải đang chạy.")
    if container.name != "compiler":
        pinfo("Đang xóa container...")
        container.remove()
    else:
        pwarn("Container biên soạn đã được phát hiện, giữ lại container.")


# Prepare judging environment
pinfo("Đang chuẩn bị môi trường chấm bài (Docker). Chắc chắn rằng bạn đã kết nối Internet.")
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
    
def run_container(image_name, mounted_dir: str, name: str):
    # Get the current working directory on the host
    current_directory = filePath + mounted_dir
    
    # Mount the host's current directory into /mnt inside the container
    container = client.containers.run(
        image_name,
        volumes={current_directory: {'bind': '/mnt', 'mode': 'rw'}},  # Mount the current directory to /mnt
        detach=True,  # Run in the background
        working_dir='/mnt',  # Set the working directory inside the container to /mnt (host's current directory)
        name=name
    )
    
    container.start()
    container.reload()
    pinfo(f"Container {container.id} đang chạy với /tempWorking được mount, tình hình hiện tại: {container.status}")
    return container

if not check_image_exists("atomic-python"):
    pwarn("Không tìm thấy ảnh môi trường, đang xây dựng...")
    pyimage, build_logs = client.images.build(path=filePath+"/templates", dockerfile="Dockerfile.python", tag="atomic-python", rm=True)
    for log in build_logs:
        pinfo(log.get('stream', '').strip())
    pok(f"Ảnh đã được xây dựng với ID: {pyimage.id}")
pinfo("Đang tạo container mới cho môi trường")
runcontainer = run_container("atomic-python", "/tempWorking", "execution")

if not check_image_exists("atomic-compile"):
    pwarn("Không tìm thấy ảnh biên soạn, đang xây dựng...")
    pinfo("Đang xây dựng môi trường biên soạn...")
    try:
        compileimage, build_logs = client.images.build(path=filePath+"/templates", dockerfile="Dockerfile.compile", tag="atomic-compile")
        for log in build_logs:
            pinfo(log.get('stream', '').strip())
        pok(f"Ảnh đã được xây dựng với IDID: {compileimage.id}")
    except Exception as e:
        perr("Lỗi trong quá trình xây dựng ảnh môi trường biên soạn: " + str(e))
        runcontainer.remove()
        exit(-125)

if not check_container_exists("compiler"):
    pwarn("Không tìm thấy container biên soạn, đang làm...")
    container = client.containers.create("atomic-compile", name="compiler")
    pok(f"Container {container.name} đã được tạo dựng với ID {container.id}")



pinfo("Đang bắt đầu container biên soạn")
try:
    compilecontainer = client.containers.get("compiler")
    compilecontainer.reload()
    if compilecontainer.status != "running":
        pwarn("Container không đang chạy.")
        compilecontainer.start()
        compilecontainer.reload()
        pok(f"Thành công bật container biên soạn, tình hình hiện tại: {compilecontainer.status}")
    else: pinfo("Không bật container vì nó đã được bật.")
    # compilecontainer.exec_run("")
except Exception as e:
    perr(f"Bật container biên soạn không thành công, lỗi: {e}")
    runcontainer.remove()
    exit(-124)

pinfo("Chuẩn bị bắt đầu chấm bài...")


def compile_code(filecontents: str, lang: str):
    try:
        pinfo("[Biên soạn] Đang tạo thư mục tạm thời ở máy chủ.")
        host_temp_working_dir = filePath + "/tempWorking"
        os.makedirs(host_temp_working_dir, exist_ok=True)

        # Get the compile container
        compilecontainer = client.containers.get("compiler")
        compilecontainer.start()
        pinfo("[Biên soạn] Đang liên kết thư mục tạm thời trong container.")

        # Step 1: Write the source code to a file inside the container
        container_temp_working_dir = "/tmp/tempWorking"
        compilecontainer.exec_run(f"mkdir -p {container_temp_working_dir}")

        pinfo("[Biên soạn] Đang kiểm tra xem ngôn ngữ có được hỗ trợ dựa trên phần mở rộng.")
        source_filename = "source_code"
        if lang == "cpp" or lang == "c++":
            source_filename += ".cpp"
        elif lang == "pascal" or lang == "pas":
            source_filename += ".pas"
        else:
            perr(f"[Biên soạn] Ngôn ngữ không được hỗ trợ. Các ngôn ngữ được hỗ trợ: C++, Pascal, Python (Không biên soạn). Ngôn ngữ được đưa vào: {lang}")
            return [-1, "Unsupported language"]

        # Create the file using heredoc - fixed indentation issues
        pinfo("[Biên soạn] Đang viết dữ liệu...")
        cat_command = f"""cat << 'EOF' > {container_temp_working_dir}/{source_filename}
{filecontents.strip()}
EOF"""

        exec_result = compilecontainer.exec_run(['sh', '-c', cat_command])
        # pinfo("[Biên soạn] Executing command: " + cat_command)
        if exec_result.exit_code != 0:
            error_msg = exec_result.output.decode()
            perr(f"[Biên soạn] Thất bại trong quá trình viết file mã nguồn:\n{error_msg}")
            return [-5, f"Failed to create source compilation file: {error_msg}"]
        
        # Verify file was created and has content
        verify_result = compilecontainer.exec_run(f"test -s {container_temp_working_dir}/{source_filename}")
        if verify_result.exit_code != 0:
            perr("[Biên soạn] File không được tạo ra HOẶC trống")
            return [-5, "File creation failed - file is empty or missing"]
            
        pok("[Biên soạn] Đã tạo file mã nguồn trong container")

        # Rest of compilation logic remains the same...
        if lang == "cpp" or lang == "c++" or lang == "cplusplus":
            compile_command = f"g++ {container_temp_working_dir}/{source_filename} -o {container_temp_working_dir}/a.out"
        elif lang == "pas" or lang == "pascal":
            compile_command = f"fpc {container_temp_working_dir}/{source_filename} -o{container_temp_working_dir}/a.out"

        exec_result = compilecontainer.exec_run(compile_command)
        if exec_result.exit_code != 0:
            perr(f"[Biên soạn] Biên soạn thất bại:\n{exec_result.output.decode()}")
            return [-3, exec_result.output.decode()]

        # Copy binary back to host
        binary_stream, _ = compilecontainer.get_archive(f"{container_temp_working_dir}/a.out")
        host_binary_path = os.path.join(host_temp_working_dir, "a.out")

        with open(host_binary_path, "wb") as f:
            for chunk in binary_stream:
                f.write(chunk)

        pok(f"[Biên soạn] Biên soạn thành công, file có thể chạy đã được lưu vào máy chủ tại '{host_binary_path}'.")
        return [0, ""]

    except Exception as e:
        perr(f"Lỗi khi đang biên soạn: {str(e)}")
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

def runAndGet(logfile, filedIn: bool, filedOut: bool, inputstr: str, filedInName: str = "", filedOutName: str = "", timeout: float = 1):
    """
    Runs a program in container and returns (return_code, output, execution_time, error, timed_out, excmsg)
    """
    try:
        # Create and use a working directory within the container
        work_dir = "/mnt/work"
        runcontainer.exec_run(f"mkdir -p {work_dir}")
        
        # Untar the executable file into the working directory
        runcontainer.exec_run(f"tar -xvf /mnt/a.out -C {work_dir}")
        
        # Construct command with input creation entirely in the container
        if not filedIn:
            # Inline input via heredoc
            cmd = f"""cd {work_dir} && cat << 'EOF' | ./a.out 2>&1
{inputstr.strip()}
EOF"""
        else:
            # Create the input file inside the container using heredoc
            # This writes inputstr to a file named filedInName in work_dir
            runcontainer.exec_run(f"""sh -c "cd {work_dir} && cat << 'EOF' > {filedInName}
{inputstr.strip()}
EOF" """)
            # Then run the program (which is assumed to know how to read the file)
            cmd = f"cd {work_dir} && ./a.out 2>&1"

        # Run the command with timeout
        exec_result, execution_time, timed_out = exec_with_timeout(
            runcontainer, 
            ['sh', '-c', cmd],
            timeout
        )
        
        # Get raw output and split into lines
        output = exec_result.output.decode()
        lines = output.splitlines()
        error = None
        
        if not filedOut:
            # Use the output from the container command
            program_output = '\n'.join(lines[:-1])
        else:
            # Check for the expected output file in the container
            file_check = runcontainer.exec_run(
                ['sh', '-c', f"test -f {work_dir}/{filedOutName} && echo 'exists'"]
            )
            if b'exists' not in file_check.output:
                perr(f"Output file {filedOutName} not found in execution directory")
                execution_folder = runcontainer.exec_run(
                    ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                ).output.decode()
                perr("Execution folder contents: " + execution_folder)
                logfile.write("[LỖI] Không tìm thấy file kết quả.\n")
                return exec_result.exit_code, None, execution_time, FileNotFoundError, False, "Không tìm thấy file kết quả."
            
            # Read the output file from the container
            exec_result_2 = runcontainer.exec_run(
                ['sh', '-c', f"cat {work_dir}/{filedOutName}"]
            )
            try:
                program_output = exec_result_2.output.decode()
                # Remove the output file from the container
                runcontainer.exec_run(["sh", "-c", f"rm -rf {work_dir}/{filedOutName}"])
                if exec_result_2.exit_code != 0:
                    perr("Error reading output file. Error: " + program_output)
                    execution_folder = runcontainer.exec_run(
                        ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                    ).output.decode()
                    perr("Execution folder: " + execution_folder)
                    program_output = None
            except UnicodeDecodeError:
                perr("File returned non-utf8 bytes. Marking as wrong...")
                logfile.write("Lỗi giải mã: Tìm thấy ký tự vượt quá phạm trù bảng mã UTF-8\n")
                program_output = None
                error = UnicodeDecodeError
            except Exception as e:
                perr("Error reading output file. Error: " + str(e))
                execution_folder = runcontainer.exec_run(
                    ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                ).output.decode()
                perr("Execution folder: " + execution_folder)
                program_output = None
                error = e
        
        # Clean up the container working directory
        runcontainer.exec_run(["sh", "-c", f"rm -rf {work_dir}"])
        excmsg = None
        final_output = program_output if filedOut else str(output).strip()
        return exec_result.exit_code, final_output, execution_time, error, timed_out, excmsg
        
    except Exception as e:
        perr(f"Unexpected error in runAndGet: {str(e)}")
        return -1, None, 0, e

def runPython(logfile, filedIn: bool, filedOut: bool, inputstr: str, filedInName: str = "", filedOutName: str = "", timeout: float = 1):
    try:
        # Create and use a working directory within the container
        work_dir = "/mnt/work"
        runcontainer.exec_run(f"mkdir -p {work_dir}")
        
        # Copy the python file into the working directory
        runcontainer.exec_run(f"cp /mnt/a.py {work_dir}/a.py")
        
        # Construct command with input creation entirely in the container
        if not filedIn:
            cmd = f"""cd {work_dir} && cat << 'EOF' | python ./a.py 2>&1
{inputstr.strip()}
EOF"""
        else:
            # Create input file inside the container
            runcontainer.exec_run(f"""sh -c "cd {work_dir} && cat << 'EOF' > {filedInName}
{inputstr.strip()}
EOF" """)
            cmd = f"cd {work_dir} && python ./a.py 2>&1"

        # Run the command with timeout
        exec_result, execution_time, timed_out = exec_with_timeout(
            runcontainer, 
            ['sh', '-c', cmd],
            timeout
        )
        
        # Get raw output and split into lines
        output = exec_result.output.decode()
        lines = output.splitlines()
        error = None
        
        if not filedOut:
            program_output = '\n'.join(lines[:-1])
        else:
            # Check for the expected output file in the container
            file_check = runcontainer.exec_run(
                ['sh', '-c', f"test -f {work_dir}/{filedOutName} && echo 'exists'"]
            )
            if b'exists' not in file_check.output:
                perr(f"Output file {filedOutName} not found in execution directory")
                execution_folder = runcontainer.exec_run(
                    ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                ).output.decode()
                perr("Execution folder contents: " + execution_folder)
                logfile.write("[LỖI] Không tìm thấy file kết quả.\n")
                return exec_result.exit_code, None, execution_time, FileNotFoundError, False, "Không tìm thấy file kết quả."
            
            # Read the output file from the container
            exec_result_2 = runcontainer.exec_run(
                ['sh', '-c', f"cat {work_dir}/{filedOutName}"]
            )
            try:
                program_output = exec_result_2.output.decode()
                runcontainer.exec_run(["sh", "-c", f"rm -rf {work_dir}/{filedOutName}"])
                if exec_result_2.exit_code != 0:
                    perr("Error reading output file. Error: " + program_output)
                    execution_folder = runcontainer.exec_run(
                        ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                    ).output.decode()
                    perr("Execution folder: " + execution_folder)
                    program_output = None
            except UnicodeDecodeError:
                perr("File returned non-utf8 bytes. Marking as wrong...")
                logfile.write("Lỗi giải mã: Tìm thấy ký tự vượt quá phạm trù bảng mã UTF-8\n")
                program_output = None
                error = UnicodeDecodeError
            except Exception as e:
                perr("Error reading output file. Error: " + str(e))
                execution_folder = runcontainer.exec_run(
                    ['sh', '-c', f"cd {work_dir} && pwd && ls -la"]
                ).output.decode()
                perr("Execution folder: " + execution_folder)
                program_output = None
                error = e
        
        # Clean up the container working directory
        runcontainer.exec_run(["sh", "-c", f"rm -rf {work_dir}"])
        
        excmsg = None
        final_output = program_output if filedOut else str(output).strip()
        return exec_result.exit_code, final_output, execution_time, error, timed_out, excmsg
        
    except Exception as e:
        perr(f"Unexpected error in runPython: {str(e)}")
        return -1, None, 0, e


# Judging function. Used for checking if a submit had done okay.
def judge(fullfilename: str, show_test: bool):
    try:
        pinfo("Đang tải lại thông tin bài thi...")
        with open(filePath+CONTEST_PATH, "r", encoding="utf-8") as file:
            contests = json.loads(file.read())
            normalized_contests = {key.lower(): value for key, value in contests.items()}
    except:
        perr("Không tìm thấy `contests.json`, xin hãy thiết lập.")
        return [-4, None]

    # Get file data in this format [username][submission_name].extension
    # into a list: [username, submission_name, extension] ---> filedata
    # The worst way to do this
    pinfo("> Đang phân tách dữ liệu...")
    filedata = fullfilename.replace("][", ".").replace("[", "").replace("]", "").split(".")
    filename = filedata[1].lower() # FUCK, I forgot this once and errors comes. FUCK
    pok("[!] Đã phân tách thành DANH SÁCH (LIST): " + str(filedata))

    username = filedata[0]
    with open(filePath + USERFILE_PATH, "r", encoding='utf-8') as usersFile:
        users = json.load(usersFile)

    # Make sure we only shows the valid contests
    userclass = users[username]["class"]
    badContests = []
    for contest in normalized_contests:
        validClass = False
        for cls in userclass:
            if cls in normalized_contests[contest]["Classes"]:
                validClass = True
                break
        if not validClass:
            badContests.append(contest)

    for contest in badContests:
        del normalized_contests[contest]

    # We test if the file's name is one of the things we need to judge, else just remove the file for storage's sake and return its name into result?
    if filename in normalized_contests:
        pinfo(f"Đang chấm {filename}...")

        # Prepare to write logs
        logfile = open(filePath+LOG_PATH+fullfilename+".log", "w", encoding="utf-8")
        logfile.write("BÀI: " + fullfilename + "\n")
        # Get extensions of file
        ext = filedata[2]
        if ext == "py" or ext == "pyw" or ext == "python":
            pinfo("> Phát hiện file Python. Đang kiểm tra...")
            shutil.copy(filePath+"/workspace/queue/"+fullfilename, filePath+"/tempWorking/a.py")
            logfile.write("Đang kiểm tra...\n")
            score = 0.0
            for i in range(normalized_contests[filename]["TestAmount"]):
                # Write logs when we test the script with the tests in contests.json
                logfile.write("\n------------------------------------\n" + filename + " :: Test " + str(i) + "\n------------------------------------\n") 
                try: 
                    inputFromFile = bool(normalized_contests[filename]["InputType"] == "file")
                    outputFromFile = bool(normalized_contests[filename]["OutputType"] == "file")
                    inputstr = normalized_contests[filename]['Tests'][i][0].strip()

                    # Get data
                    code, output, exectime, exc, timedOut, excmsg = runPython(
                        logfile,
                        inputFromFile,
                        outputFromFile,
                        inputstr + "\n",
                        normalized_contests[filename]["InputFile"],
                        normalized_contests[filename]["OutputFile"]
                    )
                    pok(f"Đã chạy file với exit code {code} trong {exectime} giây")
                    if exc or excmsg:
                        # If an exception happened
                        perr(f"> Lỗi trong khi chạy. Lỗi: {exc.args[0]}")
                        if show_test: logfile.write(f"Đầu vào:\n{inputstr}\n\nĐầu ra chính xác:\n{strippedDestine}\n\nChương trình chạy sinh lỗi: ")
                        else:
                            logfile.write("Chương trình chạy sinh lỗi: ")
                        if excmsg:
                            pinfo("> DEBUG: "+excmsg)
                            logfile.write(excmsg)
                        else:
                            if exc == UnicodeDecodeError:
                                # This shouldn't happen
                                # Yet it does
                                # Fuck
                                perr("> File trả về bytes không vừa bảng mã UTF-8.")
                            logfile.write(f"[{str(exec)}]\nĐể biết thêm thông tin chi tiết, vui lòng tìm kiếm lỗi này trong ngôn ngữ lập trình Python.\n\n")
                    else:
                        # if no exception has happened. We check the results if it matches and is correct
                        # also, write into the logs
                        
                        strippedOutput = output.strip()
                        strippedDestine = normalized_contests[filename]["Tests"][i][1].strip()
                        if show_test: logfile.write(f"Đầu vào:\n{inputstr}\n\nĐầu ra chương trình:\n{strippedOutput}\n\nĐầu ra chính xác:\n{strippedDestine}\n\n")

                        # God bless this check. This is probably the worst way I could do this
                        if not timedOut:
                            logfile.write(f"Exit code {code} :: {exectime} giây\n")
                        else:
                            logfile.write(f"Exit code {code} :: {exectime}+ giây\n")

                        # In case there is no time pass-through
                        if exectime <= float(normalized_contests[filename]["TimeLimit"]) and not timedOut:
                            # If the result/output matches with the supposed answer
                            if strippedOutput == strippedDestine:
                                pinfo(f"> Test {i} đúng!")
                                logfile.write("Đầu ra chương trình giống đầu ra dự kiến\n")
                                score += 10
                            else:
                                pinfo(f"> Test {i} thất bại!")
                                logfile.write("Đầu ra chương trình KHÁC đầu ra dự kiến\n")
                        else:
                            pinfo(f"> Test {i} quá thời gian!")
                            logfile.write("Chương trình chạy quá thời gian\n")
                        
                except Exception as e:
                    perr(f"Đã có lỗi xảy ra trong khi chạy: {str(e)}")
                    # logfile.write(f"Lỗi HỆ THỐNG: {str(e)}\n")
                    logfile.write("Đã có lỗi xảy ra trong quá trình chạy chương trình.\n")
                    continue
            score /= normalized_contests[filename]["TestAmount"]
            pinfo("> Điểm kết quả: " + str(score))
                    
            try:
                os.remove(filePath+"/tempWorking/a.py")
            except:
                # perr("Cannot remove a.py in temporary working folder, probably deleted beforehand.")
                perr("Không thể xóa a.py trong thư mục làm việc tạm thời, khả năng cao đã bị xóa từ trước.")

            # If an existing userdata json file exists
            if os.path.exists(filePath+"/workspace/result/"+filedata[0]+".json"):
                with open(filePath+"/workspace/result/"+filedata[0]+".json", "r") as file:
                    userdata = json.loads(file.read())
            else:
                userdata = {}

            logfile.write("\nĐiểm TỔNG: " + str(score))
            userdata[filedata[1]] = score

            with open(filePath+"/workspace/result/"+filedata[0]+".json", "w") as file:
                file.write(json.dumps(userdata))

            return [0, score]
        else:
            pinfo("> Không phải file python, đang biên soạn...")
            pinfo("> Đang lấy dữ liệu file")
            try:
                with open(filePath+"/workspace/queue/"+fullfilename) as file:
                    filecontents = file.read()
                    logfile.write("Đã đọc được bài nộp. Đang biên soạn...\n")
            except:
                logfile.write("Không đọc được bài nộp\n")
                logfile.close()
                perr(f"> Không đọc được file. Vui lòng kiểm tra tính toàn vẹn của file. Lỗi: {str(e)}")
                return [-2, None]
            
            comres, exception = compile_code(filecontents, ext.lower()) # Compile results, too
            if comres == 0: # If compilation successes
                pok("> Thành công biên soạn. Đang sang bước kiểm tra...")
                logfile.write("Biên soạn thành công\nĐang kiểm tra...\n")
                score = 0.0
                for i in range(normalized_contests[filename]["TestAmount"]):
                    # Write logs when we test the script with the tests in contests.json
                    logfile.write("\n------------------------------------\n" + filename + " :: Test " + str(i) + "\n------------------------------------\n") 
                    try: 
                        inputFromFile = bool(normalized_contests[filename]["InputType"] == "file")
                        outputFromFile = bool(normalized_contests[filename]["OutputType"] == "file")
                        pinfo("I/O Status: ")
                        pinfo("- isInputFile: " + str(inputFromFile))
                        pinfo("- isOutputFile: " + str(outputFromFile))
                        inputstr = normalized_contests[filename]['Tests'][i][0].strip()

                        # Get data
                        code, output, exectime, exc, timedOut, excmsg = runAndGet(
                            logfile,
                            inputFromFile,
                            outputFromFile,
                            inputstr + "\n",
                            normalized_contests[filename]["InputFile"],
                            normalized_contests[filename]["OutputFile"]
                        )
                        pok(f"Đã chạy file với exit code {code} trong {exectime} giây")
                        if exc or excmsg:
                            # If an exception happened
                            perr(f"> Lỗi trong khi chạy. Lỗi: {exc.args[0]}")
                            if show_test: logfile.write(f"Đầu vào:\n{inputstr}\n\nĐầu ra chính xác:\n{strippedDestine}\n\nChương trình chạy sinh lỗi: ")
                            else: logfile.write("Chương trình chạy sinh lỗi: ")
                            if excmsg:
                                pinfo("> DEBUG: "+excmsg)
                                logfile.write(excmsg)
                            else:
                                if exc == UnicodeDecodeError:
                                    # This shouldn't happen
                                    # Yet it does
                                    # Fuck
                                    perr("> File trả về bytes không vừa bảng mã UTF-8.")
                                logfile.write(f"[{str(exec)}]\nĐể biết thêm thông tin chi tiết, vui lòng tìm kiếm lỗi này trong ngôn ngữ lập trình Python.\n\n")
                        else:
                            # if no exception has happened. We check the results if it matches and is correct
                            # also, write into the logs
                            
                            strippedOutput = output.strip()
                            strippedDestine = normalized_contests[filename]["Tests"][i][1].strip()
                            if show_test: logfile.write(f"Đầu vào:\n{inputstr}\n\nĐầu ra chương trình:\n{strippedOutput}\n\nĐầu ra chính xác:\n{strippedDestine}\n\n")

                            # God bless this check. This is probably the worst way I could do this
                            if not timedOut:
                                logfile.write(f"Exit code {code} :: {exectime} giây\n")
                            else:
                                logfile.write(f"Exit code {code} :: {exectime}+ giây\n")

                            # In case there is no time pass-through
                            if exectime <= float(normalized_contests[filename]["TimeLimit"]) and not timedOut:
                                # If the result/output matches with the supposed answer
                                if strippedOutput == strippedDestine:
                                    pinfo(f"> Test {i} vượt qua!")
                                    logfile.write("Đầu ra chương trình giống đầu ra dự kiến\n")
                                    score += 10
                                else:
                                    pinfo(f"> Test {i} thất bại!")
                                    logfile.write("Đầu ra chương trình KHÁC đầu ra dự kiến\n")
                            else:
                                pinfo(f"> Test {i} quá thời gian!")
                                logfile.write("Chương trình chạy quá thời gian\n")
                            
                    except Exception as e:
                        perr(f"Đã có lỗi xảy ra khi đang chạy chương trình: {str(e)}")
                        # logfile.write(f"Lỗi HỆ THỐNG: {str(e)}\n")
                        logfile.write("Đã có lỗi xảy ra trong quá trình chạy chương trình.\n")
                        continue
                
                score /= normalized_contests[filename]["TestAmount"]
                pinfo("> Điểm kết quả: " + str(score))

                # If an existing userdata json file exists
                if os.path.exists(filePath+"/workspace/result/"+filedata[0]+".json"):
                    with open(filePath+"/workspace/result/"+filedata[0]+".json", "r") as file:
                        userdata = json.loads(file.read())
                else:
                    userdata = {}

                logfile.write("\nĐiểm TỔNG: " + str(score))
                userdata[filedata[1]] = score

                with open(filePath+"/workspace/result/"+filedata[0]+".json", "w") as file:
                    file.write(json.dumps(userdata))

                os.remove(filePath+"/tempWorking/a.out")
                return [0, score]
            else:
                perr(f"> Biên soạn thất bại với code {comres}. Hãy kiểm tra tính toàn vẹn của code.")
                perr(f"> Lỗi:\n {exception}")
                logfile.write("Lỗi trong quá trình biên soạn:\n"+str(exception))
                return [-3, exception]

    else:
        pinfo(f"Tìm thấy bài nộp không vừa với bài thi hiện có nào, đang xóa {fullfilename}.")
        os.remove(filePath+"/workspace/queue/"+fullfilename)
        return [-1, None]

def reload_containers():
    global runcontainer

    # Remove the current container if it exists
    if runcontainer:
        pinfo("Đã xóa container thực hiện chương trình với: force=True.")
        runcontainer.remove(force=True)
    else:
        pwarn("Container thực hiện chương trình không tồn tại.")

    # Start a new container and reassign to runcontainer
    runcontainer = run_container("atomic-python", "/tempWorking", "execution")

    # Log the action
    pinfo("Đã tải lại container thực hiện chương trình.")

pinfo("Hiện đang kiểm tra các bài nộp.")

running = True
def signal_handler(sig, frame):
    global running
    pwarn(f"Quá trình dừng lại bằng {sig}. Đang dừng lại...")
    running = False

# Register signal handlers for both Windows and Unix-like systems
signal.signal(signal.SIGINT, signal_handler)  # Handles Ctrl+C, os.kill(pid, SIGINT)
signal.signal(signal.SIGTERM, signal_handler)  # Handles graceful termination on Unix
if os.name == 'nt':  # Windows-specific
    signal.signal(signal.SIGBREAK, signal_handler)  # Handles GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT)

lastReload = time.time()
while running:
    try:
        with open(filePath+SETTINGS_PATH, "r", encoding="utf-8") as file:
            settings = json.loads(file.read())
            wait_time = settings["wait_time"]
    except Exception as e:
        perr(f"Đọc settings.json không thành công: {str(e)}")
    try:
        dirs = os.listdir(filePath + "/workspace/queue/")
        if len(dirs) > 0:
            # There is files, actual files
            for file in dirs:
                statusdata = {}
                
                statusdata["action"] = "judging"
                statusdata["target"] = file

                # Write status into file
                # If file exists, delete it beforehand.
                # That's the kind of description I like for this code segment
                if os.path.exists(filePath+STATUS_PATH):
                    os.remove(filePath+STATUS_PATH)
                with open(filePath+STATUS_PATH, "w") as statusfile:
                    statusfile.write(json.dumps(statusdata))

                try:
                    judge(file, settings["show_test"])
                except Exception as e:
                    traceback.print_exc()
                    running = False
                    break

                try: 
                    # os.remove(filePath+"/workspace/queue/"+file)
                    # Extract the file informations.
                    # Now it has this list: [username, submission, extension]
                    filedata = file.replace("][", ".").replace("[", "").replace("]", "").split(".")
                    if not os.path.exists(filePath+"/userdata/"+filedata[0]+"/"):
                        pwarn("Người dùng không có một thu mục có sẵn, đang tạo một thư mục mới...")
                        os.mkdir(filePath+"/userdata/"+filedata[0]+"/")
                    shutil.move(filePath+"/workspace/queue/"+file, filePath+"/userdata/"+filedata[0]+"/"+filedata[1]+"."+filedata[2])
                except:
                    perr(f"Không thể di chuyển {file} đến USERDATA, có gì đó đã xảy ra?")
        else:
            # With nothing to judge/held onto, we returns as if we're idling
            statusdata = {}

            statusdata["action"] = "idling" # Yessir, this is because of my English vocabulary isn't quite as good
            statusdata["target"] = None

            if os.path.exists(filePath+STATUS_PATH):
                os.remove(filePath+STATUS_PATH)
            with open(filePath+STATUS_PATH, "w") as statusfile:
                statusfile.write(json.dumps(statusdata))
        # Reduce drive I/O stress
        if keyboard.is_pressed("a") and keyboard.is_pressed("x") and keyboard.is_pressed("z"):
            # Keybind: A + X + Z
            input("Đang tạm dừng...")
        
        # Reloading functionality.
        # This functionality is used to make sure abitrary code execution wouldn't last long.
        # After all, we are dealing with containers, not real OSes, kind of.
        curtime = time.time()
        if curtime - lastReload >= settings["reload_time"]:
            reload_containers()
            lastReload = curtime

        # This is just to make sure the stress on system components wouldn't hit quite as hard
        time.sleep(wait_time)
    except KeyboardInterrupt:
        pwarn("Quá trình dừng lại bằng SIGINT, đang thoát ATOMIC...")
        running = False
        break
    # except Exception as e:
    #     perr("Error occurred: " + str(e))
    #     pwarn("Terminating process due to error occurence...")
    #     running = False
    #     break

# Remove the container for running tasks

# Get all containers, both running and not running
containers = client.containers.list(all=True)
# pinfo("Currently running containers: ")
# for container in containers:
#     print(" - " + container.name)

pinfo("Đang làm sạch containers...")

index = 0
for container in containers:
    index+=1
    pinfo(f"Container ({index}/{len(containers)}): " + container.name)
    if container.status == "running":
        pinfo("Container đang chạy, đang dừng lại...")
        container.stop()
    else:
        pwarn("Container không đang chạy.")


    if container.name != "compiler":
        pinfo("Đang xóa container...")
        container.remove()
    else:
        pwarn("Phát hiện container biên soạn, bỏ qua...")
