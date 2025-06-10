#ifndef CST_PLAINTEXTDIALOG_H
#define CST_PLAINTEXTDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>

class CST_PlainTextDialog: public QDialog {
    Q_OBJECT

    public:
    CST_PlainTextDialog(QWidget *parent = (QWidget *)nullptr, QString title = "Nhập dữ liệu", QString label = "Nhập dữ liệu: ");
    QString getResult() const;
    QLineEdit *editor;

    private:
};

#endif