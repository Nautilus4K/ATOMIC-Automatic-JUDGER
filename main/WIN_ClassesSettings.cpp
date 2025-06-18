/* WIN_ClassesSettings.cpp */
/* ------------------------------------
 * Implementation of WIN_ClassesSettings
 * ------------------------------------ */


#include "WIN_ClassesSettings.h"  // Main declaration
#include "CST_PlainTextDialog.h"  // Dialog
#include "utilities.h"            // Handmade utils. Isn't that cute :3

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>

#include <QtGui/QCloseEvent>
#include <QtGui/QAction>

#include <nlohmann/json.hpp>

using nlohmann::json;

// STRUCTOR
WIN_ClassesSettings::WIN_ClassesSettings(QWidget *parent) : QWidget(parent, Qt::Window) {
    setObjectName("container");
    setWindowIcon(parent->windowIcon());
    setStyleSheet(parent->styleSheet());
    setWindowTitle("Cài đặt lớp học");
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning
    
    setMinimumHeight(400);
    setMinimumWidth(600);
    resize(700, 450);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    table->setColumnCount(4);
    table->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    
    // Connect custom context menu
    connect(table, &QTableWidget::customContextMenuRequested, this, &WIN_ClassesSettings::showContextMenu);

    layout->addWidget(table);

    // SETTING THE HEADERS FOR THE TABLE
    // QTableWidgetItem *nameCol = new QTableWidgetItem("Tên (Mã định danh)");
    // QTableWidgetItem *shortnameCol = new QTableWidgetItem("Tên ngắn");
    // QTableWidgetItem *longnameCol = new QTableWidgetItem("Tên dài");
    // QTableWidgetItem *scoreboardCol = new QTableWidgetItem("Hiện bảng điểm?");

    // table->setHorizontalHeaderItem(0, nameCol);
    // table->setHorizontalHeaderItem(1, shortnameCol);
    // table->setHorizontalHeaderItem(2, longnameCol);
    // table->setHorizontalHeaderItem(3, scoreboardCol);

    // ADDING NEW CLASS with a button on the bottom left
    QWidget *buttonArea = new QWidget(this);
    layout->addWidget(buttonArea);
    QHBoxLayout *buttonAreaLayout = new QHBoxLayout(buttonArea);
    buttonArea->setLayout(buttonAreaLayout);
    buttonAreaLayout->setContentsMargins(0, 0, 0, 0);

    // buttonAreaLayout->addStretch(1);
    // This is a note label
    QLabel *noteLabel = new QLabel(buttonArea);
    noteLabel->setStyleSheet(STYLE_SMALLALEL);
    noteLabel->setText("* Bấm chuột phải hoặc chỉnh sửa các ô trong bảng để xem các cài đặt cho từng lớp học và thông tin trong ô.");
    noteLabel->setWordWrap(true);
    noteLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    buttonAreaLayout->addWidget(noteLabel);

    QPushButton *newClassBtn = new QPushButton(buttonArea);
    newClassBtn->setObjectName("genericBtn");
    newClassBtn->setText("+ Thêm lớp học");
    newClassBtn->setFixedWidth(150);

    connect(newClassBtn, &QPushButton::clicked, this, &WIN_ClassesSettings::newClass);

    buttonAreaLayout->addWidget(newClassBtn);

    // INITLIALIZE THE THING
    init();

    // CONNECT AFTER INITALIZATION
    connect(table, &QTableWidget::itemChanged, this, [this](QTableWidgetItem *item) {
        updateInfo(table->row(item), table->column(item));
    });
}

