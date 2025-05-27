#include "WIN_ContestsSettings.h"
#include "consts.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTableWidget>

#include <QtGui/QCloseEvent>

#include <fstream>
#include <iostream>

WIN_ContestsSettings::WIN_ContestsSettings(QWidget *parent) {
    setObjectName("container");
    setWindowIcon(parent->windowIcon());
    setStyleSheet(parent->styleSheet());
    setWindowTitle("Cài đặt bài thi");
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning
    
    setMinimumHeight(400);
    setMinimumWidth(600);
    resize(QSize(700, 450)); // this is tiring asf ngl
    
    // LAYOUT
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);
    
    // SPLITTER (for sidebar and details pane)
    QSplitter *splitter = new QSplitter();
    layout->addWidget(splitter);
    
    // The label showing the currently selected contest. Needs to be way up here since this has
    // to be the first reference (except for the part to register this variable).
    contestNameLabel->setStyleSheet(STYLE_BIGLABEL);
    contestNameLabel->setText("Chưa chọn bài thi nào"); // TODO: Fix this useless crap

    // Save button. Reference before fetch contests. All details widgets needs to be referenced first
    // before fetchContests() is called since that's gotta be how it works or else it will be overwritten
    // by the default settings.
    saveBtn->setObjectName("genericBtn");
    saveBtn->setText("Lưu");
    saveBtn->setEnabled(false); // Turns disabled IF there is no contest to save to.
    saveBtn->setFixedWidth(100);
    connect(saveBtn, &QPushButton::clicked, this, WIN_ContestsSettings::saveInfo);

    // INITIALIZATION STEP. I have no idea why I need to put this function caller way up here. But whatever.
    // after all it does work and thats what matters.
    fetchContests(true);
    
    // SIDEBAR
    listView->setObjectName("con_sec");
    listView->setMaximumWidth(280);
    
    // CONTEST DETAILS (A normal containable widget)
    QScrollArea *contestDetailsScrollable = new QScrollArea();
    contestDetailsScrollable->setWidgetResizable(true); // IMPORTANT: This makes the scroll area resize the widget
    contestDetailsScrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    contestDetailsScrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Set size policy to expand in both directions
    contestDetailsScrollable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QWidget *contestDetails = new QWidget();
    contestDetails->setObjectName("con_sec");
    contestDetails->setStyleSheet(styleSheet());
    QVBoxLayout *contestDetailsLayout = new QVBoxLayout(contestDetails);
    
    // Setting the layout
    contestDetails->setLayout(contestDetailsLayout);

    contestDetailsLayout->addWidget(contestNameLabel);

    // The label showing the description section
    QLabel *descLabel = new QLabel(contestDetails);
    descLabel->setObjectName("con_lab");
    descLabel->setText("Mô tả bài thi");

    QLabel *classLabel = new QLabel(contestDetails);
    classLabel->setObjectName("con_lab");
    classLabel->setText("Các lớp nhận bài thi");
    
    // Configuring classList
    classList->setReadOnly(true);

    // Label for the table that would represent tests
    QLabel *cnTestLabel = new QLabel(contestDetails);
    cnTestLabel->setObjectName("con_lab");
    cnTestLabel->setText("Các bộ kiểm tra kết quả");

    // Table that will represent tests
    testTable->setMinimumHeight(130);

    // Adding in the widgets in order (for the looks actually.).
    contestDetailsLayout->addWidget(descLabel);
    contestDetailsLayout->addWidget(descEdit);
    contestDetailsLayout->addWidget(classLabel);
    contestDetailsLayout->addWidget(classList);
    contestDetailsLayout->addWidget(cnTestLabel);
    contestDetailsLayout->addWidget(testTable);
    contestDetailsLayout->addWidget(saveBtn);
    
    contestDetailsScrollable->setWidget(contestDetails);
    
    splitter->addWidget(listView);
    splitter->addWidget(contestDetailsScrollable);
}

// --------------------------
// Purpose: Look at the name
// --------------------------
void WIN_ContestsSettings::reloadContestsVar() {
    std::fstream contestsFile(dirPath + CONTESTS_PATH, std::ios::in);
    if (contestsFile.is_open()) {
        // No errors
        try {
            // Apply the neccessary data
            contests = json::parse(contestsFile);

            std::cout << contests << '\n';
        } catch (const json::parse_error& e) {
            // Yes errors
            errorDialog("Không thể đọc dữ liệu bài thi (Tệp bị hỏng?).");
            close();
        }
    } else {
        std::cout << "Unable to open contestsFile\n";
        errorDialog("Không thể truy cập dữ liệu bài thi.");
        close();
    }
    contestsFile.close();
}

