def application(environ, start_response):
    # Get the HTTP method (GET or POST)
    method = environ.get('REQUEST_METHOD', 'GET')

    # Get the path from the URL
    path = environ.get('PATH_INFO', '/')

    # Get the user's IP address
    ip_address = environ.get('REMOTE_ADDR', 'Unknown')

    # Prepare the response headers
    headers = [('Content-Type', 'text/plain')]

    # Create the response body
    response_body = f"Method: {method}\nPath: {path}\nIP Address: {ip_address}\nHeaders: {headers}\n"

    # Encode the response body into bytes
    response_body_bytes = response_body.encode('utf-8')

    # Send the response status and headers
    start_response('200 OK', headers)

    # Return the response body as a list of bytes
    return [response_body_bytes]
