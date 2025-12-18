import os
import mimetypes
import json
import random
import time
import string
import traceback
from hashlib import sha256
from base64 import b64decode

from PIL import Image
import io

# Variables
ALIASES_FILENAME = "aliases.json"
WEBSITERULE_FILENAME = "websites.json"
ERROR_WEBFILE = "/www/reserved/error.html"
USERPROFILE_WEBFILE = "/www/reserved/userprofile.html"
VERSION_JSON = "/source/version.json"
# USERDATA_JSON = "/source/users.json"
LASTACCESS_JSON = "/central/lastaccess.json"
SESSION_JSON = "/central/sessions.json"
STATSBAR_HTML = "/www/reserved/statsbar.html"
SETTINGS_JSON = "/source/settings.json"
DEFAULTUSER_IMAGE = "/assets/defaultuser.png"
USERPROFILEPICTURE_PATH = "/www/userpictures/"
CONTESTS_JSON = "/source/contests.json"
RESULT_DIR = "/workspace/result/"
SUBMIT_DIR = "/workspace/queue/"
CONTEST_WEBFILE = "/www/reserved/contest.html"
LOG_DIR = "/workspace/result/logs/"

# from usermanage import USERFILE_PATH as USERDATA_JSON, CLASSFILE_PATH
CLASSFILE_PATH = "/source/classes.json"
USERDATA_JSON = "/source/users.json"

dirPath = os.path.dirname(os.path.abspath(__file__))
reservedPaths = ["/debug", "/reserved", "/api"]

def save_session(username: str) -> str:
    """
    Function to save a new session
    Returns:
        randomizedKey: new session token:
        ```python
        (str)
        ```
    """
    with open(dirPath+SESSION_JSON, "r", encoding='utf-8') as sessionFile:
        currentSessions = json.load(sessionFile)

    randomizedKey = ''.join(random.choice(string.ascii_letters+string.digits) for _ in range(50))
    while randomizedKey in currentSessions:
        randomizedKey = ''.join(random.choice(string.ascii_letters+string.digits) for _ in range(50))

    currentSessions[randomizedKey] = {
        "username": username,
        "lastactive": int(time.time())
    }

    with open(dirPath+SESSION_JSON, "w", encoding='utf-8') as sessionFile:
        json.dump(currentSessions, sessionFile)

    return randomizedKey

def get_session(token: str) -> str | None:
    """
    Function to get back the username of a session
    Returns:
        username:
        ```python
        (str | None)
        ```
    """
    with open(dirPath+SESSION_JSON, "r", encoding='utf-8') as sessionFile:
        currentSessions = json.load(sessionFile)

    if token in currentSessions:
        # If session exists already
        currentSessions[token]["lastactive"] = int(time.time())
        username = currentSessions[token]["username"]
    else:
        username = None

    with open(dirPath+SESSION_JSON, "w", encoding='utf-8') as sessionFile:
        json.dump(currentSessions, sessionFile)
    
    return username

def del_session(token: str):
    with open(dirPath+SESSION_JSON, "r", encoding='utf-8') as sessionFile:
        currentSessions = json.load(sessionFile)

    if token in currentSessions:
        del currentSessions[token]
    with open(dirPath+SESSION_JSON, "w", encoding='utf-8') as sessionFile:
        json.dump(currentSessions, sessionFile)

def resize_image(image_bytes, max_size=(600, 600), format="JPEG"):
    # Create an image object from bytes
    img = Image.open(io.BytesIO(image_bytes))
    
    # Step 1: Crop to square using the shortest side
    width, height = img.size
    min_side = min(width, height)
    
    # Calculate coordinates to center-crop the image
    left = (width - min_side) // 2
    upper = (height - min_side) // 2
    right = left + min_side
    lower = upper + min_side
    
    img = img.crop((left, upper, right, lower))  # Now it's a square

    # Step 2: Resize if necessary
    if min_side > max_size[0]:  # compare to one side, since it's square
        img = img.resize(max_size, Image.LANCZOS)

    # Step 3: Save back to bytes
    output_bytes = io.BytesIO()
    img_format = img.format if img.format else format
    img.save(output_bytes, format=img_format)
    output_bytes.seek(0)

    return output_bytes.getvalue()


