#include "WIN_UsersSettings.h"    // Main method
#include "CST_PlainTextDialog.h"  // New user dialog
#include "consts.h"               // User-defined constants
#include "utilities.h"            // Utilities functions (all are inlines)

#include <QtWidgets/QVBoxLayout>  // Vertical layout
#include <QtWidgets/QHBoxLayout>  // Horizontal layout
#include <QtWidgets/QSplitter>    // Splitting widgets
#include <QtWidgets/QMessageBox>  // Message box
#include <QtWidgets/QLabel>       // Label
#include <QtWidgets/QPushButton>  // A button

#include <QtGui/QCloseEvent>

#include <QtCore/QTimer>

WIN_UsersSettings::WIN_UsersSettings(QWidget *parent) : QWidget(parent, Qt::Window) {
    setObjectName("container");
    setWindowIcon(parent->windowIcon());
    setStyleSheet(parent->styleSheet());
    setWindowTitle("Cài đặt học sinh");
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning
    
    setMinimumHeight(400);
    setMinimumWidth(600);
    resize(700, 450);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    // Split the details and the list
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    layout->addWidget(splitter);

    // Main part of the initialization of this shit
    QWidget *sidebar = new QWidget(splitter);
    sidebar->setMaximumWidth(280);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebar->setLayout(sidebarLayout);
    splitter->addWidget(sidebar);

    // Sidebar configuration
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->addWidget(listView);

    connect(listView, &QListWidget::itemClicked, this, [this](const QListWidgetItem *item) {
        const int rowIndex = listView->row(item);

        toUser(userByRowOrder[rowIndex]);
    });

    // Declare details
    QWidget *details = new QWidget(splitter);
    details->setObjectName("con_sec");
    QVBoxLayout *detailsLayout = new QVBoxLayout(details);
    details->setLayout(detailsLayout);
    splitter->addWidget(details);

    // Details layout configuration
    detailsLayout->setAlignment(Qt::AlignTop);

    // Details' elements
    QLabel *classLabel = new QLabel(details);
    classLabel->setText("Các lớp học sinh thuộc về");

    QPushButton *saveBtn = new QPushButton(details);
    saveBtn->setObjectName("genericBtn");
    saveBtn->setText("Lưu");
    saveBtn->setFixedWidth(100);

    connect(saveBtn, &QPushButton::clicked, this, [this] {
        saveInfo(currentUser);
    });

    classListing->setReadOnly(true);

    passwdLabel->setText("Mật khẩu đã mã hoá SHA256 hiện tại: ");
    passwdLabel->setWordWrap(true);

    QPushButton *passwdBtn = new QPushButton(details);
    passwdBtn->setObjectName("genericBtn");
    passwdBtn->setText("Đổi mật khẩu");

    connect(passwdBtn, &QPushButton::clicked, this, WIN_UsersSettings::changePassword);

    // Details' elements adding in order
    detailsLayout->addWidget(passwdLabel);
    detailsLayout->addWidget(passwdBtn);

    detailsLayout->addWidget(classLabel);
    detailsLayout->addWidget(classListing);

    detailsLayout->addWidget(saveBtn);

    // Initialization commands
    QTimer::singleShot(0, this, &WIN_UsersSettings::initAct);
}

void WIN_UsersSettings::initAct() {
    // Now, we just fetch the users
    loadUsers();

    selectUserFromRow(0);
}

void WIN_UsersSettings::loadUsers() {
    // Alright. Let's first open the file.
    reloadVars();

    // Okay. That might looks good on paper, but we also have to validify the fact that
    // there is AT LEAST 1 guy here. Else it doesn't really work and still, adding one like that is a good touch!
    if (users.size() == 0) {
        std::cout << "[*UsersSettings] No users\n";
    }

    // Now we will cycle through each user to get a nlohmann/json item object,
    // which consists of two main things we will use:
    // - item.key()
    // - item.value()
    // Also I will refresh the variables related first
    userByRowOrder.clear();
    userByRowOrder.reserve(users.size());

    // Do ze thin
    for (const auto& item : users.items()) {
        std::cout << "Processing " << item.key() << "\n";

        try {
            QListWidgetItem *listItem = new QListWidgetItem(listView);
        
            QWidget *itemWidget = new QWidget(listView);
            QHBoxLayout *itemWidgetLayout = new QHBoxLayout(itemWidget);
            itemWidget->setLayout(itemWidgetLayout);

            QLabel *userLabel = new QLabel(itemWidget);
            userLabel->setStyleSheet(STYLE_BIGLABEL);
            userLabel->setText(QString::fromStdString(item.key()));
            itemWidgetLayout->addWidget(userLabel);

            // Adding the widget and item into the listview
            listItem->setSizeHint(itemWidget->sizeHint());
            listView->setItemWidget(listItem, itemWidget);

            userByRowOrder.push_back(item.key());

        } catch (const std::bad_alloc& e) {
            errorDialog("Không đủ bộ nhớ để tạo giao diện");
            close();
        }
    }
}

