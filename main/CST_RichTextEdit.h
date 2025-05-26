#ifndef CST_RICHTEXTEDIT_H
#define CST_RICHTEXTEDIT_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>

// ---------------------------------------------------------------------
// Feature: To have a RICH text editor widget. Which will be very nice.
//          Along with some features such as image inserting...
// ---------------------------------------------------------------------
class CST_RichTextEdit: public QWidget {
    Q_OBJECT

    public:
    CST_RichTextEdit(QWidget *parent = (QWidget*)nullptr);

    void initVars();
    void reloadCurrentCursorState();
    void toggleFormatting(char frm);
    void append(QString text);
    void setText(QString text, bool reset);
    QString toHtml() const;
    void setHtml(const QString& text);

    QWidget *formatBar = new QWidget(this);
    QTextEdit *editor = new QTextEdit(this);
    QPushButton *boldFrmBtn = new QPushButton(formatBar);
    QPushButton *italicFrmBtn = new QPushButton(formatBar);
    QPushButton *ulFrmBtn = new QPushButton(formatBar); // The name is quite ridiculous but its 'underline'

    protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif