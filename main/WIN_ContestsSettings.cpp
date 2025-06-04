#include "WIN_ContestsSettings.h" // The main shyt
#include "CST_TestCaseDialog.h"   // For the dialog of the test cases
#include "consts.h"               // Constants
#include "utilities.h"            // Utilities (custom conversion functions)

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
    
    // List of test cases
    testCasesList->setMinimumHeight(150);

    // Adding test cases functionality
    QPushButton *addTestCaseBtn = new QPushButton(this);
    addTestCaseBtn->setObjectName("genericBtn");
    addTestCaseBtn->setFixedWidth(110);
    addTestCaseBtn->setText("Thêm bộ test");
    connect(addTestCaseBtn, &QPushButton::clicked, this, WIN_ContestsSettings::addCase);

    // Adding in the widgets in order (for the looks actually.).
    contestDetailsLayout->addWidget(descLabel);
    contestDetailsLayout->addWidget(descEdit);
    contestDetailsLayout->addWidget(classLabel);
    contestDetailsLayout->addWidget(classList);
    contestDetailsLayout->addWidget(cnTestLabel);
    contestDetailsLayout->addWidget(testCasesList);
    contestDetailsLayout->addWidget(addTestCaseBtn);
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

    // Now some I/O trickery because to be honest, Idk how it works either
    bool successfullyOpenFile = false;
    std::fstream contestsFile(dirPath + CONTESTS_PATH, std::ios::out);
    if (contestsFile.is_open()) {
        contestsFile << contests;

        // Tbh I am just gonna refresh the entire thing to make sure its safe and
        // up to date. WELP. IT DOESN'T WORK THAT WAY. Let's set a variable after we closed this fully.
        // toCnts(currentCnts); // SOUNDS GOOD
        successfullyOpenFile = true;
    } else {
        errorDialog("Mở tệp bài làm không thành công. Đã có lỗi xảy ra.");
    }
    contestsFile.close();

    if (successfullyOpenFile) {
        toCnts(currentCnts);
    }
}

void WIN_ContestsSettings::addCase() {
    // Let's do this
    std::cout << "WIN_ContestsSettings::addCase() called\n";

    CST_TestCaseDialog *dialog = new CST_TestCaseDialog(this, 
        "", ""
    );

    if (dialog->exec() == QDialog::Accepted) {
        QStringList result = dialog->getResult();

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

            CST_TestCaseDialog *dialog = new CST_TestCaseDialog(this, 
                QString::fromStdString(modifiedValue.first), 
                QString::fromStdString(modifiedValue.second)
            );

            if (dialog->exec() == QDialog::Accepted) {
                // If the user actually finished the damn thing normally
                // Get the result of their choices
                QStringList result = dialog->getResult();

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
    indexesToBeModified.clear();
    indexesToBeAdded.clear();
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

        // The QWidget storing a smaller QWidget and a label 🔥
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

        // The buttons 🍶
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
            delete widgetToDelete;
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

            CST_TestCaseDialog *dialog = new CST_TestCaseDialog(this, 
                QString::fromStdString(modifiedValue.first), 
                QString::fromStdString(modifiedValue.second)
            );

            if (dialog->exec() == QDialog::Accepted) {
                // If the user actually finished the damn thing normally
                // Get the result of their choices
                QStringList result = dialog->getResult();

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
    // These work, I guess since I don't really understand how these work.
    // Maybe some time later. Still, these are fucking magic.
    // The whole concept of using silicon to answer mathematical questions
    // is already absurd enough. And now we have this.
    // Nice.
    emit closed();
    event->accept();
    this->deleteLater(); // Cleaning.
}