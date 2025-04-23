import os
import json
from hashlib import sha256

def printctr(content: str):
    """
    Like Python's `print()` function, but print in the center of screen
    """
    termsize = os.get_terminal_size()
    content_length = len(content)
    content_startpos = int((termsize[0] - content_length)/2)
    for i in range(content_startpos):
        print(" ", end='')

    print(content)

dirPath = os.path.dirname(os.path.abspath(__file__))
CLASSFILE_PATH = "/source/classes.json"
USERFILE_PATH = "/source/users.json"

if __name__ == "__main__":
    with open(dirPath+CLASSFILE_PATH, "r", encoding='utf-8') as classFile:
        classdata = json.load(classFile)

    printctr(">>>> CLASSES <<<<")

    for cla in classdata:
        print("Codename:", cla)
        print("- Shortname (Useless)", classdata[cla]["shortname"])
        print("- Fullname:", classdata[cla]["longname"])

    print("\n")
    nclass = str(input("Enter new class (BLANK IF IGNORE): "))
    if nclass != "":
        print("Class detected. Proceeding...")
        if len(nclass) > 6: print("[WARNING]: Class codename too long.")
        if nclass in classdata:
            print("Class already exists. Aborting...")
        else:
            print("Class doesn't exist. Proceeding...")
            shortname = str(input("Enter a short name (BLANK IF SAME AS CODENAME): "))
            if shortname == "":
                print("Did not enter one. Defaulting to codename...")
                shortname = nclass
            
            longname = str(input("Enter a long name (BLANK IF SAME AS CODENAME): "))
            if longname == "":
                print("Did not enter one. Defaulting to codename...")
                longname = nclass
            
        print(f"\nCodename: {nclass}\nShort name: {shortname}\nLong name: {longname}")
        classdata[nclass] = {
            "shortname": shortname,
            "longname": longname
        }

    with open(dirPath+CLASSFILE_PATH, "w", encoding='utf-8') as classFile:
        json.dump(classdata, classFile)

    print("\n\n")

    with open(dirPath+USERFILE_PATH, "r", encoding='utf-8') as userFile:
        userdata = json.load(userFile)

    # Print out the information of EACH user
    printctr(">>>> PRIVILLEGE SYSTEM <<<<")
    print("+-----------------------------------------------+")
    print("|[2] Administrator (Hardcoded) - Full Read/Write|")
    print("|[1] Teacher                   - Full Read      |")
    print("|[0] Student                   - Limited Read   |")
    print("+-----------------------------------------------+")
    print("\n\n")

    printctr(">>>> USERS <<<<")
    for user in userdata:
        print(user)
        print("- Password (Hashed):", userdata[user]["password"])
        print("- Full name:", userdata[user]["fullname"])
        print("- Class:", userdata[user]["class"])
        print("- Description:\n----------------------------------\n" + str(userdata[user]["desc"]))
        print("----------------------------------")
        print("- Custom profile picture?", userdata[user]["picture"])

        # This is how privillege level will work:
        # ----------------------------------------------------
        # 2 - Administrator (Only in hardcoded accounts) - FULL READ/WRITE
        # 1 - Teacher - NO WRITE, FULL READ
        # 0 - Standard students - NO WRITE, LIMITED READ
        # ----------------------------------------------------
        print("- Privillege level:", userdata[user]["priv"])
        print("\n")

    username = str(input("Enter username (BLANK IF IGNORE): "))
    if username != "":
        print("Username detected. Proceeding...")
        password = str(sha256(str(input("Enter password (ASCII): ")).encode('ascii')).hexdigest())
        print("Hashed Password:", password)
        fullname = str(input("Full name: "))
        print("Classes")
        with open(dirPath+CLASSFILE_PATH, "r", encoding='utf-8') as classFile:
            classdata = json.load(classFile)

            classes = []
            index = 0
            for ncsl in classdata:
                print(index, "-", ncsl)
                classes.append(ncsl)
                index+=1
        classindex = str(input("Enter class index (separated by space): "))
        classIndexes = classindex.split(' ')
        resultIndexes = []
        print("Selected classes: ", end="")
        for CLSindex in classIndexes:
            CLSindex = int(CLSindex)
            if CLSindex < index and CLSindex >= 0:
                print(classes[CLSindex], end=", ")
                resultIndexes.append(classes[CLSindex])
            else:
                print("Error: OUT OF BOUNDS")
                exit()
        print("")
        
        desc = str(input("Description: "))
        priv = int(input("Privillege level: "))

        userdata[username] = {
            "password": password,
            "fullname": fullname,
            "class": resultIndexes,
            "desc": desc,
            "picture": False,
            "priv": priv
        }

    with open(dirPath+USERFILE_PATH, "w", encoding='utf-8') as userFile:
        json.dump(userdata, userFile)
