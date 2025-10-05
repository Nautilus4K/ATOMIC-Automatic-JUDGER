#include "WIN_ContestsSettings.h"        // The main shyt
#include "WIN_GenerateTestCasesDialog.h" // Test cases generation
#include "CST_TestCaseDialog.h"          // For the dialog of the test cases
#include "CST_PlainTextDialog.h"         // Plain text dialog (for new contests)
#include "consts.h"                      // Constants
#include "utilities.h"                   // Utilities (custom conversion functions)

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTableWidget>

#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>

#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

WIN_ContestsSettings::WIN_ContestsSettings(QWidget *parent, const std::unordered_map<std::string, QFont>& parentFonts) : QWidget(parent, Qt::Window) {
    setObjectName("container");
    setWindowIcon(parent->windowIcon());
    setStyleSheet(parent->styleSheet());
    setWindowTitle("Cài đặt bài thi");
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning
    
    setMinimumHeight(400);
    setMinimumWidth(600);
    resize(QSize(700, 450)); // this is tiring asf ngl

    fonts = parentFonts;
    
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
    connect(saveBtn, &QPushButton::clicked, this, &WIN_ContestsSettings::saveInfo);

    // INITIALIZATION STEP. I have no idea why I need to put this function caller way up here. But whatever.
    // after all it does work and thats what matters.
    fetchContests(true);
    
    // SIDEBAR
    QWidget *sidebar = new QWidget(this);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebar->setLayout(sidebarLayout);
    sidebar->setMaximumWidth(280);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    listView->setObjectName("con_sec");
    sidebarLayout->addWidget(listView);

    // Connecting the signal of selection.
    connect(listView, &QListWidget::itemClicked, this, [this](const QListWidgetItem *item) {
        const int index = listView->row(item);

        std::cout << "Clicked on " << contestByRowOrder[index] << '\n';
        currentCnts = contestByRowOrder[index];
        toCnts(contestByRowOrder[index]);
    });

    QPushButton *addContestBtn = new QPushButton(sidebar);
    addContestBtn->setObjectName("genericBtn");
    addContestBtn->setText("+ Bài thi mới");
    sidebarLayout->addWidget(addContestBtn);

    connect(addContestBtn, &QPushButton::clicked, this, [this] {
        newContest(false);
    });
    
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
    
    // List of test cases
    testCasesList->setMinimumHeight(150);

    // Adding test cases functionality
    QWidget *addTestCaseArea = new QWidget(this); // We are going to have 2 buttons so adding this is good
    QHBoxLayout *addTestCaseAreaLayout = new QHBoxLayout(addTestCaseArea);
    addTestCaseArea->setLayout(addTestCaseAreaLayout);
    addTestCaseAreaLayout->setContentsMargins(0, 0, 0, 0);
    addTestCaseAreaLayout->setAlignment(Qt::AlignRight);

    QPushButton *addTestCaseBtn = new QPushButton(sidebar);
    addTestCaseBtn->setObjectName("genericBtn");
    addTestCaseBtn->setFixedWidth(150);
    addTestCaseBtn->setText("Thêm bộ test");
    connect(addTestCaseBtn, &QPushButton::clicked, this, &WIN_ContestsSettings::addCase);

    QPushButton *aiGenTestCaseBtn = new QPushButton(sidebar);
    aiGenTestCaseBtn->setObjectName("genericBtn");
    aiGenTestCaseBtn->setFixedWidth(40);
    // aiGenTestCaseBtn->setText("Tạo bộ test AI");
    aiGenTestCaseBtn->setIcon(QIcon(AIGENICON_PATH));
    connect(aiGenTestCaseBtn, &QPushButton::clicked, this, [this]{
        // lambda is the most convenient thing everrrr
        generateTestCases(currentCnts);
    });

    addTestCaseAreaLayout->addWidget(addTestCaseBtn);
    addTestCaseAreaLayout->addWidget(aiGenTestCaseBtn);

    QLabel *testCaseSectionLabel = new QLabel(this);
    testCaseSectionLabel->setObjectName("con_lab");
    testCaseSectionLabel->setStyleSheet(STYLE_BOLDLABEL);
    testCaseSectionLabel->setText("CÀI ĐẶT KIỂM TRA BÀI THI");

    // Input to file? (As a fucking QWidget for the better looks)
    QWidget *fileInputCheckArea = new QWidget(this);
    QHBoxLayout *fileInputCheckAreaLayout = new QHBoxLayout(fileInputCheckArea);
    fileInputCheckArea->setLayout(fileInputCheckAreaLayout);
    fileInputCheckAreaLayout->setContentsMargins(0, 0, 0, 4);
    fileInputCheckAreaLayout->setAlignment(Qt::AlignLeft);

    QLabel *fileInputLabel = new QLabel(this);
    fileInputLabel->setText("Đầu vào đọc từ tệp?");

    fileInputCheckAreaLayout->addWidget(fileInputCheck);
    fileInputCheckAreaLayout->addWidget(fileInputLabel);

    QLabel *fileInputNameLabel = new QLabel(this);
    fileInputNameLabel->setText("Tên tệp đầu vào");

    // Connect stuffs in
    connect(fileInputCheck, &QCheckBox::stateChanged, this, [this] {
        onTestCaseCheckBoxToggled("in");
    });

    // Output to file?
    QWidget *fileOutputCheckArea = new QWidget(this);
    QHBoxLayout *fileOutputCheckAreaLayout = new QHBoxLayout(fileOutputCheckArea);
    fileOutputCheckArea->setLayout(fileOutputCheckAreaLayout);
    fileOutputCheckAreaLayout->setContentsMargins(0, 0, 0, 4);
    fileOutputCheckAreaLayout->setAlignment(Qt::AlignLeft);

    QLabel *fileOutputLabel = new QLabel(this);
    // fileOutputLabel->setStyleSheet(STYLE_BOLDLABEL);
    fileOutputLabel->setText("Đầu vào đọc từ tệp?");

    fileOutputCheckAreaLayout->addWidget(fileOutputCheck);
    fileOutputCheckAreaLayout->addWidget(fileOutputLabel);

    QLabel *fileOutputNameLabel = new QLabel(this);
    fileOutputNameLabel->setText("Tên tệp đầu ra");

    // Connect stuffs out
    connect(fileOutputCheck, &QCheckBox::stateChanged, this, [this] {
        onTestCaseCheckBoxToggled("out");
    });

    // Time limit
    QLabel *timeLimitLabel = new QLabel(this);
    timeLimitLabel->setText("Thời gian thực hiện chương trình tối đa (giây)");

    QDoubleValidator *timeLimitValidator = new QDoubleValidator(0, 100, 2, this);
    timeLimit->setValidator(timeLimitValidator);

    // Adding in the widgets in order (for the looks actually.).
    contestDetailsLayout->addWidget(descLabel);
    contestDetailsLayout->addWidget(descEdit);

    contestDetailsLayout->addWidget(classLabel);
    contestDetailsLayout->addWidget(classList);

    contestDetailsLayout->addWidget(testCaseSectionLabel);

    contestDetailsLayout->addWidget(fileInputCheckArea);
    contestDetailsLayout->addWidget(fileInputNameLabel);
    contestDetailsLayout->addWidget(fileInputName);

    contestDetailsLayout->addWidget(fileOutputCheckArea);
    contestDetailsLayout->addWidget(fileOutputNameLabel);
    contestDetailsLayout->addWidget(fileOutputName);

    contestDetailsLayout->addWidget(timeLimitLabel);
    contestDetailsLayout->addWidget(timeLimit);
    
    contestDetailsLayout->addWidget(cnTestLabel);
    contestDetailsLayout->addWidget(testCasesList);
    contestDetailsLayout->addWidget(addTestCaseArea);

    contestDetailsLayout->addWidget(saveBtn);
    
    contestDetailsScrollable->setWidget(contestDetails);
    
    splitter->addWidget(sidebar);
    splitter->addWidget(contestDetailsScrollable);

    // Initialize some variables
    *generatingTestCases = false;
}

