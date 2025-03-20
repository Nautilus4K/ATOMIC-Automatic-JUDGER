################################
# Gathering required libraries #
################################

# For arguments and exec
import sys

# For system-related operations
import os

# For parsing data
import json

# For GUI:                  Main app      Window       Subpages    Tabs     Layout       Menu bar  Menu   Labels
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QTabWidget, QVBoxLayout, QMenuBar, QMenu, QLabel

#############
# JSON Data #
#############
VERSION_JSON = "/source/version.json"

# Directory position
dirPath = os.path.dirname(os.path.abspath(__file__))

########################
# Qt Style Sheet (QSS) #
########################

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
        
        # Layout of window, preparing tabs
        self.container = QWidget()
        self.layout = QVBoxLayout(self.container)

        # Create Tabs Object
        self.tabs = QTabWidget()

        # Creating Tabs
        # - There will be multiple tabs:
        #   + Judging management
        #   + Webserver management
        #   + Contests management
        #   + Classes management
        #   + Students management

        self.judgingTab = QWidget()
        self.webserverTab = QWidget()
        self.manageTab = QWidget()

        # Adding tabs into self.tabs
        self.tabs.addTab(self.judgingTab, "Chấm bài")
        self.tabs.addTab(self.webserverTab, "Trực tuyến")
        self.tabs.addTab(self.manageTab, "Quản lý")

        # Adding self.tab into Layout
        self.layout.addWidget(self.tabs)

        # Setting container as central widget
        self.setCentralWidget(self.container)
        
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
        self.menuBar.fileMenu.exitButton.triggered.connect(self.exit)

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


    def about(self):
        print("Help/About called")

        # Misc (self to make sure window is not destroyed after execution)
        self.aboutFrame = QWidget()
        self.aboutFrame.setFixedSize(300, 100)
        self.aboutFrame.show()
        self.aboutFrame.setWindowTitle("Về ATOMIC")

        with open(dirPath + VERSION_JSON, "r", encoding='utf-8') as versionFile:
            version = json.load(versionFile)

        version_str = f'Codename {version["version"]}\nAPI Version: {version["api"]}\nBranch: {version["branch"]}\nSig: {version["signature"]}\nStable: {"Yes" if version["stable"] else "No"}'
        # print(version_str)

        self.aboutFrame.layoutObj = QVBoxLayout()

        versionLabel = QLabel(self)
        versionLabel.setText(version_str)
        self.aboutFrame.layoutObj.addWidget(versionLabel)

        # sys.exit()
        # Applying layout
        self.aboutFrame.setLayout(self.aboutFrame.layoutObj)


    def backup(self):
        print("File/Backup called")


    def loadBackup(self):
        print("File/LoadBackup called")


    def exit(self):
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