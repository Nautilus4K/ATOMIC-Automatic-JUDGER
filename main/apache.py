# This script is built to run the WSGI Interface. This does not have any
# affliation with the Apache HTTP Server Project or anthing related but
# a way to run WSGI Interface using waitress.

# ERROR CODES (Exit codes)
# ---------------------------------------
# -74  :  Socket opening error

# Nickname? Hell yea
# Valkyrie.

from waitress import serve
from wsgi_interface import application

import socket
import multiprocessing
import traceback

# Run Waitress in a separate PROCESS instead of a thread
def httpInterface():
    serve(application, host='0.0.0.0', port=80, ident="Valkyrie", url_scheme="https")

if __name__ == "__main__":
    httpProcess = multiprocessing.Process(target=httpInterface, daemon=True)
    httpProcess.start()

    HOST = "127.0.0.1"  # Local only
    PORT = 28473       # Custom control port
    running = True

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        try:
            server.bind((HOST, PORT))
            print(f"Opened Socket at {HOST}:{PORT}")
        except Exception as e:
            print(f"Error opening socket at {HOST}:{PORT}: {e}")
            traceback.print_exc()
            exit(1)
        server.listen(1)
        print("Now opened socket with capacity of 1 concurrent connection.")

        while running:
            try:
                conn, addr = server.accept()
                with conn:
                    print("Connected with a target. Awaiting instructions...")
                    data = conn.recv(1024).decode().strip()
                    if data == "exit":
                        print("Stopped by communication interface. Shutting down waitress...")
                        httpProcess.terminate()  # Stop the Waitress process
                        httpProcess.join()
                        print("Successful.")
                        break
            except Exception as e:
                print(f"SOCKET error: {e}")
                traceback.print_exc()
                running = False
                break
