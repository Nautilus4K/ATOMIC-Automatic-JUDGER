# aigen.py
# script to generate AI test cases
# Let's fucking gooooo
# To use this. We need a way to convey whatever the fuck the user wants. And to do that socket might just be the one
import socket
# import threading
import time
import os
# import datetime
from ollama import chat
from ollama import ChatResponse
from bs4 import BeautifulSoup

dirPath = os.path.dirname(os.path.abspath(__file__))

HOST = "127.0.0.1"
PORT = 28474

CONNECTED = False # A connection has been established
# EXIT_SIGN = False # This is when THE connection is in and it says we have to say goodbye and terminate it all

def parse_response(response: str):
    pre_start = response.find("```")
    start = response.find("\n", pre_start)
    # print(start)
    end = response.find("```", start + 3)

    return response[start:end]

def listener():
    global CONNECTED
    CONNECTED = False
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.bind((HOST, PORT))
            s.listen(1) # Listen on HOST:PORT. Currently 127.0.0.1:28474. We just need 1 request because you know.
            print(f"Listening on {HOST}:{PORT} with maximum 1 requests")

            conn, addr = s.accept()
            with conn:
                print("Connected.")
                CONNECTED = True
                try:
                    # Receive once only
                    data = conn.recv(8192) # Should be good enough
                    if not data or data == b"exit":
                        print("No data received or exit command issued.")
                        return

                    soup = BeautifulSoup(data.decode('utf-8'), "html.parser")
                    text = soup.get_text(separator=" ", strip=True)

                    print("Received:", text)

                    with open("ollama_sysprompt.txt", "r", encoding='utf-8') as f:
                        sysprompt = f.read()

                    stream = chat(
                        model='llama3.1:8b',
                        messages=[
                            {'role': 'system', 'content': sysprompt},
                            {'role': 'user', 'content': text}
                        ],
                        stream=True,
                    )

                    full_response = ""

                    for chunk in stream:
                        conn.sendall(chunk['message']['content'].encode('utf-8'))
                        full_response += chunk['message']['content']
                    conn.sendall("\x03".encode('utf-8'))

                    # print(full_response)
                    # Parse this response
                    print(parse_response(full_response))

                except Exception as e:
                    print(f"Error during connection handling: {e}")
                finally:
                    conn.close()
        except Exception as e:
            print(f"Error setting up the socket: {e}")
        finally:
            s.close()

if __name__ == "__main__":
    listener()
    exit(0)