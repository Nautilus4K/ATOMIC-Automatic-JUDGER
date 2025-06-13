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
#include <QtWidgets/QApplication>      // For application
#include <QtWidgets/QMainWindow>       // For main window
#include <QtWidgets/QWidget>           // For a window-like area
#include <QtWidgets/QVBoxLayout>       // Verical Layout of QWidgets
#include <QtWidgets/QHBoxLayout>       // Horizontal Layout of QWidgets
#include <QtWidgets/QScrollArea>       // Scrollable area. Used with QWidgets
#include <QtWidgets/QFrame>            // Frames. Like the one outline in QScrollArea
#include <QtWidgets/QSplitter>         // Splitter
#include <QtWidgets/QTabWidget>        // Tabs
#include <QtWidgets/QMenuBar>          // Menu bar (The toolbar on window)
#include <QtWidgets/QMenu>             // Menu (Entries in QMenuBar)
#include <QtWidgets/QMessageBox>       // Message boxes
#include <QtWidgets/QLabel>            // Labels
#include <QtWidgets/QLineEdit>         // An edit area that is a line edit. No multiple lines.
#include <QtWidgets/QCheckBox>         // Checkboxes. Of course. What did you expect, sucker?
#include <QtWidgets/QPushButton>       // Buttons as we know it
#include <QtWidgets/QTextEdit>         // A text frame
#include <QtWidgets/QComboBox>         // Dropdown. Yeah. Qt is very nice with their names
#include <QtWidgets/QTableWidget>      // Table of content
#include <QtWidgets/QStyleFactory>     // Styling. Once again
#include <QtWidgets/QDialog>           // Dialogs, even custom ones
#include <QtWidgets/QRadioButton>      // Radio buttons
#include <QtWidgets/QButtonGroup>      // Grouping multiple buttons
#include <QtWidgets/QListWidget>       // List view BUT able to put multiple elements into this
#include <QtWidgets/QListView>         // QListWidget but worse. However, much more lightweight (Also not as simple).
#include <qtWidgets/QInputDialog>      // INPUT.IN.A.DIALOG. Yessssirrrr
#include <QtGui/QAction>               // Action for menus. Wonder what fucker thought to put it in QtGui
#include <QtGui/QCloseEvent>           // Close event. The action of 'X' button
#include <QtGui/QDoubleValidator>      // Validator for edits.
#include <QtGui/QIntValidator>         // Another validator, also for edits, now more like INTEGER revolved.
#include <QtGui/QIcon>                 // Icon readings
#include <QtGui/QPixmap>               // Picture reading
#include <QtGui/QFont>                 // Fonts
#include <QtGui/QColor>                // Colors
#include <QtGui/QTextDocumentFragment> // Producing a fragment of an HTML document. Sorta like a wrapper cleaner for HTMLs

// Importing Qt related features
#include <QtCore/Qt>       // A bunch of constants
#include <QtCore/QProcess> // Process running
#include <QtCore/QStringListModel> // QStringList model? Idk

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
#include "Windows.h"
#include "winsock2.h" // Socket programming
#include "shellapi.h" // ShellExecuteA, etc...
#include "ws2tcpip.h"
#include <filesystem> // File managing

#pragma comment(lib, "Ws2_32.lib") // Required library to link with

// C++ Features
#include <vector>
#include <map>
#include <sstream>

// Debug console
#include <iostream>

// Custom shits
#include "utilities.h"
#include "consts.h"
#include "CST_RadioButtonDialog.h"
#include "CST_TextEditorDialog.h"
#include "CST_RichTextEdit.h"
#include "CST_Listing.h"
#include "WIN_ContestsSettings.h"
#include "WIN_UsersSettings.h"

// -> Color values
QColor COLOR_CONSOLE_ERROR;
QColor COLOR_CONSOLE_WARNING;
QColor COLOR_CONSOLE_OK;
QColor COLOR_CONSOLE_DEFAULT;

