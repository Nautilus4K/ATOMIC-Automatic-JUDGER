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

NOTE:
THIS IS THE WORST OF SPAGHETTI CODES
LIKE, DUDE
NEVER SEEN ANYTHING THIS BAD

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
#include <QtWidgets/QInputDialog>      // INPUT.IN.A.DIALOG. Yessssirrrr
#include <QtWidgets/QFileDialog>       // Let's go fetch some files
#include <QtGui/QAction>               // Action for menus. Wonder what fucker thought to put it in QtGui
#include <QtGui/QCloseEvent>           // Close event. The action of 'X' button
#include <QtGui/QDoubleValidator>      // Validator for edits.
#include <QtGui/QIntValidator>         // Another validator, also for edits, now more like INTEGER revolved.
#include <QtGui/QIcon>                 // Icon readings
#include <QtGui/QPixmap>               // Picture reading
#include <QtGui/QFont>                 // Fonts
#include <QtGui/QFontDatabase>         // Font database. For loading custom fonts
#include <QtGui/QColor>                // Colors
#include <QtGui/QTextDocumentFragment> // Producing a fragment of an HTML document. Sorta like a wrapper cleaner for HTMLs

// Importing Qt related features
#include <QtCore/Qt>       // A bunch of constants
#include <QtCore/QProcess> // Process running
#include <QtCore/QStringListModel> // QStringList model? Idk

#include <QtGui/QDesktopServices> // For desktop related services. Like opening a web browser

// File I/O actions and getting data
#include <fstream>
#include <nlohmann/json.hpp>
// Turn nlohmann::json to json for short.
using json = nlohmann::json;

// System-related actions
// #ifndef WIN32_LEAN_AND_MEAN // Because of historical reasons, including Windows.h
// #define WIN32_LEAN_AND_MEAN // along with winsock2.h will need to have this
// #endif

// Socket
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <thread> // Threading

#include <filesystem> // File managing

// #pragma comment(lib, "Ws2_32.lib") // Required library to link with
// aint gon lie i was quite foolish

// C++ Features
#include <vector>
#include <map>
#include <sstream>
#include <unordered_map>

// Debug console
#include <iostream>

// Excel manipulation
#include <xlnt/xlnt.hpp>

