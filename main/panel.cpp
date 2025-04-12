/**********************************

      ::::    :::     :::     :::    ::: ::::::::::: ::::::::::: :::       :::    :::  ::::::::      :::     :::    ::: 
     :+:+:   :+:   :+: :+:   :+:    :+:     :+:         :+:     :+:       :+:    :+: :+:    :+:    :+:      :+:   :+:   
    :+:+:+  +:+  +:+   +:+  +:+    +:+     +:+         +:+     +:+       +:+    +:+ +:+          +:+ +:+   +:+  +:+     
   +#+ +:+ +#+ +#++:++#++: +#+    +:+     +#+         +#+     +#+       +#+    +:+ +#++:++#++  +#+  +:+   +#++:++       
  +#+  +#+#+# +#+     +#+ +#+    +#+     +#+         +#+     +#+       +#+    +#+        +#+ +#+#+#+#+#+ +#+  +#+       
 #+#   #+#+# #+#     #+# #+#    #+#     #+#         #+#     #+#       #+#    #+# #+#    #+#       #+#   #+#   #+#       
###    #### ###     ###  ########      ###     ########### ########## ########   ########        ###   ###    ###   

This is free software made by Nautilus4K a.k.a Quang Vinh.
Any form of credits is neccessary as described in the license in the repository

Repo link: https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER/


Compilation arguments:
"-fdiagnostics-color=always",
"-I\"C:\\Qt\\6.5.3\\mingw_64\\include\"",
"-I\"C:\\Qt\\Tools\\mingw1120_64\\include\"",
"-L\"C:\\Qt\\6.5.3\\mingw_64\\lib\"",
"-L\"C:\\Qt\\Tools\\mingw1120_64\\lib\"",
"${file}",
"${fileBasenameNoExtension}.o",
"-lQt6Widgets",
"-lQt6Core",
"-lQt6Gui",
"-o",
"${fileDirname}\\${fileBasenameNoExtension}.exe",

This panel.cpp is for WINDOWS. So, yeah, sorry Linux folks.
It's all cause most Vietnamese people uses Windows so yea
Sorrie

Also, credits to:
- Qt for making such an awesome GUI framework. I'm in love with it.
- nlomann for making a JSON manipulation API. https://github.com/nlohmann/json

**********************************/

// Importing Qt stuffs
#include <QtWidgets/QApplication>  // For application
#include <QtWidgets/QMainWindow>   // For main window
#include <QtWidgets/QWidget>       // For a window-like area
#include <QtWidgets/QVBoxLayout>   // Verical Layout of QWidgets
#include <QtWidgets/QHBoxLayout>   // Horizontal Layout of QWidgets
#include <QtWidgets/QScrollArea>   // Scrollable area. Used with QWidgets
#include <QtWidgets/QFrame>        // Frames. Like the one outline in QScrollArea
#include <QtWidgets/QSplitter>     // Splitter
#include <QtWidgets/QTabWidget>    // Tabs
#include <QtWidgets/QMenuBar>      // Menu bar (The toolbar on window)
#include <QtWidgets/QMenu>         // Menu (Entries in QMenuBar)
#include <QtWidgets/QMessageBox>   // Message boxes
#include <QtWidgets/QLabel>        // Labels
#include <QtWidgets/QLineEdit>     // An edit area that is a line edit. No multiple lines.
#include <QtWidgets/QCheckBox>     // Checkboxes. Of course. What did you expect, sucker?
#include <QtWidgets/QPushButton>   // Buttons as we know it
#include <QtWidgets/QTextEdit>     // A text frame
#include <QtGui/QAction>           // Action for menus. Wonder what fucker thought to put it in QtGui
#include <QtGui/QCloseEvent>       // Close event. The action of 'X' button
#include <QtGui/QDoubleValidator>  // Validator for edits.
#include <QtGui/QIntValidator>     // Another validator, also for edits, now more like INTEGER revolved.
#include <QtGui/QIcon>             // Icon readings
#include <QtGui/QPixmap>           // Picture reading
#include <QtGui/QFont>             // Fonts
#include <QtGui/QColor>            // Colors

// Importing Qt related features
#include <QtCore/Qt>       // A bunch of constants
#include <QtCore/QProcess> // Process running

// File I/O actions and getting data
#include <fstream>
#include <nlohmann/json.hpp>
// Turn nlohmann::json to json for short.
using json = nlohmann::json;

// System-related actions
#ifndef WIN32_LEAN_AND_MEAN // Because of historical reasons, including Windows.h
#define WIN32_LEAN_AND_MEAN // along with winsock2.h will need to have this
#endif

#include <filesystem>
#include <Windows.h>
#include <winsock2.h> // Socket programming
#include <shellapi.h> // ShellExecute, etc...
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Required library to link with

// C++ Features
#include <vector>
#include <map>

// Debug console
#include <iostream>

// Constants
// -> Paths
const std::string THEME_PATH = "/source/theme.qss";
const std::string THEMECOLORS_PATH = "/source/theme_color.opt";
const std::string SETTINGS_PATH = "/source/settings.json";
const std::string VERSION_PATH = "/source/version.json";
const std::string ICON_PATH = "/icon.ico";
const std::string PYDIR = "/.venv/Scripts/python.exe";
const std::string JUDGING_PATH = "/judge.py";
const std::string WEBSERVER_PATH = "/apache.py";
const std::string LOG_PATH = "/central/valkyrie.log";

// -> Qt Style Sheet
const QString STYLE_BIGLABEL = "font-size: 16px; font-weight: bold;";

// -> Others
const std::string GITHUB_PAGE = "\"https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER\"";
const int JUDGING_EXITPORT = 28472;
const std::string JUDGING_EXITADDR = "127.0.0.1";
const int WEBSERVER_EXITPORT = 28473;
const std::string WEBSERVER_EXITADDR = "127.0.0.1";