// --------------------------------------
// Main panel window for all operations.
// --------------------------------------
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
    QLineEdit *webserverAliasWebnameInput = new QLineEdit();
    QLineEdit *webserverAliasSloganInput = new QLineEdit();
    QLineEdit *webserverAliasHostnameInput = new QLineEdit();

    // Main manager for the management of data and profiles
    QComboBox *classDropdown = new QComboBox();
    QTableWidget *currentTable = new QTableWidget();

    // Sidebar elements
    QPushButton *judgingProcessButton = new QPushButton();
    QTextEdit *judgingProcessConsole = new QTextEdit();
    QPushButton *webserverProcessButton = new QPushButton();

    // Data variables
    json settings; // NULL at first
    json version; // NULL at first
    json aliases; // NULL at first
    json classes; // Classes object. NULL at first
    json contests; // Contests object. NULL at first
    json users; // Users object. NULL at first
    QPixmap iconPixmap;
    QFont monospaceFont; // NULL at first
    QString styleSheetResult;
    
    // Processes
    QProcess *judgingProcess = new QProcess();
    bool judgingEnabled;
    QProcess *webserverProcess = new QProcess();
    bool webserverEnabled;

    // Fine shyt
    QTabWidget *tabs = new QTabWidget();

    // Settings bars
    QPushButton *contestsSettings = new QPushButton(this);
    QPushButton *usersSettings = new QPushButton(this);

    PanelWindow(QWidget *parent) : QMainWindow(parent) { // Extremely powerful? Extremely complex.
        // This is the configuration part of panelWindow.
        // Initialization will be another one

        ///////////////////////////////////
        // Setting window configurations //
        ///////////////////////////////////
        setWindowTitle("Bảng điều khiển ATOMIC");
        resize(QSize(950, 550));
        setMinimumSize(QSize(600, 400));
        setAttribute(Qt::WA_DeleteOnClose);

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
        exitApp->setIcon(QIcon(EXITICON_PATH));
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
        sidebar->setMaximumWidth(350);

        QVBoxLayout *sidebarLayout = new QVBoxLayout();

        // JUDGING
        QLabel *judgingProcessLabel = new QLabel();
        judgingProcessLabel->setText("Hệ thống chấm bài");
        judgingProcessLabel->setWordWrap(true);
        sidebarLayout->addWidget(judgingProcessLabel);

        judgingProcessButton->setText("Bật chấm bài");
        sidebarLayout->addWidget(judgingProcessButton);
        // Connect to desired function
        connect(judgingProcessButton, &QPushButton::clicked, this, [this] {
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
        
        // On tab switches action
        connect(tabs, &QTabWidget::currentChanged, this, &PanelWindow::onTabSwitches);
        
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

        // +------------+
        // | Manage tab |
        // +------------+
        QVBoxLayout *manageTabLayout = new QVBoxLayout();

        // Adding the neccessary settings as buttons on a horizontal grid.
        // To achieve this, we need a QScrollArea, QWidget and a QHBoxLayout for that QWidget
        QWidget *settingsLine = new QWidget(this);
        QHBoxLayout *settingsLayout = new QHBoxLayout(settingsLine);

        const int btnWidth = 55, btnHeight = 55;  // Perfect square

        contestsSettings->setObjectName("genericBtn");
        contestsSettings->setToolTip("Cài đặt bài thi");
        contestsSettings->setFixedHeight(btnHeight); contestsSettings->setFixedWidth(btnWidth);
        // Applying icon
        QPixmap contestsPXMP(CONTESTSICON_PATH);
        QIcon contestsIcon(contestsPXMP);
        contestsSettings->setIcon(contestsIcon);
        contestsSettings->setIconSize(QSize(btnWidth, btnHeight));
        connect(contestsSettings, &QPushButton::clicked, this, [this] {
            showButtonInfoFromBarType("contests");
        });

        // Now for user settings
        usersSettings->setObjectName("genericBtn");
        usersSettings->setToolTip("Cài đặt người dùng");
        usersSettings->setFixedHeight(btnHeight); usersSettings->setFixedWidth(btnWidth);
        QPixmap usersPXMP(USERSICON_PATH);
        QIcon usersIcon(usersPXMP);
        usersSettings->setIcon(usersIcon);
        usersSettings->setIconSize(QSize(btnWidth, btnHeight));
        connect(usersSettings, &QPushButton::clicked, this, [this] {
            showButtonInfoFromBarType("users");
        });

        // Adding in
        settingsLayout->addWidget(contestsSettings);
        settingsLayout->addWidget(usersSettings);
        settingsLayout->setContentsMargins(0, 0, 0, 0);

        settingsLine->setLayout(settingsLayout);

        // Managing scrollablity
        QScrollArea *settingsLineScrollable = new QScrollArea(this);
        settingsLineScrollable->setWidget(settingsLine);
        
        // Calculating height
        int stLeft, stTop, stRight, stBottom;
        settingsLayout->getContentsMargins(&stLeft, &stTop, &stRight, &stBottom);

        int totalHeight = btnHeight + stTop + stBottom;
        settingsLineScrollable->setFixedHeight(totalHeight);

        // Overriding mouse events
        settingsLine->setMouseTracking(true);
        settingsLineScrollable->setMouseTracking(true);
        contestsSettings->setMouseTracking(true);
        settingsLineScrollable->viewport()->setMouseTracking(true);
        contestsSettings->setFocusPolicy(Qt::NoFocus);

        manageTabLayout->addWidget(settingsLineScrollable);

        // Main table part
        classDropdown->setFixedWidth(100);
        QWidget *classDropdownPopUp = classDropdown->view()->window();
        classDropdownPopUp->setAttribute(Qt::WA_TranslucentBackground);        // Allow transparency
        classDropdownPopUp->setWindowFlag(Qt::FramelessWindowHint, true);      // Remove window frame
        classDropdownPopUp->setWindowFlag(Qt::NoDropShadowWindowHint, true);   // Disable drop shadow

        connect(classDropdown, &QComboBox::currentIndexChanged, this, [this] {
            refreshTable();
        });

        manageTabLayout->addWidget(classDropdown);
        
        currentTable->setContextMenuPolicy(Qt::CustomContextMenu); // Uses custom context menu
        connect(currentTable, &QTableWidget::customContextMenuRequested, this, &PanelWindow::showScoreContextMenu);
        manageTabLayout->addWidget(currentTable);

        // Settings
        manageTab->setLayout(manageTabLayout);
        

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
            onInputChanges("wait_time", text);
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
            onInputChanges("reload_time", text);
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
            onInputChanges("show_test", QString::fromStdString(intToString(state)));
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
        
        QIntValidator *webserverLogInSecsValidator = new QIntValidator(0, 10000000); // Equivalent of nearly 116 days of not logged in (or 1158 if you count the variations)
        webserverLogInSecsInput->setValidator(webserverLogInSecsValidator);
        webserverTabLayout->addWidget(webserverLogInSecsInput);

        // Connect the action of changing settings to a function through connect and lambda
        connect(webserverLogInSecsInput, &QLineEdit::textChanged, this, [this](const QString &value) {
            onInputChanges("maximum_login_seconds", value);
        });

        // ALIASES
        QLabel *webserverAliasLabel = new QLabel();
        webserverAliasLabel->setText("Thông tin trang web");
        webserverAliasLabel->setStyleSheet(STYLE_BIGLABEL);
        webserverTabLayout->addWidget(webserverAliasLabel);

        // Alias: Website name
        QLabel *webserverAliasWebnameLabel = new QLabel();
        webserverAliasWebnameLabel->setText("Tên Website (hoặc lớp học. VD: Trang web chấm bài tự động, lớp học chuyên tin thầy N...):");
        webserverAliasWebnameLabel->setWordWrap(true);
        webserverAliasWebnameLabel->setStyleSheet(STYLE_BOLDLABEL);
        webserverTabLayout->addWidget(webserverAliasWebnameLabel);

        webserverTabLayout->addWidget(webserverAliasWebnameInput);

        // Connecting to function
        connect(webserverAliasWebnameInput, &QLineEdit::textChanged, this, [this](const QString &value) {
            onInputChanges("website_name", value);
        });

        // Alias: Slogan
        QLabel *webserverAliasSloganLabel = new QLabel();
        webserverAliasSloganLabel->setText("Khẩu hiệu (VD: Càng học càng vui; keep typing, keep loving; Trên bước đường thành công không có dấu chân của kẻ thất bại...):");
        webserverAliasSloganLabel->setWordWrap(true);
        webserverAliasSloganLabel->setStyleSheet(STYLE_BOLDLABEL);
        webserverTabLayout->addWidget(webserverAliasSloganLabel);

        webserverTabLayout->addWidget(webserverAliasSloganInput);

        connect(webserverAliasSloganInput, &QLineEdit::textChanged, this, [this](const QString &value) {
            onInputChanges("slogan", value);
        });

        // Alias: Host name
        QLabel *webserverAliasHostnameLabel = new QLabel();
        webserverAliasHostnameLabel->setText("Tên chủ sở hữu (hoặc quản trị viên. VD: Nguyễn Văn A, Lê Văn B, Lê Quang N...):");
        webserverAliasHostnameLabel->setWordWrap(true);
        webserverAliasHostnameLabel->setStyleSheet(STYLE_BOLDLABEL);
        webserverTabLayout->addWidget(webserverAliasHostnameLabel);

        webserverTabLayout->addWidget(webserverAliasHostnameInput);

        connect(webserverAliasHostnameInput, &QLineEdit::textChanged, this, [this](const QString &value) {
            onInputChanges("hostname", value);
        });

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

        // Settings boxes info
        contestOpened = false;
        userOpened = false;

        // Make sure segfault doesn't happen for no fucking reason, like, dude. Idk man
        // It only happens when I click on the contest page immediately after I opened
        // and not after I switched some tabs, and I suspect it might be because of the
        // fact that the whole thing wasn't ready yet. So now we have this
        // tabs->setCurrentIndex(0);
        // tabs->setCurrentIndex(1);
        // tabs->setCurrentIndex(2);

        // Apparently, it doesn't work. And will never work.
        // I am just gonna leave it there

        // Attaching event filter
        manageTab->installEventFilter(this);
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
                QString licensingText = "====== Giấy phép ======\n\nPhần mềm này được phát hành dưới Giấy phép Mã nguồn mở MIT (MIT License) đã qua sửa đổi. Thông tin chi tiết truy cập trang dự án GitHub.\n\n====== Ghi nhận ======\n\nPhần mềm được phát triển dựa trên hoặc sử dụng các dự án mã nguồn mở sau:\n\n- Qt GUI Framework (https://qt.io)  \n- Python Interpreter (https://python.org)  \n- waitress (https://github.com/Pylons/waitress)  \n- Docker (https://docker.com)  \n- nlohmann/json - JSON for Modern C++ (https://github.com/nlohmann/json)  \n- boppreh/keyboard - Python keyboard library (https://github.com/boppreh/keyboard)\n- Cascadia Code Font (https://github.com/microsoft/cascadia-code)";

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
                } else {
                    std::cout << "[THEMECOLOR OPTION PARSER] Unknown parameter: " << name.toStdString() << '\n';
                }
            }
        }
    }

    void gitHub() {
        // Executing the github repo into Windows
        ShellExecuteA(0, "open", GITHUB_PAGE.c_str(), 0, 0, SW_SHOWNORMAL); // Using the ANSI version and not Unicode
    }

    void openHttpWebsite() {
        // Opening the opened HTTP website
        ShellExecuteA(0, "open", "http://127.0.0.1/", 0, 0, SW_SHOWNORMAL);
    }

    void backUp() {

    }

    void loadBackUp() {

    }

    private: // PRIVATE FUNCTIONS. These cannot be connected to outside of whatever this object is.
    bool contestOpened; // A variable for showing the information of if the contest settings is opened or not
    bool userOpened;
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
                    std::cout << "Successfully ran WEBSERVER process with PID " << webserverProcess->processId() << "\n";
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
        QStringList lines = source.split('\n'); // Shit is plain ol' magic

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
            QMessageBox *msgBox = new QMessageBox(this);
            msgBox->setText(message);
            msgBox->setWindowTitle(windowTitle);
            msgBox->setIcon(QMessageBox::Critical);
            msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
            // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
            msgBox->setWindowIcon(QIcon(iconPixmap));
            
            msgBox->setStyleSheet(styleSheetResult);
            msgBox->show();
        } else {
            // If the software exitted gracefully, we do a little bit of windowin'
            // by simply showing a response to assure the user nothing is BAD
            QMessageBox *msgBox = new QMessageBox(this);
            msgBox->setText("Dừng hệ thống chấm bài thành công!");
            msgBox->setWindowTitle("Thành công");
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
            // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
            msgBox->setWindowIcon(QIcon(iconPixmap));

            msgBox->setStyleSheet(styleSheetResult);
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
                ShellExecuteA(0, "open", (dirPath + LOG_PATH).c_str(), 0, 0, SW_SHOWNORMAL);
            }
        } else {
            // Alright its fine
            // QMessageBox::information(this, "Đã tắt trang web chấm bài trực tuyến");
        }
    }

    // --------------------------------------------------
    // Purpose: To refresh the classlist in the dropdown
    // -------------------------------------------------
    void refreshClassDropdown() {
        // classDropdown->addItem("Wassup");
        // classDropdown->addItem("Boys");

        // Getting amount of items in dropdown
        int amount = classDropdown->count();

        // 2 possibilities
        if (amount == 0) { // In case the dropdown has not been loaded
            std::cout << "[classDropdown] Dropdown EMPTY\n";
            for (auto& item: classes.items()) { // Iterate through EACH element
                classDropdown->addItem(QString::fromStdString(item.key()));
            }
        } else { // In case the dropdown still has something
            std::cout << "[classDropdown] Dropdown NOT EMPTY\n";
            
            const std::string oldCurrentValue = classDropdown->currentText().toStdString(); // For the sake of keeping the selection correct
            classDropdown->clear();

            bool currentTextStillExists = false;
            for (auto& item: classes.items()) { // Iterate through EACH element
                classDropdown->addItem(QString::fromStdString(item.key()));

                // Check if the current text still exists in the new texts
                if (oldCurrentValue == item.key()) currentTextStillExists = true;
            }

            if (currentTextStillExists)
                classDropdown->setCurrentText(QString::fromStdString(oldCurrentValue));
        }
    }

    // ----------------------------------------------------------------
    // Purpose: To display the information of a particular submission.
    // ----------------------------------------------------------------
    void displaySubmissionInfo(std::string username, std::string contestName) {
        // Prepare the name for future use
        const std::string filenameNoEXT = std::string("[") + username + std::string("][") + contestName + std::string("]");

        // Check if a log exists while also accounting for multiple log files OF multiple EXTENSIONS
        bool doesExist = false;
        std::vector<std::string> extensionsFound;
        for (std::string extension : supportedExtensions) {
            if (std::filesystem::exists(dirPath + SUBMITLOG_DIR + filenameNoEXT + extension + logExt)) {
                extensionsFound.push_back(extension);
                doesExist = true;
            }
        }

        if (!doesExist) {
            errorDialog("Người dùng không có tệp thông tin bài làm của bài thi này.");
            return; // Halt right away if continuing will only become a nuisance
        }

        // Use the same method as the replaceTestToQueue one when there are multiple
        // extensions' log file.
        std::string selectedExtension;
        if (extensionsFound.size() > 1) { // More than 1 language submitted that has generated an information file
            QStringList languages;
            for (std::string extension: extensionsFound) {
                languages << QString::fromStdString(extension);
            }

            std::cout << "[replaceTestIntoQueue()] Found more than 1 extension. Opening dialog...\n";

            CST_RadioButtonDialog *dialog = new CST_RadioButtonDialog(
                this, "Đuôi tệp", 
                QString::fromStdString(contestName + std::string(": Tìm thấy nhiều đuôi đã được nộp. Hãy chọn đuôi cần biết thông tin.")), languages
            );

            if (dialog->exec() == QDialog::Accepted) {
                selectedExtension = dialog->selectedOption().toStdString();
                std::cout << "Selected " << selectedExtension << '\n';
            }
        } else {
            // Assumes that the first and only language is the one the user would want to check out
            selectedExtension = extensionsFound[0];
        }

        // Now, we just run up the text editor?
        CST_TextEditorDialog *diag = new CST_TextEditorDialog(
            this, QString::fromStdString(filenameNoEXT),
            dirPath + SUBMITLOG_DIR + filenameNoEXT + selectedExtension + logExt,
            true
        );

        diag->setText("Thông tin bài làm học sinh:");
        diag->show();
    }

    // ------------------------------------------------------------
    // Purpose: To move submissions back into queue for retesting.
    // ------------------------------------------------------------
    void replaceTestToQueue(std::string username, std::string contestName, bool allTest) {
        if (allTest) { // In case we have to retestALL tests, use recursive
            // Get all the tests with the column's display
            int cAmount = currentTable->columnCount();
            for (int i = 0; i < cAmount; i++) {
                QString text = currentTable->horizontalHeaderItem(i)->text();
                replaceTestToQueue(username, text.toStdString(), false);
            }
        } else {
            // First, check if the file exists (the user's submission has already been tested once before)
            // We check for EACH extension
            bool doesExist = false;
            std::vector<std::string> extensionsFound;
            for (std::string extension : supportedExtensions) {
                if (std::filesystem::exists(dirPath + USERSUBHISTORY_DIR + username + std::string("/") + contestName + extension)) {
                    extensionsFound.push_back(extension);
                    doesExist = true;
                }
            }

            if (!doesExist) { // If the file does not exists
                // errorDialog("Người dùng không có bài làm nào đã chấm thuộc về bài thi này.");
                return; // HALT immediately
            }

            std::string selectedExtension;
            if (extensionsFound.size() > 1) { // More than 1 language submitted???
                QStringList languages;
                for (std::string extension: extensionsFound) {
                    languages << QString::fromStdString(extension);
                }

                std::cout << "[replaceTestIntoQueue()] Found more than 1 extension. Opening dialog...\n";
                CST_RadioButtonDialog *dialog = new CST_RadioButtonDialog(this, "Đuôi tệp", QString::fromStdString(contestName + std::string(": Hãy chọn đuôi cần chấm lại.")), languages);
                if (dialog->exec() == QDialog::Accepted) {
                    selectedExtension = dialog->selectedOption().toStdString();
                    std::cout << "Selected " << selectedExtension << '\n';
                }

            } else { // When there is only 1 language
                selectedExtension = extensionsFound[0];
            }

            // Prepare the name for future use
            const std::string filename = std::string("[") + username + std::string("][") + contestName + std::string("]") + selectedExtension;

            // Check if a submission awaiting testing exists for this same user
            if (std::filesystem::exists(dirPath + USERQUEUE_DIR + filename)) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Xác nhận vị trí", 
                    "Hiện đang có một bài nộp của bài thi này và của học sinh này chưa được chấm. Bạn có chắc muốn tiếp tục (ghi đè)?",
                    QMessageBox::Yes | QMessageBox::No);
                
                if (reply == QMessageBox::No) { // If user doesn't want to continue
                    return;
                }
            }

            // Actually doing the job
            std::fstream queueFile(dirPath + USERQUEUE_DIR + filename, std::ios::out | std::ios::trunc); // Truncates the file if already exists
            std::fstream orgFile(dirPath + USERSUBHISTORY_DIR + username + std::string("/") + contestName + selectedExtension, std::ios::in);

            if (queueFile.is_open()) {
                if (orgFile.is_open()) {
                    // Passed all loophole tests
                    // We do the job
                    queueFile << orgFile.rdbuf();
                } else {
                    errorDialog("Không thể mở tệp bài nộp (đã chấm) hiện tại của học sinh.");
                }
            } else {
                errorDialog("Thất bại trong quá trình tạo/chỉnh sửa tệp. Có lẽ tệp đang được sử dụng bởi một ứng dụng khác?");
            }

            queueFile.close();
            orgFile.close();
        }
    }

    // -----------------------------------------------------------------------------
    // Purpose: To show a custom context menu for each user's result score in order
    //          to help the user interface more intuitive
    // -----------------------------------------------------------------------------
    void showScoreContextMenu(const QPoint &pos) {
        QTableWidgetItem *item = currentTable->itemAt(pos);
        if (!item) return; // Only execute if the cell was clicked on

        QMenu contextMenu(this); // The context menu in which the options are built upon

        QAction *refresh = contextMenu.addAction("Làm mới");

        QAction *reTest = contextMenu.addAction("Chấm lại");
        
        // Getting information before checking anything
        QString username = currentTable->verticalHeaderItem(item->row())->text();
        if (item->column() == 0) {
            // So user wants to do this for ALL submissions
            QAction *selectedAction = contextMenu.exec(currentTable->viewport()->mapToGlobal(pos));

            if (selectedAction == reTest) {
                replaceTestToQueue(username.toStdString(), "", true);
                std::cout << "[showScoreContextMenu()] " << username.toStdString() << ": All test replaced into queue.\n";
            } else if (selectedAction == refresh) {
                refreshClassDropdown();
                refreshTable();
            }
        } else {
            QAction *showInfo = contextMenu.addAction("Thông tin bài làm");
            QAction *selectedAction = contextMenu.exec(currentTable->viewport()->mapToGlobal(pos));
            
            QString contestName = currentTable->horizontalHeaderItem(item->column())->text();
            if (selectedAction == reTest) {
                std::cout << "[showScoreContextMenu()] " << username.toStdString() << ": Test " << contestName.toStdString() << " replaced into queue.\n";
                replaceTestToQueue(username.toStdString(), contestName.toStdString(), false);
            } else if (selectedAction == showInfo) {
                std::cout << "[showScoreContextMenu()] " << username.toStdString() << ": Test " << contestName.toStdString() << " infomation shown.\n";
                displaySubmissionInfo(username.toStdString(), contestName.toStdString());
            } else if (selectedAction == refresh) {
                refreshClassDropdown();  // No need for refreshTable as this already generates a signal that will call
                                         // refreshTable
                // refreshTable();
            }
        }
    }

    // --------------------------------------------------------------------------
    // Purpose: To refresh the table to account for changes in the whole result. 
    //          Probably needs to be better made but I am running out of time.
    // --------------------------------------------------------------------------
    void refreshTable() {
        // To refresh the table for the results in the management tab
        // To achieve this. We have to clear the table first
        currentTable->clear();

        // Get the current class
        std::string currentClass = classDropdown->currentText().toStdString();

        // Okay. Fine. Let's finally get a vector<std::string> of contests
        std::vector<std::string> currentContests;

        for (const auto& item: contests.items()) {
            // std::cout << item.value() << '\n';  // Used for debugging only

            // Now: Read through and process one by one if the class we need and the class its
            // valid for is in the same place
            bool classExists = false;
            for (const std::string classValue: item.value()["Classes"]) {
                // std::cout << classValue << '\n'; // Debugging

                // So... it seems like the classes are in fact, valid in this way.
                // So we compare with the current class
                if (classValue == currentClass) {
                    classExists = true;
                    break;
                }
            }

            // If the class does exists in the list of valid classes of the contest we are pointing to
            // then we add it into the vector created previously
            if (classExists) currentContests.push_back(item.key());
        }

        // With the current contests of this tab processed through. We prepare the space needed for them
        // to occupy (set into). Also accounting the collumn for the names and the sum of points too
        currentTable->setColumnCount(currentContests.size() + 1);

        QTableWidgetItem *columnHeaderSumOfPointsText = new QTableWidgetItem();
        columnHeaderSumOfPointsText->setFlags(columnHeaderSumOfPointsText->flags() & ~Qt::ItemFlag::ItemIsEditable);
        columnHeaderSumOfPointsText->setText("Tổng");
        currentTable->setHorizontalHeaderItem(0, columnHeaderSumOfPointsText);

        // With the header for the users column done. We add in the columns for contests
        int index = 1;
        for (const std::string& contest : currentContests) {
            // For each contest we has for this class

            QTableWidgetItem *columnHeaderContestText = new QTableWidgetItem();
            columnHeaderContestText->setFlags(columnHeaderContestText->flags() & ~Qt::ItemFlag::ItemIsEditable);
            columnHeaderContestText->setText(QString::fromStdString(contest));
            currentTable->setHorizontalHeaderItem(index, columnHeaderContestText);
            index++;
        }

        // Now. We move onto gathering the required informations for the users
        std::vector<std::string> usersList;

        for (const auto& item : users.items()) {
            // Automatically account for difference in classes
            // usersList.push_back(item.key());
            bool classExists = false;

            for (const std::string cls : item.value()["class"]) {
                // Just like the contest purification.
                // We find the users with matching classes

                if (cls == currentClass) {
                    classExists = true;
                    break;
                }
            }

            if (classExists) {
                usersList.push_back(item.key());
                // std::cout << "[refreshTable() / DEBUGGING #182] " << item.key() << '\n';  // Should not exists to reserve I/O speed
            }
            
        }

        // Variable for contests' points
        // contestsPointsByOrder[username] -> {int contest1Point, int contest2Point...}
        // Order will go with the order of currentContests (std::vector<>)
        std::unordered_map<std::string, std::vector<int>> contestsPointsByOrder;
        std::vector<std::pair<int, std::string>> usersSums; // More sorting friendly

        // We go through EACH user
        for (const std::string user : usersList) {
            // Get the contests of EACH user (in JSON format)
            std::fstream userSubmitResultFile(dirPath + USERSTATS_DIR + user + std::string(".json"), std::ios::in);
            if (userSubmitResultFile.is_open()) {
                try {
                    const json submissions = json::parse(userSubmitResultFile);

                    // Success? Nice. Now we just need to do some checking through.
                    // Now we read the submissions relative to the contests
                    int sum = 0;
                    for (std::string contest : currentContests) {
                        // Okay. Maybe also calculate the sums along the way?
                        if (submissions.contains(contest)) {
                            const int relativePoints = submissions[contest];
                            sum += relativePoints;

                            // Now, we add the constest points
                            contestsPointsByOrder[user].push_back(relativePoints);
                        } else {
                            contestsPointsByOrder[user].push_back(NONE_PLACEHOLDER);
                        }
                    }

                    // Adding the user's sum into the destination vector
                    // to prepare for the sorting later on
                    usersSums.push_back({sum, user});
                    // std::cout << "[refreshTable() / DEBUGGING #412] " << user << " -> " << sum << "\n";  // Another I/O waste
                } catch (const json::parse_error& e) {
                    // If error caught. We just delete that file.
                    remove((dirPath + USERSTATS_DIR + user + std::string(".json")).c_str());

                    // Fill the value with NOTHING? Let's just put LOWEST INT VALUE as a placeholder
                    contestsPointsByOrder[user].push_back(NONE_PLACEHOLDER);

                    // Adding user's sum in for sorting
                    usersSums.push_back({0, user});
                }
            } else {
                // Fill the value with NOTHING? Let's just put LOWEST INT VALUE as a placeholder
                contestsPointsByOrder[user].push_back(NONE_PLACEHOLDER);

                // Adding user's sum in for sorting
                usersSums.push_back({0, user});
            }
        }

        sort(usersSums.begin(), usersSums.end(), [](const auto &a, const auto &b) { // Descending
            return a.first > b.first;
        });

        // Now? Sink it all in. Because... We are gonna INITIATE THIS SHIT
        // Prepare room for user values to sit in (putting those values into a table)
        currentTable->setRowCount(usersSums.size());

        // Add the users in one by one
        index = 0; // Reuse the variable
        for (const auto &userSorterValue : usersSums) {
            // Go through usersSums as we already sorted
            QTableWidgetItem *usernameItem = new QTableWidgetItem();
            usernameItem->setFlags(usernameItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
            usernameItem->setText(QString::fromStdString(userSorterValue.second)); // Second is name, actually.
            currentTable->setVerticalHeaderItem(index, usernameItem);

            // Set the sum amount of points (0 is hardcoded cus of the position of the sum column)
            QTableWidgetItem *pointsItem = new QTableWidgetItem();
            pointsItem->setFlags(pointsItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
            pointsItem->setText(QString::fromStdString(intToString(userSorterValue.first)));  // First is the actual index value
            currentTable->setItem(index, 0, pointsItem);

            // With this done, let's move onto the final nail - the contest points
            if (contestsPointsByOrder[userSorterValue.second].size() == 1 && contestsPointsByOrder[userSorterValue.second][0] == NONE_PLACEHOLDER) {
                // In case this contest does NOT exist (the user never done the contest)
                const int& amountOfDestBlankCells = currentTable->columnCount();

                for (int i = 0; i < amountOfDestBlankCells; i++) {
                    QTableWidgetItem *blankItem = new QTableWidgetItem();
                    blankItem->setFlags(blankItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
                    currentTable->setItem(index, i, blankItem);
                }
            } else {
                int cindex = 1; // Index of column
                for (const int &point : contestsPointsByOrder[userSorterValue.second]) {
                    // Yeah it looks like I cant reuse any QTableWidgetItem, actually.
                    QTableWidgetItem *contestPointsItem = new QTableWidgetItem();
                    contestPointsItem->setFlags(contestPointsItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
                    contestPointsItem->setText(QString::fromStdString(intToString(point)));
                    currentTable->setItem(index, cindex, contestPointsItem);

                    // Prepare for next loop
                    cindex++;
                }
            }

            // Prepare for next loop
            index++;
        }

        std::cout << "[currentTable] Table refreshed!\n";
    }

    // -----------------------------------------------------------------
    // Purpose: Showing the neccessary subwindow for EACH option on the
    //          weird bar above the table in the management tab
    // -----------------------------------------------------------------
    void showButtonInfoFromBarType(std::string type) {
        if (type == "contests" && !contestOpened) {
            // Sadly, stack allocation can only be used with QDialogs since they will pause
            // all current operations to focus on only the QDialog.
            // This will instantly close on open with stack allocation, so heap allocation (pointers)
            // is the only way (for now)
            WIN_ContestsSettings *cstWin = new WIN_ContestsSettings(this);
            cstWin->show();

            contestOpened = true;
            contestsSettings->setEnabled(false); // Disable the button

            connect(cstWin, &WIN_ContestsSettings::closed, this, [this]{
                contestOpened = false;
                contestsSettings->setEnabled(true);
            });
        } else if (type == "users" && !userOpened) {
            // Alright. So the user want to modify students settings and webserver is NOT RUNNING
            if (webserverProcess->state() == QProcess::Running) {
                QMessageBox::critical(this, 
                    "Không thể chỉnh sửa cài đặt người dùng", 
                    "Không thể chỉnh sửa cài đặt người dùng khi trang web chấm bài đang hoạt động!", 
                    QMessageBox::Ok
                );

                return; // HALT
            }

            WIN_UsersSettings *usrWin = new WIN_UsersSettings(this);
            usrWin->show();

            userOpened = true;
            usersSettings->setEnabled(false);

            connect(usrWin, &WIN_UsersSettings::closed, this, [this] {
                userOpened = false;
                usersSettings->setEnabled(true);
            });
        }
    }

    // ------------------------------------------------
    // Purpose: Refresh current configurations on each
    //          tab switch to update contents and also
    //          update variables.
    // ------------------------------------------------
    void onTabSwitches(int index) {
        // When user switches tabs. It's good to update all contents of ALL TABS.
        // This is just for the sake of accuracy. Future optimizations might
        // reduce the stress on refreshing.
        std::cout << "onTabSwitches(): Called. currentSwitched.\n";

        // Reading data into variables related to them
        settings = getSettingsInfo();
        aliases = getAliasesInfo();
        classes = getClassesInfo();
        contests = getContestsInfo();
        users = getUsersInfo();

        judgingWaitTimeInput->setText(QString::fromStdString(doubleToString(settings["wait_time"])));
        judgingReloadTimeInput->setText(QString::fromStdString(doubleToString(settings["reload_time"])));
        judgingShowTestCheckbox->setCheckState(settings["show_test"] ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

        webserverLogInSecsInput->setText(QString::fromStdString(intToString(settings["max_not_logged_in_session_seconds"])));

        std::string webname = aliases["website_name"];
        webserverAliasWebnameInput->setText(QString::fromUtf8(webname.c_str()));

        std::string slogan = aliases["slogan"];
        webserverAliasSloganInput->setText(QString::fromUtf8(slogan.c_str()));

        std::string hostname = aliases["hostname"];
        webserverAliasHostnameInput->setText(QString::fromUtf8(hostname.c_str()));

        refreshClassDropdown();
        refreshTable();
    }

    // -------------------------------------------------------------------------
    // Purpose: Showing errors faster than having to type an actual long command
    // -------------------------------------------------------------------------
    void errorDialog(std::string error) {
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setText(QString::fromStdString("Đã có lỗi xảy ra: " + error));
        msgBox->setWindowTitle("Lỗi");
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
        // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
        msgBox->setWindowIcon(QIcon(iconPixmap));
        msgBox->setStyleSheet(styleSheetResult);

        msgBox->show();
    }

    // -------------------------------------------
    // Purpose: Change settings for inputs from a
    //          predetermined 'type' value
    // -------------------------------------------
    void onInputChanges(std::string type, QString value) {
        std::cout << "onInputChanges(std::string type, std::string value): " << type << ": -> " << value.toStdString() << '\n';

        // Checking which type the input belongs to
        if (type == "wait_time" || type == "reload_time" || type == "show_test" || type == "maximum_login_seconds") {            
            // Checking which value does the input correspond to
            if (type == "wait_time") {
                // Applying changes
                settings["wait_time"] = stringToDouble(value.toStdString());
            } else if (type == "reload_time") {
                // This is reload time. So we will change the reload_time key
                settings["reload_time"] = stringToDouble(value.toStdString());
            } else if (type == "show_test") {
                // Alright. This is showing tests, so we save into show_test key
                // std::cout << "!!!";
                settings["show_test"] = value == "2" ? true : false;
            } else if (type == "maximum_login_seconds") {
                // Now, save into the correct key.
                // NOTICE: the key is not the same as type this time. Might happen in the future tho
                settings["max_not_logged_in_session_seconds"] = stringToInt(value.toStdString());
            }
            
            saveSettingsInfo(settings);
        } else if (type == "website_name" || type == "slogan" || type == "hostname") {
            // Checking which type?
            if (type == "website_name") {
                aliases["website_name"] = value.toUtf8().toStdString();
            } else if (type == "slogan") {
                aliases["slogan"] = value.toUtf8().toStdString();
            } else if (type == "hostname") {
                aliases["hostname"] = value.toUtf8().toStdString();
            }
            
            saveAliasesInfo(aliases);
        }
    }

    protected: // Events in which are native Qt events
    void closeEvent(QCloseEvent *event) override {
        if (judgingProcess->state() == QProcess::Running || webserverProcess->state() == QProcess::Running) {
            // If these processes are still running
            QMessageBox::warning(this, "Không thể hoàn thành yêu cầu", 
                "Không thể thoát vì hệ thống chấm bài hoặc website chấm bài trực tuyến vẫn còn đang chạy hoặc đang trong quá trình tắt. Vui lòng đợi và thử lại sau.",
                QMessageBox::Ok);

            event->ignore(); // Prevent event from closing
        } else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Xác nhận", "Bạn có chắc muốn thoát?",
                                        QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                event->accept();  // Allow closing
            } else {
                event->ignore();  // Prevent closing
            }
        }
    }

    bool eventFilter(QObject* obj, QEvent* event) override {
        // std::cout << "Caught event on tab " << tabs->currentIndex() << "\n";

        if (tabs->currentIndex() == 0 && event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

            switch (keyEvent->key()) {
                case Qt::Key_F5:
                    // std::cout << "Refreshing table called\n";
                    refreshClassDropdown();
                    return true;
            }
        }

        return QObject::eventFilter(obj, event);
    }
};

/* PROPOSED BY CLAUDE. IDK HOW THIS WORKS BUT LOOKS LIKE ITS WAY BETTER */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1) Simple console check from command line
    bool hideConsole = true;
    std::wstring cmdLine = GetCommandLineW();
    if (cmdLine.find(L"--console") != std::wstring::npos) {
        hideConsole = false;
    }

    // 2) Simple Qt arguments - just pass the program name
    int qt_argc = 1;
    char programName[] = "ATOMIC";
    char* qt_argv[] = { programName };
    char** qt_argv_ptr = qt_argv;

    // 3) Optionally allocate a console
    if (!hideConsole) {
        if (AllocConsole()) {
            // Redirect standard streams safely
            FILE* fp = nullptr;
            if (freopen_s(&fp, "CONOUT$", "w", stdout) == 0) {
                setvbuf(stdout, nullptr, _IONBF, 0);
            }
            if (freopen_s(&fp, "CONOUT$", "w", stderr) == 0) {
                setvbuf(stderr, nullptr, _IONBF, 0);
            }
            if (freopen_s(&fp, "CONIN$", "r", stdin) == 0) {
                // Input stream setup successful
            }

            // Use safer output methods
            std::cout << "Console attached!\n";
            std::cout << "Application running with console support.\n";
            
            // ASCII art
            const char* asciiArt = R"(
      ::::    :::     :::     :::    ::: ::::::::::: ::::::::::: :::       :::    :::  ::::::::      :::     :::    ::: 
     :+:+:   :+:   :+: :+:   :+:    :+:     :+:         :+:     :+:       :+:    :+: :+:    :+:    :+:      :+:   :+:   
    :+:+:+  +:+  +:+   +:+  +:+    +:+     +:+         +:+     +:+       +:+    +:+ +:+          +:+ +:+   +:+  +:+     
   +#+ +:+ +#+ +#++:++#++: +#+    +:+     +#+         +#+     +#+       +#+    +:+ +#++:++#++  +#+  +:+   +#++:++       
  +#+  +#+#+# +#+     +#+ +#+    +#+     +#+         +#+     +#+       +#+    +#+        +#+ +#+#+#+#+#+ +#+  +#+       
 #+#   #+#+# #+#     #+# #+#    #+#     #+#         #+#     #+#       #+#    #+# #+#    #+#       #+#   #+#   #+#       
###    #### ###     ###  ########      ###     ########### ########## ########   ########        ###   ###    ###  
            )";
            std::cout << asciiArt << std::endl;
            std::cout.flush();
        }
    }

    // 4) Initialize Winsock
    WSADATA wsaData;
    int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsResult != 0) {
        if (!hideConsole) {
            std::cout << "WSAStartup failed: " << wsResult << std::endl;
        }
        
        // Create a temporary QApplication just for the message box if needed
        if (hideConsole) {
            QApplication tempApp(qt_argc, qt_argv_ptr);
            QMessageBox::critical(
                nullptr,
                "Lỗi mở WINSOCK",
                "Đã gặp lỗi thiết lập WINSOCK. Thiết bị của bạn có thể không tương thích với phần mềm này",
                QMessageBox::StandardButton::Ok
            );
        }
        return 1;
    }

    int ret = 1; // Default error return
    
    try {
        // 5) Launch Qt application
        QApplication app(qt_argc, qt_argv_ptr);
        app.setApplicationDisplayName("ATOMIC");
        app.setApplicationName("ATOMIC");
        app.setApplicationVersion("v0.1");
        Q_INIT_RESOURCE(qres);

        PanelWindow panel(nullptr);
        panel.initialize();

        if (!hideConsole) {
            QMessageBox::warning(
                nullptr, 
                "Hiện bảng lịch sử hoạt động chương trình (logging)", 
                "Bạn đang bắt đầu ATOMIC bằng tham số --console, điều này không được khuyến nghị bởi vì sẽ có thể xảy ra các lỗi liên quan đến bộ nhớ khi I/O chưa hoàn thiện. Nếu bạn không có ý định gỡ rối chương trình, hãy tránh sử dụng tham số --console khi khởi động chương trình", 
                QMessageBox::Ok
            );
        }

        panel.show();
        ret = app.exec();
        
    } catch (const std::exception& e) {
        if (!hideConsole) {
            std::cout << "Exception caught: " << e.what() << std::endl;
        } else {
            // For GUI-only mode, show message box for critical errors
            QMessageBox::critical(nullptr, "Error", 
                QString("An unexpected error occurred: %1").arg(e.what()), 
                QMessageBox::Ok);
        }
        ret = -1;
    } catch (...) {
        if (!hideConsole) {
            std::cout << "Unknown exception caught" << std::endl;
        } else {
            QMessageBox::critical(nullptr, "Error", "An unknown error occurred", QMessageBox::Ok);
        }
        ret = -1;
    }

    // 6) Cleanup Winsock
    WSACleanup();

    if (!hideConsole) {
        std::cin.get();
    }
    return ret;
}