void WIN_UsersSettings::reloadVars() {
    // Now, we resync the whole... thing... there...?
    // I mean, the `users` variable, right? I will reload it by
    // simply calling a function. Yessir!!!
    users = getUsersInfo();
}

void WIN_UsersSettings::selectUserFromRow(int row) {
    listView->setCurrentRow(row);

    toUser(userByRowOrder[row]);
}

void WIN_UsersSettings::toUser(std::string username) {
    reloadVars();
    
    // Updating some variables
    currentUser = username;

    // Updating the classes hash status
    passwdLabel->setText("Mật khẩu đã mã hoá SHA256 hiện tại: " + QString::fromStdString(privatizesha256(users[username]["password"], 8)));
    currentPasswdHash = users[username]["password"];

    // Updating the class listing
    std::vector<std::string> classes;
    classes.reserve(users[username]["class"].size()); // Yeah users["class"] needs to exists, else it will crash.
    classes = users[username]["class"];
    // Converting classes to a QStringList
    QStringList classesInQStringList = {};
    for (const std::string& i : classes) {
        classesInQStringList << QString::fromStdString(i);
    }
    classListing->setEntries(classesInQStringList);

    std::cout << "[*UsersSettings] Refreshed user \"" << username << "\"\n";
}

void WIN_UsersSettings::changePassword() {
    std::cout << "[*UsersSettings] Change password!\n";

    CST_PlainTextDialog dialog(
        this, 
        "Mật khẩu", 
        "Hãy nhập mật khẩu mới cho học sinh " + QString::fromStdString(currentUser) + ". Phần mềm chấm bài sẽ tự động mã hoá bằng thuật toán HASH SHA256."
    );

    if (dialog.exec() == QDialog::Accepted) {
        std::string hashed = sha256(dialog.getResult().toStdString());
        std::cout << "New hashed password: " << hashed << '\n';

        currentPasswdHash = hashed;
        passwdLabel->setText("Mật khẩu đã mã hoá SHA256 hiện tại: " + QString::fromStdString(privatizesha256(hashed, 8)) + " (chưa lưu)");
    }
}

void WIN_UsersSettings::saveInfo(std::string username) {
    // Now we fetch the current classes of the student. It's needed, after all.
    QStringList currentClassesQ = classListing->getEntries();
    std::vector<std::string> classes;
    classes.reserve(currentClassesQ.size());
    for (const QString& c : currentClassesQ) {
        classes.push_back(c.toStdString());
    }

    users[username]["class"] = classes;
    users[username]["password"] = currentPasswdHash;

    // Saving the info into the json file.
    if (!saveUsersInfo(users)) {
        errorDialog("Lưu dữ liệu không thành công.");
    } else {
        // If saving the data really is done without any hardships, we can safely removes
        // the caching (IF THERE EXISTS)
        // Also, it should be fine to refresh the whole page.
        toUser(username);
    }

    std::cout << "[*UsersSettings] Saved user information " << username << '\n';
}

void WIN_UsersSettings::closeEvent(QCloseEvent *event) {
    emit closed();
    event->accept();
    this->deleteLater();
}

void WIN_UsersSettings::errorDialog(std::string error) {
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText(QString::fromStdString("Đã có lỗi xảy ra: " + error));
    msgBox->setWindowTitle("Lỗi");
    msgBox->setIcon(QMessageBox::Critical);
    msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
    // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
    msgBox->setWindowIcon(windowIcon());
    msgBox->setStyleSheet(styleSheet());

    msgBox->show();
}