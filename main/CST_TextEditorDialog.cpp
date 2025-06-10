#include "CST_TextEditorDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <fstream>
#include <sstream>

CST_TextEditorDialog::CST_TextEditorDialog(QWidget *parent, QString title, std::string filePath, bool readOnly) {
    setObjectName("dialog");
    setWindowTitle(title);
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning
    if (parent) {
        setWindowIcon(parent->windowIcon());
        setStyleSheet(parent->styleSheet());
    }

    QVBoxLayout *layout = new QVBoxLayout();

    QTextEdit *lineage = new QTextEdit();
    lineage->setReadOnly(readOnly);

    // Opening the file (use a buffer to read that file's I/O)
    std::fstream file(filePath.c_str(), std::ios::in);
    if (file.is_open()) {
        std::stringstream fileData;
        fileData << file.rdbuf();

        lineage->setText(QString::fromStdString(fileData.str()));
    } else {
        lineage->setText("Mở tệp không thành công.");
    }

    // Adding the main elements for dialog. Which isn't really a dialog.
    layout->addWidget(mainText);
    layout->addWidget(lineage);
    setLayout(layout);
};

void CST_TextEditorDialog::setText(QString text) {
    mainText->setText(text);
};