def api_interface(path: str, headers, ip_addr, body) -> dict:
    # print("API called from ANONYMOUS DEVICE")

    """
    Function that monitors API calls (Requests to /api/)\n
    Returns dictionary object as JSON.
    """

    json_response = {}
    message = ""

    if path == "/api/login" and "USERNAME" in headers and "PASSWD" in headers:
        with open(dirPath+LASTACCESS_JSON, "r", encoding='utf-8') as lastAccessFile:
            lastaccesses = json.load(lastAccessFile)
        if "login" in lastaccesses:
            # Login key exists
            if ip_addr in lastaccesses["login"]:
                # If we tried logging in by sending API key with this IP address
                timePassed = int(time.time()) - lastaccesses["login"][ip_addr]["time"]
                # print(timePassed)
                if timePassed < 60:
                    # If this guys spams
                    if lastaccesses["login"][ip_addr]["amount"] > 5:
                        # print("Exceeds")
                        # Exceeds the recommended amount of API calls
                        json_response = {
                            "success": False,
                            "message": "Vui lòng đợi và thử lại sau."
                        }
                        return json_response
                else:
                    del lastaccesses["login"][ip_addr]

        else: lastaccesses["login"] = {}

        with open(dirPath+USERDATA_JSON, "r", encoding='utf-8') as userdataFile:
            userdata = json.load(userdataFile)

        if headers["USERNAME"] in userdata:
            user = userdata[headers["USERNAME"]]

            hashedpasswd = user["password"]
            hashedrecievedpasswd = sha256(headers["PASSWD"].encode('ascii')).hexdigest()

            # same = (hashedpasswd.strip() == hashedrecievedpasswd.strip())
            if hashedpasswd.strip() == hashedrecievedpasswd.strip():
                same = True
                message = save_session(headers["USERNAME"])
            else:
                same = False
                message = "Tên đăng nhập hoặc mật khẩu không đúng."
                if ip_addr in lastaccesses["login"]:
                    lastaccesses["login"][ip_addr]["amount"] += 1
                    lastaccesses["login"][ip_addr]["time"] = int(time.time())
                else:
                    lastaccesses["login"][ip_addr] = {
                        "time": int(time.time()),
                        "amount": 1
                    }

        else: 
            same = False
            message = "Tên đăng nhập hoặc mật khẩu không đúng."
            same = False
            message = "Tên đăng nhập hoặc mật khẩu không đúng."
            if ip_addr in lastaccesses["login"]:
                lastaccesses["login"][ip_addr]["amount"] += 1
            else:
                lastaccesses["login"][ip_addr] = {
                    "time": int(time.time()),
                    "amount": 1
                }
            
        json_response = {
            "success": same,
            "message": message
        }

        with open(dirPath+LASTACCESS_JSON, "w", encoding='utf-8') as lastAccessFile:
            json.dump(lastaccesses, lastAccessFile)

    elif path == "/api/getbroadinfo" and "TOKEN" in headers:
        username = ""
        fullname = ""
        picture = ""
        _class = ""
        priv = ""
        contestsAnmount = 0

        try:
            with open(dirPath+SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
                sessions = json.load(sessionsFile)

            with open(dirPath+SETTINGS_JSON, "r", encoding='utf-8') as settingsFile:
                settings = json.load(settingsFile)

            with open(dirPath+USERDATA_JSON, "r", encoding='utf-8') as userdataFile:
                users = json.load(userdataFile)

            # Check if token exists before accessing
            if headers["TOKEN"] in sessions:
                # Update last active time
                sessions[headers["TOKEN"]]["lastactive"] = int(time.time())

                # Save updated sessions
                with open(dirPath + SESSION_JSON, "w", encoding='utf-8') as sessionsFile:
                    json.dump(sessions, sessionsFile)

                # Check session expiry
                if time.time() - sessions[headers["TOKEN"]]["lastactive"] <= settings["max_not_logged_in_session_seconds"]:
                    username = sessions[headers["TOKEN"]]["username"]
                    
                    # Check if user still exists
                    if username in users:
                        fullname = users[username]["fullname"]
                        picture = users[username]["picture"]
                        _class = users[username]["class"]
                        priv = users[username]["priv"]

                        # Get contest count
                        if os.path.exists(dirPath + RESULT_DIR + username + ".json"):
                            try:
                                with open(dirPath + RESULT_DIR + username + ".json", "r", encoding='utf-8') as resultFile:
                                    contestsAnmount = len(json.load(resultFile))
                            except:
                                contestsAnmount = 0
                        else:
                            contestsAnmount = 0

        except Exception as e:
            print(f"Error in getbroadinfo: {str(e)}")
            traceback.print_exc()
            username = "ErrorHandling2␀"
            # Keep default empty values on error

        json_response = {
            "username": username,
            "fullname": fullname, 
            "picture": picture,
            "class": _class,
            "priv": priv,
            "contestAmount": contestsAnmount
        }

    elif path == "/api/getfullinfo" and "TOKEN" in headers:
        # Initialize default values
        user_data = {
            "username": "",
            "fullname": "",
            "picture": "",
            "class": "",
            "priv": "",
            "desc": "",
            "contestAmount": 0
        }

        try:
            # Load required data files
            with open(dirPath+SESSION_JSON, "r", encoding='utf-8') as f:
                sessions = json.load(f)
            with open(dirPath+SETTINGS_JSON, "r", encoding='utf-8') as f:
                settings = json.load(f)
            with open(dirPath+USERDATA_JSON, "r", encoding='utf-8') as f:
                users = json.load(f)

            token = headers["TOKEN"]
            if token in sessions:
                # Update last active time
                current_time = int(time.time())
                sessions[token]["lastactive"] = current_time

                # Save updated sessions
                with open(dirPath + SESSION_JSON, "w", encoding='utf-8') as f:
                    json.dump(sessions, f)

                # Check if session is still valid
                if current_time - sessions[token]["lastactive"] <= settings["max_not_logged_in_session_seconds"]:
                    username = sessions[token]["username"]
                    user = users[username]

                    # Get contest count
                    contest_file = dirPath + RESULT_DIR + username + ".json"
                    if os.path.exists(contest_file):
                        with open(contest_file, "r", encoding='utf-8') as f:
                            user_data["contestAmount"] = len(json.load(f))

                    # Update user data
                    user_data.update({
                        "username": username,
                        "fullname": user["fullname"],
                        "picture": user["picture"], 
                        "class": user["class"],
                        "priv": user["priv"],
                        "desc": user["desc"]
                    })

        except Exception as e:
            print(f"Error in getfullinfo: {str(e)}")
            traceback.print_exc()
            username = "ErrorHandling2␀"

        json_response = user_data

    elif path == "/api/delsession" and "TOKEN" in headers:
        with open(dirPath+SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
            sessions = json.load(sessionsFile)
        with open(dirPath+SESSION_JSON, "w", encoding='utf-8') as sessionsFile:
            sessions.pop(headers["TOKEN"], None)
            json.dump(sessions, sessionsFile)

        json_response = {}

    elif path == "/api/uploadprofilepic" and "TOKEN" in headers:
        try:
            success = True
            message = ""
            with open(dirPath + SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
                sessions = json.load(sessionsFile)

                # Get user's username from the session token
                username = sessions[headers["TOKEN"]]["username"]

            sessions[headers["TOKEN"]]["lastactive"] = int(time.time())
            with open(dirPath + SESSION_JSON, "w", encoding='utf-8') as sessionsFile:
                json.dump(sessions, sessionsFile)

            with open(dirPath + USERDATA_JSON, "r", encoding='utf-8') as userdataFile:
                userdata = json.load(userdataFile)

            userdata[username]["picture"] = True

            with open(dirPath + USERDATA_JSON, "w", encoding='utf-8') as userdataWrite:
                json.dump(userdata, userdataWrite)

            # # Read image from body (raw bytes)
            # imgdata = io.BytesIO(body)
            # img = Image.open(imgdata)

            # # Convert to RGB (if it's PNG or another format)
            # img = img.convert("RGB")

            # # Save the image as JPEG
            # print("Saved to", dirPath + USERPROFILEPICTURE_PATH + username + ".jpg")
            with open(dirPath + USERPROFILEPICTURE_PATH + username + ".jpg", "wb") as userprofilepicturefile:
                # img.save(userprofilepicturefile, format="JPEG", quality=95)
                # print(body.decode('utf-8'))
                # Command:
                # b64decode(body.decode('utf-8'))
                userprofilepicturefile.write(resize_image(b64decode(body.decode('utf-8')), (600, 600), "JPEG"))

        except Exception as e:
            success = False
            message = str(e)
            print("/api/uploadprofilepic" + str(e))

        json_response = {
            "success": success,
            "message": message
        }

    # Path for updating user details (which is display name and description)
    elif path == "/api/updatedetails" and "TOKEN" in headers:
        try:
            success = True
            message = ""
            
            body_info = json.loads(body.decode('utf-8'))

            name = body_info["name"]
            desc = body_info["desc"]

            # Get username from token
            with open(dirPath + SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
                sessions = json.load(sessionsFile)
            
            # Extract username from dictionary (JSON-like/map-like) object
            username = sessions[headers["TOKEN"]]["username"]

            sessions[headers["TOKEN"]]["lastactive"] = int(time.time())
            with open(dirPath + SESSION_JSON, "w", encoding='utf-8') as sessionsFile:
                json.dump(sessions, sessionsFile)

            # Get users information
            with open(dirPath + USERDATA_JSON, "r", encoding='utf-8') as userdataFile:
                users = json.load(userdataFile)

            # Modify intended user's information from within dictionary object
            users[username]["fullname"] = name
            users[username]["desc"] = desc

            # Save modified user's information into users file
            with open(dirPath + USERDATA_JSON, "w", encoding='utf-8') as userdataWrite:
                json.dump(users, userdataWrite)

        except Exception as e:
            # Throw an exception
            success = False
            message = str(e)
            print("/api/updatedetails" + str(e))

        json_response = {
            "success": success,
            "message": message
        }
    
    elif path == "/api/setpasswd" and "TOKEN" in headers and "OLDPASS" in headers and "NEWPASS" in headers:
        try:
            success = True
            message = "Thành công thay đổi mật khẩu"

            # Get username from token
            with open(dirPath + SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
                sessions = json.load(sessionsFile)

            username = sessions[headers["TOKEN"]]["username"]
            sessions[headers["TOKEN"]]["lastactive"] = int(time.time())
            with open(dirPath + SESSION_JSON, "w", encoding='utf-8') as sessionsFile:
                json.dump(sessions, sessionsFile)

            with open(dirPath + USERDATA_JSON, "r", encoding='utf-8') as userdataFile:
                users = json.load(userdataFile)

            typedPasswordHashed = sha256(headers["OLDPASS"].encode('ascii')).hexdigest()
            if typedPasswordHashed != users[username]["password"]:
                # If passwords are not the same
                success = False
                message = "Mật khẩu cũ không đúng"
            else:
                # If passwords are, in fact, the same. We proceed to change the password of user
                users[username]["password"] = sha256(headers["NEWPASS"].encode('ascii')).hexdigest()

                # Save it all
                with open(dirPath + USERDATA_JSON, "w", encoding='utf-8') as userdataWrite:
                    json.dump(users, userdataWrite)

        except Exception as e:
            success = False
            print("/api/setpasswd" + str(e))
            message = "Lỗi: " + str(e)

        json_response = {
            "success": success,
            "message": message
        }

    elif path == "/api/getcontests" and "TOKEN" in headers:
        try:
            success = True
            message = ""
            scontests = []
            with open(dirPath + SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
                sessions = json.load(sessionsFile)
            username = sessions[headers["TOKEN"]]["username"]
            sessions[headers["TOKEN"]]["lastactive"] = int(time.time())
            with open(dirPath + SESSION_JSON, "w", encoding='utf-8') as sessionsFile:
                json.dump(sessions, sessionsFile)

            with open(dirPath + USERDATA_JSON, "r", encoding='utf-8') as userdataFile:
                userdata = json.load(userdataFile)
                userclass = userdata[username]["class"]

            with open(dirPath + CONTESTS_JSON, "r", encoding='utf-8') as contestsFile:
                contests = json.load(contestsFile)

            # For each contest. We got the key names
            for contest in contests:
                # Now we check through for each class present in the contest
                # print("-", contest)
                added_amount = 0
                for cls in contests[contest]["Classes"]:
                    # If the class is present in the user's class
                    # print("  -", cls)
                    if cls in userclass and added_amount <= 0:
                        # We add the contest to the list
                        # print("    -", contest)

                        # Also, get user's score
                        # print(dirPath + RESULT_DIR + username + ".json")
                        if (os.path.exists(dirPath + RESULT_DIR + username + ".json")):
                            with open(dirPath + RESULT_DIR + username + ".json", "r", encoding='utf-8') as file:
                                resultJson = json.load(file)
                            
                            # print(resultJson)
                            if contest in resultJson:
                                score = resultJson[contest]
                            else:
                                score = 0.0
                        else:
                            score = 0.0
                        scontests.append([contest, score])
                        added_amount += 1

        except Exception as e:
            success = False
            message = str(e)
            scontests = []
            print("/api/getcontests" + str(e))


        json_response = {
            "success": success,
            "message": message,
            "contests": scontests,
        }

    elif path == "/api/submitcode" and "TOKEN" in headers and "LANG" in headers and "CONT" in headers and body:
        try:
            success = True
            message = ""

            with open(dirPath + SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
                sessions = json.load(sessionsFile)
            username = sessions[headers["TOKEN"]]["username"]
            sessions[headers["TOKEN"]]["lastactive"] = int(time.time())

            # print("Hey")

            with open(dirPath + SESSION_JSON, "w", encoding='utf-8') as sessionsFile:
                json.dump(sessions, sessionsFile)

            with open(dirPath + SUBMIT_DIR + f"[{username}][{headers['CONT']}].{headers['LANG']}", "w", encoding='utf-8') as submitFile:
                submitFile.write(body.decode('utf-8'))

        except Exception as e:
            success = False
            message = str(e)
            print("/api/submitcode" + str(e))
        
        json_response = {
            "success": success,
            "message": message
        }

    elif path == "/api/getscore" and "TOKEN" in headers:
        try:
            success = True
            message = ""
            resultclasses = {}
            contestList = {}

            with open(dirPath + SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
                sessions = json.load(sessionsFile)
            username = sessions[headers["TOKEN"]]["username"]
            sessions[headers["TOKEN"]]["lastactive"] = int(time.time())

            with open(dirPath + CLASSFILE_PATH, "r", encoding='utf-8') as classFile:
                classes = json.load(classFile)

            # Get user's data
            with open(dirPath + USERDATA_JSON, "r", encoding='utf-8') as userdataFile:
                userdata = json.load(userdataFile)

            userclass = userdata[username]["class"]  # Get list object

            with open(dirPath + CONTESTS_JSON, "r", encoding='utf-8') as contestsFile:
                contests = json.load(contestsFile)  # Dictionary object
            
            # with open(dirPath + RESULT_DIR)

            # For each class the user belongs in, we add a key to resultClasses.
            # In each class, we fetch all users and get which users belong to which classes and
            # then add their results in along with the contest results.

            for _class in userclass:
                if classes[_class]["scoreboard"] or userdata[username]["priv"] > 0: # DO NOT EVER DARES TO DREAM OF ANYTHING IF YOU'RE NOT ALLOWED TO
                    # Initialize class dict if not exists
                    if _class not in resultclasses:
                        resultclasses[_class] = {}

                    # Get contests for this class
                    coincideContests = [contest for contest in contests 
                                    if _class in contests[contest]["Classes"]]

                    contestList[_class] = coincideContests

                    # Get users in this class
                    class_users = [user for user in userdata 
                                if _class in userdata[user]["class"]]

                    # Process results for users in this class
                    for user in class_users:
                        result_file = dirPath + RESULT_DIR + user + ".json"
                        if os.path.exists(result_file):
                            with open(result_file, "r", encoding='utf-8') as resultFile:
                                result = json.load(resultFile)
                                # Initialize user dict if not exists
                                if user not in resultclasses[_class]:
                                    resultclasses[_class][user] = {}
                                # Add results for contests in this class
                                for contest in coincideContests:
                                    if contest in result:
                                        resultclasses[_class][user][contest] = result[contest]

        except Exception as e:
            success = False
            message = str(e)
            print("/api/getscore" + str(e))
            resultclasses = {}
            contestList = {}

        json_response = {
            "success": success,
            "message": message,
            "result": resultclasses,
            "contestslist": contestList,
        }

    elif path == "/api/getsubmissioninfo" and "TOKEN" in headers and "CONTEST" in headers:
        try:
            success = True
            message = ""
            logdata = "Không có dữ liệu"
            result = -1
            
            # Get username
            with open(dirPath + SESSION_JSON, "r", encoding='utf-8') as sessionsFile:
                sessions = json.load(sessionsFile)
            username = sessions[headers["TOKEN"]]["username"]

            # Get log file's data
            logfiles = os.listdir(dirPath + LOG_DIR)
            for log in logfiles:
                if log.startswith(f"[{username}][{headers['CONTEST']}]"):
                    with open(dirPath + LOG_DIR + log, "r", encoding='utf-8') as logFile:
                        logdata = logFile.read()

            # Get submission grades (points)
            if os.path.exists(dirPath + RESULT_DIR + username + ".json"):
                with open(dirPath + RESULT_DIR + username + ".json", "r", encoding='utf-8') as resultFile:
                    resultJson = json.load(resultFile)

            if headers["CONTEST"] in resultJson:
                result = resultJson[headers["CONTEST"]]

        except Exception as e:
            success = False
            message = str(e)
            logdata = "Đã có lỗi xảy ra"
            result = -1
            print("/api/getsubmissioninfo" + str(e))

        json_response = {
            "success": success,
            "message": message,
            "log": logdata,
            "result": result
        }

    return json_response

def serve_website(path: str, response_headers):
    with open(path, "r", encoding='utf-8') as targetFile:
        response_body = targetFile.read()
        response_code = "200 OK"

        # with open(dirPath + "/source/" + WEBSITERULE_FILENAME) as websiteRuleFile:
        #     websiteRules = json.load(websiteRuleFile)
        #     if websiteRules["home"]:
        #         response_body = response_body.replace("%%home_display%%", "inline-block")
        #     else:
        #         response_body = response_body.replace("%%home_display%%", "none")
        #         print("No more home")
        #         if path == "/":
        #             response_headers.append(('Location', websiteRules["home_redirect"]))
        #             response_code = "301 Moved Permanently"
        #             return ['Trang web đã được di dời'.encode('utf-8')]
                
        #     if websiteRules["courses"]:
        #         response_body = response_body.replace("%%courses_display%%", "inline-block")
        #     else:
        #         response_body = response_body.replace("%%courses_display%%", "none")
        #         if path == "/courses":
        #             response_headers.append(('Location', "/"))
        #             response_code = "301 Moved Permanently"
        #             return ['Trang web đã được di dời'.encode('utf-8')]
            
        #     if websiteRules["credits"]:
        #         response_body = response_body.replace("%%credits_display%%", "inline-block")
        #     else:
        #         response_body = response_body.replace("%%credits_display%%", "none")
        #         if path == "/credits":
        #             response_headers.append(('Location', "/"))
        #             response_code = "301 Moved Permanently"
        #             return ['Trang web đã được di dời'.encode('utf-8')]
                
        #     if websiteRules["scoreboard"]:
        #         response_body = response_body.replace("%%scoreboard_display%%", "inline-block")
        #     else:
        #         response_body = response_body.replace("%%scoreboard_display%%", "none")
        #         if path == "/scoreboard":
        #             response_headers.append(('Location', "/"))
        #             response_code = "301 Moved Permanently"
        #             return ['Trang web đã được di dời'.encode('utf-8')]

        #     if websiteRules["forum"]:
        #         response_body = response_body.replace("%%forum_display%%", "inline-block")
        #     else:
        #         response_body = response_body.replace("%%forum_display%%", "none")
        #         if path == "/forum":
        #             response_headers.append(('Location', "/"))
        #             response_code = "301 Moved Permanently"
        #             return ['Trang web đã được di dời'.encode('utf-8')]

        with open(dirPath + STATSBAR_HTML, "r", encoding='utf-8') as statsbarfile:
            response_body = response_body.replace("%%statsbar%%", statsbarfile.read())

        with open(dirPath + "/source/" + ALIASES_FILENAME, "r", encoding="utf-8") as aliasfile:
            aliases = json.load(aliasfile)
            for aliaskey in aliases:
                response_body = response_body.replace('%%'+aliaskey+'%%', aliases[aliaskey])
                
        response_headers = [('Content-Type', 'text/html; charset=utf-8')]
    return response_body, response_headers, response_code

def application(environ, start_response):
    # Get the HTTP method (GET or POST)
    method = environ.get('REQUEST_METHOD', 'GET')

    # Get the path from the URL
    path = environ.get('PATH_INFO', '/')

    # Get the user's IP address
    ip_address = environ.get('REMOTE_ADDR', 'Unknown')

    # Extract the headers
    headers = {
        key[5:]: value
        for key, value in environ.items()
        if key.startswith('HTTP_')
    }

    # print(headers)

    # Extract the request body (if any)
    try:
        content_length = int(environ.get('CONTENT_LENGTH', 0))
    except (ValueError, TypeError):
        content_length = 0

    if content_length > 0:
        request_body = environ['wsgi.input'].read(content_length)
    else:
        request_body = None

    # Prepare the response headers
    response_headers = [('Content-Type', 'text/plain; charset=utf-8')]

    try:
        if path == "/debug":
            # Create the response body
            response_body = (f"Method: {method}\n"+f"Path: {path}\n"+f"IP Address: {ip_address}\n"+f"Headers: {headers}\n"+f"Body: {request_body}\n").encode('utf-8')
        elif path.startswith("/api/"):
            # If trying to contact API
            with open(dirPath+VERSION_JSON, "r", encoding='utf-8') as versionFile:
                api_version = json.load(versionFile)["api"]

            json_base_response = {"api_version": api_version}
            json_process_response = api_interface(path, headers, ip_address, request_body)
            json_base_response.update(json_process_response)
            response_body = json.dumps(json_base_response).encode('utf-8')
        else:
            # Check for reserved paths and handle them
            # if path in reservedPaths:
            #     # Block access to the folder itself
            #     if os.path.isdir(dirPath + "/www" + path):
            #         start_response('403 Forbidden', response_headers)
            #         return ['Không có quyền truy cập vào thư mục này.'.encode('utf-8')]
                
            #     # Handle specific reserved paths (like "/debug")
            #     start_response('403 Forbidden', response_headers)
            #     return ['Không có quyền truy cập vào trang này. Yêu cầu truy cập trực tiếp về mặt thể chất.'.encode('utf-8')]
            for reservedPath in reservedPaths:
                if path == reservedPath or path == reservedPath+"/":
                    if headers["USER_AGENT"].startswith("curl"):
                        start_response('403 Forbidden', response_headers)
                        return ['Không có quyền truy cập vào trang này. Yêu cầu truy cập trực tiếp về mặt thể chất.'.encode('utf-8')]
                    else:
                        response_headers = [('Content-Type', 'text/html; charset=utf-8')]
                        if os.path.exists(dirPath + ERROR_WEBFILE):
                            response_body, response_headers, response_code = serve_website(dirPath + ERROR_WEBFILE, response_headers)
                        else:
                            start_response('403 Forbidden', response_headers)
                            return ['Không có quyền truy cập vào trang này. Yêu cầu truy cập trực tiếp về mặt thể chất.'.encode('utf-8')]
                        start_response('403 Forbidden', response_headers)
                        response_body = response_body.replace("%%error_code%%", "403").replace("%%error_description%%", "Không có đủ quyền hạn truy cập vào trang web")
                        return [response_body.encode('utf-8')]

                elif path.startswith(reservedPath):
                    if headers["USER_AGENT"].startswith("curl"):
                        start_response('403 Forbidden', response_headers)
                        return ['Không có quyền truy cập vào dữ liệu thư mục này. Yêu cầu truy cập trực tiếp về mặt thể chất.'.encode('utf-8')]
                    else:
                        if os.path.exists(dirPath + ERROR_WEBFILE):
                            response_body, response_headers, response_code = serve_website(dirPath + ERROR_WEBFILE, response_headers)
                        else:
                            start_response('403 Forbidden', response_headers)
                            return ['Không có quyền truy cập vào trang này. Yêu cầu truy cập trực tiếp về mặt thể chất.'.encode('utf-8')]
                        start_response('403 Forbidden', response_headers)
                        response_body = response_body.replace("%%error_code%%", "403").replace("%%error_description%%", "Không có đủ quyền hạn truy cập vào thư mục")
                        return [response_body.encode('utf-8')]
            
            if os.path.exists(dirPath + "/www"+path+".html"):
                response_body, response_headers, response_code = serve_website(dirPath + "/www"+path+".html", response_headers)
                start_response(str(response_code), response_headers)
                return [response_body.encode('utf-8')]

            elif os.path.exists(dirPath + "/www" + path):
                mimetype, encoding = mimetypes.guess_type(path)
                if not mimetype:
                    mimetype = "text/plain"
                
                if mimetype and mimetype.startswith('text/'):
                    # Handle text files
                    try:
                        with open(dirPath + "/www" + path, "r", encoding='utf-8') as targetFile:
                            response_body = targetFile.read().encode('utf-8')
                            response_headers = [('Content-Type', f'{mimetype}; charset=utf-8')]
                    except UnicodeDecodeError:
                        # If we can't decode as text, fall back to binary handling
                        with open(dirPath + "/www" + path, "rb") as targetFile:
                            response_body = targetFile.read()
                            response_headers = [('Content-Type', f'{mimetype}')]
                else:
                    # Handle binary files
                    with open(dirPath + "/www" + path, "rb") as targetFile:
                        response_body = targetFile.read()
                    response_headers = [('Content-Type', f'{mimetype or "application/octet-stream"}')]

            # Now, userprofile paths
            # This will be called on when the user IS trying to look at a profile
            elif path.startswith("/user/"):
                # Good, now the user is trying to look at a user
                username = path[6:]
                # print(username)

                # Now we got the username, we gather the informations specifically about THIS user
                try:
                    with open(dirPath+USERDATA_JSON, "r", encoding='utf-8') as userFile:
                        user = json.load(userFile)[username]

                    if os.path.exists(dirPath+USERPROFILE_WEBFILE):
                        response_body, response_headers, response_code = serve_website(dirPath+USERPROFILE_WEBFILE, response_headers)
                    else:
                        start_response('404 Not Found', response_headers)
                        return ['Không tìm thấy người dùng.'.encode('utf-8')]
                    start_response(response_code, response_headers)

                    response_body = response_body.replace("%%name%%", user["fullname"]).replace("%%codename%%", username).replace("%%desc%%", user["desc"])
                    #.replace("%%class%%", user["class"]).replace("%%fullclass%%", dcls["longname"])

                    # Because there are many classes in one user, we might as well just add them in one by one
                    # But first, we need the classes variable
                    with open(dirPath+CLASSFILE_PATH, "r", encoding='utf-8') as classFile:
                        classes = json.load(classFile)
                    
                    # Now, we run one by one in user's classes
                    fullstring = ""
                    for _class in user["class"]:
                        # Get the full name of class
                        fullname = classes[_class]["longname"]
                        fullstring += f"<p class=\"class\">{_class} - {fullname}</p>"

                    response_body = response_body.replace("%%class%%", fullstring)

                    # Checking if user has custom profile picture
                    if user["picture"]:
                        response_body = response_body.replace("%%usericon%%", str("/userpictures/" + username + ".jpg"))
                        # print("Y")
                    else:
                        # print("F")
                        response_body = response_body.replace("%%usericon%%", DEFAULTUSER_IMAGE)

                    response_body = response_body.encode('utf-8')
                    response_headers = [('Content-Type', 'text/html; charset=utf-8')]

                    return [response_body] 
                    # start_response('200 OK', response_headers)

                except Exception as e:
                    print(e)
                    print("USER ERRROR",str(e))
                    if os.path.exists(dirPath+ERROR_WEBFILE):
                        response_body, response_headers, response_code = serve_website(dirPath+ERROR_WEBFILE, response_headers)
                    else:
                        start_response('500 Internal Server Error', response_headers)
                        return ['Lỗi trong quá trình tìm kiếm người dùng.'.encode('utf-8')]
                    start_response(response_code, response_headers)
                    response_body = response_body.replace("%%error_code%%", "500").replace("%%error_description%%", "Đã có lỗi xảy ra trong quá trình tìm kiếm người dùng.")
                    return [response_body.encode('utf-8')]
            elif path.startswith("/contest/"):
                targetContestName = path[9:]
                with open(dirPath + CONTESTS_JSON, "r", encoding='utf-8') as contestFile:
                    contests = json.load(contestFile)

                if targetContestName in contests:
                    # If the contest exists, we return the data of the contest in the form of a webpage
                    contestData = contests[targetContestName]

                    response_headers = [('Content-Type', 'text/html; charset=utf-8')]
                    start_response("200 OK", response_headers)
                    response_body, response_headers, response_code = serve_website(dirPath+CONTEST_WEBFILE, response_headers)
                    response_body = response_body.replace("%%contest_name%%", targetContestName).replace("%%contest_desc%%", contestData["Desc"])
                    return [response_body.encode('utf-8')]
                else:
                    if headers["USER_AGENT"].startswith("curl"):
                        start_response('404 Not Found', response_headers)
                        return ['Không tìm thấy bài thi.'.encode('utf-8')]
                    else:
                        response_headers = [('Content-Type', 'text/html; charset=utf-8')]
                        start_response('404 Not Found', response_headers)
                        if os.path.exists(dirPath+ERROR_WEBFILE):
                            response_body, response_headers, response_code = serve_website(dirPath+ERROR_WEBFILE, response_headers)
                            response_body = response_body.replace("%%error_code%%", "404").replace("%%error_description%%", "Không tìm thấy bài thi")
                            return [response_body.encode('utf-8')]
                        else:
                            return ['Không tìm thấy bài thi.'.encode('utf-8')]
            else:
                if headers["USER_AGENT"].startswith("curl"):
                    start_response('404 Not Found', response_headers)
                    return ['Không tìm thấy địa chỉ.'.encode('utf-8')]
                else:
                    response_headers = [('Content-Type', 'text/html; charset=utf-8')]
                    start_response('404 Not Found', response_headers)
                    if os.path.exists(dirPath+ERROR_WEBFILE):
                        response_body, response_headers, response_code = serve_website(dirPath+ERROR_WEBFILE, response_headers)
                        response_body = response_body.replace("%%error_code%%", "404").replace("%%error_description%%", "Không tìm thấy địa chỉ")
                        return [response_body.encode('utf-8')]
                    else:
                        return ['Không tìm thấy địa chỉ.'.encode('utf-8')]
    except Exception as e:
        print(e)
        print("Unidentified Error: " + str(e))
        traceback.print_exc()
        if headers["USER_AGENT"].startswith("curl"):
            start_response('500 Internal Server Error', response_headers)
            return ['Đã có lỗi xảy ra trong hệ thống máy chủ, vui lòng báo lại với quản trị viên của trang Web.'.encode('utf-8')]
        else:
            response_headers = [('Content-Type', 'text/html; charset=utf-8')]
            if os.path.exists(dirPath+ERROR_WEBFILE):
                start_response('500 Internal Server Error', response_headers)
                response_body, response_headers, response_code = serve_website(dirPath+ERROR_WEBFILE, response_headers)
            else:
                start_response('500 Internal Server Error', response_headers)
                return ['Lỗi hệ thống.'.encode('utf-8')]
            # start_response(response_code, response_headers)
            response_body = response_body.replace("%%error_code%%", "500").replace("%%error_description%%", "Đã có lỗi xảy ra trong hệ thống máy chủ.")
            return [response_body.encode('utf-8')]

    # Send the response status and headers
    start_response('200 OK', response_headers)
    # Return the response body as a list of bytes
    return [response_body]
