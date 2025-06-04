/*
 * Implementation of CST_TestCaseDialog
 * Compiled with a special script so that it will work in tandem
 * with Qt 6.5.3
 */

#include "CST_TestCaseDialog.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

CST_TestCaseDialog::CST_TestCaseDialog(QWidget *parent, QString currentInput, QString currentOutput) : QDialog(parent) {
    // Setting up the window prefs (REQUIRED, BECAUSE WHY NOT)
    setObjectName("dialog");
    setStyleSheet(parent->styleSheet());
    setWindowIcon(parent->windowIcon());
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning

    // Setting up some certainly required variables.
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    // An area for two inputs (I/O)
    // This will be horizontally placed (side-by-side) for maximum easiness (to read)
    QWidget *dataArea = new QWidget(this);
    QHBoxLayout *dataLayout = new QHBoxLayout(dataArea);
    dataLayout->setContentsMargins(0, 0, 0, 0);
    dataArea->setLayout(dataLayout);

    layout->addWidget(dataArea);

    // Also, each input area will also be QWidgets since they will have QLabels and QTextEdit vertically aligned
    // INPUT AREA
    QWidget *inputArea = new QWidget(dataArea);
    dataLayout->addWidget(inputArea);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputArea);
    inputLayout->setContentsMargins(0, 0, 0, 0);
    inputArea->setLayout(inputLayout);

    QLabel *inputLabel = new QLabel(inputArea);
    inputLabel->setText("Đầu vào:");
    inputLayout->addWidget(inputLabel);

    inputEditField->setText(currentInput);
    inputEditField->setMinimumSize(240, 150);
    inputEditField->setAcceptDrops(false);
    inputEditField->setAcceptRichText(false);
    inputLayout->addWidget(inputEditField);

    // OUTPUT AREA
    QWidget *outputArea = new QWidget(dataArea);
    dataLayout->addWidget(outputArea);
    QVBoxLayout *outputLayout = new QVBoxLayout(outputArea);
    outputLayout->setContentsMargins(0, 0, 0, 0);
    outputArea->setLayout(outputLayout);

    QLabel *outputLabel = new QLabel(outputArea);
    outputLabel->setText("Đầu ra:");
    outputLayout->addWidget(outputLabel);

    outputEditField->setText(currentOutput);
    outputEditField->setMinimumSize(240, 150);
    outputEditField->setAcceptDrops(false);
    outputEditField->setAcceptRichText(false);
    outputLayout->addWidget(outputEditField);

    // Confirmation button
    QPushButton *confirmBtn = new QPushButton(this);
    confirmBtn->setText("OK");
    layout->addWidget(confirmBtn);

    connect(confirmBtn, &QPushButton::clicked, this, &CST_TestCaseDialog::accept);
}

QStringList CST_TestCaseDialog::getResult() const {
    QStringList result;
    result << inputEditField->toPlainText() << outputEditField->toPlainText();

    return result;
}