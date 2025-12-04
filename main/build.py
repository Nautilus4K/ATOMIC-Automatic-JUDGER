# build.py
# Code to build this kinda thing
from rich.console import Console
import subprocess
import platform
import json
import os

BUILDNAME = "+ ATOMIC +"

console = Console()
def prstat(text: str, formattedText: str = ""):
    # rich.print(f"[blue]{BUILDNAME}[/blue] //", text)
    console.print(f"[blue bold]{BUILDNAME}[/blue bold] [magenta]|[/magenta]", end=" ")
    print(text, end="")
    if formattedText != "":
        console.print(formattedText)
    else:
        print()

osStr = platform.platform()
isWindows = osStr.startswith("Windows")

# Constants definitions
if isWindows:
    # print("Windows detected.")
    prstat(f"Windows detected. Platform: {osStr}")
    # With the main things done let's do this!
    CXX = "C:/Qt/Tools/mingw1310_64/bin/g++.exe"
    CXXFLAGS = "-fdiagnostics-color=always -Wall -O2 -std=c++17 -fPIC -mwindows"
    LDFLAGS = "-L/Qt/Tools/mingw1310_64/lib -L/Qt/6.10.1/mingw_64/lib -L/msys64/mingw64/lib -lxlnt -lQt6Widgets -lQt6Core -lQt6Gui -lssl -lcrypto -lpthread -lws2_32"
    INCLUDES = "-I./include -I. -I/msys64/mingw64/include -I/Qt/6.10.1/mingw_64/include -I/Qt/Tools/mingw1310_64/include"
    WINDEPLOYQT = "/Qt/6.10.1/mingw_64/bin/windeployqt.exe"
    RCC = "/Qt/6.10.1/mingw_64/bin/rcc.exe"
    MOC = "/Qt/6.10.1/mingw_64/bin/moc.exe"

    TARGET = "panel.exe"
else:
    print(f"Linux/macOS detected. Platform: {osStr}")
    CXX = "g++"
    CXXFLAGS = "-fdiagnostics-color=always -Wall -O2 -std=c++17 -fPIC"
    LDFLAGS = "-lxlnt -lQt6Widgets -lQt6Core -lQt6Gui -lssl -lcrypto -lpthread"
    INCLUDES = "-I./include -I. -I."
    WINDEPLOYQT = "" # Let's be real it doesn't exist
    RCC = "/usr/lib/qt6/rcc"
    MOC = "/usr/lib/qt6/moc"

    TARGET = "panel"

# Constants definitions (same on all platforms)
TARGETFOLDER = "./bin"

# Build targets (unchanging despite platform differences)
OBJECTS = [
    "panel.o", "timezone_stub.o", "qres.o",
    "CST_Listing.o", "moc_CST_Listing.o",
    "CST_PlainTextDialog.o", "moc_CST_PlainTextDialog.o",
    "CST_RadioButtonDialog.o", "moc_CST_RadioButtonDialog.o",
    "CST_RichTextEdit.o", "moc_CST_RichTextEdit.o",
    "CST_Separator.o", "moc_CST_Separator.o",
    "CST_TestCaseDialog.o", "moc_CST_TestCaseDialog.o",
    "CST_TextEditorDialog.o", "moc_CST_TextEditorDialog.o",
    "WIN_ClassesSettings.o", "moc_WIN_ClassesSettings.o",
    "WIN_ContestsSettings.o", "moc_WIN_ContestsSettings.o",
    "WIN_UsersSettings.o", "moc_WIN_UsersSettings.o",
    "WIN_GenerateTestCasesDialog.o", "moc_WIN_GenerateTestCasesDialog.o",
]

# Build actions for each targets
PASSED_OBJS = {}
ACTIONS = {}

CONSOLE_SIZE = os.get_terminal_size()
CONSOLE_COLS = CONSOLE_SIZE.columns
CONSOLE_LINES = CONSOLE_SIZE.lines

# Main actions
if __name__ == "__main__":
    prstat("Build target: ", f"[green]{TARGET}[/green]")

    prstat(f"Build objects: ")
    toBePrintedLine = ""
    for obj in OBJECTS:
        if len(toBePrintedLine) + len(obj) + 2 <= CONSOLE_COLS:
            toBePrintedLine += obj + ", "
        elif len(obj) + 2 > CONSOLE_COLS:
            toBePrintedLine += obj + ", "
        else:
            console.print(toBePrintedLine, style="dim")
            toBePrintedLine = obj + ", "

    prstat("Commencing build process...")
    for obj in OBJECTS:
        prstat("Building object: ", f"[green]{obj}[/green]")

        PASSED_OBJS[obj] = True

        # Type in the parameters
        try:
            reqs = str(input("Requirements for this object (split by spaces): ")).split(" ")
        except KeyboardInterrupt:
            console.print("[red]Aborting...[/red]")
            exit(0)

        # So much for this
        print("Commands to compile this build object (END to end command typing. Enter to write new command):")
        cmds = []
        lineNum = 0
        while True:
            lineNum += 1
            lineNumDisplay = "0" + str(lineNum) if len(str(lineNum)) < 2 else str(lineNum)

            console.print(f"[white dim italic]{lineNumDisplay}.[/white dim italic] ", end="")

            try:
                cmd = str(input())
            except KeyboardInterrupt:
                console.print("[red]Aborting...[/red]")
                exit(0)

            if cmd == "END": break
            
            if cmd != "":
                cmds.append(cmd)
            else: lineNum -= 1

        console.print(f"{len(cmds)} lines.")

        ACTIONS[obj] = {
            "requirements": reqs,
            "commands": cmds
        }

    console.print_json(json.dumps(ACTIONS))