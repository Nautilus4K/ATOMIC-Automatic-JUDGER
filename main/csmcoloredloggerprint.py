import time
import datetime

def pinfo(content: str):
    print(f"\x1b[0m[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [INFO] {content}\x1b[0m")

def perr(content: str):
    print(f"\x1b[31m[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [ERROR] {content}\x1b[0m")

def pwarn(content: str):
    print(f"\x1b[33m[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [WARNING] {content}\x1b[0m")

def pok(content: str):
    print(f"\x1b[32m[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [SUCCESS] {content}\x1b[0m")