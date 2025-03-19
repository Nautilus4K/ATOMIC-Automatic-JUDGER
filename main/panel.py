################################
# Gathering required libraries #
################################

# For arguments
import sys

# For GUI:                  Main app      Window       Subpages    Tabs     Layout       Menu bar  Menu
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QTabWidget, QVBoxLayout, QMenuBar, QMenu

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
        self.contestTab = QWidget()
        self.classTab = QWidget()
        self.studentTab = QWidget()

        # Adding tabs into self.tabs
        self.tabs.addTab(self.judgingTab, "Chấm bài")
        self.tabs.addTab(self.webserverTab, "Trực tuyến")
        self.tabs.addTab(self.contestTab, "Bài thi")
        self.tabs.addTab(self.classTab, "Lớp học")
        self.tabs.addTab(self.studentTab, "Học sinh")

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

        self.menuBar_fileMenu = QMenu("&Tệp", self)

        # Backup button. Used for backing up stuffs
        menuBar_fileMenu_backupButton = self.menuBar_fileMenu.addAction("Sao lưu...")
        menuBar_fileMenu_backupButton.triggered.connect(self.backup)

        # Load backups, loading backups made with backup button
        menuBar_fileMenu_loadbackupButton = self.menuBar_fileMenu.addAction("Mở sao lưu...")
        menuBar_fileMenu_loadbackupButton.triggered.connect(self.loadBackup)

        # Exit button
        menuBar_fileMenu_exitButton = self.menuBar_fileMenu.addAction("Thoát")
        menuBar_fileMenu_exitButton.triggered.connect(self.exit)

        # Add menu in
        self.menuBar.addMenu(self.menuBar_fileMenu)

        self.menuBar_helpMenu = QMenu("&Trợ giúp", self)
        self.menuBar.addMenu(self.menuBar_helpMenu)

        # +------------------+
        # | Judging settings |
        # +------------------+

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