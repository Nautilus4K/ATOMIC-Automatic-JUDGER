#ifndef CST_TEXTEDITORDIALOG_H
#define CST_TEXTEDITORDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

// ---------------------------------------------------------------------
// Feature: To have a dialog consisting of text input. Or in short, a
//          text editor dialog (that doesn't interrupt the main window)
// ---------------------------------------------------------------------

class CST_TextEditorDialog: public QWidget {
    Q_OBJECT

    public:
    explicit CST_TextEditorDialog(QWidget *parent = (QWidget *)nullptr, QString title = "", std::string filePath = "", bool readOnly = false);
    void setText(QString text);
    
    QLabel *mainText = new QLabel();
};

#endif