// -> ANSI Color codes
const std::string INFO_COL = "\x1b[0m";
const std::string ERROR_COL = "\x1b[31m";
const std::string WARN_COL = "\x1b[33m";
const std::string OK_COL = "\x1b[32m";
const std::string RESET_COL = "\x1b[0m";

// -> QColor values
QColor COLOR_CONSOLE_ERROR;
QColor COLOR_CONSOLE_WARNING;
QColor COLOR_CONSOLE_OK;
QColor COLOR_CONSOLE_DEFAULT;

// Custom functions as tools
std::string intToString(int n) {
    // Conversion from INTEGER to STRING
    std::string value = "";
    while (n > 0) {
        value += char((n % 10) + '0'); // Literal magic
        n /= 10;
    }
    std::reverse(value.begin(), value.end());
    return value;
}

std::string doubleToString(double d) {
    // Conversion from DOUBLE to STRING
    std::ostringstream oss;
    oss << d; // Yeah this is the true magic
    return oss.str();
}

double stringToDouble(const std::string &str) {
    try {
        return std::stod(str); // Built-in function
    } catch (const std::invalid_argument &e) {
        return 0.0; // Handle invalid input (e.g., empty string or non-numeric data)
    } catch (const std::out_of_range &e) {
        return 0.0; // Handle out-of-range errors
    }
}

class PanelWindow: public QMainWindow { // This is based on QMainWindow
    public: // PUBLIC. ACCESSIBLE FROM ANYWHERE
    QWidget *manageTab = new QWidget();
    QWidget *judgingTab = new QWidget();
    QWidget *webserverTab = new QWidget();

    // Option elements (In tabs)
    QLineEdit *judgingWaitTimeInput = new QLineEdit();
    QLineEdit *judgingReloadTimeInput = new QLineEdit();
    QCheckBox *judgingShowTestCheckbox = new QCheckBox();

    QLineEdit *webserverLogInSecsInput = new QLineEdit();

    // Sidebar elements
    QPushButton *judgingProcessButton = new QPushButton();
    QTextEdit *judgingProcessConsole = new QTextEdit();
    QPushButton *webserverProcessButton = new QPushButton();

    // Data variables
    json settings; // NULL at first
    json version; // NULL at first
    QPixmap iconPixmap;
    QFont monospaceFont; // NULL at first
    QString styleSheetResult;

    // Processes
    QProcess *judgingProcess = new QProcess();
    bool judgingEnabled;
    QProcess *webserverProcess = new QProcess();
    bool webserverEnabled;

    // Paths
    std::string dirPath = std::filesystem::current_path().string();
    PanelWindow() { // Extremely powerful? Extremely complex.
        // This is the configuration part of panelWindow.
        // Initialization will be another one

        ///////////////////////////////////
        // Setting window configurations //
        ///////////////////////////////////
        setWindowTitle("Bảng điều khiển ATOMIC");
        resize(QSize(950, 550));
        setMinimumSize(QSize(600, 400));

        iconPixmap = QPixmap(QString::fromStdString(dirPath + ICON_PATH), "ICO", Qt::AutoColor);
        setWindowIcon(QIcon(iconPixmap));

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
            styleSheetResult = themeData;
            setStyleSheet(themeData);
        } else {
            // If file is unable to open
            std::cout << "Cannot find theme file\n";
        }

        // Setting fonts
        monospaceFont.setFamilies({"Source Code Pro", "Cascadia Code", "Consolas", "Courier New", "Ubuntu Mono", "monospace"});

        // Preparing processes
        // Before starting the judgingProcess, set environment variables for proper Unicode support
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("PYTHONIOENCODING", "utf-8");  // Force Python to use UTF-8 for stdin/stdout
        env.insert("PYTHONUTF8", "1");            // Force UTF-8 mode in Python 3.7+

        // For Windows specifically
        #ifdef Q_OS_WIN
        env.insert("PYTHONLEGACYWINDOWSSTDIO", "0");  // Disable legacy stdio for better Unicode handling
        #endif

        judgingProcess->setProcessEnvironment(env);

        judgingEnabled = false;
        judgingProcess->setProgram(QString::fromStdString(dirPath + PYDIR));
        judgingProcess->setArguments(QStringList() << "-u" << QString::fromStdString(dirPath + JUDGING_PATH)); // Wrap inside QStringList for arguments

        webserverEnabled = false;
        webserverProcess->setProgram(QString::fromStdString(dirPath + PYDIR));
        webserverProcess->setArguments(QStringList() << QString::fromStdString(dirPath + WEBSERVER_PATH));


        // Connecting with functions
        connect(judgingProcess, &QProcess::readyReadStandardOutput, this, &PanelWindow::judgingHandleStandardOutput);
        connect(judgingProcess, &QProcess::readyReadStandardError, this, &PanelWindow::judgingHandleStandardError);
        connect(judgingProcess, &QProcess::finished, this, &PanelWindow::stoppedJudging);
        connect(webserverProcess, &QProcess::finished, this, &PanelWindow::stoppedWebserver);

        judgingProcess->setReadChannel(QProcess::StandardOutput);
        judgingProcess->setProcessChannelMode(QProcess::MergedChannels); // Merges stdout and stderr


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

        // Action of about GITHUB page
        QAction *openAbout = new QAction();
        openAbout->setText("Về ATOMIC...");
        helpMenu->addAction(openAbout);
        // QAction connected function
        connect(openAbout, &QAction::triggered, this, &PanelWindow::about);


        ///////////////////////////////////
        // Window elements configuration //
        ///////////////////////////////////
        // Main element main window.
        // The so-called `container` will manage the main window's layout and widgets
        QWidget *container = new QWidget();
        container->setObjectName("container"); // Setting name for QSS

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


