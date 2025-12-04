# This script is built to run the WSGI Interface. This does not have any
# affliation with the Apache HTTP Server Project or anthing related but
# a way to run WSGI Interface using waitress.

# ERROR CODES (Exit codes)
# ---------------------------------------
# -74  :  Socket opening error

# Setting I/O
import os
import sys
import time
import datetime
dirPath = os.path.dirname(os.path.abspath(__file__))
LOG_PATH_REL = "/central/valkyrie.log"

log_path = dirPath + LOG_PATH_REL  # Or any custom path

existedPrior = os.path.exists(log_path)

log_file = open(log_path, "a", encoding="utf-8")

if not existedPrior:
    log_file.write("      ::::    :::     :::     :::    ::: ::::::::::: ::::::::::: :::       :::    :::  ::::::::      :::     :::    ::: \n     :+:+:   :+:   :+: :+:   :+:    :+:     :+:         :+:     :+:       :+:    :+: :+:    :+:    :+:      :+:   :+:   \n    :+:+:+  +:+  +:+   +:+  +:+    +:+     +:+         +:+     +:+       +:+    +:+ +:+          +:+ +:+   +:+  +:+     \n   +#+ +:+ +#+ +#++:++#++: +#+    +:+     +#+         +#+     +#+       +#+    +:+ +#++:++#++  +#+  +:+   +#++:++       \n  +#+  +#+#+# +#+     +#+ +#+    +#+     +#+         +#+     +#+       +#+    +#+        +#+ +#+#+#+#+#+ +#+  +#+       \n #+#   #+#+# #+#     #+# #+#    #+#     #+#         #+#     #+#       #+#    #+# #+#    #+#       #+#   #+#   #+#       \n###    #### ###     ###  ########      ###     ########### ########## ########   ########        ###   ###    ###   \n")
    log_file.write("\nMade with Pylons/waitress. Put together by Nautilus4K with love 💖.\n")

log_file.write(f"\n\n\n------------------------------\nEXEC DATE: {datetime.datetime.now()}\n\n") # Writing headers for each section

# Redirect standard output and error to log file
# sys.stdout = log_file
# sys.stderr = log_file

# Doing prior cleaning
import json
SETTINGS_PATH = "/source/settings.json"
SESSIONS_PATH = "/central/sessions.json"

with open(dirPath + SETTINGS_PATH, "r", encoding='utf-8') as settingsFile:
    settings = json.load(settingsFile)

with open(dirPath + SESSIONS_PATH, "r", encoding='utf-8') as sessionsFile:
    sessions = json.load(sessionsFile)

awaiting_removal = []
for session in sessions:
    # If last active is too high
    if (int(time.time()) - sessions[session]["lastactive"]) > settings["max_not_logged_in_session_seconds"]:
        awaiting_removal.append(session)

for removableSession in awaiting_removal:
    del sessions[removableSession]

with open(dirPath + SESSIONS_PATH, "w", encoding='utf-8') as sessionsFile:
    json.dump(sessions, sessionsFile)

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