void WIN_ClassesSettings::showContextMenu(const QPoint& pos) {
    // This will show the custom context menu for the tables.
    // Ofc it will wore anywhere, as long as its in a cell. It should be capable of showing
    // 2 options:
    // - Refresh
    // - Remove
    // That will be alr

    // Make sure the request is valid and fetch the cell in which the menu is called upon
    QTableWidgetItem *item = table->itemAt(pos);
    if (!item) return;

    // Reconstruct the menu with STACK ALLOCATION
    QMenu menu(table);

    // Actions preparation
    QAction *reloadAction = menu.addAction("Làm mới");
    QAction *removeAction = menu.addAction("Xoá");

    // ADDING ICONS WAS A BAD IDEA
    // QPixmap removePXMP(DELETEICON_PATH);
    // QIcon removeIcon(removePXMP);

    // removeAction->setIcon(removeIcon);

    // Execution (showing the menu) & get the action clicked (in ptr)
    QAction *selectedAction = menu.exec(table->viewport()->mapToGlobal(pos));
    
    // Now lets branch tf out
    if (selectedAction == reloadAction) {
        loadClasses();
    } else if (selectedAction == removeAction) { // Removes the current selected class
        // Get the class's name, we're removing this shit
        std::string className = table->item(item->row(), 0)->text().toStdString();
        std::cout << "[TABLE] Removal: " << className << '\n';

        // Apply it into the JSON data
        classes.erase(className);

        // Save it into the file
        saveClassesInfo(classes);

        // Reload
        loadClasses();
    }
}

void WIN_ClassesSettings::newClass() {
    CST_PlainTextDialog dialog(this, "Lớp học mới", "Hãy nhập tên (mã định danh) lớp học mới (VD: 5C, CB1, ...)");

    if (dialog.exec() == QDialog::Accepted && !dialog.getResult().isEmpty()) {
        // If we got the data needed, we just need to redeclare now.
        // I mean, add new data into the JSON and save it, then reload the whole thing
        std::string name = dialog.getResult().toStdString();
        classes[name] = json::parse("{\"longname\":\"" + name + "\",\"scoreboard\":true,\"shortname\":\"" + name + "\"}");

        saveClassesInfo(classes); // Save to file
        loadClasses(); // Reload the whole thing
    }
}

void WIN_ClassesSettings::updateInfo(int row, int column) {
    std::cout << "[updateInfo()] ROW " << row << " | COLUMN " << column << '\n';

    std::string className = table->item(row, 0)->text().toStdString();

    if (column != 3) { // If this is a string cell
        std::string val = table->item(row, column)->text().toStdString();
        std::cout << "VAL / " << val << '\n';

        switch (column) {
            case 1: // This is SHORTNAME
                classes[className]["shortname"] = val;
                break;
            case 2: // This is LONGNAME
                classes[className]["longname"] = val;
                break;
        }

    } else { // If this is a boolean cell (checkbox)
        bool val = table->item(row, column)->checkState();
        std::cout << "VAL / " << (val ? "YES" : "NO") << '\n';

        classes[className]["scoreboard"] = val;
    }

    saveClassesInfo(classes);
}

void WIN_ClassesSettings::init() {
    loadClasses();
}

void WIN_ClassesSettings::loadClasses() {
    // Clearing old ASS items
    table->clear();

    // Re-adding the headers names
    QStringList headers;
    headers << "Tên (Mã định danh)" << "Tên ngắn" << "Tên dài" << "Hiện bảng điểm?";
    table->setHorizontalHeaderLabels(headers);

    table->setColumnWidth(0, 140);

    // Fetching new data
    classes = getClassesInfo();
    table->setRowCount(classes.size());

    int index = 0;
    for (const auto& item : classes.items()) {
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(item.key()));
        nameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(index, 0, nameItem);

        QTableWidgetItem *shortnameItem = new QTableWidgetItem(QString::fromStdString(item.value()["shortname"]));
        shortnameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
        table->setItem(index, 1, shortnameItem);

        QTableWidgetItem *longnameItem = new QTableWidgetItem(QString::fromStdString(item.value()["longname"]));
        longnameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
        table->setItem(index, 2, longnameItem);

        QTableWidgetItem *scoreboardItem = new QTableWidgetItem();
        scoreboardItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        scoreboardItem->setCheckState(item.value()["scoreboard"] ? Qt::Checked : Qt::Unchecked);
        table->setItem(index, 3, scoreboardItem);

        index++;
    }
}







// When closed?
void WIN_ClassesSettings::closeEvent(QCloseEvent *event) {
    emit closed();
    event->accept();
    this->deleteLater();
}