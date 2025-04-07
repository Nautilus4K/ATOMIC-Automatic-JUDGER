import time
import datetime

# INFO_COL = "\x1b[0m"
# ERROR_COL = "\x1b[31m"
# WARN_COL = "\x1b[33m"
# OK_COL = "\x1b[32m"
# RESET_COL = "\x1b[0m"
INFO_COL = "\x1b[0m"
ERROR_COL = "\x1b[31m"
WARN_COL = "\x1b[33m"
OK_COL = "\x1b[32m"
RESET_COL = "\x1b[0m"

def pinfo(content: str):
    print(f"{INFO_COL}[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [INFO] {content}{RESET_COL}", flush=True)

def perr(content: str):
    print(f"{ERROR_COL}[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [ERROR] {content}{RESET_COL}", flush=True)

def pwarn(content: str):
    print(f"{WARN_COL}[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [WARNING] {content}{RESET_COL}", flush=True)

def pok(content: str):
    print(f"{OK_COL}[{datetime.datetime.fromtimestamp(int(time.time())).strftime('%H:%M:%S %Y-%m-%d')}] [SUCCESS] {content}{RESET_COL}", flush=True)