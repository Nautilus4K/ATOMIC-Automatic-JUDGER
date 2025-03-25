#######################
# Judging Error codes #
#######################
# -127: Docker environment error
# -126: Docker pinging error
# -125: Compile image failed to be built
# -124: Compile container failed to start

################################
# Gathering required libraries #
################################

# For arguments and exec
import sys
from sys import platform

# For system-related operations
import os
import signal
import ctypes
import webbrowser as wb

# For parsing data
import json

# For GUI:                  Main app      Window       Subpages    Tabs     Layout       Menu bar  Menu   Labels  Text input  Sidebar    Checkbox   Scrolling    Frames   Buttons      Multiple lines   Msgbox
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QTabWidget, QVBoxLayout, QMenuBar, QMenu, QLabel, QLineEdit,  QSplitter, QCheckBox, QScrollArea, QFrame,  QPushButton, QTextEdit,       QMessageBox
#                       Float input       Icon   Image    Fonts  Text Cursor  Colors
from PyQt6.QtGui import QDoubleValidator, QIcon, QPixmap, QFont, QTextCursor, QColor

# More Qt stuffs         Some states variable     Running processes
from PyQt6.QtCore import Qt,                      QProcess

#############
# JSON Data #
#############
VERSION_JSON = "/source/version.json"
SETTINGS_JSON = "/source/settings.json"
ICON_PATH = "/icon.ico"

#################
# Scripts Paths #
#################
JUDGING_PATH = "/judge.py"
WSGI_PATH = "/wsgi_interface.py"

####################
# EXECUTABLE PATHS #
####################
PYDIR = "/.venv/Scripts/python.exe" # Currently using virual python environment

# Directory position
dirPath = os.path.dirname(os.path.abspath(__file__))

########################
# Qt Style Sheet (QSS) #
########################
STYLE_BIGLABEL = """
    font-size: 16px;
    font-weight: bold;
"""
STYLE_OPTION = """
    margin-bottom: 20px;
"""
STYLE_SIDEBAR_BUTTONS = """
    margin-top: 10px;
    margin-bottom: 20px;
    padding: 10px;
"""
STYLE_SIDEBAR_LABELS = """
    font-weight: bold;
"""
STYLE_CONSOLE = """
QTextEdit {
    background: black;
    color: white;
    border-radius: 5px;
}
QScrollBar:vertical {
    border: none;
    background: #0F0F0F;
    width: 4px;
    margin: 0px 0px 0px 0px;
}

QScrollBar::handle:vertical {
    background:rgb(83, 83, 83);
    min-height: 20px;
    width: 4px;
    border-radius: 2px;
}

QScrollBar::handle:vertical:hover {
    background: #888888;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    background: none;
}
"""

#############
# Colorings #
#############
COLOR_CONSOLE_ERROR = QColor(255, 82, 82)
COLOR_CONSOLE_WARNING = QColor(255, 209, 82)
COLOR_CONSOLE_OK = QColor(82, 255, 88)
COLOR_CONSOLE_DEFAULT = QColor(255, 255, 255)

###########
# Console #
###########
from csmcoloredloggerprint import INFO_COL, ERROR_COL, WARN_COL, OK_COL, RESET_COL

