import os
import mimetypes
import json

# Variables
ALIASES_FILENAME = "aliases.json"
ERROR_WEBFILE = "/www/reserved/error.html"

dirPath = os.path.dirname(os.path.abspath(__file__))
reservedPaths = ["/debug", "/reserved"]
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
                        start_response('403 Forbidden', response_headers)
                        with open(dirPath + ERROR_WEBFILE, "r", encoding='utf-8') as errorNotify:
                            response_body = errorNotify.read().replace("%%error_code%%", "403").replace("%%error_description%%", "Không có đủ quyền hạn truy cập vào trang web")
                            return [response_body.encode('utf-8')]

                elif path.startswith(reservedPath):
                    if headers["USER_AGENT"].startswith("curl"):
                        start_response('403 Forbidden', response_headers)
                        return ['Không có quyền truy cập vào dữ liệu thư mục này. Yêu cầu truy cập trực tiếp về mặt thể chất.'.encode('utf-8')]
                    else:
                        response_headers = [('Content-Type', 'text/html; charset=utf-8')]
                        start_response('403 Forbidden', response_headers)
                        with open(dirPath + ERROR_WEBFILE, "r", encoding='utf-8') as errorNotify:
                            response_body = errorNotify.read().replace("%%error_code%%", "403").replace("%%error_description%%", "Không có đủ quyền hạn truy cập vào thư mục")
                            return [response_body.encode('utf-8')]
            
            if os.path.exists(dirPath + "/www"+path+"/index.html"):
                with open(dirPath + "/www"+path+"/index.html", "r", encoding='utf-8') as targetFile:
                    response_body = targetFile.read()
                    with open(dirPath + "/source/" + ALIASES_FILENAME, "r", encoding="utf-8") as aliasfile:
                        aliases = json.load(aliasfile)
                        for aliaskey in aliases:
                            response_body = response_body.replace('%%'+aliaskey+'%%', aliases[aliaskey])
                    response_body = response_body.encode('utf-8')
                    response_headers = [('Content-Type', 'text/html; charset=utf-8')]

            elif os.path.exists(dirPath + "/www"+path):
                mimetype, encoding = mimetypes.guess_type(path)
                if not mimetype:
                    mimetype = "text/plain"
                if mimetype and mimetype.startswith('text/'):
                    with open(dirPath + "/www"+path, "r", encoding='utf-8') as targetFile:
                        response_body = targetFile.read().encode('utf-8')
                        response_headers = [('Content-Type', f'{mimetype}; charset=utf-8')]
                else:
                    # Serve binary files
                    with open(dirPath + "/www"+path, "rb") as targetFile:
                        response_body = targetFile.read()
                    response_headers = [('Content-Type', f'{mimetype or "application/octet-stream"}')]
                    
            else:
                if headers["USER_AGENT"].startswith("curl"):
                    start_response('404 Not Found', response_headers)
                    return ['Không tìm thấy địa chỉ.'.encode('utf-8')]
                else:
                    response_headers = [('Content-Type', 'text/html; charset=utf-8')]
                    start_response('404 Not Found', response_headers)
                    with open(dirPath + ERROR_WEBFILE, "r", encoding='utf-8') as errorNotify:
                        response_body = errorNotify.read().replace("%%error_code%%", "404").replace("%%error_description%%", "Không tìm thấy địa chỉ")
                        return [response_body.encode('utf-8')]
    except Exception as e:
        print(e)
        print(str(e))
        if headers["USER_AGENT"].startswith("curl"):
            start_response('500 Internal Server Error', response_headers)
            return ['Đã có lỗi xảy ra trong hệ thống máy chủ, vui lòng báo lại với quản trị viên của trang Web.'.encode('utf-8')]
        else:
            response_headers = [('Content-Type', 'text/html; charset=utf-8')]
            start_response('500 Internal Server Error', response_headers)
            with open(dirPath + ERROR_WEBFILE, "r", encoding='utf-8') as errorNotify:
                response_body = errorNotify.read().replace("%%error_code%%", "500").replace("%%error_description%%", "Đã có lỗi xảy ra trong hệ thống máy chủ, vui lòng báo lại với quản trị viên của trang Web.")
                return [response_body.encode('utf-8')]

    # Send the response status and headers
    start_response('200 OK', response_headers)
    # Return the response body as a list of bytes
    return [response_body]