void WIN_ContestsSettings::generateTestCases(std::string contestName) {
    // QMessageBox::information(this, "Chức năng chưa hoàn thiện", 
    //     "Chức năng này hiện chưa được hoàn thiện. Mặc dù hiện tại đã có để được sử dụng nhưng kết quả được trả về bởi AI có thể không như mong đợi.", 
    //     QMessageBox::Ok
    // );

    saveInfo();

    if (*generatingTestCases) {
        QMessageBox::warning(this, "Đang trong quá trình tạo dựng bộ test",
            "Có 1 bộ test vẫn đang trong quá trình tạo dựng. Chỉ 1 bộ test có thể được tạo dựng cùng lúc với trí tuệ nhân tạo.",
            QMessageBox::Ok
        );

        return;
    }

    WIN_GenerateTestCasesDialog *dlg = new WIN_GenerateTestCasesDialog(this, generatingTestCases, contestName); // Change this bool straight outta the box
    dlg->show();

    // Let's connect to the signal thing
    connect(dlg, &WIN_GenerateTestCasesDialog::exportResultsEvent, this, [this](std::vector<testCase> v, bool o) {
        std::cout << "Tests: " << v.size();
        if (o) std::cout << " (override)";
        std::cout << '\n';

        // Let's now branch it all

        // Override?
        if (o) {
            // indexesToBeRemoved.push_back(index);
            for (int i = 0; i < contests[currentCnts]["Tests"].size(); i++) {
                indexesToBeRemoved.push_back(i);
            }
        }

        // No override flag
        // Let's add all v's test cases into the current contests' cases shall we?
        // Copy pasted
        for (const testCase& vs : v) {
            indexesToBeAdded.push_back({vs.input, vs.output}); // Add in the queue
        }

        saveInfo();
    });
}