#########################################
# Main panel window class using QWidget #
#########################################
class MainPanel(QMainWindow):
    """
    Main Window in which all the useless things sit in.\n
    This is the place of declaration. \n
    """
    def __init__(self):
        # Notifying if the user is entering for the first time
        with open(dirPath + SETTINGS_JSON, "r", encoding='utf-8') as settingsFile:
            settings = json.load(settingsFile)

        if settings["panelfirst"]:
            settings["panelfirst"] = False
            with open(dirPath + SETTINGS_JSON, "w", encoding='utf-8') as settingsFile:
                json.dump(settings, settingsFile)

            # If this is the first time the users enter the Panel
            dlg = QMessageBox()
            dlg.setWindowTitle("Thông báo lần đầu")
            dlg.setText("Nếu đây là lần đầu bạn sử dụng ATOMIC, hãy vào mục Trợ giúp (Ở thanh công cụ phía trên ứng dụng), và chọn Hướng Dẫn. Ở đó bạn sẽ được hướng dẫn cách sử dụng ATOMIC, và các tính năng của nó.")
            dlg.setStandardButtons(QMessageBox.StandardButton.Ok)
            dlg.setIcon(QMessageBox.Icon.Information)
            dlg.setWindowIcon(QIcon(dirPath + ICON_PATH))
            dlg.exec()


        print("Initializing GUI...", end="", flush=True)
        super().__init__()
        print("done")

        # Preparation steps
        print("Setting up variables...", end="", flush=True)

        # Judging
        self.judgingEnabled = False
        self.judgingProcess = QProcess(self)

        # Connecting functions
        self.judgingProcess.readyReadStandardOutput.connect(self.readJudgingOutput)
        self.judgingProcess.readyReadStandardError.connect(self.readJudgingOutput)
        self.judgingProcess.finished.connect(self.stoppedJudging)
        print("done")

        # Fonts
        self.monospaceFont = QFont(["Cascadia Code", "Consolas", "Ubuntu Mono", "Source Code Pro", "monospace"], 8)

        # Setting looks and feel
        print("Loading Panel Window...", end="", flush=True)
        self.setWindowTitle("Bảng điều khiển ATOMIC")
        self.resize(950, 550)
        self.setMinimumSize(600, 400) # Make sure things does not go too small
        self.setWindowIcon(QIcon(dirPath + ICON_PATH))
        print("done")

        if platform.startswith("win"): # If using Windows, uses a workaround to make taskbar uses correct icon
            print("User is using WINDOWS")

            # Setting up program
            self.judgingProcess.setProgram(f"{dirPath}{PYDIR}")
            self.judgingProcess.setArguments([f"{dirPath}{JUDGING_PATH}"])

            # Fixing taskbar icon
            import ctypes
            myappid = u'nautilus4k.atomic.panel.unidentified' # arbitrary string
            ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
        else:
            print("User is NOT using WINDOWS")
            self.judgingProcess.setProgram(f"python")
            self.judgingProcess.setArguments([f"{dirPath}{JUDGING_PATH}"])
        
        # Layout of window, preparing tabs
        self.container = QWidget()
        self.layout = QVBoxLayout(self.container)

        # Splitter for a sidebar and the main layout of tabs
        self.splitter = QSplitter(self)
        self.splitter.setOrientation(Qt.Orientation.Horizontal)
        # self.splitter.setOpaqueResize(True)

        # Create Tabs Object
        self.tabs = QTabWidget()

        # Creating Tabs
        # - There will be multiple tabs:
        #   + Judging management
        #   + Webserver management
        #   + Contests management
        #   + Classes management
        #   + Students management

        self.manageTab = QWidget()
        self.judgingTab = QWidget()
        self.webserverTab = QWidget()

        # Adding tabs into self.tabs
        self.tabs.addTab(self.manageTab, "Quản lý")

        self.judgingScrollable = QScrollArea(self)
        self.judgingScrollable.setWidgetResizable(True)
        self.judgingScrollable.setWidget(self.judgingTab)
        self.judgingScrollable.setFrameShape(QFrame.Shape.NoFrame)
        self.tabs.addTab(self.judgingScrollable, "Chấm bài")

        self.tabs.addTab(self.webserverTab, "Trực tuyến")

        # Doing a bunch of actions when tabs are changed
        self.tabs.currentChanged.connect(self.tabChanged)

        # Adding sidebar frame
        self.sidebar = QWidget(self)
        self.sidebar.setMinimumWidth(140)
        self.sidebar.setMaximumWidth(250)
        # self.sidebar.width
        self.sidebar.layout = QVBoxLayout(self.sidebar)


        # +-----------------+
        # | Sidebar entries |
        # +-----------------+
        # ENABLE JUDGING
        self.sidebar.judgingLabel = QLabel(self)
        self.sidebar.judgingLabel.setText("Hệ thống chấm bài")
        self.sidebar.judgingLabel.setStyleSheet(STYLE_SIDEBAR_LABELS)
        self.sidebar.judgingLabel.setAlignment(Qt.AlignmentFlag.AlignTop)
        self.sidebar.layout.addWidget(self.sidebar.judgingLabel)

        self.sidebar.judgingButton = QPushButton(self)
        self.sidebar.judgingButton.setText("Bắt đầu chấm bài")
        self.sidebar.judgingButton.clicked.connect(self.toggleJudging)
        self.sidebar.judgingButton.setStyleSheet(STYLE_SIDEBAR_BUTTONS)
        self.sidebar.layout.addWidget(self.sidebar.judgingButton)


        # JUDGING CONSOLE
        self.sidebar.judgingConsole = QTextEdit(self)
        self.sidebar.judgingConsole.setFixedHeight(140)
        self.sidebar.judgingConsole.setAlignment(Qt.AlignmentFlag.AlignTop)
        self.sidebar.judgingConsole.setStyleSheet(STYLE_CONSOLE)
        self.sidebar.judgingConsole.setFont(self.monospaceFont)
        self.sidebar.judgingConsole.setReadOnly(True)
        self.sidebar.layout.addWidget(self.sidebar.judgingConsole)

        # ALIGNMENT
        self.sidebar.layout.addStretch() # Pushing elements upwards
        self.sidebar.layout.setAlignment(Qt.AlignmentFlag.AlignTop)

        # Adding self.sidebar into Layout (Splitter)
        self.splitter.addWidget(self.sidebar)
        # Adding self.tab into Layout (Splitter)
        self.splitter.addWidget(self.tabs)
        # Yeah that part is specifically oriented to make the side bar on the left

        # Setting container as central widget and using the splitter as main
        self.setCentralWidget(self.container)
        self.layout.addWidget(self.splitter)
        
        # Menu bar
        self.menuBar = QMenuBar(self)
        self.setMenuBar(self.menuBar)
        
        
        # +------------------+
        # | Menu bar entries |
        # +------------------+

        self.menuBar.fileMenu = QMenu("&Tệp", self)

        # Backup button. Used for backing up stuffs
        self.menuBar.fileMenu.backupButton = self.menuBar.fileMenu.addAction("Sao lưu...")
        self.menuBar.fileMenu.backupButton.triggered.connect(self.backup)

        # Load backups, loading backups made with backup button
        self.menuBar.fileMenu.loadbackupButton = self.menuBar.fileMenu.addAction("Mở sao lưu...")
        self.menuBar.fileMenu.loadbackupButton.triggered.connect(self.loadBackup)

        # Exit button
        self.menuBar.fileMenu.exitButton = self.menuBar.fileMenu.addAction("Thoát")
        self.menuBar.fileMenu.exitButton.triggered.connect(self.closeEvent)

        # Add menu in
        self.menuBar.addMenu(self.menuBar.fileMenu)

        self.menuBar.helpMenu = QMenu("&Trợ giúp", self)

        # Project page button. Prints project's GitHub page
        self.menuBar.helpMenu.gitHubButton = self.menuBar.helpMenu.addAction("Trang dự án...")
        self.menuBar.helpMenu.gitHubButton.triggered.connect(self.gitHub)

        # About button. Spits information about the application
        self.menuBar.helpMenu.aboutButton = self.menuBar.helpMenu.addAction("Về ATOMIC...")
        self.menuBar.helpMenu.aboutButton.triggered.connect(self.about)

        # Adding menu in
        self.menuBar.addMenu(self.menuBar.helpMenu)



        # +------------------+
        # | Judging settings |
        # +------------------+
        self.judgingTab.layout = QVBoxLayout()


        # WAIT TIME
        # Label
        self.judgingTab.waitTimeLabel = QLabel(self)
        self.judgingTab.waitTimeLabel.setText("Thời gian đợi quét")
        self.judgingTab.waitTimeLabel.setWordWrap(True)
        self.judgingTab.waitTimeLabel.setStyleSheet(STYLE_BIGLABEL)
        self.judgingTab.layout.addWidget(self.judgingTab.waitTimeLabel)

        # Description
        self.judgingTab.waitTimeDesc = QLabel(self)
        self.judgingTab.waitTimeDesc.setText("Thời gian tạm dừng giữa các lần quét bài làm của học sinh. Hữu dụng khi cần chấm bài trục tuyến mà cần giảm lượng CPU sử dụng cho quá trình chấm bài, tuy nhiên sẽ làm giảm tốc độ chấm bài. (giây)")
        self.judgingTab.waitTimeDesc.setWordWrap(True)
        self.judgingTab.layout.addWidget(self.judgingTab.waitTimeDesc)

        # Text input (Float)
        self.judgingTab.waitTimeInput = QLineEdit(self)
        self.judgingTab.waitTimeInput.doubleValidator = QDoubleValidator(0.0, 100.0, 2, self)  # Accepts numbers from 0 -> 100 with 2 decimal places
        self.judgingTab.waitTimeInput.setValidator(self.judgingTab.waitTimeInput.doubleValidator)
        self.judgingTab.waitTimeInput.textChanged.connect(self.setWaitTime)
        self.judgingTab.waitTimeInput.setStyleSheet(STYLE_OPTION)
        self.judgingTab.layout.addWidget(self.judgingTab.waitTimeInput)
            

        # RELOAD TIME
        # Label
        self.judgingTab.reloadTimeLabel = QLabel(self)
        self.judgingTab.reloadTimeLabel.setText("Thời gian làm mới")
        self.judgingTab.reloadTimeLabel.setWordWrap(True)
        self.judgingTab.reloadTimeLabel.setStyleSheet(STYLE_BIGLABEL)
        self.judgingTab.layout.addWidget(self.judgingTab.reloadTimeLabel)

        # Description
        self.judgingTab.reloadTimeDesc = QLabel(self)
        self.judgingTab.reloadTimeDesc.setText("Thời gian làm mới máy ảo để kiểm tra bài làm học sinh. Máy ảo được tạo ra để đảm bảo sự an toàn cho những tác vụ chạy trên file C++ của học sinh mà không cần mất đi tính năng nào như iofstream hay freopen. Thời gian làm mới là thời gian để máy ảo được làm mới nhằm đảm bảo sự an toàn của máy tính giáo viên và xóa những tác vụ đã xảy ra trước đó. (giây)")        
        self.judgingTab.reloadTimeDesc.setWordWrap(True)
        self.judgingTab.layout.addWidget(self.judgingTab.reloadTimeDesc)

        # Text input (Float)
        self.judgingTab.reloadTimeInput = QLineEdit(self)
        self.judgingTab.reloadTimeInput.doubleValidator = QDoubleValidator(0.0, 100.0, 2, self)  # Accepts numbers from 0 -> 100 with 2 decimal places
        self.judgingTab.reloadTimeInput.setValidator(self.judgingTab.reloadTimeInput.doubleValidator)
        self.judgingTab.reloadTimeInput.textChanged.connect(self.setReloadTime)
        self.judgingTab.reloadTimeInput.setStyleSheet(STYLE_OPTION)
        self.judgingTab.layout.addWidget(self.judgingTab.reloadTimeInput)

        # SHOW TESTS
        # Label
        self.judgingTab.showTestLabel = QLabel(self)
        self.judgingTab.showTestLabel.setText("Hiện đáp án (test)")
        self.judgingTab.showTestLabel.setWordWrap(True)
        self.judgingTab.showTestLabel.setStyleSheet(STYLE_BIGLABEL)
        self.judgingTab.layout.addWidget(self.judgingTab.showTestLabel)

        # Description
        self.judgingTab.showTestDesc = QLabel(self)
        self.judgingTab.showTestDesc.setText("Khi kiểm tra bài thi, kết quả kiểm tra sẽ được ghi ra một tệp kết quả cùng với thông tin chi tiết về quá trình kiểm tra. Cài đặt này sẽ hiện thông tin chi tiết như đầu vào, đầu ra chương trình và đầu ra đáp án.")
        self.judgingTab.showTestDesc.setWordWrap(True)
        self.judgingTab.layout.addWidget(self.judgingTab.showTestDesc)

        # Checkbox
        self.judgingTab.showTestCheckbox = QCheckBox(self)
        self.judgingTab.showTestCheckbox.setStyleSheet(STYLE_OPTION)
        self.judgingTab.showTestCheckbox.stateChanged.connect(self.setShowTest)
        self.judgingTab.layout.addWidget(self.judgingTab.showTestCheckbox)

        # Setting layout
        self.judgingTab.layout.setAlignment(Qt.AlignmentFlag.AlignTop)
        self.judgingTab.setLayout(self.judgingTab.layout)


    def about(self):
        print("Help/About called")

        # Misc (self to make sure window is not destroyed after execution)
        self.aboutFrame = QWidget()
        self.aboutFrame.setFixedSize(300, 100)
        self.aboutFrame.show()
        self.aboutFrame.setWindowTitle("Về ATOMIC")
        self.aboutFrame.setWindowIcon(QIcon(dirPath + ICON_PATH))

        self.aboutFrame.splitter = QSplitter(self)
        self.aboutFrame.splitter.setOrientation(Qt.Orientation.Horizontal)
        self.aboutFrame.splitter.setChildrenCollapsible(False)

        with open(dirPath + VERSION_JSON, "r", encoding='utf-8') as versionFile:
            version = json.load(versionFile)

        version_str = f'Codename {version["version"]}\nAPI Version: {version["api"]}\nBranch: {version["branch"]}\nSig: {version["signature"]}\nStable: {"Yes" if version["stable"] else "No"}'
        # print(version_str)

        self.aboutFrame.layoutObj = QVBoxLayout()

        versionLabel = QLabel(self)
        versionLabel.setText(version_str)
        self.aboutFrame.splitter.addWidget(versionLabel)

        self.aboutFrame.image = QLabel(self)

        # loading in image
        pixmap = QPixmap(dirPath + ICON_PATH)
        pixmap = pixmap.scaled(75, 75, Qt.AspectRatioMode.KeepAspectRatio, Qt.TransformationMode.SmoothTransformation)
        self.aboutFrame.image.setPixmap(pixmap)
        self.aboutFrame.image.setAlignment(Qt.AlignmentFlag.AlignRight)
        # self.aboutFrame.image.setFixedSize(100, 100)
        self.aboutFrame.splitter.addWidget(self.aboutFrame.image)

        self.aboutFrame.layoutObj.addWidget(self.aboutFrame.splitter)

        # sys.exit()
        # Applying layout
        self.aboutFrame.setLayout(self.aboutFrame.layoutObj)


    def gitHub(self):
        print("Help/GitHub called")
        wb.open(r"https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER")


    def toggleJudging(self):
        """Toggling judging process"""
        self.judgingEnabled = not self.judgingEnabled
        print(f"Toggled Judging to {self.judgingEnabled}!")

        if self.judgingEnabled:
            # Print the program and arguments for debugging
            program = self.judgingProcess.program()
            args = self.judgingProcess.arguments()
            print(f"Starting process: {program} {' '.join(args)}")
            
            # Check if the judging script file exists
            script_path = f"{dirPath}{JUDGING_PATH}"
            print(f"Checking if script exists at: {script_path}")
            print(f"File exists: {os.path.exists(script_path)}")
            
            # Start the process
            self.judgingProcess.start()
            
            # Check if process started
            if self.judgingProcess.state() == QProcess.ProcessState.Running:
                print(f"Process started successfully with PID: {self.judgingProcess.processId()}")
                self.sidebar.judgingButton.setText("Dừng chấm bài")
            else:
                print(f"Process failed to start. Error: {self.judgingProcess.error()}")
        else:
            # self.judgingProcess.terminate()
            # Stopping judging
            if self.judgingProcess.state() == QProcess.ProcessState.Running:
                self.sidebar.judgingButton.setEnabled(False) # Disabling toggle button when stopping
                pid = self.judgingProcess.processId() # Get process ID

                if (platform.startswith("win")):
                    kernel32 = ctypes.windll.kernel32
                    kernel32.FreeConsole()

                    if (kernel32.AttachConsole(pid)):
                        kernel32.GenerateConsoleCtrlEvent(0, 0)  # Send Ctrl+C (SIGINT) signal
                        kernel32.FreeConsole()
                else:
                    os.kill(pid, signal.SIGINT)

            # if platform.startswith("win")
            

    def stoppedJudging(self, exitCode, exitStatus):
        # When judging script stopped running
        self.sidebar.judgingButton.setEnabled(True)
        self.judgingEnabled = False
        self.sidebar.judgingButton.setText("Bắt đầu chấm bài")

        title = "Lỗi chấm bài"
        desc = "Đã có lỗi xảy ra. Nếu không hiểu rõ, hãy vào hướng dẫn (Mục Trợ giúp ở thanh menu trên đầu ứng dụng) và dò tìm mã: " + str(exitCode)
        buttons = QMessageBox.StandardButton.Ok

        if exitCode == -127:
            desc = "Đã có lỗi xảy ra. Hãy chắc chắn rằng bạn đã bật Docker Desktop và chạy Docker Environment. Nếu bối rối, hãy xem lại hướng dẫn (Mục Trợ giúp ở thanh menu trên đầu ứng dụng) về việc bật hệ thống chấm bài."
        elif exitCode == -126:
            desc = "Môi trường Docker không hoạt động. Hãy thử chạy lại hệ thống chấm bài hoặc khởi động lại Docker (Desktop và Environment)."

        if exitCode != 0:
            dlg = QMessageBox(self)
            dlg.setWindowTitle(title)
            dlg.setText(desc)
            dlg.setStandardButtons(QMessageBox.StandardButton.Ok)
            dlg.setIcon(QMessageBox.Icon.Critical)
            dlg.exec()


    def eraseCharacters(self, s: str, chars: str) -> str:
        # Create a new string including only characters not in 'chars'
        return ''.join(c for c in s if c not in chars)

    
    def readJudgingOutput(self):
        # Reading output of judging process
        output = self.judgingProcess.readAllStandardOutput().data().decode("utf-8")
        error_output = self.judgingProcess.readAllStandardError().data().decode("utf-8")
        
        # Merge outputs and append to QTextEdit instead of replacing
        if output:
            # self.sidebar.judgingConsole.append(output)
            # If output is fine, check the colors.
            # There are ONLY 4 TYPES OF COLORS
            outputList = output.split("\n")
            # print(outputList)
            for outputLine in outputList:
                outputLine = self.eraseCharacters(outputLine, "\r")
                # print(outputLine)
                if outputLine:  # Check if outputLine is not empty
                    # Get the ANSI color tag at the start if it exists
                    tag = outputLine.split("m", 1)[0] if outputLine.startswith("\x1b") else ""
                    # Remove ANSI color tags at the start
                    outputLine = outputLine.split("m", 1)[-1] if outputLine.startswith("\x1b") else outputLine
                    # Remove trailing \r and reset color ANSI tags
                    outputLine = outputLine.rstrip("\r").rsplit("\x1b", 1)[0] if outputLine.endswith(RESET_COL) else outputLine

                    # print(repr(tag))
                    # Check if that same line we had belongs in any ANSI color tags
                    tag += "m"
                    if tag == INFO_COL:
                        self.sidebar.judgingConsole.setTextColor(COLOR_CONSOLE_DEFAULT)
                    elif tag == ERROR_COL:
                        self.sidebar.judgingConsole.setTextColor(COLOR_CONSOLE_ERROR)
                    elif tag == WARN_COL:
                        self.sidebar.judgingConsole.setTextColor(COLOR_CONSOLE_WARNING)
                    elif tag == OK_COL:
                        self.sidebar.judgingConsole.setTextColor(COLOR_CONSOLE_OK)
                    

                    self.sidebar.judgingConsole.append(outputLine)
                    self.sidebar.judgingConsole.setTextColor(COLOR_CONSOLE_DEFAULT)
        
        if error_output:
            self.sidebar.judgingConsole.setTextColor(COLOR_CONSOLE_ERROR)
            self.sidebar.judgingConsole.append(error_output)
            self.sidebar.judgingConsole.setTextColor(COLOR_CONSOLE_DEFAULT)
            
        # Scroll to the bottom to show latest output
        self.sidebar.judgingConsole.moveCursor(QTextCursor.MoveOperation.End)


    def setWaitTime(self, text):
        # print(text)

        # Parsing settings json
        with open(dirPath + SETTINGS_JSON, "r", encoding='utf-8') as settingsFile:
            settings = json.load(settingsFile)

        # Writing the new data
        if text == "": text = 0
        settings["wait_time"] = float(text)
        with open(dirPath + SETTINGS_JSON, "w", encoding='utf-8') as settingsFile:
            json.dump(settings, settingsFile)


    def setReloadTime(self, text):
        # Parsing settings json
        with open(dirPath + SETTINGS_JSON, "r", encoding='utf-8') as settingsFile:
            settings = json.load(settingsFile)

        # Writing new data
        if text == "": text = 0
        settings["reload_time"] = float(text)
        with open(dirPath + SETTINGS_JSON, "w", encoding='utf-8') as settingsFile:
            json.dump(settings, settingsFile)


    def setShowTest(self, state):
        checked = bool(state == 2)  # 2 is the code behind it all
        # Parsing settings json
        with open(dirPath + SETTINGS_JSON, "r", encoding='utf-8') as settingsFile:
            settings = json.load(settingsFile)

        # Writing new data
        settings["show_test"] = checked
        with open(dirPath + SETTINGS_JSON, "w", encoding='utf-8') as settingsFile:
            json.dump(settings, settingsFile)


    def backup(self):
        print("File/Backup called")


    def loadBackup(self):
        print("File/LoadBackup called")

    def tabChanged(self, index):
        # print(index)
        # Fixing some editing errors
        # Set old settings from settings file
        with open(dirPath + SETTINGS_JSON, "r", encoding='utf-8') as settingsFile:
            settings = json.load(settingsFile)
        
        self.judgingTab.waitTimeInput.setText(str(settings["wait_time"]))
        self.judgingTab.reloadTimeInput.setText(str(settings["reload_time"]))
        self.judgingTab.showTestCheckbox.setCheckState(Qt.CheckState.Checked if settings["show_test"] else Qt.CheckState.Unchecked)

    def closeEvent(self, event):
        # print(event)
        print("CloseEvent called")
        super().closeEvent(event)
        exit()


#########################
# Codebase/Instructions #
#########################

app = QApplication(sys.argv)  # Create application

# Use the main panel class
window = MainPanel()

# Showing the window
window.show()

sys.exit(app.exec()) # Run the application loop, like MainLoop