        // +-----------------------+
        // | Side                  |
        // | bar configuration     |
        // +-----------------------+
        QWidget *sidebar = new QWidget();
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumWidth(150);
        sidebar->setMaximumWidth(250);

        QVBoxLayout *sidebarLayout = new QVBoxLayout();

        // JUDGING
        QLabel *judgingProcessLabel = new QLabel();
        judgingProcessLabel->setText("Hệ thống chấm bài");
        judgingProcessLabel->setWordWrap(true);
        sidebarLayout->addWidget(judgingProcessLabel);

        judgingProcessButton->setText("Bật chấm bài");
        sidebarLayout->addWidget(judgingProcessButton);
        // Connect to desired function
        connect(judgingProcessButton, QPushButton::clicked, this, [this] {
            // Using lambda to inject special arguments onto desired function
            // Pretty basic stuffs, eh?
            onSidebarFeatureButtonPushed("judging");
        });

        // Console for judging process
        judgingProcessConsole->setObjectName("console");
        judgingProcessConsole->setReadOnly(true);
        judgingProcessConsole->setAlignment(Qt::AlignTop);
        judgingProcessConsole->setFont(monospaceFont);
        sidebarLayout->addWidget(judgingProcessConsole);
        

        // WEBSERVER
        QLabel *webserverProcessLabel = new QLabel();
        webserverProcessLabel->setText("Trang chấm bài trực tuyến");
        webserverProcessLabel->setWordWrap(true);
        sidebarLayout->addWidget(webserverProcessLabel);

        // We're gonna be putting the webserver toggle and the website fast shortcut on the
        // same line babyyyyyy.
        // First, create a sub-QWidget and then add QHBoxLayout and finally add buttons
        QWidget *webserverProcessButtonsLine = new QWidget();
        QHBoxLayout *webserverProcessButtonsLineLayout = new QHBoxLayout();

        webserverProcessButton->setText("Bật Website");
        connect(webserverProcessButton, &QPushButton::clicked, this, [this] {
            onSidebarFeatureButtonPushed("webserver");
        });
        webserverProcessButtonsLineLayout->addWidget(webserverProcessButton);

        QPushButton *webserverOpener = new QPushButton();
        webserverOpener->setText("Mở");
        connect(webserverOpener, &QPushButton::clicked, this, &PanelWindow::openHttpWebsite); // Connecting to function
        webserverProcessButtonsLineLayout->addWidget(webserverOpener);

        webserverProcessButtonsLine->setLayout(webserverProcessButtonsLineLayout);
        sidebarLayout->addWidget(webserverProcessButtonsLine);


        // Applying layout
        sidebarLayout->setAlignment(Qt::AlignTop);
        sidebar->setLayout(sidebarLayout);

        // Adding sidebar to splitter
        splitter->addWidget(sidebar);


        // +--------------------+
        // | Tabs configuration |
        // +--------------------+
        QTabWidget *tabs = new QTabWidget();
        
        // On tab switches action
        connect(tabs, QTabWidget::currentChanged, this, &PanelWindow::onTabSwitches);
        
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
        

        // +-------------+
        // | Judging tab |
        // +-------------+
        /*
         * Note: This part here is solely for the judging tab
         * They will have these options
         * 
         * - Wait Time (INPUT) OK
         * - Reload Time (INPUT) OK
         * - Show Tests (CHECKBOX) OK
         * 
         * Update: Please save me from this hellhole
         */
        QVBoxLayout *judgingTabLayout = new QVBoxLayout();

        // WAIT TIME
        QLabel *judgingWaitTimeLabel = new QLabel();
        judgingWaitTimeLabel->setText("Thời gian đợi quét");
        judgingWaitTimeLabel->setStyleSheet(STYLE_BIGLABEL);
        judgingTabLayout->addWidget(judgingWaitTimeLabel);

        QLabel *judgingWaitTimeDesc = new QLabel();
        judgingWaitTimeDesc->setText("Thời gian tạm dừng giữa các lần quét bài làm của học sinh. Hữu dụng khi cần chấm bài trục tuyến mà cần giảm lượng CPU sử dụng cho quá trình chấm bài, tuy nhiên sẽ làm giảm tốc độ chấm bài. (giây)");
        judgingWaitTimeDesc->setWordWrap(true);
        judgingTabLayout->addWidget(judgingWaitTimeDesc);

        QDoubleValidator *judgingWaitTimeInputValidator = new QDoubleValidator(0.0, 100.0, 2); // 0 -> 100 with 2 decimal places
        judgingWaitTimeInput->setValidator(judgingWaitTimeInputValidator);
        judgingTabLayout->addWidget(judgingWaitTimeInput);
        // Connecting action of changing text to a function (Using Lambda)
        connect(judgingWaitTimeInput, &QLineEdit::textChanged, this, [this](const QString text) {
            onInputChanges("wait_time", text.toStdString());
        });


        // RELOAD TIME
        QLabel *judgingReloadTimeLabel = new QLabel();
        judgingReloadTimeLabel->setText("Thời gian làm mới");
        judgingReloadTimeLabel->setStyleSheet(STYLE_BIGLABEL);
        judgingTabLayout->addWidget(judgingReloadTimeLabel);

        QLabel *judgingReloadTimeDesc = new QLabel();
        judgingReloadTimeDesc->setText("Thời gian làm mới máy ảo để kiểm tra bài làm học sinh. Máy ảo được tạo ra để đảm bảo sự an toàn cho những tác vụ chạy trên file bài làm của học sinh mà không cần mất đi tính năng nào như iofstream hay freopen. Thời gian làm mới là thời gian để máy ảo được làm mới nhằm đảm bảo sự an toàn của máy tính giáo viên và xóa những tác vụ đã xảy ra trước đó. (giây)");
        judgingReloadTimeDesc->setWordWrap(true);
        judgingTabLayout->addWidget(judgingReloadTimeDesc);

