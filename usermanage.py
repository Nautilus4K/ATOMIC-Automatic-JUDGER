import sqlite3
import os
from hashlib import sha256

try:
    from prettytable import PrettyTable
except ImportError:
    os.system("pip install prettytable")
    from prettytable import PrettyTable

filePath = os.path.dirname(os.path.abspath(__file__))
db_name = "data.db"
try:
    db = sqlite3.connect(filePath+db_name)
except Exception as e:
    print("Oh hell nah. Error occured")
    print(str(e))
    exit()

cursor = db.cursor()

# Create a table for users. This will be an SQLite database designed for storing userdata (EXCLUDING ADMINISTRATOR ACCOUNTS).
cursor.execute('''
    CREATE TABLE IF NOT EXISTS users (
        username TEXT NOT NULL UNIQUE,
        password TEXT NOT NULL,
        name TEXT,
        date_created INTEGER,
        phone TEXT
    )
''')
