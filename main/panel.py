################################
# Gathering required libraries #
################################

# For arguments and exec
import sys
from sys import platform

# For system-related operations
import os

# For parsing data
import json

# For GUI:                  Main app      Window       Subpages    Tabs     Layout       Menu bar  Menu   Labels  Text input  Sidebar    Checkbox
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QTabWidget, QVBoxLayout, QMenuBar, QMenu, QLabel, QLineEdit,  QSplitter, QCheckBox
#                       Float input       Icon   Image
from PyQt6.QtGui import QDoubleValidator, QIcon, QPixmap

# More Qt stuffs
from PyQt6.QtCore import Qt

#############
# JSON Data #
#############
VERSION_JSON = "/source/version.json"
SETTINGS_JSON = "/source/settings.json"
ICON_PATH = "/icon.ico"

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

#########################################
# Main panel window class using QWidget #
#########################################
class MainPanel(QMainWindow):
    """
    Main Window in which all the useless things sit in.\n
    This is the place of declaration. \n
    """
    def __init__(self):
        super().__init__()

        # Setting looks and feel
        self.setWindowTitle("Bảng điều khiển ATOMIC")
        self.resize(950, 550)
        self.setMinimumSize(600, 400) # Make sure things does not go too small
        self.setWindowIcon(QIcon(dirPath + ICON_PATH))

        if platform == "win32": # If using Windows, uses a workaround to make taskbar uses correct icon
            print("User is using WINDOWS")
            import ctypes
            myappid = u'nautilus4k.atomic.panel.unidentified' # arbitrary string
            ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
        else:
            print("User is NOT using WINDOWS")
        
        # Layout of window, preparing tabs
        self.container = QWidget()
        self.layout = QVBoxLayout(self.container)

        # Splitter for a sidebar and the main layout of tabs
        self.splitter = QSplitter(self)
        self.splitter.setOrientation(Qt.Orientation.Horizontal)

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
        self.tabs.addTab(self.judgingTab, "Chấm bài")
        self.tabs.addTab(self.webserverTab, "Trực tuyến")

        # Doing a bunch of actions when tabs are changed
        self.tabs.currentChanged.connect(self.tabChanged)

        # Adding sidebar frame
        self.sidebar = QWidget(self)
        self.sidebar.layout = QVBoxLayout(self.sidebar)

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

        self.aboutFrame.splitter = QSplitter(self)
        self.aboutFrame.splitter.setOrientation(Qt.Orientation.Horizontal)

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