// ---------------------------------------------------------------
// Purpose: A way to fetch new contests when I just opened it up.
// ---------------------------------------------------------------
void WIN_ContestsSettings::fetchContests(bool selectEntryAutomatically) {
    reloadContestsVar();
    listView->clear();

    // After we've got the required data to secure the ticket to showing whats needed
    // A.K.A showing the contests in a LIST VIEW. Which is pretty much obsolete at this
    // point but I dont have any other alternative that is much easier to use.
    for (const auto& item : contests.items()) {
        // std::cout << "[*ContestsSettings] Now processing: " << item.key() << '\n';
        contestByRowOrder.push_back(item.key());
        const QString contestName = QString::fromStdString(item.key());

        QListWidgetItem *listItem = new QListWidgetItem(listView);

        // Actually. Even though it will be kinda a memory hog, I still wants to make it looks BEAUTIFUL
        // So. What's the looks?
        // +-----------------------+
        // | BIG LABEL FOR CONTEST |
        // | smaller for classes   |
        // +-----------------------+
        // This will be achievable with a QWidget
        QWidget *listItemWidget = new QWidget(this);
        QVBoxLayout *listItemWidgetLayout = new QVBoxLayout(listItemWidget); // Actually, I don't like the names to be too
                                                                                // fucking long. But I have no other choices

        listItemWidget->setLayout(listItemWidgetLayout); // Setting the layout. Now we just need to make all these work by
                                                            // Adding QLabels
        
        QLabel *contestHeader = new QLabel(listItemWidget);
        contestHeader->setStyleSheet(STYLE_BIGLABEL);
        contestHeader->setText(contestName);

        QLabel *contestInf = new QLabel(listItemWidget); // The smaller information label

        // Setting text
        contestInf->setText(QString::fromStdString(contests[item.key()]["Desc"]));
        contestInf->setTextFormat(Qt::RichText);
        contestInf->setWordWrap(false); // No automatic wrapping
        contestInf->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed); // No expanding.
        contestInf->setFixedHeight(contestInf->fontMetrics().height() + 4); // Housing ONE LINE only
        // std::cout << item.key() << ": " << item.value() << '\n';

        // Adding the newly made headers and inf elements
        listItemWidgetLayout->addWidget(contestHeader);
        listItemWidgetLayout->addWidget(contestInf);

        // Now we apply the QWidget in
        listItem->setSizeHint(listItemWidget->sizeHint());
        listView->setItemWidget(listItem, listItemWidget);

        if (contestByRowOrder.size() == 1 && selectEntryAutomatically) { // If this is a first
            // This is created so that it will execute only if there is at least one contest
            // available.
            // Else then nothing will show and it should be disabled (by default).
            listView->setCurrentItem(listItem);
            contestNameLabel->setText(contestName);
            saveBtn->setEnabled(true);
            currentCnts = contestName.toStdString();

            toCnts(contestName.toStdString());
            std::cout << "Performed special move on contestByRowOrder. Guess what it is :)\n";
        }
    }

    // Styling to make the list view works like a navigation pane
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Prevent editing

    // Connecting the signal of selection.
    connect(listView, &QListWidget::itemClicked, this, [this](const QListWidgetItem *item) {
        const int index = listView->row(item);

        std::cout << "Clicked on " << contestByRowOrder[index] << '\n';
        currentCnts = contestByRowOrder[index];
        toCnts(contestByRowOrder[index]);
    });
}

// --------------------------------------------------------------------
// Purpose: To save the information to the FILE. Alright? Not the JSON
//          variable but the FILE ITSELF. Kinda like syncing
// --------------------------------------------------------------------
void WIN_ContestsSettings::saveInfo() {
    contests[currentCnts]["Desc"] = descEdit->toHtml().toStdString();

    // Classes will be different since it is a list
    std::vector<std::string> classVectorList;
    for (const QString& className : classList->getEntries()) {
        classVectorList.push_back(className.toStdString());
    }
    contests[currentCnts]["Classes"] = classVectorList;
    // std::cout << descEdit->toHtml().toStdString();

    // Now some I/O trickery because to be honest, Idk how it works either
    std::fstream contestsFile(dirPath + CONTESTS_PATH, std::ios::out);
    if (contestsFile.is_open()) {
        contestsFile << contests;
    } else {
        errorDialog("Mở tệp bài làm không thành công. Đã có lỗi xảy ra.");
    }
}

// --------------------------------------------------------
// Purpose: To refresh currently clicked on contest with a
//          compact function
// --------------------------------------------------------
void WIN_ContestsSettings::toCnts(std::string contestName) {
    reloadContestsVar();

    contestNameLabel->setText(QString::fromStdString(contestName));
    descEdit->setHtml(QString::fromStdString(contests[contestName]["Desc"]));

    // Identifying all classes that will be recieving this contest
    QStringList classStringList;
    for (const std::string& className : contests[contestName]["Classes"]) {
        classStringList << QString::fromStdString(className); // Incorporate to a QStringList
        std::cout << className << ", "; // Debugging
    }
    std::cout << "\b\b\n"; // Idk my man

    // Apply into the classList
    classList->setEntries(classStringList);

    // Now we see the amount of tests in correspondant with the amount of rows
    testTable->setRowCount(contests[contestName]["TestAmount"]);
    
    std::cout << "[*ContestsSettings] Refreshed Information (details) panel.\n";
}

// ---------------------------------------------------------------------------
// Purpose: Showing errors faster than having to type an actual long command.
//          Can't believe I have to make a duplicate
// ---------------------------------------------------------------------------
void WIN_ContestsSettings::errorDialog(std::string error) {
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setText(QString::fromStdString("Đã có lỗi xảy ra: " + error));
    msgBox->setWindowTitle("Lỗi");
    msgBox->setIcon(QMessageBox::Critical);
    msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
    // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
    msgBox->setWindowIcon(windowIcon());
    msgBox->setStyleSheet(styleSheet());

    msgBox->show();
}

void WIN_ContestsSettings::closeEvent(QCloseEvent *event) {
    emit closed();
    event->accept();
    this->deleteLater(); // Cleaning.
}