void WIN_ContestsSettings::remContest(QListWidgetItem *item) {
    QMessageBox::StandardButton reply = QMessageBox::warning(this, 
        "Hành động nguy hiểm", "Bạn có chắc muốn tiếp tục xoá bài thi này? Hành động này cũng sẽ xoá tất cả các bài làm học sinh thuộc về bài thi này.", 
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::No) {
        return;
    }

    std::cout << "[*] Gathering required informations\n";
    // Get the row
    int row = listView->row(item);
    int selectedRow = listView->currentRow();

    // Logging and gather neccessary informations
    std::string contestName = contestByRowOrder[row]; // Will desync if not refreshed properly
    std::cout << "Removes contest on row " << row << " which corresponds to contest with the name \"" << contestName << "\"\n";

    // We remove... the contest.
    if (contests.contains(contestName)) {
        contests.erase(contestName);
    }

    // Save it in...
    saveContestsInfo(contests);

    std::cout << "[*] Performing clean up sequence...\n";
    // Tie up loose ends
    listView->itemWidget(item)->deleteLater();

    // To do this, we need to list all items in the user data directory
    for (const auto& entry : fs::directory_iterator(dirPath + USERSUBHISTORY_DIR)) {
        if (fs::is_directory(entry)) {
            // Now that this entry is valid, we can proceed to find even more things inside.
            // Let's first get the directory name first.
            const std::string subDirName = entry.path().filename().string();

            std::cout << "[*] - Checking " << dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".cpp" << '\n';
            if (fs::exists(dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".cpp")) {
                fs::remove(dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".cpp");
            }

            std::cout << "[*] - Checking " << dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".pas" << '\n';
            if (fs::exists(dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".pas")) {
                fs::remove(dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".pas");
            }

            std::cout << "[*] - Checking " << dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".py" << '\n';
            if (fs::exists(dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".py")) {
                fs::remove(dirPath + USERSUBHISTORY_DIR + subDirName + "/" + contestName + ".py");
            }
        }
    }

    std::cout << "[*] Refreshing window...\n";
    // We refresh the entire thing
    fetchContests(false);

    // We need to check if the new row has to change much
    std::cout << "[*] Switching SELECTED ROW\n";
    if (row == selectedRow) {
        // Okay. Then change the now selected row to something else. NEWER. Yes
        QTimer::singleShot(0, this, [this] {
            selectNewContest(contestByRowOrder[0]);
        });
    } else if (selectedRow > row) // Account for offsets
        QTimer::singleShot(0, this, [this, selectedRow] {
            selectNewContest(contestByRowOrder[selectedRow - 1]);
        });
    else
        QTimer::singleShot(0, this, [this, selectedRow] {
            selectNewContest(contestByRowOrder[selectedRow]);
        });
}

void WIN_ContestsSettings::selectNewContest(std::string contestName) {
    std::cout << "[selectNewContest()] Locating contest index...\n";
    int i = 0;
    for (i; i < contestByRowOrder.size(); i++) {
        if (contestByRowOrder[i] == contestName) {
            break;
        }
    }

    std::cout << "[selectNewContest()] Setting row...\n";
    listView->setCurrentRow(i);
    contestNameLabel->setText(QString::fromStdString(contestByRowOrder[i]));
    saveBtn->setEnabled(true);
    currentCnts = contestByRowOrder[i];
    
    // Call toCnts() AFTER the UI is fully set up
    std::cout << "[selectNewContest()] AWAITING signal with QTimer for refreshing with toCnts()\n";
    QTimer::singleShot(0, this, [this]() {
        toCnts(currentCnts);
    });
}

