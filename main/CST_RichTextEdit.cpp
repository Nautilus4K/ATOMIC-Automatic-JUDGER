#include "CST_RichTextEdit.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

#include <QtGui/QTextDocumentFragment>

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>

CST_RichTextEdit::CST_RichTextEdit(QWidget *parent) : QWidget(parent) {
    setStyleSheet(parent->styleSheet());
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);

    // The formatting bar should be a QWidget to house multiple buttons. Should work, too.
    QHBoxLayout *formatBarLayout = new QHBoxLayout(formatBar);
    formatBarLayout->setAlignment(Qt::AlignLeft);
    formatBarLayout->setContentsMargins(0, 0, 0, 0);

    boldFrmBtn->setObjectName("formatBtn");
    boldFrmBtn->setStyleSheet("font-weight: bold;");
    boldFrmBtn->setFixedSize(20, 20);
    boldFrmBtn->setText("B");
    connect(boldFrmBtn, &QPushButton::clicked, this, [this] {
        toggleFormatting('B');
    });

    formatBarLayout->addWidget(boldFrmBtn); // Finalize with a pop

    italicFrmBtn->setObjectName("formatBtn");
    italicFrmBtn->setStyleSheet("font-style: italic;");
    italicFrmBtn->setFixedSize(20, 20);
    italicFrmBtn->setText("I");
    connect(italicFrmBtn, &QPushButton::clicked, this, [this] {
        toggleFormatting('I');
    });

    formatBarLayout->addWidget(italicFrmBtn); // Add the italic button in

    ulFrmBtn->setObjectName("formatBtn");
    ulFrmBtn->setStyleSheet("text-decoration: underline;");
    ulFrmBtn->setFixedSize(20, 20);
    ulFrmBtn->setText("U");
    connect(ulFrmBtn, &QPushButton::clicked, this, [this] {
        toggleFormatting('U');
    });

    formatBarLayout->addWidget(ulFrmBtn);

    layout->addWidget(formatBar);

    // The editor
    editor->installEventFilter(this);
    editor->setAcceptRichText(false);
    editor->setMinimumHeight(140); // Yeah we WILL definitely need this. ngl its trickery but still...

    connect(editor, &QTextEdit::cursorPositionChanged, this, &CST_RichTextEdit::reloadCurrentCursorState);
    connect(editor, &QTextEdit::selectionChanged, this, &CST_RichTextEdit::reloadCurrentCursorState);
    layout->addWidget(editor);

    // Initialize variables (and statuses...)
    initVars();
}

void CST_RichTextEdit::initVars() {
    // isCurrentlyBold = false;
    // isCurrentlyItalic = false;
    // isCurrentlyUnderlined = false;
    editor->setFontWeight(QFont::Normal);
    editor->setFontItalic(false);
    editor->setFontUnderline(false);

    // Setting editor stats
    editor->setFontWeight(QFont::Normal);
}

void CST_RichTextEdit::reloadCurrentCursorState() {
    // To reload the current cursor's state (including its formatting, area, etc...)
    // QTextCursor cursor = editor->textCursor();

    // If current font weight is bold
    if (editor->fontWeight() == QFont::Bold) boldFrmBtn->setStyleSheet("background-color: #9cc2ff; font-weight: bold; color: #000000");
    else boldFrmBtn->setStyleSheet("background-color: #ffffff; font-weight: bold; color: #000000");

    if (editor->fontItalic()) italicFrmBtn->setStyleSheet("background-color: #9cc2ff; font-weight: bold; color: #000000");
    else italicFrmBtn->setStyleSheet("background-color: #ffffff; font-weight: bold; color: #000000");

    if (editor->fontUnderline()) ulFrmBtn->setStyleSheet("background-color: #9cc2ff; font-weight: bold; color: #000000");
    else ulFrmBtn->setStyleSheet("background-color: #ffffff; font-weight: bold; color: #000000");
}

// A function for toggling a certain 'kind' of formatting
//
// For instance, can be used as: toggleFormatting('B') to toggle bold
// formatting, so on and so forth
void CST_RichTextEdit::toggleFormatting(char frm) {
    if (frm == 'B' || frm == 'b') {
        if (editor->fontWeight() != QFont::Bold) {
            editor->setFontWeight(QFont::Bold);
        } else {
            editor->setFontWeight(QFont::Normal);
        }
    } else if (frm == 'I' || frm == 'i') {
        if (!editor->fontItalic()) {
            editor->setFontItalic(true); // Now its italic
        } else {
            editor->setFontItalic(false);
        }
    } else if (frm == 'U' || frm == 'u') {
        if (!editor->fontUnderline()) {
            editor->setFontUnderline(true);
        } else {
            editor->setFontUnderline(false);
        }
    }

    reloadCurrentCursorState();
}

// Appending text. Kinda nice, isn't it?
void CST_RichTextEdit::append(QString text) {
    editor->append(text);
}

// Fast oneclick formatless text setting
void CST_RichTextEdit::setText(QString text, bool reset) {
    if (reset) initVars();
    editor->setText(text);
}

QString CST_RichTextEdit::toHtml() const {
    QTextDocumentFragment frag(editor->document());
    QString fullHtml = frag.toHtml();

    // Time to slice this to pieces
    // So we already know that there will be tags:
    // <!--StartFragment-->
    // <!--EndFragment-->
    // And because of that, it should be safe to say
    // we dont need anything else over this.
    static const QString& startTag = "<!--StartFragment-->";
    static const QString& endTag = "<!--EndFragment-->";

    int slStart = fullHtml.indexOf(startTag);
    int slEnd = fullHtml.indexOf(endTag);

    // Validating the newly accquired positions
    if (slStart >= 0 && slEnd > slStart) {
        // Slicing and returning.
        // Also skipping the unneccessary tags
        return fullHtml.mid(slStart + startTag.length(), slEnd - (slStart + startTag.length()));
    }

    return "";
}

void CST_RichTextEdit::setHtml(const QString& text) {
    editor->clear();
    editor->insertHtml(text);
}

bool CST_RichTextEdit::eventFilter(QObject* obj, QEvent* event) {
    if (obj == editor && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        
        if (keyEvent->modifiers() & Qt::ControlModifier) {
            switch (keyEvent->key()) {
                case Qt::Key_B:
                    toggleFormatting('B');
                    return true;
                case Qt::Key_I:
                    toggleFormatting('I');
                    return true;
                case Qt::Key_U:
                    toggleFormatting('U');
                    return true;
            }
        }
    }
    
    return QObject::eventFilter(obj, event);
}