import wsgiref.simple_server

from wsgi_interface import *

# Define the host and port to serve the application
host = "localhost"
port = 8000

# Create a WSGI server and run the application
with wsgiref.simple_server.make_server(host, port, application) as server:
    print(f"Serving on http://{host}:{port} ...")
    try:
        # Serve until a keyboard interruption occurs
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nServer stopped.")
        