void WIN_ContestsSettings::newContest(bool forced) {
    // Do the set of actions
    std::cout << "[newContest()] Called up!\n"; // Logging

    // We need an input box, you know, that kinda thing. Weird shit
    CST_PlainTextDialog dialog(this, "Nhập tên bài thi", "Hãy nhập tên bài thi mới");
    if (forced) dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowCloseButtonHint);

    if (dialog.exec() == QDialog::Accepted && dialog.getResult() != "") {
        std::cout << "DIALOG Accepted\n";
        std::string newContestName = dialog.getResult().toStdString();
        std::cout << newContestName << '\n';

        // Let's do a quick one!
        // We add a sorta template-y json entry for the new contest
        // and the rest will be supplemented by the existing functionalities
        contests[newContestName] = json::parse(R"(
            {
                "Classes": [],
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

        saveContestsInfo(contests);
        fetchContests(false);
        selectNewContest(newContestName);
    } else if (forced) {
        close();
    }
}

void WIN_ContestsSettings::onTestCaseCheckBoxToggled(std::string which) {
    if (which == "in" || which == "both") {
        bool checked = (fileInputCheck->checkState() == Qt::CheckState::Checked);
        std::cout << "[INPUT] Current state: " << (checked ? "FILE" : "RAW") << '\n';

        // If this is not checked, it means we are not outputing to a file. AND SO WE WILL
        // DISABLE THE input field of the input file name and also default it to NAME + INP

        if (!checked) {
            // Disable
            fileInputName->setEnabled(false);
            
            // Now let's extract the contest's name and turn it uppercase
            fileInputName->setText(QString::fromStdString(turnStringUppercaseA(currentCnts) + ".INP"));

        } else fileInputName->setEnabled(true);
    } 
    
    if (which == "out" || which == "both") {
        bool checked = (fileOutputCheck->checkState() == Qt::CheckState::Checked);
        std::cout << "[OUTPUT] Current state: " << (checked ? "FILE" : "RAW") << '\n';

        if (!checked) {
            fileOutputName->setEnabled(false);

            fileOutputName->setText(QString::fromStdString(turnStringUppercaseA(currentCnts) + ".OUT"));

        } else fileOutputName->setEnabled(true);
    }
    // Not any? IGNORE
}

// --------------------------
// Purpose: Look at the name
// --------------------------
void WIN_ContestsSettings::reloadContestsVar() {
    // Refresh. As they say
    contests = getContestsInfo();
}

// ---------------------------------------------------------------
// Purpose: A way to fetch new contests when I just opened it up.
// ---------------------------------------------------------------
void WIN_ContestsSettings::fetchContests(bool selectFirstEntry) {
    reloadContestsVar();

    std::cout << "Contest amounts: " << contests.size();

    if (contests.size() == 0)
        QTimer::singleShot(0, this, [this] {
            newContest(true);
        });

    contestByRowOrder.clear();
    contestByRowOrder.reserve(contests.size()); // Reserve memory alloc
    listView->clear();

    if (!contests.is_object() || contests.empty()) {
        // That is not safe. Also I/O is stuck.
        // std::cout << "[*] INVALID CONTEST JSON. ABORT IMMEDIATELY. NO SEGMENTATION FAULT ALLOWED\n";
        // _sleep(100); // 100 miliseconds and DARN IT DOESN'T WORK. WHY THO

        // If this happens, we will be so limited on stuffs that it will be like this:
        // No IO (stuck), no function calling (crash) and the other idk.
        return; // or handle the empty case appropriately
    }

    // After we've got the required data to secure the ticket to showing whats needed
    // A.K.A showing the contests in a LIST VIEW. Which is pretty much obsolete at this
    // point but I dont have any other alternative that is much easier to use.
    int row = 0;
    for (const auto& item : contests.items()) {
        // std::cout << "[*ContestsSettings] Now processing: " << item.key() << '\n';
        contestByRowOrder.push_back(item.key());
        const QString contestName = QString::fromStdString(item.key());

        try {
            std::cout << "Processing " << item.key() << '\n';
            QListWidgetItem *listItem = new QListWidgetItem(listView);

            // Actually. Even though it will be kinda a memory hog, I still wants to make it looks BEAUTIFUL
            // So. What's the looks?
            // +-----------------------+
            // | BIG LABEL FOR CONTEST |
            // | smaller for classes   |
            // +-----------------------+
            // This will be achievable with a QWidget
            QWidget *listItemWidget = new QWidget(this);
            QHBoxLayout *listItemWidgetLayout = new QHBoxLayout(listItemWidget); // Actually, I don't like the names to be too
                                                                                // fucking long. But I have no other choices

            listItemWidget->setLayout(listItemWidgetLayout); // Setting the layout. Now we just need to make all these work by
                                                            // Adding QLabels

            // std::cout << item.key() << ": " << item.value() << '\n';

            // QWidget for labels part
            QWidget *labelArea = new QWidget(listItemWidget);
            QVBoxLayout *labelAreaLayout = new QVBoxLayout(labelArea);
            labelArea->setLayout(labelAreaLayout);
            labelArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            labelAreaLayout->setContentsMargins(0, 0, 0, 0);

            // Labels
            QLabel *contestHeader = new QLabel(listItemWidget);
            contestHeader->setStyleSheet(STYLE_BIGLABEL);
            contestHeader->setText(contestName);

            QLabel *contestInf = new QLabel(listItemWidget); // The smaller information label
            
            // Setting text
            QString contestDescription = QString::fromStdString(contests[item.key()]["Desc"]);
            if (contestDescription == "") contestDescription = "<Không có mô tả>";
            contestInf->setText(contestDescription);
            contestInf->setTextFormat(Qt::RichText);
            contestInf->setWordWrap(false); // No automatic wrapping
            contestInf->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed); // No expanding.
            contestInf->setFixedHeight(contestInf->fontMetrics().height() + 4); // Housing ONE LINE only

            // Adding the newly made headers and inf elements
            labelAreaLayout->addWidget(contestHeader);
            labelAreaLayout->addWidget(contestInf);

            listItemWidgetLayout->addWidget(labelArea);

            // Removal button
            QPushButton *delBtn = new QPushButton(listItemWidget);
            delBtn->setObjectName("genericBtn");
            QPixmap delBtnPixmap(DELETEICON_PATH);
            QIcon delBtnIcon(delBtnPixmap);
            delBtn->setIcon(delBtnIcon);
            delBtn->setFixedHeight(30);
            delBtn->setFixedWidth(30);
            delBtn->setToolTip("Xoá bài thi này");

            connect(delBtn, &QPushButton::clicked, this, [listItem, this] {
                remContest(listItem); // FUCK THIS IS THE ONLY WAYYYY???? UGHHH
            });
            
            // listItemWidgetLayout->addStretch(1);
            listItemWidgetLayout->addWidget(delBtn);
            listItemWidgetLayout->setAlignment(Qt::AlignCenter);
            
            // Now we apply the QWidget in
            listItem->setSizeHint(listItemWidget->sizeHint());
            listView->setItemWidget(listItem, listItemWidget);
            
            row++;

            if (contestByRowOrder.size() == 1 && selectFirstEntry) { // If this is a first
                // This is created so that it will execute only if there is at least one contest
                // available.
                // Else then nothing will show and it should be disabled (by default).
                listView->setCurrentItem(listItem);
                contestNameLabel->setText(contestName);
                saveBtn->setEnabled(true);
                currentCnts = contestName.toStdString();
                
                // Don't call toCnts() here, just set the selection
                if (selectFirstEntry && !contestByRowOrder.empty()) {
                    selectNewContest(contestName.toStdString());
                }
                std::cout << "Performed special move on contestByRowOrder. Guess what it is :)\n";
            }   
        } catch (const std::bad_alloc& e) {
            errorDialog("Không đủ bộ nhớ để tạo giao diện");
            close();
        }
    }

    // if (!selectFirstEntry) {
    //     listView->setCurrentRow(contestByRowOrder.size() - 1);
    //     contestNameLabel->setText(QString::fromStdString(contestByRowOrder[contestByRowOrder.size() - 1]));
    //     saveBtn->setEnabled(true);
    //     currentCnts = contestByRowOrder[contestByRowOrder.size() - 1];
    // }

    // Styling to make the list view works like a navigation pane
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Prevent editing
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

    // -----------------------------------------------
    // Remove the tests that are marked to be removed.
    // -----------------------------------------------
    // First, we need to sort the vector again since its very highly possible is
    // not organized from Big to Small. The reason?
    // -> When removing the smaller indexes, the bigger indexes will be shifted, causing some bad behavior.
    // So we need to sort first (cuz the indexes are indexes of a stationary vector, not an ever changing one)
    std::sort(indexesToBeRemoved.begin(), indexesToBeRemoved.end(), [](const int& a, const int& b) { // Use lambda because this is easy
        return a > b; // Descending order.
    });

    // Now we go through each and everything in the indexes
    for (const int& i : indexesToBeRemoved) {
        // Remove the test cases with that same indexes
        contests[currentCnts]["Tests"].erase(i);
    }

    // ---------------------------
    // Applying edited test cases
    // ---------------------------
    // This is done because of the edit functionality, which saved its edit history into a vector variable.
    // We have to go through each element in that vector and apply the data that is correct
    for (const auto& iv : indexesToBeModified) { // iv means index value
        // The newly created pair will have this layout:
        // {index, {input, output}}
        // We now just needs to save it all in

        // Meaning: Set Test input data at the specified index to specified input data
        contests[currentCnts]["Tests"][iv.first][0] = iv.second.first;

        // Also similar
        contests[currentCnts]["Tests"][iv.first][1] = iv.second.second;
    }

    // -------------------
    // Adding added cases
    // -------------------
    // This is done for the add case functionality in here.
    // It's very ass and does not have the capability of removing cases, but I mean, its fine, right?
    for (const auto& v : indexesToBeAdded) {
        // Adding the values to a predetermined variable
        // std::cout << v.first << ", " << v.second << '\n';
        std::vector<std::string> value = {v.first, v.second};

        // Adding it in
        contests[currentCnts]["Tests"].push_back(value);
    }
    
    // Also, because we are in fact, modifying the tests amount too, so we also need to update that.
    contests[currentCnts]["TestAmount"] = contests[currentCnts]["Tests"].size();

    // ---------------------------------------
    // Setting file/raw I/O status and values
    // ---------------------------------------
    contests[currentCnts]["InputFile"] = fileInputName->text().toStdString();
    contests[currentCnts]["OutputFile"] = fileOutputName->text().toStdString();

    contests[currentCnts]["InputType"] = ((fileInputCheck->checkState() == Qt::CheckState::Checked) ? "file" : "raw");
    contests[currentCnts]["OutputType"] = ((fileOutputCheck->checkState() == Qt::CheckState::Checked) ? "file" : "raw");

    // --------------------------
    // Setting time limit values
    // --------------------------
    contests[currentCnts]["TimeLimit"] = stringToDouble(timeLimit->text().toStdString());

    // Now some I/O trickery because to be honest, Idk how it works either
    bool successfullyOpenFile = saveContestsInfo(contests);

    if (successfullyOpenFile) {
        toCnts(currentCnts);
    }
}

