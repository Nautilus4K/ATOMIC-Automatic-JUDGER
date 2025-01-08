import sqlite3
import os
import time
from hashlib import sha256

try:
    from prettytable import PrettyTable
except ImportError:
    os.system("pip install prettytable")
    from prettytable import PrettyTable

filePath = os.path.dirname(os.path.abspath(__file__))
db_name = "data.db"
try:
    db = sqlite3.connect(filePath+"/"+db_name)
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
        desc TEXT,
        class TEXT NOT NULL
    )
''')

# Retrieve and display data using PrettyTable
cursor.execute('SELECT * FROM users')
rows = cursor.fetchall()

# Print the data in a pretty table
table = PrettyTable()
table.field_names = ["Username", "Password", "Name", "Date created", "Description", "Class"]

###########################################
#   +--------------------------------+    #
#   | Add rows to the PrettyTable to |    #
#   |     display out with STYLE     |    #
#   +--------------------------------+    #
###########################################

maxcharlimit = 20 # Limit to make sure the text don't go too long.
                  # Applied so that hash strings and description
                  # strings don't go overboard
for row in rows:
    # Assuming row is a tuple
    row = tuple(str(item[:maxcharlimit-3]) + "..." if len(str(item)) > maxcharlimit else item for item in row)
    table.add_row(row)

# Display the table
print(table)

nusername=str(input("Enter new username (BLANK IF IGNORE): "))
if (nusername != ""):
    npasswd = sha256(str(input("Enter password: ")).encode("utf-8")).hexdigest() # Hash the password so that its extra private
    nname = str(input("Enter new name (BLANK IF NULL): "))
    if nname == "": nname = None
    
    ndesc = str(input("Enter description (BLANK IF NULL): "))
    if ndesc == "": ndesc = None

    nclass = str(input("Enter class: "))

    data = (nusername, npasswd, nname, int(time.time()), ndesc, nclass)

    # Insert data into the table
    cursor.execute('''
        INSERT OR IGNORE INTO users (username, password, name, date_created, desc, class)
        VALUES (?, ?, ?, ?, ?, ?)
    ''', data)

    db.commit()