// Custom shits
#include "utilities.h"
#include "consts.h"
#include "CST_RadioButtonDialog.h"
#include "CST_TextEditorDialog.h"
#include "CST_RichTextEdit.h"
#include "CST_Listing.h"
#include "CST_Separator.h"
#include "WIN_ContestsSettings.h"
#include "WIN_UsersSettings.h"
#include "WIN_ClassesSettings.h"

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
    QWidget *manageTab = new QWidget(this);
    QWidget *judgingTab = new QWidget(this);
    QWidget *webserverTab = new QWidget(this);

    // Option elements (In tabs)
    QLineEdit *judgingWaitTimeInput = new QLineEdit(this);
    QLineEdit *judgingReloadTimeInput = new QLineEdit(this);
    QCheckBox *judgingShowTestCheckbox = new QCheckBox(this);

    QLineEdit *webserverLogInSecsInput = new QLineEdit(this);
    QLineEdit *webserverAliasWebnameInput = new QLineEdit(this);
    QLineEdit *webserverAliasSloganInput = new QLineEdit(this);
    QLineEdit *webserverAliasHostnameInput = new QLineEdit(this);

    // Main manager for the management of data and profiles
    QComboBox *classDropdown = new QComboBox(this);
    QTableWidget *currentTable = new QTableWidget(this);

    // Sidebar elements
    QPushButton *judgingProcessButton = new QPushButton(this);
    QTextEdit *judgingProcessConsole = new QTextEdit(this);
    QPushButton *webserverProcessButton = new QPushButton(this);

    // Data variables
    json settings; // NULL at first
    json version; // NULL at first
    json aliases; // NULL at first
    json classes; // Classes object. NULL at first
    json contests; // Contests object. NULL at first
    json users; // Users object. NULL at first
    QPixmap iconPixmap;
    // QFont monospaceFont; // NULL at first
    QString styleSheetResult;
    std::unordered_map<std::string, QFont> fonts;
    
    // Processes
    QProcess *judgingProcess = new QProcess(this);
    bool judgingEnabled;
    QProcess *webserverProcess = new QProcess(this);
    bool webserverEnabled;
    QProcess *ollamaProcess = new QProcess(this);
    bool ollamaEnabled;

    // Fine shyt
    QTabWidget *tabs = new QTabWidget();

    // Settings bars
    QPushButton *contestsSettings = new QPushButton(this);
    QPushButton *usersSettings = new QPushButton(this);
    QPushButton *classesSettings = new QPushButton(this);
    QPushButton *userSubmitDir = new QPushButton(this);
    QPushButton *convertToExcel = new QPushButton(this);
    QPushButton *loadFromExcel = new QPushButton(this);
    QPushButton *backUpAction = new QPushButton(this);
    QPushButton *loadBackUpAction = new QPushButton(this);

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
        // monospaceFont.setFamilies({"Source Code Pro", "Cascadia Code", "Consolas", "Courier New", "Ubuntu Mono", "monospace"});
        int monospaceId = QFontDatabase::addApplicationFont(MONOSPACEFONT_PATH);
        if (monospaceId < 0) {
            std::cerr << "Failed to load monospace font from " << MONOSPACEFONT_PATH.toStdString() << "\n";
            fonts["monospace"].setFamilies({"Consolas", "Courier New", "Ubuntu Mono", "monospace"});
        } else {
            QStringList loadedFamilies = QFontDatabase::applicationFontFamilies(monospaceId);
            if (!loadedFamilies.isEmpty()) {
                fonts["monospace"].setFamily(loadedFamilies.at(0));
                std::cout << "Loaded monospace font: " << loadedFamilies.at(0).toStdString() << "\n";
            } else {
                std::cout << "No families found in loaded monospace font\n";
                fonts["monospace"].setFamilies({"Consolas", "Courier New", "Ubuntu Mono", "monospace"});
            }
        }

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


        ollamaProcess->setProcessEnvironment(env);

        ollamaEnabled = false;
        ollamaProcess->setProgram(OLLAMA_PATH);
        ollamaProcess->setArguments(QStringList() << "serve");

        // Look I am sane but we need an actual way to check on this ollama when it crashes so whatever man
        connect(ollamaProcess, &QProcess::finished, this, &PanelWindow::stoppedOllama);
        connect(ollamaProcess, &QProcess::errorOccurred, this, &PanelWindow::stoppedOllama);


        //////////////
        // Menu bar //
        //////////////
        QMenuBar *menuBar = new QMenuBar(this);
        setMenuBar(menuBar);

        // FILE MENU
        QMenu *fileMenu = new QMenu("&Tệp");
        menuBar->addMenu(fileMenu);

        // Action of backing up files (Currently, well, not worked on yet)
        QAction *fileBackup = new QAction(this);
        fileBackup->setIcon(QIcon(QPixmap(BACKUPICON_PATH)));
        fileBackup->setText("Sao lưu...");
        fileMenu->addAction(fileBackup);
        // QAction connected function
        connect(fileBackup, &QAction::triggered, this, &PanelWindow::backUp);

        // Action of loading up backup files
        QAction *loadBackup = new QAction(this);
        loadBackup->setIcon(QIcon(QPixmap(LOADBACKUPICON_PATH)));
        loadBackup->setText("Mở sao lưu...");
        fileMenu->addAction(loadBackup);
        // QAction connected function
        connect(loadBackup, &QAction::triggered, this, &PanelWindow::loadBackUp);

        // Action of resetting everything
        QAction *resetAllAction = new QAction(this);
        // resetAll->setIcon(QIcon(QPixmap(RESETICON_PATH)));
        resetAllAction->setText("Đặt lại cài đặt...");
        fileMenu->addAction(resetAllAction);
        // QAction connected function
        connect(resetAllAction, &QAction::triggered, this, &PanelWindow::resetAll);

        // Action of exitting
        QAction *exitApp = new QAction(this);
        exitApp->setText("Thoát");
        exitApp->setIcon(QIcon(EXITICON_PATH));
        fileMenu->addAction(exitApp);
        // QAction connected function
        connect(exitApp, &QAction::triggered, this, &PanelWindow::close);

        
        // HELP MENU
        QMenu *helpMenu = new QMenu("&Trợ giúp");
        menuBar->addMenu(helpMenu);

        // Action of about GITHUB page
        QAction *openGitHub = new QAction(this);
        openGitHub->setText("Trang dự án...");
        helpMenu->addAction(openGitHub);
        // QAction connected function
        connect(openGitHub, &QAction::triggered, this, &PanelWindow::gitHub);

        // Action of about GITHUB page
        QAction *openAbout = new QAction(this);
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
        // The reason why I had to type new QVBoxLayout(this) is because of the fact that
        // good ol QVBoxLayout mainLayout; is on stack, but setLayout requires a pointer
        QVBoxLayout *mainLayout = new QVBoxLayout(this);


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

        QVBoxLayout *sidebarLayout = new QVBoxLayout(this);

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
        // judgingProcessConsole->setAlignment(Qt::AlignTop);
        judgingProcessConsole->setFont(fonts["monospace"]);
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
        QVBoxLayout *manageTabLayout = new QVBoxLayout(manageTab);

        // Adding the neccessary settings as buttons on a horizontal grid.
        // To achieve this, we need a QScrollArea, QWidget and a QHBoxLayout for that QWidget
        QWidget *settingsLine = new QWidget(this);
        QHBoxLayout *settingsLayout = new QHBoxLayout(settingsLine);

        const int btnWidth = 55, btnHeight = 55;  // Perfect square

        // Set the height to account for the scroll bar too
        settingsLine->setFixedHeight(btnHeight + 5);

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

        // Now with classes settings
        classesSettings->setObjectName("genericBtn");
        classesSettings->setToolTip("Cài đặt lớp học");
        classesSettings->setFixedHeight(btnHeight); classesSettings->setFixedWidth(btnWidth);
        QPixmap classesPXMP(CLASSESICON_PATH);
        QIcon classesIcon(classesPXMP);
        classesSettings->setIcon(classesIcon);
        classesSettings->setIconSize(QSize(btnWidth, btnHeight));
        connect(classesSettings, &QPushButton::clicked, this, [this] {
            showButtonInfoFromBarType("classes");
        });

        // Add a separator
        CST_Separator *separator1 = new CST_Separator(this, 10);

        // Add a button so that it will open to the user submissions directory
        userSubmitDir->setObjectName("genericBtn");
        userSubmitDir->setToolTip("Mở thư mục chứa các bài làm học sinh");
        userSubmitDir->setFixedHeight(btnHeight); userSubmitDir->setFixedWidth(btnWidth);
        QPixmap userSubmitPXMP(SUBMITDIRICON_PATH);
        QIcon userSubmitIcon(userSubmitPXMP);
        userSubmitDir->setIcon(userSubmitIcon);
        userSubmitDir->setIconSize(QSize(btnWidth, btnHeight));
        connect(userSubmitDir, &QPushButton::clicked, this, &PanelWindow::submitDir);

        // A button that convert the whole thing as excel
        convertToExcel->setObjectName("genericBtn");
        convertToExcel->setToolTip("Xuất file Excel");
        convertToExcel->setFixedHeight(btnHeight); convertToExcel->setFixedWidth(btnWidth);
        QPixmap toExcelPXMP(TOEXCELICON_PATH);
        QIcon toExcelIcon(toExcelPXMP);
        convertToExcel->setIcon(toExcelIcon);
        convertToExcel->setIconSize(QSize(btnWidth, btnHeight));
        connect(convertToExcel, &QPushButton::clicked, this, &PanelWindow::toExcel);

        // A button that reads from excel
        // A button that convert the whole thing as excel
        loadFromExcel->setObjectName("genericBtn");
        loadFromExcel->setToolTip("Tải môi trường từ file Excel");
        loadFromExcel->setFixedHeight(btnHeight); loadFromExcel->setFixedWidth(btnWidth);
        QPixmap fromExcelPXMP(FROMEXCELICON_PATH);
        QIcon fromExcelIcon(fromExcelPXMP);
        loadFromExcel->setIcon(fromExcelIcon);
        loadFromExcel->setIconSize(QSize(btnWidth, btnHeight));
        connect(loadFromExcel, &QPushButton::clicked, this, &PanelWindow::fromExcel);

        CST_Separator *separator2 = new CST_Separator(this, 10);

        backUpAction->setObjectName("genericBtn");
        backUpAction->setToolTip("Sao lưu dữ liệu vào file");
        backUpAction->setFixedHeight(btnHeight); backUpAction->setFixedWidth(btnWidth);
        QPixmap backUpPXMP(BACKUPICON_PATH);
        QIcon backUpIcon(backUpPXMP);
        backUpAction->setIcon(backUpIcon);
        backUpAction->setIconSize(QSize(btnWidth, btnHeight));
        connect(backUpAction, &QPushButton::clicked, this, &PanelWindow::backUp);

        loadBackUpAction->setObjectName("genericBtn");
        loadBackUpAction->setToolTip("Tải môi trường từ tệp sao lưu");
        loadBackUpAction->setFixedHeight(btnHeight); loadBackUpAction->setFixedWidth(btnWidth);
        QPixmap loadBackUpPXMP(LOADBACKUPICON_PATH);
        QIcon loadBackUpIcon(loadBackUpPXMP);
        loadBackUpAction->setIcon(loadBackUpIcon);
        loadBackUpAction->setIconSize(QSize(btnWidth, btnHeight));
        connect(loadBackUpAction, &QPushButton::clicked, this, &PanelWindow::loadBackUp);

        // Adding in
        settingsLayout->addWidget(contestsSettings);
        settingsLayout->addWidget(usersSettings);
        settingsLayout->addWidget(classesSettings);
        settingsLayout->addWidget(separator1);
        settingsLayout->addWidget(userSubmitDir);
        settingsLayout->addWidget(convertToExcel);
        settingsLayout->addWidget(loadFromExcel);
        settingsLayout->addWidget(separator2);
        settingsLayout->addWidget(backUpAction);
        settingsLayout->addWidget(loadBackUpAction);
        settingsLayout->setContentsMargins(0, 0, 0, 0);

        settingsLine->setLayout(settingsLayout);

        // Managing scrollablity
        QScrollArea *settingsLineScrollable = new QScrollArea(this);
        settingsLineScrollable->setWidget(settingsLine);
        settingsLineScrollable->setWidgetResizable(false); // Let widget keep its natural size
        settingsLineScrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        settingsLineScrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        
        // Calculating height
        int stLeft, stTop, stRight, stBottom;
        settingsLayout->getContentsMargins(&stLeft, &stTop, &stRight, &stBottom);

        int totalHeight = btnHeight + stTop + stBottom + 10 ; // A lil bit of padding too
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

        QLabel *noteLabel = new QLabel(manageTab);
        noteLabel->setStyleSheet(STYLE_SMALLALEL);
        noteLabel->setText("* Bấm chuột phải các ô trong bảng để xem các cài đặt hoặc thông tin cho từng bài nộp.");
        noteLabel->setWordWrap(true);
        noteLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        manageTabLayout->addWidget(noteLabel);

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
        QVBoxLayout *judgingTabLayout = new QVBoxLayout(this);

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
        connect(judgingShowTestCheckbox, &QCheckBox::checkStateChanged, this, [this](const int state) {
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
        QVBoxLayout *webserverTabLayout = new QVBoxLayout(this);
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
        classOpened = false;

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

        // Also let's begin ollama
        ollamaProcess->start();
        ollamaEnabled = true;

        ollamaRetries = 0;
    }

    int ollamaRetries = 0;
    void stoppedOllama() {
        if (ollamaRetries >= 5) {
            std::cerr << "[ollama] Retried too many goddamn times\n";
            errorDialog("Không thể mở ollama (Cần thiết cho tính năng tạo dựng bộ test bằng AI). Có thể đã có một ollama khác đang chạy. Việc mở ollama ở trong phiên này sẽ bị hủy bỏ.");

            return;
        }

        std::cerr << "[ollama] Stopped??? What the fuck. Okay restarting rq.\n";
        ollamaProcess->start();
        ollamaRetries++;
    }

    void toExcel() {
        refreshClassDropdown(); // Do some refreshing first

        std::cout << "Convert/map to XLSX. Asking user...\n";
        std::string userdocPath = getenv("USERPROFILE") + std::string("/Documents");

        // Fetching file name
        std::string filename = QFileDialog::getSaveFileName(this, "Lưu tệp Excel", QString::fromStdString(userdocPath), "Tệp Excel (*.xlsx)").toStdString();

        if (filename.empty()) return;

        // Now we ball
        xlnt::workbook wb; // First we need to create a workbook obj cause its the start of everything

        // Now removes the default "Sheet1"
        wb.remove_sheet(wb.active_sheet());

        // Modifying the font
        xlnt::font headerFont;
        headerFont.color(xlnt::rgb_color(XLSX_HEADERTEXTHEXCOL.r, XLSX_HEADERTEXTHEXCOL.g, XLSX_HEADERTEXTHEXCOL.b));
        headerFont.name("Arial");
        headerFont.bold(true);

        xlnt::font cellFont;
        cellFont.name("Arial");
        headerFont.bold(false);

        // For each classes
        for (const auto& classItem : classes.items()) {
            std::string className = classItem.key();

            xlnt::worksheet ws = wb.create_sheet(); // Get current worksheet
        
            // Customization of worksheet -> Rename it
            // Specifications:
            // - Multiple worksheets, each for each classes
            // Houses only scoreboard
            // fromExcel will automatically generate the values later.
            ws.title(className);

            // Now we configure the looks of the main headers
            // To do this, we have to get all the values.

            // First we need to set a blank color for the intersection
            ws.cell("A1").value("");
            ws.cell("A1").fill(xlnt::fill::solid(xlnt::rgb_color(XLSX_HEADERHEXCOL.r, XLSX_HEADERHEXCOL.g, XLSX_HEADERHEXCOL.b)));

            // Also let's modify some spaces
            xlnt::column_properties col1props;
            col1props.width = 20.0;
            col1props.custom_width = true;
            ws.add_column_properties(1, col1props);

            // Now let's do the more normal things like configuring the contest columns
            xlnt::column_properties cntsColProps;
            cntsColProps.width = 12.5;
            cntsColProps.custom_width = true;

            // A column dedicated to sum of contests
            // Draw the header position in the worksheet
            ws.cell(2, 1).fill(xlnt::fill::solid(xlnt::rgb_color(XLSX_HEADERHEXCOL.r, XLSX_HEADERHEXCOL.g, XLSX_HEADERHEXCOL.b)));
            ws.cell(2, 1).value("Tổng");

            // Applying the font
            ws.cell(2, 1).font(headerFont);

            // Applying properties
            ws.add_column_properties(2, cntsColProps);

            // Now we find the contests and while we're at it,
            // let's also apply the labels too
            std::vector<std::string> currentContests; // The variable containing the order of contests
            int index = 3; // The index for the Column it will be working with (starts with 3 because of the intersection and sum)
            for (const auto& contestItem : contests.items()) {
                // Check if the contestItem have the same, like, class yk
                bool classExists = false;
                for (const std::string classVal : contestItem.value()["Classes"]) {
                    if (classVal == className) {
                        classExists = true;
                        break;
                    }
                }

                if (classExists) {
                    // Add it in if it worked
                    currentContests.push_back(contestItem.key());

                    // Draw the header position in the worksheet
                    ws.cell(index, 1).fill(xlnt::fill::solid(xlnt::rgb_color(XLSX_HEADERHEXCOL.r, XLSX_HEADERHEXCOL.g, XLSX_HEADERHEXCOL.b)));
                    ws.cell(index, 1).value(contestItem.key());

                    // Applying the font
                    ws.cell(index, 1).font(headerFont);

                    // Applying properties
                    ws.add_column_properties(index, cntsColProps);

                    index++; // Only increments if the contest is actually in the list and is shown
                }
            }

            std::vector<std::pair<double, std::string>> sumOfStudents;
            sumOfStudents.reserve(users.size()); // Probably too big but whatever
            // On the wayoh
            // Now we get each students' values (or sums) to construct the vector sumOfStudents
            for (const auto& studentItem : users.items()) {
                std::cout << "XLSX Processing user " << studentItem.key() << '\n';
                // Validify if the student is in the class
                bool classExists = false;
                for (const std::string classVal : studentItem.value()["class"]) {
                    if (classVal == className) {
                        classExists = true;
                        break;
                    }
                }

                if (!classExists) continue;

                std::string name = studentItem.key();
                std::cout << "USER " << name << " EXISTS\n";

                // Wadawa
                json sub = getSubmissionInfo(name);

                // Now we calculate the sum of all the contest we had
                double sum = 0;
                for (const std::string& contestName : currentContests) {
                    if (sub.contains(contestName)) {
                        sum += sub[contestName].get<double>(); // Get double value from this shit
                    }
                }

                sumOfStudents.push_back({sum, name});
            }

            // Process the sums (which means sorting them out)
            sort(sumOfStudents.begin(), sumOfStudents.end(), [](const auto &a, const auto &b) { // Descending
                return a.first > b.first;
            });

            // With the whole thing there done, we can finally browse through EACH AND EVERY user with the order
            // done prematurely
            int rindex = 2; // Starts at 2 cuz 1 is the header bruh

            // Cuz we start at 2, the offset is gonna be -2 to get 0, which is the root of all things
            // const int rvaloffset = -rindex;
            for (const auto& p : sumOfStudents) { // Now let's create another header, this time on the left side
                std::cout << "Placing user " << p.second << " on row " << rindex << '\n';
                // Draw the header position in the worksheet
                ws.cell(mapToExcelCell(rindex, 1)).fill(xlnt::fill::solid(xlnt::rgb_color(XLSX_HEADERHEXCOL.r, XLSX_HEADERHEXCOL.g, XLSX_HEADERHEXCOL.b)));
                ws.cell(mapToExcelCell(rindex, 1)).value(p.second);
                
                // Applying the font
                ws.cell(mapToExcelCell(rindex, 1)).font(headerFont);
                
                // Setting sums too
                // We need to place the sum on col 2
                xlnt::alignment valCellAlignment;
                valCellAlignment.horizontal(xlnt::horizontal_alignment::center);
                    
                // Formatting
                ws.cell(mapToExcelCell(rindex, 2)).alignment(valCellAlignment);
                ws.cell(mapToExcelCell(rindex, 2)).font(cellFont);

                // LIGHTING UP THE SKYYYY
                ws.cell(mapToExcelCell(rindex, 2)).fill(xlnt::fill::solid(xlnt::rgb_color(XLSX_USERSUMCELL.r, XLSX_USERSUMCELL.g, XLSX_USERSUMCELL.b)));
                
                // Set the sum value
                ws.cell(mapToExcelCell(rindex, 2)).value(p.first);

                // While we are at it, let's fill in the remaining datas too (each contests values)
                int cindex = 3; // 1 is users headers, 2 is sums, 3 is the beginning
                for (const std::string& contestName : currentContests) {
                    // Let's work with this
                    // We fetch the submissions info
                    json sub = getSubmissionInfo(p.second);

                    if (sub.contains(contestName)) {
                        ws.cell(mapToExcelCell(rindex, cindex)).value(sub[contestName].get<double>());
                        ws.cell(mapToExcelCell(rindex, cindex)).alignment(valCellAlignment);
                        ws.cell(mapToExcelCell(rindex, cindex)).font(cellFont);
                    }

                    ws.cell(mapToExcelCell(rindex, cindex)).fill(xlnt::fill::solid(
                        // If this shit is on an odd or even row
                        (rindex % 2 == 0) ?
                        xlnt::rgb_color(XLSX_USERROWEVENFILL.r, XLSX_USERROWEVENFILL.g, XLSX_USERROWEVENFILL.b) :
                        xlnt::rgb_color(XLSX_USERROWODDFILL.r, XLSX_USERROWODDFILL.g, XLSX_USERROWODDFILL.b)
                    ));

                    cindex++;
                }

                // Increment index by 1. It needs to be consistent at all times
                rindex++;
            }
        }

        wb.save(filename);
        // Logging
        std::cout << "Saved as " << filename << '\n';
    }

    void fromExcel() {
        // Give a warning first, cuz the user might just not give a fuck about the destructive
        // consequences of their actions
        QMessageBox::StandardButton rep = QMessageBox::warning(this, "Tiếp tục?", "Bạn có chắc muốn tiếp tục tải từ tệp Excel? Việc này sẽ xoá tất cả dữ liệu hiện tại bạn có.", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

        if (rep == QMessageBox::StandardButton::No) return;

        std::cout << "Convert/map from XLSX. Asking user...\n";
        std::string userdocPath = getenv("USERPROFILE") + std::string("/Documents");

        std::string filename = QFileDialog::getOpenFileName(this, "Tải tệp EXCEL", QString::fromStdString(userdocPath), "Tệp Excel (*.xlsx)").toStdString();

        if (filename.empty()) return;

        // Let's get the workbook
        xlnt::workbook wb;
        wb.load(filename);

        // Now we get the data of classes and each classes while also constructing up the values of unique
        // users and contests
        std::vector<std::string>                        classNames;
        std::map<std::string, std::vector<std::string>> uniqueUsersList; // .first is name, .second is classlist
        std::map<std::string, std::vector<std::string>> uniqueContestsList; // Same as usersList

        // Now onto the scores listing
        // I will use map so that i can get something like this
        // userResult[<username>][<contestname>] = double
        std::unordered_map<std::string, std::unordered_map<std::string, double>> results;

        // Now we fetch the classes (worksheets)
        std::size_t classAmount = wb.sheet_count();
        for (std::size_t i = 0; i < classAmount; i++) {
            // With this, i guess its fine cuz now its safe.
            // Let's work with the names
            xlnt::worksheet ws = wb.sheet_by_index(i);

            std::string singular_className = ws.title(); // Get the title babyyy
            std::cout << "XLSX Operation discovered class entry " << singular_className << '\n';
            classNames.push_back(singular_className);

            // With the classes fetching done, now we needs to find the users list to add them in
            std::vector<std::string> userOrder;    // ROW OFFSET REBALANCE VALUE: -2
            std::vector<std::string> contestOrder; // COLLUMN OFFSET REBALANCE VALUE: -3

            // Now let's move onto actually finding the order
            // We are gonna be finding the user order first
            // Cuz the user values' row offset rebalance value is -2 so the opposite of
            // it is gonna be 2, therefore setting the index to 2
            int index = 2;
            while (true) {
                std::string value = ws.cell(mapToExcelCell(index, 1)).value<std::string>(); // Get the value in the form of a string

                if (value.empty()) {
                    std::cout << "End of user list at row " << index << " and column 1\n";
                    break;
                }

                std::cout << "> XLSX Operation // " << singular_className << " // Discovered student entry " << value << '\n';
                userOrder.push_back(value);
                index++;
            }

            // Now we will have to find the contests order
            // Like the user order, the contest order has its own collumn offset rebalance value, in this case -3
            // so the opposite in 3 and index is gonna be set to 3
            index = 3;
            while (true) {
                std::string value = ws.cell(mapToExcelCell(1, index)).value<std::string>(); // Get the value in form of std::string ofc

                if (value.empty()) {
                    std::cout << "End of contest list at row 1 and column " << index << "\n";
                    break;
                }

                std::cout << "> XLSX Operation // " << singular_className << " // Discovered contest entry " << value << '\n';
                contestOrder.push_back(value);
                // Announcing the operation
                index++;
            }

            // With a good enough list of these things, let's just construct them into the vector we made first
            // To avoid for repetition, for each thing we want to add in, we need to make sure they dont exist already
            // Luckily, map<> helps us with this, so now we can just add them in
            for (const std::string& v : userOrder) {
                uniqueUsersList[v].push_back(singular_className);
            }

            for (const std::string& v : contestOrder) {
                uniqueContestsList[v].push_back(singular_className);
            }

            // After doing the adding and working like that, i will now face off with a lot more thing
            // Which is adding in the points

            int rindex = 2;
            int cindex;

            for (const std::string& u : userOrder) {
                // For each user we got

                // Let's process each user's contests. In this case its on column so we set the variable
                // This variable is premade so that the memory placement does not have to be repeated multiple times over
                cindex = 3;
                for (const std::string& c : contestOrder) {
                    std::string stringVal = ws.cell(mapToExcelCell(rindex, cindex)).to_string();
                    std::cout << u << "'s score on " << c << " is " << (stringVal.empty() ? "NONE" : stringVal) << '\n';

                    if (!stringVal.empty()) {
                        // If this is a valid number, I shall grant the wish of the people
                        // sub[c] = ws.cell(mapToExcelCell(rindex, cindex)).value<double>(); // It's best that its like this

                        // Now we have to save it into a temporary variable, this case its results
                        results[u][c] = ws.cell(mapToExcelCell(rindex, cindex)).value<double>();
                    }

                    // Increment the columns by 1
                    cindex++;
                }

                // Increment the row by 1
                rindex++;
            }

            // Clean up after them
            userOrder.clear();
            contestOrder.clear();
        }
        
        std::cout << "XLSX OPERATION COMPLETED. RESULT: \n";
        std::cout << "Contests: ";

        json gen_contestData = {};
        json gen_userData = {};

        for (const auto& ent : uniqueContestsList) {
            std::cout << ent.first << ", ";

            // Now we will have to in turn, generate the contests json
            gen_contestData[ent.first] = json::parse(R"(
                {
                    "Desc": "",
                    "InputFile": "",
                    "InputType": "raw",
                    "OutputFile": "",
                    "OutputType": "raw",
                    "TestAmount": 0,
                    "Tests": [],
                    "TimeLimit": 1.0
                }
            )");

            // Also now modify the classes
            gen_contestData[ent.first]["Classes"] = ent.second;
        }
        saveContestsInfo(gen_contestData);

        std::cout << "\nStudents: ";
        for (const auto& ent : uniqueUsersList) {
            std::cout << ent.first << ", ";

            // Do the same for students
            std::string constructedJson = "{\"desc\": \"Xin chào! Tôi là một học sinh!\",\"fullname\": \"" + ent.first + "\",\"password\": \"\",\"picture\": false,\"priv\": 0}";

            gen_userData[ent.first] = json::parse(constructedJson);

            // Lets now sprinkle some modification for the classes
            gen_userData[ent.first]["class"] = ent.second;
        }
        saveUsersInfo(gen_userData);

        std::cout << '\n';

        // With that being said, we still need to apply the submissions
        // To do this, we browse through the std::map of the submissions
        for (const auto& p : results) {
            json sub = {};
            for (const auto& e : p.second) {
                sub[e.first] = e.second;
            }

            // Now we have to works towards more thing
            // in this case we save this info
            saveSubmissionInfo(p.first, sub);
        }
        
        // Announcing the operation has been completed
        std::cout << "Loaded from " << filename << '\n';

        // Clean up
        classNames.clear();
        uniqueUsersList.clear();
        uniqueContestsList.clear();
        results.clear();

        // Refresh a little bit of things
        refreshClassDropdown();
        refreshTable();
    }

    void submitDir() {
        // ShellExecuteA(0, "open", (dirPath + USERSUBHISTORY_DIR).c_str(), 0, 0, SW_SHOWNORMAL);
        // std::cout << "fixme: Open submission history folder at " << (dirPath + USERSUBHISTORY_DIR) << "\n";
        QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(dirPath + USERSUBHISTORY_DIR)));
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
                QString licensingText = "====== Giấy phép ======\n\nPhần mềm này được phát hành dưới Giấy phép Mã nguồn mở MIT (MIT License) đã qua sửa đổi. Thông tin chi tiết truy cập trang dự án GitHub.\n\n====== Ghi nhận ======\n\nPhần mềm được phát triển dựa trên hoặc sử dụng các dự án mã nguồn mở sau:\n\n- Qt GUI Framework (https://qt.io)\n- Python Interpreter (https://python.org)\n- waitress (https://github.com/Pylons/waitress)\n- Docker (https://docker.com)\n- nlohmann/json - JSON for Modern C++ (https://github.com/nlohmann/json)\n- boppreh/keyboard - Python keyboard library (https://github.com/boppreh/keyboard)\n- Cascadia Code Font (https://github.com/microsoft/cascadia-code)\n- tfussell/xlnt: Cross-platform user-friendly xlsx library for C++11+ (https://github.com/tfussell/xlnt)";

                QSplitter *licensingSplitter = new QSplitter();
                licensingSplitter->setOrientation(Qt::Orientation::Vertical);
                licensingSplitter->setChildrenCollapsible(false);
                licensingSplitter->addWidget(splitter);
                
                QTextEdit *licensingInfo = new QTextEdit();
                licensingInfo->setText(licensingText);
                licensingInfo->setReadOnly(true);
                licensingInfo->setObjectName("aboutLicensing");
                licensingInfo->setFont(fonts["monospace"]);
                licensingSplitter->addWidget(licensingInfo);

                QVBoxLayout *layout = new QVBoxLayout(this);
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
        // ShellExecuteA(0, "open", GITHUB_PAGE.c_str(), 0, 0, SW_SHOWNORMAL); // Using the ANSI version and not Unicode
        QDesktopServices::openUrl(QUrl(QString::fromStdString(GITHUB_PAGE)));
    }

    void openHttpWebsite() {
        // Opening the opened HTTP website
        // ShellExecuteA(0, "open", "http://127.0.0.1/", 0, 0, SW_SHOWNORMAL);
        QDesktopServices::openUrl(QUrl("http://127.0.0.1/"));
    }

    void resetAll() {
        QMessageBox::StandardButton rep = QMessageBox::warning(this, "Tiếp tục?", "Bạn có chắc muốn tiếp tục xoá tất cả dữ liệu hiện tại bạn có.", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
        if (rep == QMessageBox::StandardButton::No) return;

        // Resetting all the data
        saveUsersInfo({});
        saveContestsInfo({});
        saveClassesInfo({});
        saveSettingsInfo({});
        saveAliasesInfo({});

        // Also we have to clear the submissions folder too
        // Best to ask the user first
        QMessageBox::StandardButton rep2 = QMessageBox::warning(this, "Tiếp tục?", "Bạn có chắc muốn tiếp tục xoá tất cả lịch sử nộp bài của người dùng?", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
        if (rep2 == QMessageBox::StandardButton::Yes) {
            std::string submissionsDir = dirPath + USERSUBHISTORY_DIR;
            for (const auto & entry : std::filesystem::directory_iterator(submissionsDir)) {
                std::filesystem::remove_all(entry.path());
            }
        }
    }

    void backUp() {
        std::cout << "[backUp()] Functionality called! Asking user...\n";
        // The extension should be something like .abp
        std::string targetFile = QFileDialog::getSaveFileName(this, "Lưu tệp sao lưu", "", "Tệp sao lưu ATOMIC (*.tbp)").toStdString();

        if (targetFile.empty()) return;

        json usersData    = getUsersInfo();
        json contestsData = getContestsInfo();
        json classesData  = getClassesInfo();
        json settingsData = getSettingsInfo();
        json aliasesData  = getAliasesInfo();

        // Let's reconstruct the whole thing to a std::string for each one
        std::string usersReconstructed = "";
        std::string contestsReconstructed = "";
        std::string classesReconstructed = "";
        std::string settingsReconstructed = "";
        std::string aliasesReconstructed = "";
        std::string submissionsReconstructed = "";
        std::string resultsReconstructed = "";
        std::string queuedSubmissionsReconstructed = "";

        // First we reconstruct the whole users thing...
        // To do this, we must have a predefined layout for this
        // So what do we do? Simply watch and learn.
        // Also note this:
        // String is treated as string
        // Boolean is treated as string of T or F (T is true, F is false)
        // Number is treated as string by converting to its string counterpart

        // The order of the reconstructed strings are:
        //   1. users
        //   2. contests
        //   3. classes
        //   4. settings
        //   5. aliases
        //   6. submissions
        //   7. submissions results
        // 7.5. logs (inside submission results)
        //   8. queued submissions
        // These files gonna be pretty heavy

        // The bytearray (thats what i'd like to think but its just char) will look something like this:
        // *                                 |name1|classes separated with & 1|desc1|fullname1|passwd1|pic1|priv1|name2|classes separated with & 2|desc2|fullname2|...
        // ^                                 ^
        // Define the section start.     splitting value                               ...     Until the next *, not |* cuz the final priv does not end with a |
        // each value starts with |, but not end with |

        std::cout << "[backUp()] Reconstructing userData...\n";
        for (const auto& _u : usersData.items()) {
            std::cout << "     User: " << _u.key() << '\n';

            std::string classesStr;
            for (const std::string v : _u.value()["class"]) {
                classesStr += (v + "&");
            }

            std::string t_userSubstr =
                          BACKUP_SEPARATOR + sanitizeValue(_u.key()) + 
                BACKUP_EACHVALUE_SEPARATOR + classesStr + 
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_u.value()["desc"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_u.value()["fullname"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_u.value()["password"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + (_u.value()["picture"].template get<bool>() ? "T" : "F") + 
                BACKUP_EACHVALUE_SEPARATOR + intToString(_u.value()["priv"].template get<int>())
            ;

            usersReconstructed += t_userSubstr;
        }

        // std::cout << usersReconstructed << '\n';

        // Now let's move onto the contests
        // The same format allows for a easy construction of this whole thing, or FACADE, I don't know...
        // Anyway, the format of contests will look something like this:
        // |name|Classes separated by &|Desc|InputFile|InputType|OutputFile|OutputType|TestAmount|Tests|TimeLimit ...
        std::cout << "[backUp()] Reconstructing contestData...\n";
        for (const auto& _c : contestsData.items()) {
            std::cout << "     Contest: " << _c.key() << '\n';

            std::string classesStr;
            for (const std::string v : _c.value()["Classes"]) {
                classesStr += (v + "&");
            }

            std::string testsStr;
            for (const std::vector<std::string> v : _c.value()["Tests"]) {
                // [ "1 1", "2" ]
                testsStr += v[0] + BACKUP_EACHVALUE_SEPARATOR + v[1] + BACKUP_EACHGROUP_SEPARATOR;
            }

            std::string t_contestSubstr =
                          BACKUP_SEPARATOR + sanitizeValue(_c.key()) + 
                BACKUP_EACHVALUE_SEPARATOR + classesStr + 
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_c.value()["Desc"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_c.value()["InputFile"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_c.value()["InputType"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_c.value()["OutputFile"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_c.value()["OutputType"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + intToString(_c.value()["TestAmount"].template get<int>()) + 
                BACKUP_EACHVALUE_SEPARATOR + testsStr + 
                BACKUP_EACHVALUE_SEPARATOR + doubleToString(_c.value()["TimeLimit"].template get<double>())
            ;

            contestsReconstructed += t_contestSubstr;
        }

        // std::cout << contestsReconstructed << '\n';

        // With the contests done and things arent bad at all
        // Let's now move onto the more god forsaken thing of: classes
        // |name|longname|shortname|scoreboard...
        std::cout << "[backUp()] Reconstructing classesData...\n";
        for (const auto& _c : classesData.items()) {
            std::cout << "     Class: " << _c.key() << '\n';
            std::string t_classesStr =
                          BACKUP_SEPARATOR + sanitizeValue(_c.key()) +
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_c.value()["longname"].template get<std::string>()) +
                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(_c.value()["shortname"].template get<std::string>()) + 
                BACKUP_EACHVALUE_SEPARATOR + (_c.value()["scoreboard"].template get<bool>() ? "T" : "F")
            ;

            classesReconstructed += t_classesStr;
        }

        // With the classes done, we move onto our next target
        // Which is settings
        // What do we have here?
        // |max_not_logged_in_session_seconds|reload_time|show_test|wait_time
        // Also this is universal so i dont think i will need to make it in a loop
        std::cout << "[backUp()] Reconstructing settingsData...\n";
        settingsReconstructed =
                      BACKUP_SEPARATOR + doubleToString(settingsData["max_not_logged_in_session_seconds"].template get<double>()) +
            BACKUP_EACHVALUE_SEPARATOR + doubleToString(settingsData["reload_time"].template get<double>()) + 
            BACKUP_EACHVALUE_SEPARATOR + (settingsData["show_test"].template get<bool>() ? "T" : "F") +
            BACKUP_EACHVALUE_SEPARATOR + doubleToString(settingsData["wait_time"].template get<double>())
        ;

        // Now after the settings had been done and all that, lets move onto aliases
        // Aliases is just like settings, also is universal and fixed in size
        // so we dont need to make a loop either
        // |footer|hostname|slogan|softwarelink|website_name
        std::cout << "[backUp()] Reconstructing aliasesData...\n";
        aliasesReconstructed = 
                      BACKUP_SEPARATOR + sanitizeValue(aliasesData["footer"].template get<std::string>()) + 
            BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(aliasesData["hostname"].template get<std::string>()) +
            BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(aliasesData["slogan"].template get<std::string>()) + 
            BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(aliasesData["softwarelink"].template get<std::string>()) + 
            BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(aliasesData["website_name"].template get<std::string>())
        ;

        // With the aliases done, lets move onto submissions.
        // This is WAY harder because of the very bad thing called files
        // AND its something I can say is a fucking shitty thing
        // Now let's actually browse through the FILES
        std::cout << "[backUp()] Reconstructing submissions...\n";
        try {
            // Let's first browse through each registered users, it will be way easier and
            // can serve as a clean up for the whole thing when the user reloads this
            for (const auto& userItem : usersData.items()) {
                std::cout << "     User: " << userItem.key() << '\n';
                // Name is item.key()
                // Now let's browse through each of the SUBMISSIONS by finding the submissions that have the same kind of classes as our userItem!
                // Like, overlapping yk.

                std::vector<std::string> userClasses = userItem.value()["class"];

                for (const auto& contestItem : contestsData.items()) {

                    bool valid = false;
                    for (const std::string& _ucl : userClasses) {
                        for (const std::string _ccl : contestItem.value()["Classes"]) {
                            if (_ucl == _ccl) {
                                valid = true;
                                break;
                            }
                        }

                        if (valid) break;
                    }

                    if (valid) {
                        std::cout << "          Contest: " << contestItem.key() << '\n';
                        // Now we ball.
                        // First, let's get the submissions by formatting these into a path.

                        std::string subPath = dirPath + USERSUBHISTORY_DIR + userItem.key() + "/";

                        if (!std::filesystem::exists(subPath)) continue;

                        std::filesystem::path latestFile;
                        std::filesystem::file_time_type latestTime;

                        // If there are multiple items of different languages, ofc
                        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(subPath))) {
                            if (!entry.is_regular_file()) continue;

                            std::string filename = entry.path().filename().string();

                            // Check if it starts with the contest key
                            if (filename.rfind(contestItem.key(), 0) == 0) {
                                auto ftime = std::filesystem::last_write_time(entry);

                                if (latestFile.empty() || ftime > latestTime) {
                                    latestTime = ftime;
                                    latestFile = entry.path();
                                }
                            }
                        }

                        std::fstream latestFileContent(latestFile, std::ios::in);
                        if (!latestFile.empty() && latestFileContent.is_open()) {
                            // We now have the most recent submission file starting with contestItem.key()
                            std::cout << "Most recent submission for contest " << contestItem.key() << ": " << latestFile << '\n';
                            std::cout << "-------------\n";

                            std::stringstream buffer;
                            buffer << latestFileContent.rdbuf();

                            // Now we now needs to add the data in
                            // Data gonna be somthing like
                            // |username|contest|ext|<VALUE>
                            std::string t_subStr =
                                            BACKUP_SEPARATOR + sanitizeValue(userItem.key()) +
                                BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(contestItem.key()) + 
                                BACKUP_EACHVALUE_SEPARATOR + latestFile.extension().string();
                                BACKUP_EACHGROUP_SEPARATOR + sanitizeValue(buffer.str());
                            ;

                            submissionsReconstructed += t_subStr;

                            buffer.clear();
                        }
                    }
                }
            }

        } catch (std::filesystem::filesystem_error& e) {
            std::cerr << "[backUp()] Filesystem error occured in submissions reconstruction process: " << e.what() << "\n";
            
            // I hate it when this happens, but if it happens then let's just not have any of the submissions saved
            // However, we must ask the user for permission
            QMessageBox::StandardButton rep = QMessageBox::question(this, "Đã có lỗi. Tiếp tục?", "Không thể lấy dữ liệu của bài làm học sinh (Các tệp mã C++, Python hoặc Pascal). Bạn có chắc muốn tiếp tục (Tệp sao lưu sẽ không có dữ liệu bài làm học sinh)?", QMessageBox::Yes | QMessageBox::No);

            if (rep == QMessageBox::No) {
                return;
            } else {
                // If the user agrees to just exclude this shit
                // We should now make the value back to DEFAULT
                
                submissionsReconstructed = "";
            }
        }

        // Now we move onto the results.
        // The submission results.
        // This will be easy since we already had the necessary functions and now we just need to loop around the users
        // So it will be something like this
        std::cout << "[backUp()] Reconstructing submissions results (grading)...\n";
        for (const auto& userItem : usersData.items()) {
            std::cout << "     User: " << userItem.key() << '\n';
            json sub = getSubmissionInfo(userItem.key());

            // Now we just need to actually make it work
            // This can be achieved by simply just, idk, browse through each of the users.
            // Anyway, storing the values is gonna be hard so each users will have shit like this:
            // |username|contestname|grade1|contestname|grade2

            // Following the great success of submission results, I will add in the data of logs.
            // Its gonna be basically the same as submission results but instead of the grades its gonna be logs.
            // Also because its the same the data will be put into submission results to reserve precious storage

            std::string t_subStr = BACKUP_SEPARATOR + userItem.key();

            for (const auto& subv : sub.items()) {
                // Now we have to check io and stuffs ig
                // Just do the same as above...?
                // As long as it works.
                std::string subPath = dirPath + USERSUBHISTORY_DIR + userItem.key() + "/";

                if (!std::filesystem::exists(subPath)) continue;

                std::filesystem::path latestFile;
                std::filesystem::file_time_type latestTime;

                // If there are multiple items of different languages, ofc
                for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(subPath))) {
                    if (!entry.is_regular_file()) continue;

                    std::string filename = entry.path().filename().string();

                    // Check if it starts with the contest key
                    if (filename.rfind(subv.key(), 0) == 0) {
                        auto ftime = std::filesystem::last_write_time(entry);

                        if (latestFile.empty() || ftime > latestTime) {
                            latestTime = ftime;
                            latestFile = entry.path();
                        }
                    }
                }

                if (latestFile.empty()) continue;

                std::cout << "          Contest: " << userItem.key() << '\n';

                std::string logVal;

                // Now we attempt to access the log file
                std::fstream logFile(latestFile, std::ios::in);

                if (!logFile.is_open()) logVal = "";
                else {
                    std::stringstream buffer;
                    buffer << logFile.rdbuf();

                    logVal = buffer.str();

                    buffer.clear();
                }

                t_subStr +=
                    BACKUP_EACHVALUE_SEPARATOR + subv.key() + // subv.key() is contest name
                    BACKUP_EACHVALUE_SEPARATOR + doubleToString(subv.value().template get<double>()) +
                    BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(logVal);
                ;
            }

            resultsReconstructed += t_subStr;
        }

        // Now we have to do the final thing, which is the queued submissions.
        // They are especially tiring to do and is certainly not gonna be a good time.
        // So heres a plan to do it FAST
        // The layout is gonna be something like this:
        // |filename|value
        // so for example:
        // |[nautilus][test01].cpp|#include <bits/stdc++.h>...
        std::cout << "[backUp()] Reconstructing queued submissions...\n";
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(dirPath + USERQUEUE_DIR))) {
            if (entry.is_directory()) continue; // Skip this entry if it is a directory because we cannot use directories.
            
            std::filesystem::path entryPath = entry.path();
            std::cout << "     File: " << entry.path() << '\n';

            // Open the file
            std::fstream entryFile(entryPath, std::ios::in);
            if (entryFile.is_open()) {
                // Now we just have to walk it through
                // Here we will load the value through a buffer
                std::stringstream buffer;

                buffer << entryFile.rdbuf();

                // With the value of the queued file done, we can just **embed** it into the constructed substring
                // The layout of the constructed substring will have the layout as stated above and will be added into the reconstructed
                // string later on
                std::string t_queuedStr = 
                              BACKUP_SEPARATOR + sanitizeValue(entryPath.filename().string()) +
                    BACKUP_EACHVALUE_SEPARATOR + sanitizeValue(buffer.str());
                ;

                buffer.clear();

                // Now we add the constructed substring to the main (used) reconstructed string
                // Gonna be easy ig.
                queuedSubmissionsReconstructed += t_queuedStr;
            } else continue; // if failure to open file, we skip it off
        }

        // Now with ALL the little variables and algorithms done, let's do the real thing of actually outputting to a file
        // Doing this is easy. We just need to create a final, one and only string that represents the file and then output it into the file
        std::cout << "[backUp()] Finalizing...\n";
        std::string data = 
            BACKUP_SECTIONMARKER + usersReconstructed +             // 1.
            BACKUP_SECTIONMARKER + contestsReconstructed +          // 2.
            BACKUP_SECTIONMARKER + classesReconstructed +           // 3.
            BACKUP_SECTIONMARKER + settingsReconstructed +          // 4.
            BACKUP_SECTIONMARKER + aliasesReconstructed +           // 5.
            BACKUP_SECTIONMARKER + submissionsReconstructed +       // 6.
            BACKUP_SECTIONMARKER + resultsReconstructed +           // 7. + 7.5
            BACKUP_SECTIONMARKER + queuedSubmissionsReconstructed   // 8.
        ;

        // Let's now save it in
        std::fstream backupFile(targetFile, std::ios::out | std::ios::trunc);
        if (!backupFile.is_open()) {
            errorDialog("Lưu tệp sao lưu không thành công!");
            backupFile.close();
            return;
        }

        backupFile << data;
        backupFile.close();
    }

    void loadBackUp() {
        std::cout << "[loadBackUp()] Functionality called!\n";

        QMessageBox::information(this, "Chú ý", "Tính năng này vẫn chưa hoàn thiện. Hiện hệ thống vẫn chưa hỗ trợ. Hãy chờ bản cập nhật trong tương lai.");
    }

    private: // PRIVATE FUNCTIONS. These cannot be connected to outside of whatever this object is.
    bool contestOpened; // A variable for showing the information of if the contest settings is opened or not
    bool userOpened;
    bool classOpened;
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

                // Should work?
                if (!judgingProcess->waitForStarted(10000)) {
                    std::cerr << "[judgingProcess: fixme] Fuck. JudingProcess messed up big time here.\n";

                    errorDialog("Mở hệ thống chấm bài không thành công. Lỗi: Thời gian mở hệ thống quá lâu.");
                    judgingProcess->close();

                    judgingEnabled = false;
                    return;
                }

                if (judgingProcess->state() == QProcess::Running) {
                    // If judgingProcess ran without errors
                    std::cout << "Successfully ran JUDGING process with PID " << judgingProcess->processId() << "\n";
                    judgingProcessButton->setText("Dừng chấm bài");
                } else {
                    std::cout << "Process failed to start. Error: " << judgingProcess->error() << '\n'; 
                    errorDialog("Thất bại trong quá trình mở hệ thống chấm bài. Hãy thử cài đặt lại chương trình hoặc cập nhật lên bản cập nhật mới nhất");
                    judgingEnabled = false;
                }
            }
            else {
                // Turn off JUDGING process via ASIO socket
                try {
                    boost::asio::io_context io_context;
                    boost::asio::ip::tcp::socket socket(io_context);
                    
                    // Create endpoint
                    boost::asio::ip::tcp::endpoint endpoint(
                        boost::asio::ip::make_address(JUDGING_EXITADDR), 
                        JUDGING_EXITPORT
                    );

                    // Connect to target
                    boost::system::error_code connect_error;
                    socket.connect(endpoint, connect_error);
                    
                    if (connect_error) {
                        // Connection failure
                        judgingEnabled = true;
                        errorDialog("Kết nối đến hệ thống chấm bài không thành công. Vui lòng đợi một chút và thử lại sau. Nếu vẫn không thành công thì hãy thử khởi động lại hệ thống.");
                    }
                    else {
                        // Send exit message
                        const std::string exitMsg = "exit";
                        boost::system::error_code send_error;
                        boost::asio::write(socket, boost::asio::buffer(exitMsg), send_error);
                        
                        if (!send_error) {
                            // Now, we have to set the button disabled because the user might just spam it up so...
                            judgingProcessButton->setEnabled(false);
                        }
                    }
                    
                    socket.close();
                }
                catch (const std::exception& e) {
                    std::cout << "ASIO Exception in judging exit: " << e.what() << std::endl;
                    errorDialog("Lỗi kết nối ASIO. Chương trình sẽ thoát và hãy mở lại chương trình. Thiết bị của bạn có thể không tương thích với ATOMIC.");
                    close();
                    exit(0);
                }
            }
        } else if (type == "webserver") {
            // In case user is opening the users settings, which is highly bad for the webserver
            if (userOpened) {
                errorDialog("Mở website thất bại. Hãy tắt cài đặt học sinh trước khi tiếp tục!");
                return;
            }

            webserverEnabled = !webserverEnabled;
            std::cout << "Toggled webserver to: " << (webserverEnabled ? "Enabled" : "Disabled") << '\n';
            if (webserverEnabled) {
                if (!isRoot()) {
                    // We can see 2 things:
                    // Using MacOS/Linux and running this software without sudo
                    // Which means -> Running the webserver MIGHT fail because of the port restrictions
                    // Fix? Well easy. We just ask the user for the root password and then run the webserver with sudo
                    // However, this is not a good idea because of security reasons
                    // So we will just warn the user and then let them handle it themselves
                    QMessageBox::StandardButton rep = QMessageBox::warning(this, "Lỗi: Không có quyền truy cập", "Bạn đang sử dụng hệ điều hành MacOS/Linux và không chạy chương trình với quyền root (sudo). Điều này có thể khiến việc khởi động hệ thống website thất bại do các giới hạn về cổng mạng đối với những nhóm người dùng không sử dụng sudo. Nếu tiếp tục, bạn sẽ cần nhập mật khẩu người dùng hiện tại (Người dùng hiện tại cần là sudoer, nếu không thì đừng tiếp tục). Tiếp tục?", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

                    // Whatever mannnn
                    if (rep == QMessageBox::StandardButton::Yes) {
                        // If the user said yes
                        std::cout << "Hey yes! This is dangerous! We are going to step into sudo territory!\n";

                        // We need to input the data!!!!!
                        // bool ok;
                        // QString passwd = QInputDialog::getText(this, "Mật khẩu người dùng hiện tại (sudoer)", "Hãy nhập mật khẩu người dùng hiện tại. Nếu người dùng hiện tại không thể sử dụng sudo thì hãy hủy", );

                        webserverProcess->setProgram(PKEXEC_PATH); // Exec as root with a graphical interface with whatever it is...
                        
                        QStringList args;
                        args << QString::fromStdString(dirPath + PYDIR) << QString::fromStdString(dirPath + WEBSERVER_PATH);
                        webserverProcess->setArguments(args);

                        webserverProcess->start();

                        // Should work?
                        if (!webserverProcess->waitForStarted(10000)) {
                            std::cerr << "[webserverProcess: fixme] WebserverProcess messed up big time here too even with root from pkexec (asked root).\n";

                            errorDialog("Mở hệ thống chấm bài không thành công. Lỗi: Không có quyền truy cập. Tên người dùng hoặc mật khẩu không đúng hoặc thời gian nhập quá lâu");
                            webserverProcess->close();

                            webserverEnabled = false;
                            return;
                        }

                        // Check if the process started normally
                        if (webserverProcess->state() == QProcess::Running) {
                            // If webserverProcess ran without errors
                            std::cout << "Successfully ran WEBSERVER process with PID " << webserverProcess->processId() << "\n";
                            webserverProcessButton->setText("Tắt Website");
                        } else {
                            std::cout << "Process failed to start. Error: " << webserverProcess->error() << '\n'; 
                            errorDialog("Thất bại trong quá trình mở hệ thống chấm bài. Hãy thử cài đặt lại chương trình hoặc cập nhật lên bản cập nhật mới nhất");

                            webserverEnabled = false;
                        }

                        std::cout << "Started WEBSERVER process: " << webserverProcess->program().toStdString() << " " << webserverProcess->arguments().join(' ').toStdString() << '\n';

                        // Putting this here as safeguard before the actual thing works
                        // webserverEnabled = false;
                        // return;
                    } else {
                        std::cout << "Nahhhh. Yes we won't be caring about this.\n";
                        webserverEnabled = false;
                        return;
                    }
                } else { // Rooted already. Dont have to change anything
                    // If we just turned on webserver. We do the actions of turning it up
                    webserverProcess->start();
                    std::cout << "Started WEBSERVER process: " << webserverProcess->program().toStdString() << " " << webserverProcess->arguments().join(' ').toStdString() << '\n';

                    // Should work?
                    if (!webserverProcess->waitForStarted(10000)) {
                        std::cerr << "[webserverProcess: fixme] WebserverProcess messed up big time here also. This is with root from the start without any nagging.\n";

                        errorDialog("Mở hệ thống chấm bài không thành công. Lỗi: Không có quyền truy cập. Tên người dùng hoặc mật khẩu không đúng hoặc thời gian nhập quá lâu");
                        webserverProcess->close();

                        webserverEnabled = false;
                        return;
                    }

                    // Check if the process started normally
                    if (webserverProcess->state() == QProcess::Running) {
                        // If webserverProcess ran without errors
                        std::cout << "Successfully ran WEBSERVER process with PID " << webserverProcess->processId() << "\n";
                        webserverProcessButton->setText("Tắt Website");
                    } else {
                        std::cout << "Process failed to start. Error: " << webserverProcess->error() << '\n'; 
                        errorDialog("Thất bại trong quá trình mở hệ thống chấm bài. Hãy thử cài đặt lại chương trình hoặc cập nhật lên bản cập nhật mới nhất");

                        webserverEnabled = false;
                    }
                }
            } else {
                // Turn off WEBSERVER process via ASIO socket
                try {
                    boost::asio::io_context io_context;
                    boost::asio::ip::tcp::socket socket(io_context);
                    
                    // Create endpoint
                    boost::asio::ip::tcp::endpoint endpoint(
                        boost::asio::ip::make_address(WEBSERVER_EXITADDR), 
                        WEBSERVER_EXITPORT
                    );

                    // Connect to target
                    boost::system::error_code connect_error;
                    socket.connect(endpoint, connect_error);
                    
                    if (connect_error) {
                        // Connection failure
                        errorDialog("Kết nối đến hệ thống website không thành công. Vui lòng đợi một chút và thử lại sau. Nếu vẫn không thành công thì hãy thử khởi động lại hệ thống.");
                    }
                    else {
                        // Send exit message
                        const std::string exitMsg = "exit";
                        boost::system::error_code send_error;
                        boost::asio::write(socket, boost::asio::buffer(exitMsg), send_error);
                        
                        if (!send_error) {
                            // Now, we have to set the button disabled because the user might just spam it up so...
                            webserverProcessButton->setEnabled(false);
                        }
                    }
                    
                    socket.close();
                }
                catch (const std::exception& e) {
                    std::cout << "ASIO Exception in webserver exit: " << e.what() << std::endl;
                    errorDialog("Lỗi kết nối ASIO. Chương trình sẽ thoát và hãy mở lại chương trình. Thiết bị của bạn có thể không tương thích với ATOMIC.");
                    close();
                    exit(0);
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
                // ShellExecuteA(0, "open", (dirPath + LOG_PATH).c_str(), 0, 0, SW_SHOWNORMAL);
                QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(dirPath + LOG_PATH)));
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
            QAction *removeDataInfo = contextMenu.addAction("Xoá kết quả bài làm");
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
            } else if (selectedAction == removeDataInfo) {
                // This is hard
                // To do the job, we need to get the current json value
                json subVal = getSubmissionInfo(username.toStdString());
                // Then removes the submission thing...
                subVal.erase(contestName.toStdString());
                // Finally save it in
                saveSubmissionInfo(username.toStdString(), subVal);

                // Reloading
                refreshTable();
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
        std::unordered_map<std::string, std::vector<double>> contestsPointsByOrder;
        std::vector<std::pair<double, std::string>> usersSums; // More sorting friendly

        // We go through EACH user
        for (const std::string& user : usersList) {
            // Get the contests of EACH user (in JSON format)
            const json submissions = getSubmissionInfo(user);
            if (!submissions.is_null()) { // Not null
                // Success? Nice. Now we just need to do some checking through.
                // Now we read the submissions relative to the contests
                double sum = 0;
                for (std::string contest : currentContests) {
                    // Okay. Maybe also calculate the sums along the way?
                    if (submissions.contains(contest)) {
                        const double relativePoints = submissions[contest];
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
            pointsItem->setText(QString::fromStdString(doubleToString(userSorterValue.first)));  // First is the actual index value
            currentTable->setItem(index, 0, pointsItem);

            // With this done, let's move onto the final nail - the contest points
            std::cout << contestsPointsByOrder[userSorterValue.second].size() << " " << (int)contestsPointsByOrder[userSorterValue.second][0] << '\n';
            if (contestsPointsByOrder[userSorterValue.second].size() == 1 && (int)contestsPointsByOrder[userSorterValue.second][0] == NONE_PLACEHOLDER) {
                // In case this contest does NOT exist (the user never done the contest)
                std::cout << "DOES NOT EXIST\n";
                const int& amountOfDestBlankCells = currentTable->columnCount();

                for (int i = 0; i < amountOfDestBlankCells; i++) {
                    QTableWidgetItem *blankItem = new QTableWidgetItem();
                    blankItem->setFlags(blankItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
                    currentTable->setItem(index, i, blankItem);
                }
            } else {
                int cindex = 1; // Index of column
                for (const double &point : contestsPointsByOrder[userSorterValue.second]) {
                    // If point is not something the user have done
                    if (point == NONE_PLACEHOLDER) {
                        // Yeah it looks like I cant reuse any QTableWidgetItem, actually.
                        QTableWidgetItem *contestPointsItem = new QTableWidgetItem();
                        contestPointsItem->setFlags(contestPointsItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
                        contestPointsItem->setText("");
                        currentTable->setItem(index, cindex, contestPointsItem);

                        cindex++;
                        continue;
                    }

                    // Yeah it looks like I cant reuse any QTableWidgetItem, actually.
                    QTableWidgetItem *contestPointsItem = new QTableWidgetItem();
                    contestPointsItem->setFlags(contestPointsItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
                    contestPointsItem->setText(QString::fromStdString(doubleToString(point)));
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
        std::cout << "[showButtonInfoFromBarType(std::string type)] arg: " << type << '\n';

        if (type == "contests" && !contestOpened) {
            // Sadly, stack allocation can only be used with QDialogs since they will pause
            // all current operations to focus on only the QDialog.
            // This will instantly close on open with stack allocation, so heap allocation (pointers)
            // is the only way (for now)
            WIN_ContestsSettings *cstWin = new WIN_ContestsSettings(this, fonts);
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
        } else if (type == "classes" && !classOpened) {
            WIN_ClassesSettings *clsWin = new WIN_ClassesSettings(this);
            clsWin->show();

            classOpened = true;
            classesSettings->setEnabled(false);

            connect (clsWin, &WIN_ClassesSettings::closed, this, [this] {
                classOpened = false;
                classesSettings->setEnabled(true);
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
                // On this one case
                terminateProcess(static_cast<int>(ollamaProcess->processId()));
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

int main(int argc, char* argv[]) {
    // Neutralize qt6ct (and other platform themes) before QApplication
    qputenv("QT_QPA_PLATFORMTHEME", "");
    qputenv("QT_STYLE_OVERRIDE", "fusion");

    // 1) Simple console check from command line
    // bool hideConsole = true;
    // for (int i = 1; i < argc; ++i) {
    //     if (std::string(argv[i]) == "--console") {
    //         hideConsole = false;
    //     }
    // }

    // Yeah no this is useless now

    // 2) Qt arguments – reuse real argc/argv from main()
    int qt_argc = argc;
    char** qt_argv_ptr = argv;

    std::cout << "Application running with console support.\n";

    if (terminateProcessByName("ollama")) {
        std::cout << "Terminated ollama.\n";
    }

    // Alligator
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

    int ret = 1; // Default return

    try {
        // 3) Launch Qt application
        QApplication app(qt_argc, qt_argv_ptr);
        app.setApplicationDisplayName("ATOMIC");
        app.setApplicationName("ATOMIC");
        app.setApplicationVersion("v0.1");
        Q_INIT_RESOURCE(qres);

        // Now over to the default font
        QFontDatabase::removeAllApplicationFonts(); // wipes theme fonts

        QFont defaultFont;
        int defaultFontId = QFontDatabase::addApplicationFont(DEFAULTFONT_PATH);
        if (defaultFontId < 0) {
            std::cerr << "Failed to load default font from " << DEFAULTFONT_PATH.toStdString() << "\n";
            defaultFont.setFamilies({"Segoe UI", "Arial", "Tahoma", "sans-serif"});
        } else {
            QStringList loadedFamilies = QFontDatabase::applicationFontFamilies(defaultFontId);
            if (!loadedFamilies.isEmpty()) {
                defaultFont.setFamily(loadedFamilies.at(0));
                std::cout << "Loaded default font: " << loadedFamilies.at(0).toStdString() << "\n";
            } else {
                std::cout << "No families found in loaded default font\n";
                defaultFont.setFamilies({"Segoe UI", "Arial", "Tahoma", "sans-serif"});
            }
        }

        defaultFont.setPointSize(10);
        app.setFont(defaultFont); // Set the default font for the entire application
        app.setFont(defaultFont, "QWidget");

        PanelWindow panel(nullptr);
        panel.fonts["default"] = defaultFont;
        panel.initialize();

        // if (!hideConsole) {
        //     QMessageBox::warning(
        //         nullptr, 
        //         "Hiện bảng lịch sử hoạt động chương trình (logging)", 
        //         "Bạn đang bắt đầu ATOMIC bằng tham số --console, điều này không được khuyến nghị bởi vì sẽ có thể xảy ra các lỗi liên quan đến bộ nhớ khi I/O chưa hoàn thiện. Nếu bạn không có ý định gỡ rối chương trình, hãy tránh sử dụng tham số --console khi khởi động chương trình", 
        //         QMessageBox::Ok
        //     );
        // }

        panel.show();
        ret = app.exec();

    } catch (const std::exception& e) {
        // if (!hideConsole) {
        std::cout << "Exception caught: " << e.what() << std::endl;
        // } else {
        QMessageBox::critical(
            nullptr, 
            "Error", 
            QString("An unexpected error occurred: %1").arg(e.what()), 
            QMessageBox::Ok
        );
        // }
        ret = -1;
    } catch (...) {
        // if (!hideConsole) {
        std::cout << "Unknown exception caught" << std::endl;
        // } else {
        QMessageBox::critical(
            nullptr, 
            "Error", 
            "An unknown error occurred", 
            QMessageBox::Ok
        );
        // }
        ret = -1;
    }

    // if (!hideConsole) {
    //     std::cin.get();
    // }

    return ret;
}