void WIN_ContestsSettings::addCase() {
    // Let's do this
    std::cout << "WIN_ContestsSettings::addCase() called\n";

    CST_TestCaseDialog dialog(this, 
        "", ""
    );

    if (dialog.exec() == QDialog::Accepted) {
        QStringList result = dialog.getResult();

        QString inputValue = result[0];
        QString outputValue = result[1];
        
        // In the case the user entered both sides blank. We need to ask for confirmation
        if (inputValue == "" && outputValue == "") {
            QMessageBox::StandardButton askResult = QMessageBox::warning(this, "Nhập trống hoàn toàn?", "Bạn có chắc muốn để trống cả đầu vào và đầu ra không?", QMessageBox::Yes | QMessageBox::No);

            if (askResult == QMessageBox::No) return;
            else {
                std::cout << "User entered blank for both sides. Very weird\n";
            }
        }

        indexesToBeAdded.push_back({inputValue.toStdString(), outputValue.toStdString()}); // Add in the queue
        
        // ALSO, before the user actually decided to save or clear the whole thing, we need to add a
        // dummy entry (index) in the list so that the user can modify them. Very easy...? Just not
        // removing. Cuz working on that makes my head explodes
        int currentIndex = indexesToBeAdded.size() - 1; // Since we just added an entry to the list. The index
                                                        // should be the final one, which can be found using
                                                        // the good ol' size - 1 mathing*
        
        // Now, let's actually try to justify our targets of adding an entry that could be edited, but NOT removed
        // since working on these is really fucking ASS
        int orderIndex = currentIndex + contests[currentCnts]["Tests"].size();
        
        // Initialize item (COPY PASTED. VERY BAD BUT THIS IS THE ONLY WAY)
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(intToString(orderIndex)), testCasesList); // The cases is the parent since this will be inside

        QWidget *itemWidget = new QWidget(testCasesList);
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);

        QLabel *testLabel = new QLabel(itemWidget);
        testLabel->setText(QString::fromStdString("Test " + intToString(orderIndex)));
        itemLayout->addWidget(testLabel, 1); // Stretch all the way to the moon (100% of space)

        QWidget *buttonsArea = new QWidget(itemWidget);
        QHBoxLayout *btnAreaLayout = new QHBoxLayout(buttonsArea);
        itemLayout->addWidget(buttonsArea);

        // Button for editting
        QPushButton *editBtn = new QPushButton(buttonsArea);
        editBtn->setObjectName("genericBtn");
        QPixmap editBtnPixmap(EDITICON_PATH);
        QIcon editBtnIcon(editBtnPixmap);
        editBtn->setIcon(editBtnIcon);
        editBtn->setFixedSize(30, 30);
        editBtn->setToolTip("Chỉnh sửa test này");

        connect(editBtn, &QPushButton::clicked, this, [this, currentIndex] {
            // Best to construct a lambda. I'm sorry. But this is the only way.
            // Creating a separate function will be too costly for my time cus
            // this is just a passion project I made for fun
            bool indexAlreadyModified = false;
            std::pair<std::string, std::string> modifiedValue;

            // Yeah now let's make sure the data is read dynamically. We have the indexes required now too
            modifiedValue = indexesToBeAdded[currentIndex];

            CST_TestCaseDialog dialog(this, 
                QString::fromStdString(modifiedValue.first), 
                QString::fromStdString(modifiedValue.second)
            );

            if (dialog.exec() == QDialog::Accepted) {
                // If the user actually finished the damn thing normally
                // Get the result of their choices
                QStringList result = dialog.getResult();

                QString inputValue = result[0];
                QString outputValue = result[1];

                std::cout << "[CST_TestCaseDialog] Returned result\n";
                std::cout << "INPUT:\n" << inputValue.toStdString() << "\n\nOUTPUT:\n" << outputValue.toStdString() << '\n';

                // Modifying these entries? Ofc its fine! We just need to save it into indexesToBeAdded
                indexesToBeAdded[currentIndex] = {inputValue.toStdString(), outputValue.toStdString()};
            }
        });

        // Adding the buttons in a specific order (left -> right)
        btnAreaLayout->addWidget(editBtn);
        
        // Applying the layout
        btnAreaLayout->setAlignment(Qt::AlignRight);
        btnAreaLayout->setContentsMargins(0, 0, 0, 0);
        buttonsArea->setLayout(btnAreaLayout);

        itemLayout->setAlignment(Qt::AlignLeft);
        itemWidget->setLayout(itemLayout);

        // Doing the normal shyt of adding item into the list
        item->setSizeHint(itemWidget->sizeHint());
        testCasesList->addItem(item);
        testCasesList->setItemWidget(item, itemWidget);
    }
}

