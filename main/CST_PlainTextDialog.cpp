#include "CST_PlainTextDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QtCore/QRegularExpression>
#include <QtGui/QRegularExpressionValidator>

CST_PlainTextDialog::CST_PlainTextDialog(QWidget *parent, QString title, QString label) : QDialog(parent) {
    setObjectName("dialog");
    setStyleSheet(parent->styleSheet());
    // setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle(title);
    setFixedSize(250, 120);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QLabel *labelW = new QLabel(this);
    labelW->setText(label);
    layout->addWidget(labelW);

    editor = new QLineEdit(this);
    QRegularExpression regex("[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]*");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, editor);
    editor->setValidator(validator);
    layout->addWidget(editor);

    layout->addStretch(1);

    QWidget *okBtnArea = new QWidget(this);
    QHBoxLayout *okBtnAreaLayout = new QHBoxLayout(okBtnArea);
    okBtnArea->setLayout(okBtnAreaLayout);

    okBtnAreaLayout->setContentsMargins(0, 0, 0, 0);
    okBtnAreaLayout->addStretch(1);

    QPushButton *okBtn = new QPushButton(this);
    okBtn->setObjectName("genericBtn");
    okBtn->setText("OK");
    okBtnAreaLayout->addWidget(okBtn);
    connect(okBtn, &QPushButton::clicked, this, &CST_PlainTextDialog::accept);

    layout->addWidget(okBtnArea);
}

QString CST_PlainTextDialog::getResult() const {
    return editor->text();
}