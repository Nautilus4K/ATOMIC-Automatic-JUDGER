# build.py
# Code to build this kinda thing
from rich.console import Console
import platform
import json
import shutil
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

    ADDITIONALLIBS = [
        "C:/msys64/mingw64/bin/libcrypto-3-x64.dll",
        "C:/msys64/mingw64/bin/libxlnt.dll"
    ]

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

    ADDITIONALLIBS = []

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
ACTIONS = {
    "panel.o": {
        "requirements": ["panel.cpp", "utilities.h", "consts.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c panel.cpp -o panel.o"
        ]
    },
    "timezone_stub.o": {
        "requirements": ["timezone_stub.c"],
        "commands": [
            f"{CXX} {CXXFLAGS} -c timezone_stub.c -o timezone_stub.o"
        ]
    },
    "qres.o": {
        "requirements": ["qres.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} -c qres.cpp -o qres.o"
        ]
    },
    "qres.cpp": {
        "requirements": ["qres.qrc"],
        "commands": [
            f"{RCC} -name qres -o qres.cpp qres.qrc"
        ]
    },

    # CST_Listing
    "CST_Listing.o": {
        "requirements": ["CST_Listing.cpp", "CST_Listing.h", "CST_PlainTextDialog.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c CST_Listing.cpp -o CST_Listing.o"
        ]
    },
    "moc_CST_Listing.o": {
        "requirements": ["moc_CST_Listing.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_CST_Listing.cpp -o moc_CST_Listing.o"
        ]
    },
    "moc_CST_Listing.cpp": {
        "requirements": ["CST_Listing.h"],
        "commands": [
            f"{MOC} -o moc_CST_Listing.cpp CST_Listing.h"
        ]
    },
    
    # CST_PlainTextDialog
    "CST_PlainTextDialog.o": {
        "requirements": ["CST_PlainTextDialog.cpp", "CST_PlainTextDialog.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c CST_PlainTextDialog.cpp -o CST_PlainTextDialog.o"
        ]
    },
    "moc_CST_PlainTextDialog.o": {
        "requirements": ["moc_CST_PlainTextDialog.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_CST_PlainTextDialog.cpp -o moc_CST_PlainTextDialog.o"
        ]
    },
    "moc_CST_PlainTextDialog.cpp": {
        "requirements": ["CST_PlainTextDialog.h"],
        "commands": [
            f"{MOC} -o moc_CST_PlainTextDialog.cpp CST_PlainTextDialog.h"
        ]
    },

    # CST_RadioButtonDialog
    "CST_RadioButtonDialog.o": {
        "requirements": ["CST_RadioButtonDialog.cpp", "CST_RadioButtonDialog.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c CST_RadioButtonDialog.cpp -o CST_RadioButtonDialog.o"
        ]
    },
    "moc_CST_RadioButtonDialog.o": {
        "requirements": ["moc_CST_RadioButtonDialog.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_CST_RadioButtonDialog.cpp -o moc_CST_RadioButtonDialog.o"
        ]
    },
    "moc_CST_RadioButtonDialog.cpp": {
        "requirements": ["CST_RadioButtonDialog.h"],
        "commands": [
            f"{MOC} -o moc_CST_RadioButtonDialog.cpp CST_RadioButtonDialog.h"
        ]
    },

    # CST_RichTextEdit
    "CST_RichTextEdit.o": {
        "requirements": ["CST_RichTextEdit.cpp", "CST_RichTextEdit.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c CST_RichTextEdit.cpp -o CST_RichTextEdit.o"
        ]
    },
    "moc_CST_RichTextEdit.o": {
        "requirements": ["moc_CST_RichTextEdit.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_CST_RichTextEdit.cpp -o moc_CST_RichTextEdit.o"
        ]
    },
    "moc_CST_RichTextEdit.cpp": {
        "requirements": ["CST_RichTextEdit.h"],
        "commands": [
            f"{MOC} -o moc_CST_RichTextEdit.cpp CST_RichTextEdit.h"
        ]
    },

    # CST_Separator
    "CST_Separator.o": {
        "requirements": ["CST_Separator.cpp", "CST_Separator.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c CST_Separator.cpp -o CST_Separator.o"
        ]
    },
    "moc_CST_Separator.o": {
        "requirements": ["moc_CST_Separator.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_CST_Separator.cpp -o moc_CST_Separator.o"
        ]
    },
    "moc_CST_Separator.cpp": {
        "requirements": ["CST_Separator.h"],
        "commands": [
            f"{MOC} -o moc_CST_Separator.cpp CST_Separator.h"
        ]
    },

    # CST_TestCaseDialog
    "CST_TestCaseDialog.o": {
        "requirements": ["CST_TestCaseDialog.cpp", "CST_TestCaseDialog.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c CST_TestCaseDialog.cpp -o CST_TestCaseDialog.o"
        ]
    },
    "moc_CST_TestCaseDialog.o": {
        "requirements": ["moc_CST_TestCaseDialog.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_CST_TestCaseDialog.cpp -o moc_CST_TestCaseDialog.o"
        ]
    },
    "moc_CST_TestCaseDialog.cpp": {
        "requirements": ["CST_TestCaseDialog.h"],
        "commands": [
            f"{MOC} -o moc_CST_TestCaseDialog.cpp CST_TestCaseDialog.h"
        ]
    },

    # CST_TextEditorDialog
    "CST_TextEditorDialog.o": {
        "requirements": ["CST_TextEditorDialog.cpp", "CST_TextEditorDialog.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c CST_TextEditorDialog.cpp -o CST_TextEditorDialog.o"
        ]
    },
    "moc_CST_TextEditorDialog.o": {
        "requirements": ["moc_CST_TextEditorDialog.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_CST_TextEditorDialog.cpp -o moc_CST_TextEditorDialog.o"
        ]
    },
    "moc_CST_TextEditorDialog.cpp": {
        "requirements": ["CST_TextEditorDialog.h"],
        "commands": [
            f"{MOC} -o moc_CST_TextEditorDialog.cpp CST_TextEditorDialog.h"
        ]
    },

    # WIN_ClassesSettings
    "WIN_ClassesSettings.o": {
        "requirements": ["WIN_ClassesSettings.cpp", "WIN_ClassesSettings.h", "CST_PlainTextDialog.h", "utilities.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c WIN_ClassesSettings.cpp -o WIN_ClassesSettings.o"
        ]
    },
    "moc_WIN_ClassesSettings.o": {
        "requirements": ["moc_WIN_ClassesSettings.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_WIN_ClassesSettings.cpp -o moc_WIN_ClassesSettings.o"
        ]
    },
    "moc_WIN_ClassesSettings.cpp": {
        "requirements": ["WIN_ClassesSettings.h"],
        "commands": [
            f"{MOC} -o moc_WIN_ClassesSettings.cpp WIN_ClassesSettings.h"
        ]
    },

    # WIN_ContestsSettings
    "WIN_ContestsSettings.o": {
        "requirements": ["WIN_ContestsSettings.cpp", "WIN_ContestsSettings.h", "CST_TestCaseDialog.h", "CST_PlainTextDialog.h", "utilities.h", "consts.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c WIN_ContestsSettings.cpp -o WIN_ContestsSettings.o"
        ]
    },
    "moc_WIN_ContestsSettings.o": {
        "requirements": ["moc_WIN_ContestsSettings.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_WIN_ContestsSettings.cpp -o moc_WIN_ContestsSettings.o"
        ]
    },
    "moc_WIN_ContestsSettings.cpp": {
        "requirements": ["WIN_ContestsSettings.h"],
        "commands": [
            f"{MOC} -o moc_WIN_ContestsSettings.cpp WIN_ContestsSettings.h"
        ]
    },

    # WIN_UsersSettings
    "WIN_UsersSettings.o": {
        "requirements": ["WIN_UsersSettings.cpp", "WIN_UsersSettings.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c WIN_UsersSettings.cpp -o WIN_UsersSettings.o"
        ]
    },
    "moc_WIN_UsersSettings.o": {
        "requirements": ["moc_WIN_UsersSettings.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_WIN_UsersSettings.cpp -o moc_WIN_UsersSettings.o"
        ]
    },
    "moc_WIN_UsersSettings.cpp": {
        "requirements": ["WIN_UsersSettings.h"],
        "commands": [
            f"{MOC} -o moc_WIN_UsersSettings.cpp WIN_UsersSettings.h"
        ]
    },

    # WIN_GenerateTestCasesDialog
    "WIN_GenerateTestCasesDialog.o": {
        "requirements": ["WIN_GenerateTestCasesDialog.cpp", "WIN_GenerateTestCasesDialog.h"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c WIN_GenerateTestCasesDialog.cpp -o WIN_GenerateTestCasesDialog.o"
        ]
    },
    "moc_WIN_GenerateTestCasesDialog.o": {
        "requirements": ["moc_WIN_GenerateTestCasesDialog.cpp"],
        "commands": [
            f"{CXX} {CXXFLAGS} {INCLUDES} -c moc_WIN_GenerateTestCasesDialog.cpp -o moc_WIN_GenerateTestCasesDialog.o"
        ]
    },
    "moc_WIN_GenerateTestCasesDialog.cpp": {
        "requirements": ["WIN_GenerateTestCasesDialog.h"],
        "commands": [
            f"{MOC} -o moc_WIN_GenerateTestCasesDialog.cpp WIN_GenerateTestCasesDialog.h"
        ]
    },
}