        QDoubleValidator *judgingReloadTimeInputValidator = new QDoubleValidator(0.0, 100.0, 2); // Same as the one in wait time
        judgingReloadTimeInput->setValidator(judgingReloadTimeInputValidator);
        judgingTabLayout->addWidget(judgingReloadTimeInput);
        // Connecting to a function with lambda
        connect(judgingReloadTimeInput, &QLineEdit::textChanged, this, [this](const QString text) {
            onInputChanges("reload_time", text.toStdString());
        });


        // SHOW TESTS
        QLabel *judgingShowTestLabel = new QLabel();
        judgingShowTestLabel->setText("Hiện đáp án (test)");
        judgingShowTestLabel->setStyleSheet(STYLE_BIGLABEL);
        judgingTabLayout->addWidget(judgingShowTestLabel);

        QLabel *judgingShowTestDesc = new QLabel();
        judgingShowTestDesc->setText("Khi kiểm tra bài thi, kết quả kiểm tra sẽ được ghi ra một tệp kết quả cùng với thông tin chi tiết về quá trình kiểm tra. Cài đặt này sẽ hiện thông tin chi tiết như đầu vào, đầu ra chương trình và đầu ra đáp án.");
        judgingShowTestDesc->setWordWrap(true);
        judgingTabLayout->addWidget(judgingShowTestDesc);

        judgingTabLayout->addWidget(judgingShowTestCheckbox);
        // Connecting to a function w/ lambda (again)
        connect(judgingShowTestCheckbox, &QCheckBox::stateChanged, this, [this](const int state) {
            onInputChanges("show_test", intToString(state));
        });


        // Setting judging tab's layout
        judgingTabLayout->setAlignment(Qt::AlignTop);
        judgingTab->setLayout(judgingTabLayout);


        // +---------------+
        // | Webserver Tab |
        // +---------------+
        /*
         * Like with judging tab, this will have the following options:
         * - Maxmimum not logged in seconds
         * - Aliases:
         *   + Website name
         *   + Slogan
         *   + Host name
         */
        QVBoxLayout *webserverTabLayout = new QVBoxLayout();
        webserverTabLayout->setAlignment(Qt::AlignTop);

        // MAXIMUM NOT LOGGED IN SECONDS
        QLabel *webserverLogInSecsLabel = new QLabel();
        webserverLogInSecsLabel->setText("Thời gian không hoạt động tối đa");
        webserverLogInSecsLabel->setWordWrap(true);
        webserverLogInSecsLabel->setStyleSheet(STYLE_BIGLABEL);
        webserverTabLayout->addWidget(webserverLogInSecsLabel);

        QLabel *webserverLogInSecsDesc = new QLabel();
        webserverLogInSecsDesc->setText("Thời gian tối đa từ lần cuối người dùng hoạt động trên thiết bị của họ cho đến bây giờ. Tính năng này sẽ tự động xóa các dữ liệu thiết bị (không phải dữ liệu người dùng/học sinh) nhằm tiết kiệm bộ nhớ và tăng tốc các hành động. (giây)");
        webserverLogInSecsDesc->setWordWrap(true);
        webserverTabLayout->addWidget(webserverLogInSecsDesc);
        
        QIntValidator *webserverLogInSecsValidator = new QIntValidator(0, 1000000);
        webserverLogInSecsInput->setValidator(webserverLogInSecsValidator);
        webserverTabLayout->addWidget(webserverLogInSecsInput);

        // Applying neccessary layout
        webserverTab->setLayout(webserverTabLayout);


        ///////////////////////
        // Finishing touches //
        ///////////////////////

        // Settings for container
        container->setLayout(mainLayout);

