#include "CST_RadioButtonDialog.h"

CST_RadioButtonDialog::CST_RadioButtonDialog(QWidget *parent, QString title, QString question, QStringList entries) : QDialog(parent) {
    setObjectName("dialog");
    setWindowTitle(title);
    // setAttribute(Qt::WA_DeleteOnClose); // Cleaning
    if (parent) {
        setStyleSheet(parent->styleSheet());
        setWindowIcon(parent->windowIcon());
    }

    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *questionLabel =  new QLabel();
    questionLabel->setText(question);
    layout->addWidget(questionLabel);

    QButtonGroup *bGroup = new QButtonGroup();
    for (const QString &entry : entries) { // Yeah this WILL be a QString
        // Mind if I create a new QRadioButton? Hell yeah!
        QRadioButton *selection = new QRadioButton();
        selection->setText(entry);

        // Adding into vector
        choices.push_back(selection);

        // Adding buttons
        bGroup->addButton(selection);
        layout->addWidget(selection);
    }

    QPushButton *okButton = new QPushButton("OK", this); // Okay button
    layout->addWidget(okButton);

    connect(okButton, &QPushButton::clicked, this, &CST_RadioButtonDialog::finishDialog);

    setLayout(layout);
}

QString CST_RadioButtonDialog::selectedOption() const {
    for (const QRadioButton *button : choices) {
        if (button->isChecked()) {
            return button->text();
        }
    }
    
    // In case NOTHING is selected
    return "";
}

void CST_RadioButtonDialog::finishDialog() {
    bool selectedAtLeast1Button = false;
    for (const QRadioButton *button : choices) {
        if (button->isChecked()) {
            selectedAtLeast1Button = true;
            break;
        }
    }

    if (selectedAtLeast1Button) {
        accept();
    } else {
        QMessageBox::information(this, "Không thể hoàn thành tác vụ", "Hãy chọn ít nhất 1 lựa chọn.", QMessageBox::Ok);
    }
}