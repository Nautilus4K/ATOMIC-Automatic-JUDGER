import os
import mimetypes
import json
import random
import time
import string
from hashlib import sha256

# Variables
ALIASES_FILENAME = "aliases.json"
WEBSITERULE_FILENAME = "websites.json"
ERROR_WEBFILE = "/www/reserved/error.html"
USERPROFILE_WEBFILE = "/www/reserved/userprofile.html"
VERSION_JSON = "/source/version.json"
USERDATA_JSON = "/source/users.json"
LASTACCESS_JSON = "/central/lastaccess.json"
SESSION_JSON = "/central/sessions.json"
STATSBAR_HTML = "/www/reserved/statsbar.html"

dirPath = os.path.dirname(os.path.abspath(__file__))
reservedPaths = ["/debug", "/reserved"]

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

def api_interface(path, headers, ip_addr):
    # print("API called from ANONYMOUS DEVICE")

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
        request_body = environ['wsgi.input'].read(content_length).decode('utf-8')
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
            json_process_response = api_interface(path, headers, ip_address)
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
                from usermanage import USERFILE_PATH, CLASSFILE_PATH
                try:
                    with open(dirPath+USERFILE_PATH, "r", encoding='utf-8') as userFile:
                        user = json.load(userFile)[username]
                    with open(dirPath+CLASSFILE_PATH, "r", encoding='utf-8') as classFile:
                        dcls = json.load(classFile)[user["class"]]

                    if os.path.exists(dirPath+USERPROFILE_WEBFILE):
                        response_body, response_headers, response_code = serve_website(dirPath+USERPROFILE_WEBFILE, response_headers)
                    else:
                        start_response('404 Not Found', response_headers)
                        return ['Không tìm thấy người dùng.'.encode('utf-8')]
                    start_response(response_code, response_headers)

                    response_body = response_body.replace("%%name%%", user["fullname"]).replace("%%codename%%", username).replace("%%class%%", user["class"]).replace("%%desc%%", user["desc"]).replace("%%fullclass%%", dcls["longname"])
                    response_body = response_body.encode('utf-8')
                    response_headers = [('Content-Type', 'text/html; charset=utf-8')]

                    return [response_body] 
                    # start_response('200 OK', response_headers)

                except Exception as e:
                    print(e)
                    print(str(e))
                    if os.path.exists(dirPath+ERROR_WEBFILE):
                        response_body, response_headers, response_code = serve_website(dirPath+ERROR_WEBFILE, response_headers)
                    else:
                        start_response('500 Internal Server Error', response_headers)
                        return ['Không tìm thấy người dùng.'.encode('utf-8')]
                    start_response(response_code, response_headers)
                    response_body = response_body.replace("%%error_code%%", "500").replace("%%error_description%%", "Đã có lỗi xảy ra trong quá trình tìm kiếm người dùng.")
                    return [response_body.encode('utf-8')]
                    
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
        print(str(e))
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