// --------------------------------------------------------
// Purpose: To refresh currently clicked on contest with a
//          compact function
// --------------------------------------------------------
void WIN_ContestsSettings::toCnts(std::string contestName) {
    reloadContestsVar();
    indexesToBeRemoved.clear(); // Not a pointer, so its gonna be fine
    indexesToBeRemoved.reserve(contests[contestName]["Tests"].size());
    indexesToBeModified.clear();
    indexesToBeModified.reserve(contests[contestName]["Tests"].size());
    indexesToBeAdded.clear();
    indexesToBeAdded.reserve(contests[contestName]["Tests"].size());
    currentCnts = contestName;

    if (!contests.is_object() || contests.empty()) { // Save lives of thousands
        // That is not safe. Also I/O is stuck.
        // std::cout << "[*] INVALID CONTEST JSON. ABORT IMMEDIATELY. NO SEGMENTATION FAULT ALLOWED\n";
        // _sleep(100); // 100 miliseconds and DARN IT DOESN'T WORK. WHY THO

        // If this happens, we will be so limited on stuffs that it will be like this:
        // No IO (stuck), no function calling (crash) and the other idk.
        return; // or handle the empty case appropriately
    }

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

    // Get the test cases
    std::vector<std::vector<std::string>> testCases; // Kinda a 2 dimensional list, you know
    // Broad preview:
    // [ ] [0] [1]   |  0: Input    1: Output
    // [0] "I" "O"   |  Test 1
    // [1] "A" "E"   |  Test 2
    // [2] "X" "Y"   |  Test 3
    // Fetch it straight from the JSON
    testCases = contests[contestName]["Tests"];

    // Now, apply it to the list of test cases
    // Howver, we have to clear the list before anything bad happens
    testCasesList->clear();

    int index = 0; // The index of the test cases
                   // Responsible for numbering the tests
    for (const std::vector<std::string>& testCase : testCases) {
        // REMINDER [!] Index rules
        // - 0: Input
        // - 1: Output

        // Initialize item
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(intToString(index)), testCasesList); // The cases is the parent since this will be inside

        // The QWidget storing a smaller QWidget and a label
        QWidget *itemWidget = new QWidget(testCasesList);
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);

        // The QLabel of the test 💔
        QLabel *testLabel = new QLabel(itemWidget);
        testLabel->setText(QString::fromStdString("Test " + intToString(index)));
        itemLayout->addWidget(testLabel, 1); // Stretch all the way to the moon (100% of space)

        // The QWidget housing the buttons 🍷
        QWidget *buttonsArea = new QWidget(itemWidget);
        QHBoxLayout *btnAreaLayout = new QHBoxLayout(buttonsArea);
        itemLayout->addWidget(buttonsArea);

        // The buttons
        QPushButton *delBtn = new QPushButton(buttonsArea);
        delBtn->setObjectName("genericBtn");
        QPixmap delBtnPixmap(DELETEICON_PATH);
        QIcon delBtnIcon(delBtnPixmap);
        delBtn->setIcon(delBtnIcon);
        delBtn->setFixedHeight(30);
        delBtn->setFixedWidth(30);
        delBtn->setToolTip("Xoá test này");
        
        connect(delBtn, &QPushButton::clicked, this, [index, item, this] {
            // The indexes start from 0, both for the JSON and the index variable
            std::cout << "DELETE TestIndex " << '\n';
            
            int row = testCasesList->row(item);
            
            // Safely take control the item and itemWidget's memory areas
            QWidget *widgetToDelete = testCasesList->itemWidget(item);
            testCasesList->removeItemWidget(item);
            QListWidgetItem *removedItem = testCasesList->takeItem(row);
            
            // Safely removes the item
            widgetToDelete->deleteLater();
            delete removedItem;
            
            // Mark index as to be removed
            indexesToBeRemoved.push_back(index);
        });
        
        // Button for editting
        QPushButton *editBtn = new QPushButton(buttonsArea);
        editBtn->setObjectName("genericBtn");
        QPixmap editBtnPixmap(EDITICON_PATH);
        QIcon editBtnIcon(editBtnPixmap);
        editBtn->setIcon(editBtnIcon);
        editBtn->setFixedSize(30, 30);
        editBtn->setToolTip("Chỉnh sửa test này");

        connect(editBtn, &QPushButton::clicked, this, [this, contestName, index] {
            // Best to construct a lambda. I'm sorry. But this is the only way.
            // Creating a separate function will be too costly for my time cus
            // this is just a passion project I made for fun
            bool indexAlreadyModified = false;
            std::pair<std::string, std::string> modifiedValue;

            for (const auto& indexModifiedValue : indexesToBeModified) {
                // Will only happen if the list still remains => Only if the user
                // haven't changed tab or clicked on save
                if (indexModifiedValue.first == index) {
                    indexAlreadyModified = true;
                    modifiedValue = indexModifiedValue.second; // Apply the data
                    break;
                }
            }

            if (!indexAlreadyModified) {
                modifiedValue = {contests[contestName]["Tests"][index][0], contests[contestName]["Tests"][index][1]};
            }

            CST_TestCaseDialog dialog(this, 
                QString::fromStdString(modifiedValue.first), 
                QString::fromStdString(modifiedValue.second)
            );

            if (dialog.exec() == QDialog::Accepted) {
                // If the user actually finished the damn thing normally
                // Get the result of their choices
                QStringList result = dialog.getResult();

                QString inputValue = result[0];
                QString outputValue = result[1];

                std::cout << "[CST_TestCaseDialog] Returned result\n";
                std::cout << "INPUT:\n" << inputValue.toStdString() << "\n\nOUTPUT:\n" << outputValue.toStdString() << '\n';

                // Save into a variable(?) for pending saves (WILL AUTOMATICALLY BE CLEARED THE MOMENT THE ENTIRE THING IS REFRESHED)
                // indexesToBeModified.first = index;
                // indexesToBeModified.second.first = inputValue.toStdString();
                // indexesToBeModified.second.second = outputValue.toStdString();
                if (indexAlreadyModified) {
                    for (int i = 0; i < indexesToBeModified.size(); i++) {
                        // We should change the values directly
                        if (indexesToBeModified[i].first == index) {
                            // Good, found it
                            // Now we just have to apply the values
                            indexesToBeModified[i].second = {inputValue.toStdString(), outputValue.toStdString()};
                            break;
                        }
                    }
                } else {
                    std::pair<int, std::pair<std::string, std::string>> indexValue = {index, {inputValue.toStdString(), outputValue.toStdString()}};
                    indexesToBeModified.push_back(indexValue);
                }
            }
        });

        // Adding the buttons in a specific order (left -> right)
        btnAreaLayout->addWidget(editBtn);
        btnAreaLayout->addWidget(delBtn);
        
        // Applying the layout
        btnAreaLayout->setAlignment(Qt::AlignRight);
        btnAreaLayout->setContentsMargins(0, 0, 0, 0);
        buttonsArea->setLayout(btnAreaLayout);

        itemLayout->setAlignment(Qt::AlignLeft);
        itemWidget->setLayout(itemLayout);

        // Doing the normal shyt of adding item into the list
        item->setSizeHint(itemWidget->sizeHint());
        testCasesList->addItem(item);
        testCasesList->setItemWidget(item, itemWidget);

        // Yeah please dont forget this
        // ever again.
        index++;
    }

    // Also, apply the value in the contests json file too.
    // In case the checkbox was in fact, checked means that it will always be fine
    // and won't become obsolete
    fileInputName->setText(QString::fromStdString(contests[contestName]["InputFile"]));
    fileOutputName->setText(QString::fromStdString(contests[contestName]["OutputFile"]));

    bool inputCheck = (contests[contestName]["InputType"] == "file");
    bool outputCheck = (contests[contestName]["OutputType"] == "file");

    fileInputCheck->setCheckState(inputCheck ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    fileOutputCheck->setCheckState(outputCheck ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

    // Let's refresh file/raw I/O of test
    onTestCaseCheckBoxToggled("both");

    // Now, let's refresh time limit of these tests
    timeLimit->setText(QString::fromStdString(doubleToString(contests[currentCnts]["TimeLimit"])));
    
    std::cout << "[*ContestsSettings] Refreshed Information (details) panel.\n";
}

// ---------------------------------------------------------------------------
// Purpose: Showing errors faster than having to type an actual long command.
//          Can't believe I have to make a duplicate
// ---------------------------------------------------------------------------
void WIN_ContestsSettings::errorDialog(std::string error) {
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

void WIN_ContestsSettings::closeEvent(QCloseEvent *event) {
    // These work, I guess since I don't really understand how these work.
    // Maybe some time later. Still, these are fucking magic.
    // The whole concept of using silicon to answer mathematical questions
    // is already absurd enough. And now we have this.
    // Nice.
    emit closed();
    contests.clear();
    contestByRowOrder.clear();
    indexesToBeRemoved.clear();
    indexesToBeModified.clear();
    indexesToBeAdded.clear();
    event->accept();
    delete generatingTestCases;
    this->deleteLater(); // Cleaning.
}
    