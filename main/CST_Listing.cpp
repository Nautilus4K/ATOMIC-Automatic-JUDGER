#include "CST_Listing.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QInputDialog>

CST_Listing::CST_Listing(QWidget *parent) : QWidget(parent) {
    setStyleSheet(parent->styleSheet());
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    layout->setContentsMargins(0, 0, 0, 0);

    listView->setMinimumHeight(100);
    layout->addWidget(listView);
    
    // The buttons need to be on the same plane
    addBtn->setText("+ Thêm");
    subBtn->setText("- Bớt");
    addBtn->setObjectName("genericBtn"); // Idk man, we need this so that the btns have
    subBtn->setObjectName("genericBtn"); // the correct looks

    // Connecting to the neccessary functions
    connect(addBtn, &QPushButton::clicked, this, CST_Listing::addEntry);
    connect(subBtn, &QPushButton::clicked, this, CST_Listing::removeEntry);

    // Creates a horizontally aligned QWidget
    QWidget *btnPlane = new QWidget(this);
    btnPlane->setStyleSheet(styleSheet());
    QHBoxLayout *btnPlaneLayout = new QHBoxLayout(btnPlane);

    btnPlaneLayout->setAlignment(Qt::AlignLeft);
    btnPlaneLayout->setContentsMargins(0, 0, 0, 0);
    btnPlaneLayout->addWidget(addBtn);
    btnPlaneLayout->addWidget(subBtn);

    btnPlane->setLayout(btnPlaneLayout);

    layout->addWidget(btnPlane);
}

// Setting entries of list
void CST_Listing::setEntries(QStringList entList) {
    model->setStringList(entList);
    listView->setModel(model);
}

// Getting entries of list
QStringList CST_Listing::getEntries() const {
    return model->stringList();
}

void CST_Listing::setReadOnly(bool b) {
    listView->setEditTriggers(b ? QAbstractItemView::NoEditTriggers : QAbstractItemView::AllEditTriggers);
}

void CST_Listing::addEntry() {
    bool ok; // This is NOT a pointer
    // Using a pointer pointing to the ok variable (which is not a pointer) to get the value.
    // Very fun
    QString name = QInputDialog::getText(this, "Nhập tên của mục mới", "Hãy nhập tên cho mục mới bạn cần thêm:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) { // User clicked OK and did not leave name blank
        QStringList curEnt = getEntries(); // Get current entries so that I can add more

        // Validifying
        bool unique = true;
        for (const QString entName : curEnt) {
            if (entName == name)  {
                // So what this does is it browse through the LIST
                // If it found just a single matching entry, it would HALT immediately
                // and return that the one we just typed in is NOT unique.
                // If it didn't and ran through the entire QStringList, then we accepts this as unique
                unique = false;
                break;
            }
        }

        if (unique) {
            curEnt << name;
            setEntries(curEnt); // Set the new entries we just created as the current entries
        } else {
            QMessageBox::warning(this, "Chưa thêm mục", "Mục không được thêm vào vì có sự trùng lặp", QMessageBox::Ok);
            addEntry(); // Recursiveness is something I like? Idk man, its just fascinating asf.
        }
    }
}

void CST_Listing::removeEntry() {
    // To remove an entry, we need to index where is that entry we are selecting...
    QModelIndexList indexes = listView->selectionModel()->selectedRows(0);

    QStringList curEnts = getEntries();

    for (const auto& index : indexes) {
        curEnts.removeAt(index.row());
    }

    // Update string model
    setEntries(curEnts);
}