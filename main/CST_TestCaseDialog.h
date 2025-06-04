#ifndef CST_TESTCASEDIALOG_H
#define CST_TESTCASEDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>

// -----------------------------------------------------------------------
// Purpose: Have a dialog to edit test cases that needs to have multiline
//          input and output
// -----------------------------------------------------------------------
class CST_TestCaseDialog : public QDialog {
    Q_OBJECT

    public:
    CST_TestCaseDialog(QWidget *parent = (QWidget *)nullptr, QString currentInput = "", QString currentOutput = "");

    // Function to get the result of the dialog (Or messagebox, some would say)
    // This will return a `QStringList` with two values only, one for output and one for input
    // The order is this:
    // {<input>, <output>}
    // DO NOT FORGET YOU FILTHY BASTARD (its me talking to myself like a fucking autistic dickhead)
    QStringList getResult() const;

    private:
    QTextEdit *inputEditField = new QTextEdit(this);
    QTextEdit *outputEditField = new QTextEdit(this);
};

#endif