/**********************************

Compilation arguments:
"-fdiagnostics-color=always",
"-I\"C:\\Qt\\6.5.3\\mingw_64\\include\"",
"-I\"C:\\Qt\\Tools\\mingw1120_64\\include\"",
"-L\"C:\\Qt\\6.5.3\\mingw_64\\lib\"",
"-L\"C:\\Qt\\Tools\\mingw1120_64\\lib\"",
"-g",
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
#include <QtWidgets/QVBoxLayout>   // Layout of QWidgets
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
#include <QtGui/QAction>           // Action for menus. Wonder what fucker thought to put it in QtGui
#include <QtGui/QCloseEvent>       // Close event. The action of 'X' button
#include <QtGui/QDoubleValidator>  // Validator for edits.
#include <QtGui/QIcon>             // Icon readings
#include <QtGui/QPixmap>           // Picture reading

// Importing Qt values
#include <QtCore/Qt>   // A bunch of constants

// File I/O actions and getting data
#include <fstream>
#include <nlohmann/json.hpp>
// Turn nlohmann::json to json for short.
using json = nlohmann::json;

// System-related actions
#include <filesystem>
#include <Windows.h>

// Debug console
#include <iostream>

// Constants
// -> Paths
const std::string THEME_PATH = "/source/theme.qss";
const std::string SETTINGS_PATH = "/source/settings.json";
const std::string VERSION_PATH = "/source/version.json";
const std::string ICON_PATH = "/icon.ico";

// -> Qt Style Sheet
const std::string STYLE_BIGLABEL = "font-size: 16px; font-weight: bold;";

// -> Others
const std::string GITHUB_PAGE = "\"https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER\"";

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

    // Data variables
    json settings; // NULL at first
    json version; // NULL at first
    QPixmap iconPixmap;

    // Paths
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
        judgingWaitTimeLabel->setStyleSheet(QString::fromStdString(STYLE_BIGLABEL));
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
        judgingReloadTimeLabel->setStyleSheet(QString::fromStdString(STYLE_BIGLABEL));
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
        judgingShowTestLabel->setStyleSheet(QString::fromStdString(STYLE_BIGLABEL));
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


        ///////////////////////
        // Finishing touches //
        ///////////////////////

        // Settings for container
        container->setLayout(mainLayout);

        // Settings for QMainWindow
        setCentralWidget(container);
    }

    void about() {
        QWidget *aboutFrame = new QWidget();
        aboutFrame->setFixedSize(300, 100);
        aboutFrame->show();
        aboutFrame->setWindowTitle("Về ATOMIC");
        aboutFrame->setWindowIcon(QIcon(iconPixmap));
        
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
                versionLabel->setText(versionText.c_str());

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

                QVBoxLayout *layout = new QVBoxLayout();
                layout->addWidget(splitter);
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

    void gitHub() {
        // Executing the github repo into Windows
        ShellExecute(0, "open", GITHUB_PAGE.c_str(), 0, 0, SW_SHOWNORMAL);
    }

    void backUp() {

    }

    void loadBackUp() {

    }

    private: // PRIVATE FUNCTIONS. These cannot be connected to outside of whatever this object is.
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
        QMessageBox::information(this, "Lỗi", QString::fromStdString("Đã có lỗi xảy ra: " + error), QMessageBox::Ok);
    }

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