CONSOLE_SIZE = os.get_terminal_size()
CONSOLE_COLS = CONSOLE_SIZE.columns
CONSOLE_LINES = CONSOLE_SIZE.lines

def compileTarget(obj: str):
    # Check dependencies / requirements
    prstat("Compiling: ", f"[green]{obj}[/green]")
    prstat("Dependencies: ", f"[dim]{str(ACTIONS[obj]["requirements"])}[/dim]")
    for dpn in ACTIONS[obj]["requirements"]:
        if not os.path.exists(dpn):
            if dpn not in ACTIONS:
                # We are fucked
                prstat("Missing dependency: ", f"[red bold]{dpn}[/red bold]")
            elif dpn in PASSED_OBJS and PASSED_OBJS[dpn]:
                prstat("Skipping dependency: ", f"[dim]{obj}[/dim]")
            else:
                # Okay just rerun this ig
                compileTarget(dpn)

    # Good?
    PASSED_OBJS[obj] = True

    for cmd in ACTIONS[obj]["commands"]:
        print("SHELL>", cmd)
        os.system(cmd)


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

    prstat("Now cleaning up environment...")

    prstat("Removing ", f"[red bold]{TARGETFOLDER}[/red bold]")
    shutil.rmtree(TARGETFOLDER, ignore_errors=True)
    prstat("Removing ", f"[red bold]qres.cpp[/red bold]")
    if os.path.exists("qres.cpp"): os.remove("qres.cpp")

    ldirs = os.listdir()
    for filename in ldirs:
        if filename.endswith(".o") or \
           filename.endswith(".obj") or \
           (filename.startswith("moc_") and filename.endswith(".cpp")):
            
            prstat("Removing ", f"[red bold]{filename}[/red bold]")
            os.remove(filename)

    prstat("Commencing build process...")
    for obj in OBJECTS:
        # prstat("Building object: ", f"[green]{obj}[/green]")
        compileTarget(obj)

    prstat("Linking to executable...")
    if not os.path.exists(TARGETFOLDER):
        os.makedirs(TARGETFOLDER)
        prstat("Created ", f"[green]{TARGETFOLDER}[/green]")

    objectsArg = ""
    for obj in OBJECTS:
        objectsArg += obj + " "

    cmd = f"{CXX} -o {TARGETFOLDER}/{TARGET} {objectsArg} {LDFLAGS}"
    print("SHELL>", cmd)
    os.system(cmd)

    if isWindows:
        # This is limited to Windows
        prstat("Deploying Dynamic Linking Libraries...")
        deploycmd = f"{WINDEPLOYQT} {TARGETFOLDER}/{TARGET}"
        print("SHELL>", deploycmd)
        os.system(deploycmd)

        # Copy much needed dlls over
        for dll in ADDITIONALLIBS:
            targetDLL = f"{TARGETFOLDER}/{os.path.basename(dll)}"
            print(f"{dll} --> {targetDLL}")
            shutil.copy(dll, targetDLL)