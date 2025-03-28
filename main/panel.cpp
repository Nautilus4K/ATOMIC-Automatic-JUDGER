/**********************************

Compilation commandline:
g++ -I"C:\Qt\6.5.3\mingw_64\include" -L"C:\Qt\6.5.3\mingw_64\lib" panel.cpp -lQt6Widgets -lQt6Core -lQt6Gui -o panel.exe -mwindows

This panel.cpp is for WINDOWS. So, yeah, sorry Linux folks.
It's all cause most Vietnamese people uses Windows so yea
Sorrie

Also, credits to Qt for making such an awesome GUI framework. I'm in love with it.

**********************************/

// Importing Qt stuffs
#include <QtWidgets/QApplication>  // For application
#include <QtWidgets/QMainWindow>   // For main window
#include <QtWidgets/QWidget>       // For a window-like area
#include <QtWidgets/QVBoxLayout>   // Layout of QWidgets
#include <QtWidgets/QScrollArea>   // Scrollable area. Used with QWidgets
#include <QtWidgets/QFrame>        // Frames. Like the one outline in QScrollArea
#include <QtWidgets/QSplitter>     // Splitter
#include <QtWidgets/QTabWidget>    // Tabs
#include <QtWidgets/QMenuBar>      // Menu bar (The toolbar on window)
#include <QtWidgets/QMenu>         // Menu (Entries in QMenuBar)
#include <QtGui/QAction>           // Action for menus. Wonder what fucker thought to put it in QtGui
#include <QtWidgets/QMessageBox>   // Message boxes
#include <QtGui/QCloseEvent>       // Close event. The action of 'X' button

// Importing Qt values
#include <QtCore/Qt>   // A bunch of constants

// File I/O actions
#include <fstream>

// System-related actions
#include <filesystem>
#include <Windows.h>

// Debug console
#include <iostream>

// Constants
const std::string THEME_PATH = "/source/theme.qss";
const std::string GITHUB_PAGE = "\"https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER\"";