        // Settings for QMainWindow
        setCentralWidget(container);
    }

    void initialize() {
        loadThemeColors();
        judgingEnabled = false;
        webserverEnabled = false;
    }

    void about() {
        QWidget *aboutFrame = new QWidget();
        aboutFrame->setFixedSize(500, 250);
        aboutFrame->show();
        aboutFrame->setWindowTitle("Về ATOMIC");
        aboutFrame->setWindowIcon(QIcon(iconPixmap));
        aboutFrame->setObjectName("about");
        
        aboutFrame->setStyleSheet(styleSheetResult); // Setting styles

        // Reading version info
        std::fstream versionFile(dirPath + VERSION_PATH, std::ios::in);
        if (versionFile.is_open()) {
            // If file is working as intended
            try {
                version = json::parse(versionFile);

                std::string stableText = "";
                if (version["stable"]) {
                    stableText = "Yes";
                } else {
                    stableText = "No";
                }
                std::string versionText = "Codename " + std::string(version["version"]) + '\n' +
                                        "API Version: " + std::string(version["api"]) + '\n' +
                                        "Branch: " + std::string(version["branch"]) + '\n' +
                                        "Sig: " + std::string(version["signature"]) + '\n' +
                                        "Stable: " + stableText;

                std::cout << versionText << '\n';

                // The rest of the operation stays here
                QLabel *versionLabel = new QLabel();
                versionLabel->setText(QString::fromStdString(versionText));
                // versionLabel->setWordWrap(true);

                // Adding Image
                QLabel *appImage = new QLabel();
                QPixmap iconPixmapScaled = iconPixmap.scaled(QSize(75, 75), Qt::KeepAspectRatio);
                appImage->setPixmap(iconPixmapScaled);
                appImage->setAlignment(Qt::AlignRight);

                // Adding in splitter to split between a huge-sized version of 
                // iconPixmap and version info
                QSplitter *splitter = new QSplitter();
                splitter->setOrientation(Qt::Orientation::Horizontal);
                splitter->setChildrenCollapsible(false);
                splitter->addWidget(versionLabel);
                splitter->addWidget(appImage);

                // Setting width for easy splitting
                splitter->setFixedHeight(75);

                // This text is like this because of some shit.
                // It's just cause of C's poor strings...
                QString licensingText = "====== Giấy phép ======\n\nPhần mềm này được phát hành dưới Giấy phép Mã nguồn mở MIT (MIT License).\n\n====== Ghi nhận ======\n\nPhần mềm được phát triển dựa trên hoặc sử dụng các dự án mã nguồn mở sau:\n\n- Qt GUI Framework (https://qt.io)  \n- Python Interpreter (https://python.org)  \n- waitress (https://github.com/Pylons/waitress)  \n- Docker (https://docker.com)  \n- nlohmann/json - JSON for Modern C++ (https://github.com/nlohmann/json)  \n- boppreh/keyboard - Python keyboard library (https://github.com/boppreh/keyboard)";

                QSplitter *licensingSplitter = new QSplitter();
                licensingSplitter->setOrientation(Qt::Orientation::Vertical);
                licensingSplitter->setChildrenCollapsible(false);
                licensingSplitter->addWidget(splitter);
                
                QTextEdit *licensingInfo = new QTextEdit();
                licensingInfo->setText(licensingText);
                licensingInfo->setReadOnly(true);
                licensingInfo->setObjectName("aboutLicensing");
                licensingInfo->setFont(monospaceFont);
                licensingSplitter->addWidget(licensingInfo);

                QVBoxLayout *layout = new QVBoxLayout();
                layout->addWidget(licensingSplitter);
                aboutFrame->setLayout(layout);
            } catch (const json::parse_error& e) { 
                // If error got and it is JSON parsing error
                errorDialog("Tệp phiên bản đã bị hỏng. Vui lòng cập nhật để tự động sửa lỗi hoặc cài đặt lại chương trình.");
                close();
                exit(0);
            }
        }
        else {
            errorDialog("Tệp thông tin phiên bản đã bị hỏng. Vui lòng cập nhật để tự động sửa lỗi hoặc cài đặt lại chương trình.");
            aboutFrame->close();
        }
    }

    void loadThemeColors() {
        std::fstream themeColorsFile(dirPath + THEMECOLORS_PATH, std::ios::in);

        // Default values
        COLOR_CONSOLE_ERROR = QColor(185, 0, 0);
        COLOR_CONSOLE_WARNING = QColor(185, 153, 0);
        COLOR_CONSOLE_OK = QColor(0, 185, 0);
        COLOR_CONSOLE_DEFAULT = QColor(0, 0, 0);

        if (themeColorsFile.is_open()) {
            std::stringstream stream;
            stream << themeColorsFile.rdbuf();
            QString content = QString::fromStdString(stream.str());
            std::cout << content.toStdString() << '\n';

            // Now, we processes the whole thing first by removing spaces
            content = content.remove(" ", Qt::CaseSensitive);
            // Next split them into LINES
            QStringList lines = content.split('\n');

            // Finally, browse through each of them
            for (QString &line : lines) {
                if (line[0] == '#') continue;

                QStringList parts = line.split('=');
                QString name = parts[0];
                QString value = parts[1];

                if (name == "COLOR_CONSOLE_ERROR") {
                    QStringList rgb = value.split(',');
                    bool ok;
                    int valR = rgb[0].toInt(&ok);
                    int valG = rgb[1].toInt(&ok);
                    int valB = rgb[2].toInt(&ok);

                    if (ok) COLOR_CONSOLE_ERROR = QColor(valR, valG, valB);
                    else continue;
                } else if (name == "COLOR_CONSOLE_WARNING") {
                    QStringList rgb = value.split(',');
                    bool ok;
                    int valR = rgb[0].toInt(&ok);
                    int valG = rgb[1].toInt(&ok);
                    int valB = rgb[2].toInt(&ok);

                    if (ok) COLOR_CONSOLE_WARNING = QColor(valR, valG, valB);
                    else continue;
                } else if (name == "COLOR_CONSOLE_OK") {
                    QStringList rgb = value.split(',');
                    bool ok;
                    int valR = rgb[0].toInt(&ok);
                    int valG = rgb[1].toInt(&ok);
                    int valB = rgb[2].toInt(&ok);

                    if (ok) COLOR_CONSOLE_OK = QColor(valR, valG, valB);
                    else continue;
                } else if (name == "COLOR_CONSOLE_DEFAULT") {
                    QStringList rgb = value.split(',');
                    bool ok;
                    int valR = rgb[0].toInt(&ok);
                    int valG = rgb[1].toInt(&ok);
                    int valB = rgb[2].toInt(&ok);

                    if (ok) COLOR_CONSOLE_DEFAULT = QColor(valR, valG, valB);
                    else continue;
                }
            }
        }
    }

    void gitHub() {
        // Executing the github repo into Windows
        ShellExecute(0, "open", GITHUB_PAGE.c_str(), 0, 0, SW_SHOWNORMAL);
    }

    void openHttpWebsite() {
        // Opening the opened HTTP website
        ShellExecute(0, "open", "http://127.0.0.1/", 0, 0, SW_SHOWNORMAL);
    }

    void backUp() {

    }

    void loadBackUp() {

    }

    private: // PRIVATE FUNCTIONS. These cannot be connected to outside of whatever this object is.

    // -------------------------------------------------
    // Purpose: [GROUPED] (onSidebarFeatureButtonPushed, 
    //          readyJudgingOutput, stoppedJudging)
    //          For process-related functionalities
    // -------------------------------------------------
    void onSidebarFeatureButtonPushed(std::string type) {
        // When the sidebar's button is pushed on
        // Which are all features toggler

        if (type == "judging") {
            // If the button pushed is a judging button
            // => Enable the judging feature / system

            // Changing status
            judgingEnabled = !judgingEnabled;
            std::cout << "Current JUDGING status: " << (judgingEnabled ? "Enabled" : "Disabled") << "\n";
            
            if (judgingEnabled) {
                // If JUDGING is now enabled, performs the enable action

                judgingProcess->start();
                std::cout << "Started JUDGING process: " << judgingProcess->program().toStdString() << " " << judgingProcess->arguments().join(' ').toStdString() << '\n';

                if (judgingProcess->state() == QProcess::Running) {
                    // If judgingProcess ran without errors
                    std::cout << "Successfully ran JUDGING process with PID " << judgingProcess->processId() << "\n";
                    judgingProcessButton->setText("Dừng chấm bài");
                } else {
                    std::cout << "Process failed to start. Error: " << judgingProcess->error() << '\n'; 
                    errorDialog("Thất bại trong quá trình mở hệ thống chấm bài. Hãy thử cài đặt lại chương trình hoặc cập nhật lên bản cập nhật mới nhất");
                }
            }
            else {
                // Turn off JUDGING process via created socket HOLE
                SOCKET exitSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (exitSocket == INVALID_SOCKET) {
                    std::cout << "exitSocket: INVALID_SOCKET\n";
                    errorDialog("Lỗi tạo dựng SOCKET. Chương trình sẽ thoát và hãy mở lại chương trình. Thiết bị của bạn có thể không tương thích với ATOMIC.");
                    WSACleanup();
                    close();
                    // judgingProcess->terminate();
                    exit(0);
                }

                // Marking addresses for impending connection
                sockaddr_in targetAddr;
                targetAddr.sin_family = AF_INET;
                targetAddr.sin_port = htons(JUDGING_EXITPORT);
                targetAddr.sin_addr.s_addr = inet_addr(JUDGING_EXITADDR.c_str());

                // Connecting to target with Winsock's connect
                if (::connect(exitSocket, (SOCKADDR*)&targetAddr, sizeof(targetAddr)) == SOCKET_ERROR) {
                    // Connection failure:
                    // Connect to target returned SOCKET_ERROR

                    errorDialog("Kết nối đến hệ thống chấm bài không thành công. Vui lòng đợi một chút và thử lại sau. Nếu vẫn không thành công thì hãy thử khởi động lại hệ thống.");
                    closesocket(exitSocket);
                }
                else {
                    const char* exitMsg = "exit";
                    send(exitSocket, exitMsg, strlen(exitMsg), 0);

                    // No need for recieving

                    // Now, we have to set the button disabled because the user might just spam it up so...
                    judgingProcessButton->setEnabled(false);
                }
            }
        } else if (type == "webserver") {
            webserverEnabled = !webserverEnabled;
            std::cout << "Toggled webserver to: " << (webserverEnabled ? "Enabled" : "Disabled") << '\n';
            if (webserverEnabled) {
                // If we just turned on webserver. We do the actions of turning it up
                webserverProcess->start();
                std::cout << "Started WEBSERVER process: " << webserverProcess->program().toStdString() << " " << webserverProcess->arguments().join(' ').toStdString() << '\n';

                // Check if the process started normally
                if (webserverProcess->state() == QProcess::Running) {
                    // If webserverProcess ran without errors
                    std::cout << "Successfully ran WEBSERVWER process with PID " << webserverProcess->processId() << "\n";
                    webserverProcessButton->setText("Tắt Website");
                } else {
                    std::cout << "Process failed to start. Error: " << webserverProcess->error() << '\n'; 
                    errorDialog("Thất bại trong quá trình mở hệ thống chấm bài. Hãy thử cài đặt lại chương trình hoặc cập nhật lên bản cập nhật mới nhất");
                }
            } else {
                // Turning off actions
                // Turn off JUDGING process via created socket HOLE
                SOCKET exitSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (exitSocket == INVALID_SOCKET) {
                    std::cout << "exitSocket: INVALID_SOCKET\n";
                    errorDialog("Lỗi tạo dựng SOCKET. Chương trình sẽ thoát và hãy mở lại chương trình. Thiết bị của bạn có thể không tương thích với ATOMIC.");
                    WSACleanup();
                    close();
                    // judgingProcess->terminate();
                    exit(0);
                }

                // Marking addresses for impending connection
                sockaddr_in targetAddr;
                targetAddr.sin_family = AF_INET;
                targetAddr.sin_port = htons(WEBSERVER_EXITPORT);
                targetAddr.sin_addr.s_addr = inet_addr(WEBSERVER_EXITADDR.c_str());

                // Connecting to target with Winsock's connect
                if (::connect(exitSocket, (SOCKADDR*)&targetAddr, sizeof(targetAddr)) == SOCKET_ERROR) {
                    // Connection failure:
                    // Connect to target returned SOCKET_ERROR

                    errorDialog("Kết nối đến hệ thống website không thành công. Vui lòng đợi một chút và thử lại sau. Nếu vẫn không thành công thì hãy thử khởi động lại hệ thống.");
                    closesocket(exitSocket);
                }
                else {
                    const char* exitMsg = "exit";
                    send(exitSocket, exitMsg, strlen(exitMsg), 0);

                    // No need for recieving

                    // Now, we have to set the button disabled because the user might just spam it up so...
                    webserverProcessButton->setEnabled(false);
                }
            }
        }
    }

    // Add these two methods:
    void judgingHandleStandardOutput() {
        QString output = QString::fromUtf8(judgingProcess->readAllStandardOutput());
        judgingProcessOutput(output);
    }

    void judgingHandleStandardError() {
        QString output = QString::fromUtf8(judgingProcess->readAllStandardError());
        judgingProcessOutput(output);
    }

    // Move your processing function to its own method:
    void judgingProcessOutput(const QString& source) {
        QStringList lines = source.split('\n');

        for (const QString& originalLine : lines) {
            // Remove trailing '\r' while preserving Unicode characters
            QString line = originalLine;
            line.remove('\r');

            if (line.isEmpty())
                continue;

            // Check for starting ANSI escape code
            if (!line.isEmpty() && line[0] == QChar('\x1b')) {
                QString colorCode;
                int pos = 0;

                // Extract ANSI escape sequence
                while (pos < line.length() && line[pos] != QChar('m')) {
                    colorCode += line[pos];
                    pos++;
                }

                if (pos < line.length() && line[pos] == QChar('m')) {
                    colorCode += 'm';
                    pos++;
                }

                line = line.mid(pos); // Remove ANSI code prefix

                // Set text color based on ANSI code
                if (colorCode == QString::fromStdString(INFO_COL))
                    judgingProcessConsole->setTextColor(COLOR_CONSOLE_DEFAULT);
                else if (colorCode == QString::fromStdString(ERROR_COL))
                    judgingProcessConsole->setTextColor(COLOR_CONSOLE_ERROR);
                else if (colorCode == QString::fromStdString(WARN_COL))
                    judgingProcessConsole->setTextColor(COLOR_CONSOLE_WARNING);
                else if (colorCode == QString::fromStdString(OK_COL))
                    judgingProcessConsole->setTextColor(COLOR_CONSOLE_OK);
            }

            // Remove any ending ANSI color code
            int ansiPos = line.indexOf(QChar('\x1b'));
            if (ansiPos != -1) {
                line = line.left(ansiPos);
            }

            // Append line and reset color
            judgingProcessConsole->append(line);
            judgingProcessConsole->setTextColor(COLOR_CONSOLE_DEFAULT);
        }
    }

    void stoppedJudging(int exitCode, QProcess::ExitStatus exitStatus) {
        judgingProcessButton->setEnabled(true);
        judgingProcessButton->setText("Bật chấm bài");
        judgingEnabled = false;

        // RESET some stuffs
        judgingProcessConsole->setTextColor(COLOR_CONSOLE_DEFAULT);
        judgingProcessConsole->append("\n\n");

        std::cout << "Exitted with exit code: " << exitCode << '\n';

        // Exit code handling
        if (exitCode != 0) {
            // Preparing variables as default values
            QString windowTitle = "Lỗi trong quá trình chấm bài.";
            QString message = "Đã có lỗi xảy ra trong quá trình chấm bài. Hãy tra trong Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết. Mã lỗi: ";
            message += QString::fromStdString(intToString(exitCode));

            // Use switch/case for better handling of these jobs and more optimized
            // IF/ELSEs
            switch(exitCode) {
                // Error dictionary in `judge.py`'s first lines
                case -127:
                    // Docker environment error
                    message = "Lỗi -127: Thất bại trong quá trình kết nối đến môi trường Docker. Hãy chắc chắn rằng bạn đã mở Docker Desktop và bật Docker Environment trên thiết bị. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -126:
                    // Docker connection error
                    message = "Lỗi -126: Môi trường Docker không phản hồi. Xin đợi và thử lại sau. Nếu không thành công, hãy thử khởi động lại Docker hoặc tra trong Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -125:
                    // Docker compilation image building error
                    message = "Lỗi -125: Ảnh biên soạn không được xây dựng thành công. Hãy thử khởi động lại hệ thống chấm bài. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -124:
                    // Docker compile container failed to start
                    message = "Lỗi -124: Container biên soạn không được bật thành công. Hãy thử khởi động lại hệ thống chấm bài. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -123:
                    // Judging disabling socket failed to start
                    message = "Lỗi -123: Socket không được bật thành công. Hãy kiểm tra xem có phần mềm nào đang sử dụng cổng ";
                    message += QString::fromStdString(intToString(JUDGING_EXITPORT));
                    message += " hay không. Nếu có thì hãy tắt phần mềm đó. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -122:
                    // Execution image building error
                    message = "Lỗi -122: Ảnh thực hiện không được xây dựng thành công. Hãy thử khởi động lại hệ thống chấm bài. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -121:
                    // Docker compile container failed to start
                    message = "Lỗi -121: Container thực hiện không được bật thành công. Hãy thử khởi động lại hệ thống chấm bài. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
            }

            // Showing the error
            QMessageBox *msgBox = new QMessageBox();
            msgBox->setText(message);
            msgBox->setWindowTitle(windowTitle);
            msgBox->setIcon(QMessageBox::Critical);
            msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
            // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
            msgBox->setWindowIcon(QIcon(iconPixmap));

            msgBox->show();
        } else {
            // If the software exitted gracefully, we do a little bit of windowin'
            // by simply showing a response to assure the user nothing is BAD
            QMessageBox *msgBox = new QMessageBox();
            msgBox->setText("Dừng hệ thống chấm bài thành công!");
            msgBox->setWindowTitle("Thành công");
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
            // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
            msgBox->setWindowIcon(QIcon(iconPixmap));

            msgBox->show();
        }
    }

    void stoppedWebserver(int exitCode, QProcess::ExitStatus exitStatus) {
        // Handle webserver exitting, along with creating log files.
        webserverProcessButton->setText("Bật Website");
        webserverProcessButton->setEnabled(true);
        webserverEnabled = false;

        if (exitCode != 0) {
            // Error happened during execution
            QMessageBox::StandardButton reply;

            reply = QMessageBox::critical(this, "Lỗi trong web chấm bài trực tuyến", 
                                  "Đã có lỗi xảy ra trong khi đang mở website chấm bài trực tuyến. Mở nhật ký đầu ra chương trình (Tiếng Anh, Nâng cao)?",
                                  QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                // Open logfile
                std::cout << "Opened Webserver's log file.\n";
                ShellExecute(0, "open", (dirPath + LOG_PATH).c_str(), 0, 0, SW_SHOWNORMAL);
            }
        } else {
            // Alright its fine
            // QMessageBox::information(this, "Đã tắt trang web chấm bài trực tuyến");
        }
    }

    // ------------------------------------------------
    // Purpose: Refresh current configurations on each
    //          tab switch to update contents
    // ------------------------------------------------
    void onTabSwitches(int index) {
        // When user switches tabs. It's good to update all contents of ALL TABS.
        // This is just for the sake of accuracy. Future optimizations might
        // reduce the stress on refreshing.
        std::cout << "onTabSwitches(): Called. currentSwitched.\n";

        // Reading data into variables related to them
        std::fstream settingsFile(dirPath + SETTINGS_PATH, std::ios::in);
        if (settingsFile.is_open()) {
            // If file is open-able (Works as intended)
            try {
                settings = json::parse(settingsFile);

                // If file has successfully been parsed
                // std::cout << "settings[\"wait_time\"] = " << settings["wait_time"] << '\n';
                std::cout << settings << '\n';

                // Applying current settings from fcking JSON into Qt Line Input elements
                judgingWaitTimeInput->setText(QString::fromStdString(doubleToString(settings["wait_time"])));
                judgingReloadTimeInput->setText(QString::fromStdString(doubleToString(settings["reload_time"])));
                judgingShowTestCheckbox->setCheckState(settings["show_test"] ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

            } catch (const json::parse_error& e) { 
                // If error got and it is JSON parsing error
                errorDialog("Tệp cài đặt đã bị hỏng. Hãy cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
            settingsFile.close();
        }
        else {
            errorDialog("Tệp cài đặt không tồn tại. Hãy cài đặt lại ứng dụng để sửa lỗi.");
            close();
            exit(0);
        }
    }

    void errorDialog(std::string error) {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText(QString::fromStdString("Đã có lỗi xảy ra: " + error));
        msgBox->setWindowTitle("Lỗi");
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
        // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
        msgBox->setWindowIcon(QIcon(iconPixmap));

        msgBox->show();
    }

    // -------------------------------------------
    // Purpose: Change settings for inputs from a
    //          predetermined 'type' value
    // -------------------------------------------
    void onInputChanges(std::string type, std::string value) {
        std::cout << "onInputChanges(std::string type, std::string value): " << type << ": -> " << value << '\n';

        // Checking which type the input belongs to
        if (type == "wait_time" || type == "reload_time" || type == "show_test") {
            std::fstream settingsFile(dirPath + SETTINGS_PATH, std::ios::out);

            // Checking which value does the input correspond to
            if (type == "wait_time") {
                // Applying changes
                settings["wait_time"] = stringToDouble(value);
            } else if (type == "reload_time") {
                // This is reload time. So we will change the reload_time key
                settings["reload_time"] = stringToDouble(value);
            } else if (type == "show_test") {
                // Alright. This is showing tests, so we save into show_test key
                // std::cout << "!!!";
                settings["show_test"] = value == "2" ? true : false;
            }

            if (settingsFile.is_open()) {
                // Write the result
                settingsFile << settings;

                // Flush I/O
                settingsFile.close();
            } else {
                errorDialog("Tệp cài đặt không tồn tại. Hãy cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
        }
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Parse command line to check for --console
    int argc;
    LPWSTR* argv_w = CommandLineToArgvW(GetCommandLineW(), &argc);

    // std::string argOld;
    
    // Check for console argument
    bool hideConsole = true;
    for (int i = 1; i < argc; ++i) {
        // Convert wide string to regular string for comparison
        int size = WideCharToMultiByte(CP_UTF8, 0, argv_w[i], -1, NULL, 0, NULL, NULL);
        char* arg = new char[size];
        WideCharToMultiByte(CP_UTF8, 0, argv_w[i], -1, arg, size, NULL, NULL);
        
        // argOld = std::string(arg);

        if (std::string(arg) == "--console") {
            hideConsole = false;
        }
        
        delete[] arg;
    }
    
    // For Qt applications, we need to properly convert args to pass to QApplication
    int qt_argc = argc;
    char** qt_argv = new char*[argc];
    
    for (int i = 0; i < argc; i++) {
        int size = WideCharToMultiByte(CP_UTF8, 0, argv_w[i], -1, NULL, 0, NULL, NULL);
        qt_argv[i] = new char[size];
        WideCharToMultiByte(CP_UTF8, 0, argv_w[i], -1, qt_argv[i], size, NULL, NULL);
    }
    
    // Free the wide command line argument memory now that we're done with it
    LocalFree(argv_w);
    
    if (!hideConsole) {
        // Allocate a console
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);
        
        std::cout << "Console attached!\n";
        std::cout << "Application running with console support.\n";
    }

    // std::cout << argOld << '\n';

    // Initializing winsock
    std::cout << "Initializing Winsock\n";
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed\n";
        QMessageBox::critical(nullptr, "Lỗi mở WINSOCK", 
                                    "Đã gặp lỗi thiết lập WINSOCK. Thiết bị của bạn có thể không tương thích với phần mềm này", 
                                    QMessageBox::StandardButton::Ok);
        
        // Clean up before returning
        for (int i = 0; i < qt_argc; i++) {
            delete[] qt_argv[i];
        }
        delete[] qt_argv;
        
        return 1;
    }

    // Main app - use qt_argc and qt_argv here
    QApplication a(qt_argc, qt_argv);
    PanelWindow panel;
    panel.initialize();
    panel.show();
    int app_result = a.exec();
    
    // Clean up memory
    for (int i = 0; i < qt_argc; i++) {
        delete[] qt_argv[i];
    }
    delete[] qt_argv;
    
    return app_result;
}
