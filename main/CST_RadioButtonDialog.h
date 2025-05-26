#ifndef CST_RADIOBUTTONDIALOG_H
#define CST_RADIOBUTTONDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMessageBox>

// ------------------------------------------------------
// Feature: To have a dialog consisting of radio buttons
// ------------------------------------------------------
class CST_RadioButtonDialog : public QDialog {
    Q_OBJECT

    public:
    explicit CST_RadioButtonDialog(QWidget *parent = (QWidget *)nullptr, QString title = "Radio Button Dialog", QString question = "Question field", QStringList entries = {});
    std::vector<QRadioButton*> choices;
    QString selectedOption() const;

    private:
    void finishDialog();
};

#endif