class PanelWindow: public QMainWindow { // This is based on QMainWindow
    public: // PUBLIC. ACCESSIBLE FROM ANYWHERE
    QWidget *manageTab = new QWidget();
    QWidget *judgingTab = new QWidget();
    QWidget *webserverTab = new QWidget();
    std::string dirPath = std::filesystem::current_path().string();
    PanelWindow() {
        // This is the configuration part of panelWindow.
        // Initialization will be another one

        ///////////////////////////////////
        // Setting window configurations //
        ///////////////////////////////////
        setWindowTitle("Bảng điều khiển ATOMIC");
        resize(QSize(950, 550));
        setMinimumSize(QSize(600, 400));

        // Reading themes from outside files with fstream
        std::cout << "Executable directory: " << dirPath << "\n";
        std::fstream themeFile(dirPath + THEME_PATH, std::ios::in);
        std::string themeDataStr;  // Use std::string
        std::stringstream themeDataBuffer; // Read whole file
        if (themeFile.is_open()) {
            themeDataBuffer << themeFile.rdbuf();  // Read entire file into buffer
            themeFile.close();
            themeDataStr = themeDataBuffer.str();
            QString themeData = QString::fromStdString(themeDataStr);  // Convert std::string to QString
            std::cout << "Theme data:\n" << themeDataStr << "\n";
            setStyleSheet(themeData);
        } else {
            // If file is unable to open
            std::cout << "Cannot find theme file\n";
        }


        //////////////
        // Menu bar //
        //////////////
        QMenuBar *menuBar = new QMenuBar();
        setMenuBar(menuBar);

        // FILE MENU
        QMenu *fileMenu = new QMenu("&Tệp");
        menuBar->addMenu(fileMenu);

        // Action of backing up files (Currently, well, not worked on yet)
        QAction *fileBackup = new QAction();
        fileBackup->setText("Sao lưu...");
        fileMenu->addAction(fileBackup);
        // QAction connected function
        connect(fileBackup, &QAction::triggered, this, &PanelWindow::backUp);

        // Action of loading up backup files (WIP)
        QAction *loadBackup = new QAction();
        loadBackup->setText("Mở sao lưu...");
        fileMenu->addAction(loadBackup);
        // QAction connected function
        connect(loadBackup, &QAction::triggered, this, &PanelWindow::loadBackUp);

        // Action of exitting
        QAction *exitApp = new QAction();
        exitApp->setText("Thoát");
        fileMenu->addAction(exitApp);
        // QAction connected function
        connect(exitApp, &QAction::triggered, this, &PanelWindow::close);

        
        // HELP MENU
        QMenu *helpMenu = new QMenu("&Trợ giúp");
        menuBar->addMenu(helpMenu);

        // Action of about GITHUB page
        QAction *openGitHub = new QAction();
        openGitHub->setText("Trang dự án...");
        helpMenu->addAction(openGitHub);
        // QAction connected function
        connect(openGitHub, &QAction::triggered, this, &PanelWindow::gitHub);


        ///////////////////////////////////
        // Window elements configuration //
        ///////////////////////////////////
        // Main element main window.
        // The so-called `container` will manage the main window's layout and widgets
        QWidget *container = new QWidget();

        // Layout of container will be called mainLayout.
        // The reason why I had to type new QVBoxLayout() is because of the fact that
        // good ol QVBoxLayout mainLayout; is on stack, but setLayout requires a pointer
        QVBoxLayout *mainLayout = new QVBoxLayout();


        // Splitter for main window
        // The layout of main window:
        // +------+--------------------+
        // | Side | Settings (Tabs)    |
        // | bar  |                    |
        // |      |                    |
        // +------+--------------------+
        // So we need a splitter for main window
        QSplitter *splitter = new QSplitter();
        // Oh and horizontal orientation too
        splitter->setOrientation(Qt::Orientation::Horizontal);

        // Adding splitter into mainLayout
        mainLayout->addWidget(splitter);


        // +---------------+
        // | Settings tabs |
        // +---------------+
        QTabWidget *tabs = new QTabWidget();
        
        // Setting up tabs: (With scrolling support)
        // - Management tab
        // - Judging tab
        // - Webserver tab
        QScrollArea *manageScrollArea = new QScrollArea();
        manageScrollArea->setWidgetResizable(true);
        manageScrollArea->setWidget(manageTab);
        manageScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        tabs->addTab(manageScrollArea, "Quản lý");

        QScrollArea *judgingScrollArea = new QScrollArea();
        judgingScrollArea->setWidgetResizable(true);
        judgingScrollArea->setWidget(judgingTab);
        judgingScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        tabs->addTab(judgingScrollArea, "Chấm bài");

        QScrollArea *webserverScrollArea = new QScrollArea();
        webserverScrollArea->setWidgetResizable(true);
        webserverScrollArea->setWidget(webserverTab);
        webserverScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        tabs->addTab(webserverScrollArea, "Trực tuyến");

        // Add tabs into splitter
        splitter->addWidget(tabs);
        


        ///////////////////////
        // Finishing touches //
        ///////////////////////

        // Settings for container
        container->setLayout(mainLayout);

        // Settings for QMainWindow
        setCentralWidget(container);
    }

    void about() {

    }

    void gitHub() {
        // Executing the github repo into Windows
        ShellExecute(0, "open", GITHUB_PAGE.c_str(), 0, 0, SW_SHOWNORMAL);
    }

    void backUp() {

    }

    void loadBackUp() {

    }
    protected: // Events in which are native Qt events
    void closeEvent(QCloseEvent *event) override {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Xác nhận", "Bạn có chắc muốn thoát?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            event->accept();  // Allow closing
            exit(0);
        } else {
            event->ignore();  // Prevent closing
        }
    }
};

int main(int argc, char *argv[])
{
    // Hide console if there is no argument regarding the console
    bool hideConsole = true;
    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]) == "--console") hideConsole = false;
    }
    if (hideConsole) FreeConsole();

    // Main app
    QApplication a(argc, argv);
    PanelWindow panel;
    panel.show();
    return a.exec();
}
