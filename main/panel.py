################################
# Gathering required libraries #
################################

# For arguments
import sys

# For GUI                   Main app      Window   Tabs        Layout
from PyQt6.QtWidgets import QApplication, QWidget, QTabWidget, QVBoxLayout


#########################################
# Main panel window class using QWidget #
#########################################
class MainPanel(QWidget):
    def __init__(self):
        super().__init__()

        # Setting looks and feel
        self.setWindowTitle("Bảng điều khiển ATOMIC")
        self.resize(950, 550)
        self.setMinimumSize(600, 400) # Make sure things does not go too small
        
        # Layout of window, preparing tabs
        layout = QVBoxLayout()
        self.tabs = QTabWidget()

        # Creating Tabs
        # - There will be multiple tabs:
        #   + Judging management
        #   + Webserver management
        #   + Contests management
        #   + Classes management
        #   + Students management

        judgingTab = QWidget()
        webserverTab = QWidget()
        contestTab = QWidget()
        classTab = QWidget()
        studentTab = QWidget()

        # Adding tabs into self.tabs
        self.tabs.addTab(judgingTab, "Chấm bài")
        self.tabs.addTab(webserverTab, "Trực tuyến")
        self.tabs.addTab(contestTab, "Bài thi")
        self.tabs.addTab(classTab, "Lớp học")
        self.tabs.addTab(studentTab, "Học sinh")

        # Adding self.tab into Layout. Therefore showing it in main window
        layout.addWidget(self.tabs)
        self.setLayout(layout)


#########################
# Codebase/Instructions #
#########################

app = QApplication(sys.argv)  # Create application

# Use the main panel class
window = MainPanel()

# Showing the window
window.show()

sys.exit(app.exec()) # Run the application loop, like MainLoop