/**********************************

      ::::    :::     :::     :::    ::: ::::::::::: ::::::::::: :::       :::    :::  ::::::::      :::     :::    ::: 
     :+:+:   :+:   :+: :+:   :+:    :+:     :+:         :+:     :+:       :+:    :+: :+:    :+:    :+:      :+:   :+:   
    :+:+:+  +:+  +:+   +:+  +:+    +:+     +:+         +:+     +:+       +:+    +:+ +:+          +:+ +:+   +:+  +:+     
   +#+ +:+ +#+ +#++:++#++: +#+    +:+     +#+         +#+     +#+       +#+    +:+ +#++:++#++  +#+  +:+   +#++:++       
  +#+  +#+#+# +#+     +#+ +#+    +#+     +#+         +#+     +#+       +#+    +#+        +#+ +#+#+#+#+#+ +#+  +#+       
 #+#   #+#+# #+#     #+# #+#    #+#     #+#         #+#     #+#       #+#    #+# #+#    #+#       #+#   #+#   #+#       
###    #### ###     ###  ########      ###     ########### ########## ########   ########        ###   ###    ###   

This is free software made by Nautilus4K a.k.a Quang Vinh.
Any form of credits is neccessary as described in the license in the repository

Repo link: https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER/


Compilation arguments:
"-fdiagnostics-color=always",
"-I\"C:\\Qt\\6.5.3\\mingw_64\\include\"",
"-I\"C:\\Qt\\Tools\\mingw1120_64\\include\"",
"-L\"C:\\Qt\\6.5.3\\mingw_64\\lib\"",
"-L\"C:\\Qt\\Tools\\mingw1120_64\\lib\"",
"${file}",
"${fileBasenameNoExtension}.o",
"-lQt6Widgets",
"-lQt6Core",
"-lQt6Gui",
"-o",
"${fileDirname}\\${fileBasenameNoExtension}.exe",

This panel.cpp is for WINDOWS. So, yeah, sorry Linux folks.
It's all cause most Vietnamese people uses Windows so yea
Sorrie

Also, credits to:
- Qt for making such an awesome GUI framework. I'm in love with it.
- nlomann for making a JSON manipulation API. https://github.com/nlohmann/json

**********************************/

// Importing Qt stuffs
#include <QtWidgets/QApplication>      // For application
#include <QtWidgets/QMainWindow>       // For main window
#include <QtWidgets/QWidget>           // For a window-like area
#include <QtWidgets/QVBoxLayout>       // Verical Layout of QWidgets
#include <QtWidgets/QHBoxLayout>       // Horizontal Layout of QWidgets
#include <QtWidgets/QScrollArea>       // Scrollable area. Used with QWidgets
#include <QtWidgets/QFrame>            // Frames. Like the one outline in QScrollArea
#include <QtWidgets/QSplitter>         // Splitter
#include <QtWidgets/QTabWidget>        // Tabs
#include <QtWidgets/QMenuBar>          // Menu bar (The toolbar on window)
#include <QtWidgets/QMenu>             // Menu (Entries in QMenuBar)
#include <QtWidgets/QMessageBox>       // Message boxes
#include <QtWidgets/QLabel>            // Labels
#include <QtWidgets/QLineEdit>         // An edit area that is a line edit. No multiple lines.
#include <QtWidgets/QCheckBox>         // Checkboxes. Of course. What did you expect, sucker?
#include <QtWidgets/QPushButton>       // Buttons as we know it
#include <QtWidgets/QTextEdit>         // A text frame
#include <QtWidgets/QComboBox>         // Dropdown. Yeah. Qt is very nice with their names
#include <QtWidgets/QTableWidget>      // Table of content
#include <QtWidgets/QStyleFactory>     // Styling. Once again
#include <QtWidgets/QDialog>           // Dialogs, even custom ones
#include <QtWidgets/QRadioButton>      // Radio buttons
#include <QtWidgets/QButtonGroup>      // Grouping multiple buttons
#include <QtWidgets/QListWidget>       // List view BUT able to put multiple elements into this
#include <QtWidgets/QListView>         // QListWidget but worse. However, much more lightweight (Also not as simple).
#include <qtWidgets/QInputDialog>      // INPUT.IN.A.DIALOG. Yessssirrrr
#include <QtGui/QAction>               // Action for menus. Wonder what fucker thought to put it in QtGui
#include <QtGui/QCloseEvent>           // Close event. The action of 'X' button
#include <QtGui/QDoubleValidator>      // Validator for edits.
#include <QtGui/QIntValidator>         // Another validator, also for edits, now more like INTEGER revolved.
#include <QtGui/QIcon>                 // Icon readings
#include <QtGui/QPixmap>               // Picture reading
#include <QtGui/QFont>                 // Fonts
#include <QtGui/QColor>                // Colors
#include <QtGui/QTextDocumentFragment> // Producing a fragment of an HTML document. Sorta like a wrapper cleaner for HTMLs

// Importing Qt related features
#include <QtCore/Qt>       // A bunch of constants
#include <QtCore/QProcess> // Process running
#include <QtCore/QStringListModel> // QStringList model? Idk

// File I/O actions and getting data
#include <fstream>
#include <nlohmann/json.hpp>
// Turn nlohmann::json to json for short.
using json = nlohmann::json;

// System-related actions
#ifndef WIN32_LEAN_AND_MEAN // Because of historical reasons, including Windows.h
#define WIN32_LEAN_AND_MEAN // along with winsock2.h will need to have this
#endif

#include <filesystem>
#include <Windows.h>
#include <winsock2.h> // Socket programming
#include <shellapi.h> // ShellExecuteA, etc...
#include <ws2tcpip.h>
#include <filesystem> // File managing

#pragma comment(lib, "Ws2_32.lib") // Required library to link with

// C++ Features
#include <vector>
#include <map>
#include <sstream>

// Debug console
#include <iostream>

// Constants
// -> Values
#define NONE_PLACEHOLDER -0x7fffffff

// -> Extensions
const std::vector<std::string> supportedExtensions = {".cpp", ".py", ".pas"};
const std::string logExt = ".log";

// -> Paths
static const std::string THEME_PATH = "/source/theme.qss";
static const std::string THEMECOLORS_PATH = "/source/theme_color.opt";

static const std::string SETTINGS_PATH = "/source/settings.json";
static const std::string VERSION_PATH = "/source/version.json";
static const std::string ALIAS_PATH = "/source/aliases.json";
static const std::string CLASSES_PATH = "/source/classes.json";
static const std::string CONTESTS_PATH = "/source/contests.json";
static const std::string USERDATA_PATH = "/source/users.json";
static const std::string USERSTATS_DIR = "/workspace/result/";
static const std::string USERQUEUE_DIR = "/workspace/queue/";
static const std::string USERSUBHISTORY_DIR = "/userdata/";
static const std::string SUBMITLOG_DIR = "/workspace/result/logs/";  // EVERY SINGLE DIRECTORY PATH NEEDS TO HAVE A TRAILING `/`

static const std::string PYDIR = "/.venv/Scripts/python.exe";
static const std::string JUDGING_PATH = "/judge.py";
static const std::string WEBSERVER_PATH = "/apache.py";

static const std::string LOG_PATH = "/central/valkyrie.log";
static const std::string ICON_PATH = "/icon.ico";
static const QString EXITICON_PATH = ":/images/exit.png";
static const QString CONTESTSICON_PATH = ":/images/contests.png";

// Paths
const std::string dirPath = std::filesystem::current_path().string();

// -> Qt Style Sheet
const QString STYLE_BIGLABEL = "font-size: 16px; font-weight: bold;";
const QString STYLE_BOLDLABEL = "font-weight: bold;";

// -> Others
const std::string GITHUB_PAGE = "\"https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER\"";
const int JUDGING_EXITPORT = 28472;
const std::string JUDGING_EXITADDR = "127.0.0.1";
const int WEBSERVER_EXITPORT = 28473;
const std::string WEBSERVER_EXITADDR = "127.0.0.1";

// -> ANSI Color codes
const std::string INFO_COL = "\x1b[0m";
const std::string ERROR_COL = "\x1b[31m";
const std::string WARN_COL = "\x1b[33m";
const std::string OK_COL = "\x1b[32m";
const std::string RESET_COL = "\x1b[0m";

// -> Color values
QColor COLOR_CONSOLE_ERROR;
QColor COLOR_CONSOLE_WARNING;
QColor COLOR_CONSOLE_OK;
QColor COLOR_CONSOLE_DEFAULT;
QString FORMATBTN_DEFAULT;
QString FORMATBTN_SELECTED;

// Custom functions as tools
std::string intToString(int n) {
    // Conversion from INTEGER to STRING
    if (n == 0) return "0";
    
    std::string value = "";
    while (n > 0) {
        value += char((n % 10) + '0'); // Literal magic
        n /= 10;
    }
    std::reverse(value.begin(), value.end());

    return value;
}

std::string doubleToString(double d) {
    // Conversion from DOUBLE to STRING
    std::ostringstream oss;
    oss << d; // Yeah this is the true magic
    return oss.str();
}

double stringToDouble(const std::string &str) {
    try {
        return std::stod(str); // Built-in function
    } catch (const std::invalid_argument &e) {
        return 0.0; // Handle invalid input (e.g., empty string or non-numeric data)
    } catch (const std::out_of_range &e) {
        return 0.0; // Handle out-of-range errors
    }
}


int stringToInt(const std::string &s) {
    int result = 0;
    bool negative = false;
    std::size_t i = 0;

    if (s[i] == '-') {
        negative = true;
        i++;
    }

    for (; i < s.length(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            // Invalid character for number
            break;
        }
        result = result * 10 + (s[i] - '0');
    }

    return negative ? -result : result;
}

// Special widgets / functionalities

// ------------------------------------------------------
// Feature: To have a dialog consisting of radio buttons
// ------------------------------------------------------
class CST_RadioButtonDialog: public QDialog {
    public:
    // Create a vector so that we can browse through and check the result in the future
    std::vector<QRadioButton*> choices;

    CST_RadioButtonDialog(QWidget *parent = (QWidget *)nullptr, QString title = "Radio Button Dialog", QString question = "Question field", QStringList entries = {}) : QDialog(parent) {
        setObjectName("dialog");
        setWindowTitle(title);
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

    QString selectedOption() const {
        for (const QRadioButton *button : choices) {
            if (button->isChecked()) {
                return button->text();
            }
        }
        
        // In case NOTHING is selected
        return "";
    }

    private:
    void finishDialog() {
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
};

// ---------------------------------------------------------------------
// Feature: To have a dialog consisting of text input. Or in short, a
//          text editor dialog (that doesn't interrupt the main window)
// ---------------------------------------------------------------------
class CST_TextEditorDialog: public QWidget {
    public:
    QLabel *mainText = new QLabel();
    CST_TextEditorDialog(QWidget *parent = (QWidget *)nullptr, QString title = "", std::string filePath = "", bool readOnly = false) {
        setObjectName("dialog");
        setWindowTitle(title);
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
    }

    void setText(QString text) {
        mainText->setText(text);
    }
};

// ---------------------------------------------------------------------
// Feature: To have a RICH text editor widget. Which will be very nice.
//          Along with some features such as image inserting...
// ---------------------------------------------------------------------
class CST_RichTextEdit: public QWidget {
    public:
    QWidget *formatBar = new QWidget(this);
    QTextEdit *editor = new QTextEdit(this);
    QPushButton *boldFrmBtn = new QPushButton(formatBar);
    QPushButton *italicFrmBtn = new QPushButton(formatBar);
    QPushButton *ulFrmBtn = new QPushButton(formatBar); // The name is quite ridiculous but its 'underline'

    CST_RichTextEdit(QWidget *parent = (QWidget*)nullptr) {
        setStyleSheet(parent->styleSheet());

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

    void initVars() {
        // isCurrentlyBold = false;
        // isCurrentlyItalic = false;
        // isCurrentlyUnderlined = false;
        editor->setFontWeight(QFont::Normal);
        editor->setFontItalic(false);
        editor->setFontUnderline(false);

        // Setting editor stats
        editor->setFontWeight(QFont::Normal);
    }

    void reloadCurrentCursorState() {
        // To reload the current cursor's state (including its formatting, area, etc...)
        // QTextCursor cursor = editor->textCursor();

        // If current font weight is bold
        if (editor->fontWeight() == QFont::Bold) boldFrmBtn->setStyleSheet("background-color: " + FORMATBTN_SELECTED + "; font-weight: bold;");
        else boldFrmBtn->setStyleSheet("background-color: " + FORMATBTN_DEFAULT + "; font-weight: bold;");

        if (editor->fontItalic()) italicFrmBtn->setStyleSheet("background-color: " + FORMATBTN_SELECTED + "; font-style: italic;");
        else italicFrmBtn->setStyleSheet("background-color: " + FORMATBTN_DEFAULT + "; font-style: italic;");

        if (editor->fontUnderline()) ulFrmBtn->setStyleSheet("background-color: " + FORMATBTN_SELECTED + "; text-decoration: underline;");
        else ulFrmBtn->setStyleSheet("background-color: " + FORMATBTN_DEFAULT + "; text-decoration: underline;");
    }

    // A function for toggling a certain 'kind' of formatting
    //
    // For instance, can be used as: toggleFormatting('B') to toggle bold
    // formatting, so on and so forth
    void toggleFormatting(char frm) {
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
    void append(QString text) {
        editor->append(text);
    }

    // Fast oneclick formatless text setting
    void setText(QString text, bool reset) {
        if (reset) initVars();
        editor->setText(text);
    }

    QString toHtml() const {
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

    void setHtml(const QString& text) {
        editor->clear();
        editor->insertHtml(text);
    }

    protected:
    // Filtering event if it happens. Override the stock function
    bool eventFilter(QObject* obj, QEvent* event) override {
        // Check if user is pressing when selecting the editor (setting it as active)
        if (obj == editor && event->type() == QEvent::KeyPress) {
            if (!editor) {
                std::cout << "[*RICH TEXT EDIT* WIDGET] editor is nullptr\n";
                return QObject::eventFilter(obj, event);
            }

            // Get the underlying key event
            auto *keyEvent = static_cast<QKeyEvent*>(event);

            // if (!editor->focused)

            // Debugging
            // std::cout << "Key pressed:" << keyEvent->key() << "Modifiers:" << keyEvent->modifiers().toInt() << '\n';

            // FUCKING BITMASK
            // What are those shits?
            // They gives a bunch of logic errors and they are fucking nauseating...
            if ((keyEvent->modifiers() & Qt::ControlModifier) && keyEvent->key() == Qt::Key_B) {
                // Yeah Ctrl+B
                toggleFormatting('B');
                return true; // Handled. Skip default
            }

            if ((keyEvent->modifiers() & Qt::ControlModifier) && keyEvent->key() == Qt::Key_I) {
                // Ctrl+I babyyyyy
                toggleFormatting('I');
                return true;
            }

            if ((keyEvent->modifiers() & Qt::ControlModifier) && keyEvent->key() == Qt::Key_U) {
                // Ctrl+U for underline
                toggleFormatting('U');
                return true;
            }

            return false; // Not handled.
        }

        return QObject::eventFilter(obj, event);
    }
};

// ---------------------------------------------------------------
// Feature: A list view that cans be modified in terms of entries
// ---------------------------------------------------------------
class CST_Listing: public QWidget {
    public:
    QListView *listView = new QListView(this);
    QStringListModel *model = new QStringListModel(this); // NULL AT FIRST, I GUESS
    QPushButton *addBtn = new QPushButton(this); // It's shit, i know, its inefficient, i know
    QPushButton *subBtn = new QPushButton(this); // what do you honestly expect from a fucking 14 years old?

    CST_Listing(QWidget *parent = (QWidget *)nullptr) {
        setStyleSheet(parent->styleSheet());

        QVBoxLayout *layout = new QVBoxLayout(this);
        setLayout(layout);

        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(listView);
        
        // The buttons need to be on the same plane
        addBtn->setText("+ Thêm");
        subBtn->setText("- Bớt");
        addBtn->setObjectName("genericBtn"); // Idk man, we need this so that the btns have
        subBtn->setObjectName("genericBtn"); // the correct looks

        // Connecting to the neccessary functions
        connect(addBtn, &QPushButton::clicked, this, CST_Listing::addEntry);
        connect(subBtn, &QPushButton::clicked, this, CST_Listing::removeEntry);

        // Creates a horizontally aligned QWidget
        QWidget *btnPlane = new QWidget(this);
        btnPlane->setStyleSheet(styleSheet());
        QHBoxLayout *btnPlaneLayout = new QHBoxLayout(btnPlane);

        btnPlaneLayout->setAlignment(Qt::AlignLeft);
        btnPlaneLayout->setContentsMargins(0, 0, 0, 0);
        btnPlaneLayout->addWidget(addBtn);
        btnPlaneLayout->addWidget(subBtn);

        btnPlane->setLayout(btnPlaneLayout);

        layout->addWidget(btnPlane);
    }

    // Setting entries of list
    void setEntries(QStringList entList) {
        model->setStringList(entList);
        listView->setModel(model);
    }

    // Getting entries of list
    QStringList getEntries() const {
        return model->stringList();
    }

    void setReadOnly(bool b) {
        listView->setEditTriggers(b ? QAbstractItemView::NoEditTriggers : QAbstractItemView::AllEditTriggers);
    }

    private:
    void addEntry() {
        bool ok; // This is NOT a pointer
        // Using a pointer pointing to the ok variable (which is not a pointer) to get the value.
        // Very fun
        QString name = QInputDialog::getText(this, "Nhập tên của mục mới", "Hãy nhập tên cho mục mới bạn cần thêm:", QLineEdit::Normal, "", &ok);

        if (ok && !name.isEmpty()) { // User clicked OK and did not leave name blank
            QStringList curEnt = getEntries(); // Get current entries so that I can add more

            // Validifying
            bool unique = true;
            for (const QString entName : curEnt) {
                if (entName == name)  {
                    // So what this does is it browse through the LIST
                    // If it found just a single matching entry, it would HALT immediately
                    // and return that the one we just typed in is NOT unique.
                    // If it didn't and ran through the entire QStringList, then we accepts this as unique
                    unique = false;
                    break;
                }
            }

            if (unique) {
                curEnt << name;
                setEntries(curEnt); // Set the new entries we just created as the current entries
            } else {
                QMessageBox::warning(this, "Chưa thêm mục", "Mục không được thêm vào vì có sự trùng lặp", QMessageBox::Ok);
                addEntry(); // Recursiveness is something I like? Idk man, its just fascinating asf.
            }
        }
    }

    void removeEntry() {
        // To remove an entry, we need to index where is that entry we are selecting...
        QModelIndexList indexes = listView->selectionModel()->selectedRows(0);

        QStringList curEnts = getEntries();

        for (const auto& index : indexes) {
            curEnts.removeAt(index.row());
        }

        // Update string model
        setEntries(curEnts);
    }
};

// ------------------------------------------------
// Functionality: To edit/change contests settings
// ------------------------------------------------
class WIN_ContestsSettings: public QWidget {
    public:
    // Sidebar widget
    QListWidget *listView = new QListWidget(this);
    CST_RichTextEdit *descEdit = new CST_RichTextEdit(this);
    CST_Listing *classList = new CST_Listing(this);
    QLabel *contestNameLabel = new QLabel(); // Give it nullptr since I am a lazy bitch
    QPushButton *saveBtn = new QPushButton();

    json contests; // Contest data in JSON. Would allow for faster reading since the whole thing is already based around JSON
    std::string currentCnts = "";
    
    WIN_ContestsSettings(QWidget *parent = (QWidget *)nullptr) {
        setObjectName("container");
        setWindowIcon(parent->windowIcon());
        setStyleSheet(parent->styleSheet());
        setWindowTitle("Cài đặt bài thi");
        setAttribute(Qt::WA_DeleteOnClose); // Cleaning
        
        setMinimumHeight(400);
        setMinimumWidth(600);
        resize(QSize(700, 450)); // this is tiring asf ngl
        
        // LAYOUT
        QVBoxLayout *layout = new QVBoxLayout();
        setLayout(layout);
        
        // SPLITTER (for sidebar and details pane)
        QSplitter *splitter = new QSplitter();
        layout->addWidget(splitter);
        
        // The label showing the currently selected contest. Needs to be way up here since this has
        // to be the first reference (except for the part to register this variable).
        contestNameLabel->setStyleSheet(STYLE_BIGLABEL);
        contestNameLabel->setText("Chưa chọn bài thi nào"); // TODO: Fix this useless crap

        // Save button. Reference before fetch contests. All details widgets needs to be referenced first
        // before fetchContests() is called since that's gotta be how it works or else it will be overwritten
        // by the default settings.
        saveBtn->setObjectName("genericBtn");
        saveBtn->setText("Lưu");
        saveBtn->setEnabled(false); // Turns disabled IF there is no contest to save to.
        saveBtn->setFixedWidth(100);
        connect(saveBtn, &QPushButton::clicked, this, WIN_ContestsSettings::saveInfo);

        // INITIALIZATION STEP. I have no idea why I need to put this function caller way up here. But whatever.
        // after all it does work and thats what matters.
        fetchContests(true);
        
        // SIDEBAR
        listView->setObjectName("con_sec");
        listView->setMaximumWidth(280);
        
        // CONTEST DETAILS (A normal containable widget)
        QScrollArea *contestDetailsScrollable = new QScrollArea();
        contestDetailsScrollable->setWidgetResizable(true); // IMPORTANT: This makes the scroll area resize the widget
        contestDetailsScrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        contestDetailsScrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        // Set size policy to expand in both directions
        contestDetailsScrollable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        
        QWidget *contestDetails = new QWidget();
        contestDetails->setObjectName("con_sec");
        contestDetails->setStyleSheet(styleSheet());
        QVBoxLayout *contestDetailsLayout = new QVBoxLayout(contestDetails);
        
        // Setting the layout
        contestDetails->setLayout(contestDetailsLayout);

        contestDetailsLayout->addWidget(contestNameLabel);

        // The label showing the description section
        QLabel *descLabel = new QLabel(contestDetails);
        descLabel->setObjectName("con_lab");
        descLabel->setText("Mô tả bài thi");

        QLabel *classLabel = new QLabel(contestDetails);
        classLabel->setObjectName("con_lab");
        classLabel->setText("Các lớp nhận bài thi");
        
        // Configuring classList
        classList->setReadOnly(true);

        // Adding in the widgets in order (for the looks actually.).
        contestDetailsLayout->addWidget(descLabel);
        contestDetailsLayout->addWidget(descEdit);
        contestDetailsLayout->addWidget(classLabel);
        contestDetailsLayout->addWidget(classList);
        contestDetailsLayout->addWidget(saveBtn);
        
        contestDetailsScrollable->setWidget(contestDetails);
        
        splitter->addWidget(listView);
        splitter->addWidget(contestDetailsScrollable);
    }
    
    private:
    std::vector<std::string> contestByRowOrder;
    // --------------------------
    // Purpose: Look at the name
    // --------------------------
    void reloadContestsVar() {
        std::fstream contestsFile(dirPath + CONTESTS_PATH, std::ios::in);
        if (contestsFile.is_open()) {
            // No errors
            try {
                // Apply the neccessary data
                contests = json::parse(contestsFile);

                std::cout << contests << '\n';
            } catch (const json::parse_error& e) {
                // Yes errors
                errorDialog("Không thể đọc dữ liệu bài thi (Tệp bị hỏng?).");
                close();
            }
        } else {
            std::cout << "Unable to open contestsFile\n";
            errorDialog("Không thể truy cập dữ liệu bài thi.");
            close();
        }
        contestsFile.close();
    }

    // ---------------------------------------------------------------
    // Purpose: A way to fetch new contests when I just opened it up.
    // ---------------------------------------------------------------
    void fetchContests(bool selectEntryAutomatically) {
        reloadContestsVar();
        listView->clear();


        // After we've got the required data to secure the ticket to showing whats needed
        // A.K.A showing the contests in a LIST VIEW. Which is pretty much obsolete at this
        // point but I dont have any other alternative that is much easier to use.
        for (const auto& item : contests.items()) {
            // std::cout << "[*ContestsSettings] Now processing: " << item.key() << '\n';
            contestByRowOrder.push_back(item.key());
            const QString contestName = QString::fromStdString(item.key());

            QListWidgetItem *listItem = new QListWidgetItem(listView);

            // Actually. Even though it will be kinda a memory hog, I still wants to make it looks BEAUTIFUL
            // So. What's the looks?
            // +-----------------------+
            // | BIG LABEL FOR CONTEST |
            // | smaller for classes   |
            // +-----------------------+
            // This will be achievable with a QWidget
            QWidget *listItemWidget = new QWidget(this);
            QVBoxLayout *listItemWidgetLayout = new QVBoxLayout(listItemWidget); // Actually, I don't like the names to be too
                                                                                 // fucking long. But I have no other choices

            listItemWidget->setLayout(listItemWidgetLayout); // Setting the layout. Now we just need to make all these work by
                                                             // Adding QLabels
            
            QLabel *contestHeader = new QLabel(listItemWidget);
            contestHeader->setStyleSheet(STYLE_BIGLABEL);
            contestHeader->setText(contestName);

            QLabel *contestInf = new QLabel(listItemWidget); // The smaller information label

            // Setting text
            contestInf->setText(QString::fromStdString(contests[item.key()]["Desc"]));
            contestInf->setTextFormat(Qt::RichText);
            contestInf->setWordWrap(false); // No automatic wrapping
            contestInf->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed); // No expanding.
            contestInf->setFixedHeight(contestInf->fontMetrics().height() + 4); // Housing ONE LINE only
            // std::cout << item.key() << ": " << item.value() << '\n';

            // Adding the newly made headers and inf elements
            listItemWidgetLayout->addWidget(contestHeader);
            listItemWidgetLayout->addWidget(contestInf);

            // Now we apply the QWidget in
            listItem->setSizeHint(listItemWidget->sizeHint());
            listView->setItemWidget(listItem, listItemWidget);

            if (contestByRowOrder.size() == 1 && selectEntryAutomatically) { // If this is a first
                // This is created so that it will execute only if there is at least one contest
                // available.
                // Else then nothing will show and it should be disabled (by default).
                listView->setCurrentItem(listItem);
                contestNameLabel->setText(contestName);
                saveBtn->setEnabled(true);
                currentCnts = contestName.toStdString();

                toCnts(contestName.toStdString());
                std::cout << "Performed special move on contestByRowOrder. Guess what it is :)\n";
            }
        }

        // Styling to make the list view works like a navigation pane
        listView->setSelectionMode(QAbstractItemView::SingleSelection);
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Prevent editing

        // Connecting the signal of selection.
        connect(listView, &QListWidget::itemClicked, this, [this](const QListWidgetItem *item) {
            const int index = listView->row(item);

            std::cout << "Clicked on " << contestByRowOrder[index] << '\n';
            currentCnts = contestByRowOrder[index];
            toCnts(contestByRowOrder[index]);
        });
    }

    // --------------------------------------------------------------------
    // Purpose: To save the information to the FILE. Alright? Not the JSON
    //          variable but the FILE ITSELF. Kinda like syncing
    // --------------------------------------------------------------------
    void saveInfo() {
        contests[currentCnts]["Desc"] = descEdit->toHtml().toStdString();

        // Classes will be different since it is a list
        std::vector<std::string> classVectorList;
        for (const QString& className : classList->getEntries()) {
            classVectorList.push_back(className.toStdString());
        }
        contests[currentCnts]["Classes"] = classVectorList;
        // std::cout << descEdit->toHtml().toStdString();

        // Now some I/O trickery because to be honest, Idk how it works either
        std::fstream contestsFile(dirPath + CONTESTS_PATH, std::ios::out);
        if (contestsFile.is_open()) {
            contestsFile << contests;
        } else {
            errorDialog("Mở tệp bài làm không thành công. Đã có lỗi xảy ra.");
        }
    }

    // --------------------------------------------------------
    // Purpose: To refresh currently clicked on contest with a
    //          compact function
    // --------------------------------------------------------
    void toCnts(std::string contestName) {
        reloadContestsVar();

        contestNameLabel->setText(QString::fromStdString(contestName));
        descEdit->setHtml(QString::fromStdString(contests[contestName]["Desc"]));

        // Identifying all classes that will be recieving this contest
        QStringList classStringList;
        for (const std::string& className : contests[contestName]["Classes"]) {
            classStringList << QString::fromStdString(className); // Incorporate to a QStringList
            std::cout << className << ", "; // Debugging
        }
        std::cout << "\b\b\n"; // Idk my man

        // Apply into the classList
        classList->setEntries(classStringList);
        
        std::cout << "[*ContestsSettings] Refreshed Information (details) panel.\n";
    }

    // ---------------------------------------------------------------------------
    // Purpose: Showing errors faster than having to type an actual long command.
    //          Can't believe I have to make a duplicate
    // ---------------------------------------------------------------------------
    void errorDialog(std::string error) {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText(QString::fromStdString("Đã có lỗi xảy ra: " + error));
        msgBox->setWindowTitle("Lỗi");
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
        // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
        msgBox->setWindowIcon(windowIcon());
        msgBox->setStyleSheet(styleSheet());

        msgBox->show();
    }

    protected: // Events in which are native Qt events
    void closeEvent(QCloseEvent *event) override {
        event->accept();
        this->deleteLater(); // Cleaning.
    }
};

// --------------------------------------
// Main panel window for all operations.
// --------------------------------------
class PanelWindow: public QMainWindow { // This is based on QMainWindow
    public: // PUBLIC. ACCESSIBLE FROM ANYWHERE
    QWidget *manageTab = new QWidget();
    QWidget *judgingTab = new QWidget();
    QWidget *webserverTab = new QWidget();

    // Option elements (In tabs)
    QLineEdit *judgingWaitTimeInput = new QLineEdit();
    QLineEdit *judgingReloadTimeInput = new QLineEdit();
    QCheckBox *judgingShowTestCheckbox = new QCheckBox();

    QLineEdit *webserverLogInSecsInput = new QLineEdit();
    QLineEdit *webserverAliasWebnameInput = new QLineEdit();
    QLineEdit *webserverAliasSloganInput = new QLineEdit();
    QLineEdit *webserverAliasHostnameInput = new QLineEdit();

    // Main manager for the management of data and profiles
    QComboBox *classDropdown = new QComboBox();
    QTableWidget *currentTable = new QTableWidget();

    // Sidebar elements
    QPushButton *judgingProcessButton = new QPushButton();
    QTextEdit *judgingProcessConsole = new QTextEdit();
    QPushButton *webserverProcessButton = new QPushButton();

    // Data variables
    json settings; // NULL at first
    json version; // NULL at first
    json aliases; // NULL at first
    json classes; // Classes object. NULL at first
    json contests; // Contests object. NULL at first
    json users; // Users object. NULL at first
    QPixmap iconPixmap;
    QFont monospaceFont; // NULL at first
    QString styleSheetResult;

    // Processes
    QProcess *judgingProcess = new QProcess();
    bool judgingEnabled;
    QProcess *webserverProcess = new QProcess();
    bool webserverEnabled;

    PanelWindow() { // Extremely powerful? Extremely complex.
        // This is the configuration part of panelWindow.
        // Initialization will be another one

        ///////////////////////////////////
        // Setting window configurations //
        ///////////////////////////////////
        setWindowTitle("Bảng điều khiển ATOMIC");
        resize(QSize(950, 550));
        setMinimumSize(QSize(600, 400));

        iconPixmap = QPixmap(QString::fromStdString(dirPath + ICON_PATH), "ICO", Qt::AutoColor);
        setWindowIcon(QIcon(iconPixmap));

        // Reading themes from outside files with fstream
        std::cout << "Executable directory: " << dirPath << "\n";
        std::fstream themeFile(dirPath + THEME_PATH, std::ios::in);
        std::string themeDataStr;  // Use std::string
        std::stringstream themeDataBuffer; // Read whole file
        if (themeFile.is_open()) {
            themeDataBuffer << themeFile.rdbuf();  // Read entire file into buffer
            themeFile.close();
            themeDataStr = themeDataBuffer.str();
            QString themeData = QString::fromStdString(themeDataStr);  // Convert std::string to QString
            std::cout << "Theme data:\n" << themeDataStr << "\n";
            styleSheetResult = themeData;
            setStyleSheet(themeData);
        } else {
            // If file is unable to open
            std::cout << "Cannot find theme file\n";
        }

        // Setting fonts
        monospaceFont.setFamilies({"Source Code Pro", "Cascadia Code", "Consolas", "Courier New", "Ubuntu Mono", "monospace"});

        // Preparing processes
        // Before starting the judgingProcess, set environment variables for proper Unicode support
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("PYTHONIOENCODING", "utf-8");  // Force Python to use UTF-8 for stdin/stdout
        env.insert("PYTHONUTF8", "1");            // Force UTF-8 mode in Python 3.7+

        // For Windows specifically
        #ifdef Q_OS_WIN
        env.insert("PYTHONLEGACYWINDOWSSTDIO", "0");  // Disable legacy stdio for better Unicode handling
        #endif

        judgingProcess->setProcessEnvironment(env);

        judgingEnabled = false;
        judgingProcess->setProgram(QString::fromStdString(dirPath + PYDIR));
        judgingProcess->setArguments(QStringList() << "-u" << QString::fromStdString(dirPath + JUDGING_PATH)); // Wrap inside QStringList for arguments

        webserverEnabled = false;
        webserverProcess->setProgram(QString::fromStdString(dirPath + PYDIR));
        webserverProcess->setArguments(QStringList() << QString::fromStdString(dirPath + WEBSERVER_PATH));


        // Connecting with functions
        connect(judgingProcess, &QProcess::readyReadStandardOutput, this, &PanelWindow::judgingHandleStandardOutput);
        connect(judgingProcess, &QProcess::readyReadStandardError, this, &PanelWindow::judgingHandleStandardError);
        connect(judgingProcess, &QProcess::finished, this, &PanelWindow::stoppedJudging);
        connect(webserverProcess, &QProcess::finished, this, &PanelWindow::stoppedWebserver);

        judgingProcess->setReadChannel(QProcess::StandardOutput);
        judgingProcess->setProcessChannelMode(QProcess::MergedChannels); // Merges stdout and stderr


        //////////////
        // Menu bar //
        //////////////
        QMenuBar *menuBar = new QMenuBar();
        setMenuBar(menuBar);

        // FILE MENU
        QMenu *fileMenu = new QMenu("&Tệp");
        menuBar->addMenu(fileMenu);

        // Action of backing up files (Currently, well, not worked on yet)
        QAction *fileBackup = new QAction();
        fileBackup->setText("Sao lưu...");
        fileMenu->addAction(fileBackup);
        // QAction connected function
        connect(fileBackup, &QAction::triggered, this, &PanelWindow::backUp);

        // Action of loading up backup files (WIP)
        QAction *loadBackup = new QAction();
        loadBackup->setText("Mở sao lưu...");
        fileMenu->addAction(loadBackup);
        // QAction connected function
        connect(loadBackup, &QAction::triggered, this, &PanelWindow::loadBackUp);

        // Action of exitting
        QAction *exitApp = new QAction();
        exitApp->setText("Thoát");
        exitApp->setIcon(QIcon(EXITICON_PATH));
        fileMenu->addAction(exitApp);
        // QAction connected function
        connect(exitApp, &QAction::triggered, this, &PanelWindow::close);

        
        // HELP MENU
        QMenu *helpMenu = new QMenu("&Trợ giúp");
        menuBar->addMenu(helpMenu);

        // Action of about GITHUB page
        QAction *openGitHub = new QAction();
        openGitHub->setText("Trang dự án...");
        helpMenu->addAction(openGitHub);
        // QAction connected function
        connect(openGitHub, &QAction::triggered, this, &PanelWindow::gitHub);

        // Action of about GITHUB page
        QAction *openAbout = new QAction();
        openAbout->setText("Về ATOMIC...");
        helpMenu->addAction(openAbout);
        // QAction connected function
        connect(openAbout, &QAction::triggered, this, &PanelWindow::about);


        ///////////////////////////////////
        // Window elements configuration //
        ///////////////////////////////////
        // Main element main window.
        // The so-called `container` will manage the main window's layout and widgets
        QWidget *container = new QWidget();
        container->setObjectName("container"); // Setting name for QSS

        // Layout of container will be called mainLayout.
        // The reason why I had to type new QVBoxLayout() is because of the fact that
        // good ol QVBoxLayout mainLayout; is on stack, but setLayout requires a pointer
        QVBoxLayout *mainLayout = new QVBoxLayout();


        // Splitter for main window
        // The layout of main window:
        // +------+--------------------+
        // | Side | Settings (Tabs)    |
        // | bar  |                    |
        // |      |                    |
        // +------+--------------------+
        // So we need a splitter for main window
        QSplitter *splitter = new QSplitter();
        // Oh and horizontal orientation too
        splitter->setOrientation(Qt::Orientation::Horizontal);

        // Adding splitter into mainLayout
        mainLayout->addWidget(splitter);


        // +-----------------------+
        // | Side                  |
        // | bar configuration     |
        // +-----------------------+
        QWidget *sidebar = new QWidget();
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumWidth(150);
        sidebar->setMaximumWidth(250);

        QVBoxLayout *sidebarLayout = new QVBoxLayout();

        // JUDGING
        QLabel *judgingProcessLabel = new QLabel();
        judgingProcessLabel->setText("Hệ thống chấm bài");
        judgingProcessLabel->setWordWrap(true);
        sidebarLayout->addWidget(judgingProcessLabel);

        judgingProcessButton->setText("Bật chấm bài");
        sidebarLayout->addWidget(judgingProcessButton);
        // Connect to desired function
        connect(judgingProcessButton, &QPushButton::clicked, this, [this] {
            // Using lambda to inject special arguments onto desired function
            // Pretty basic stuffs, eh?
            onSidebarFeatureButtonPushed("judging");
        });

        // Console for judging process
        judgingProcessConsole->setObjectName("console");
        judgingProcessConsole->setReadOnly(true);
        judgingProcessConsole->setAlignment(Qt::AlignTop);
        judgingProcessConsole->setFont(monospaceFont);
        sidebarLayout->addWidget(judgingProcessConsole);
        

        // WEBSERVER
        QLabel *webserverProcessLabel = new QLabel();
        webserverProcessLabel->setText("Trang chấm bài trực tuyến");
        webserverProcessLabel->setWordWrap(true);
        sidebarLayout->addWidget(webserverProcessLabel);

        // We're gonna be putting the webserver toggle and the website fast shortcut on the
        // same line babyyyyyy.
        // First, create a sub-QWidget and then add QHBoxLayout and finally add buttons
        QWidget *webserverProcessButtonsLine = new QWidget();
        QHBoxLayout *webserverProcessButtonsLineLayout = new QHBoxLayout();

        webserverProcessButton->setText("Bật Website");
        connect(webserverProcessButton, &QPushButton::clicked, this, [this] {
            onSidebarFeatureButtonPushed("webserver");
        });
        webserverProcessButtonsLineLayout->addWidget(webserverProcessButton);

        QPushButton *webserverOpener = new QPushButton();
        webserverOpener->setText("Mở");
        connect(webserverOpener, &QPushButton::clicked, this, &PanelWindow::openHttpWebsite); // Connecting to function
        webserverProcessButtonsLineLayout->addWidget(webserverOpener);

        webserverProcessButtonsLine->setLayout(webserverProcessButtonsLineLayout);
        sidebarLayout->addWidget(webserverProcessButtonsLine);


        // Applying layout
        sidebarLayout->setAlignment(Qt::AlignTop);
        sidebar->setLayout(sidebarLayout);

        // Adding sidebar to splitter
        splitter->addWidget(sidebar);


        // +--------------------+
        // | Tabs configuration |
        // +--------------------+
        QTabWidget *tabs = new QTabWidget();
        
        // On tab switches action
        connect(tabs, &QTabWidget::currentChanged, this, &PanelWindow::onTabSwitches);
        
        // Setting up tabs: (With scrolling support)
        // - Management tab
        // - Judging tab
        // - Webserver tab
        QScrollArea *manageScrollArea = new QScrollArea();
        manageScrollArea->setWidgetResizable(true);
        manageScrollArea->setWidget(manageTab);
        manageScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        tabs->addTab(manageScrollArea, "Quản lý");

        QScrollArea *judgingScrollArea = new QScrollArea();
        judgingScrollArea->setWidgetResizable(true);
        judgingScrollArea->setWidget(judgingTab);
        judgingScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        tabs->addTab(judgingScrollArea, "Chấm bài");

        QScrollArea *webserverScrollArea = new QScrollArea();
        webserverScrollArea->setWidgetResizable(true);
        webserverScrollArea->setWidget(webserverTab);
        webserverScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        tabs->addTab(webserverScrollArea, "Trực tuyến");

        // Add tabs into splitter
        splitter->addWidget(tabs);

        // +------------+
        // | Manage tab |
        // +------------+
        QVBoxLayout *manageTabLayout = new QVBoxLayout();

        // Adding the neccessary settings as buttons on a horizontal grid.
        // To achieve this, we need a QScrollArea, QWidget and a QHBoxLayout for that QWidget
        QWidget *settingsLine = new QWidget(this);
        QHBoxLayout *settingsLayout = new QHBoxLayout(settingsLine);

        const int btnWidth = 55, btnHeight = 55;  // Perfect square

        QPushButton *contestsSettings = new QPushButton(settingsLine);
        contestsSettings->setObjectName("genericBtn");
        contestsSettings->setFixedHeight(btnHeight); contestsSettings->setFixedWidth(btnWidth);
        // Applying icon
        QPixmap contestsPXMP(CONTESTSICON_PATH);
        QIcon contestsIcon(contestsPXMP);
        contestsSettings->setIcon(contestsIcon);
        contestsSettings->setIconSize(QSize(btnWidth, btnHeight));
        connect(contestsSettings, &QPushButton::clicked, this, [this] {
            showButtonInfoFromBarType("contests");
        });
        // Adding in
        settingsLayout->addWidget(contestsSettings);
        settingsLayout->setContentsMargins(0, 0, 0, 0);

        settingsLine->setLayout(settingsLayout);

        // Managing scrollablity
        QScrollArea *settingsLineScrollable = new QScrollArea(this);
        settingsLineScrollable->setWidget(settingsLine);
        
        // Calculating height
        int stLeft, stTop, stRight, stBottom;
        settingsLayout->getContentsMargins(&stLeft, &stTop, &stRight, &stBottom);

        int totalHeight = btnHeight + stTop + stBottom;
        settingsLineScrollable->setFixedHeight(totalHeight);

        // Overriding mouse events
        settingsLine->setMouseTracking(true);
        settingsLineScrollable->setMouseTracking(true);
        contestsSettings->setMouseTracking(true);
        settingsLineScrollable->viewport()->setMouseTracking(true);
        contestsSettings->setFocusPolicy(Qt::NoFocus);

        manageTabLayout->addWidget(settingsLineScrollable);

        // Main table part
        classDropdown->setFixedWidth(100);
        QWidget *classDropdownPopUp = classDropdown->view()->window();
        classDropdownPopUp->setAttribute(Qt::WA_TranslucentBackground);        // Allow transparency
        classDropdownPopUp->setWindowFlag(Qt::FramelessWindowHint, true);      // Remove window frame
        classDropdownPopUp->setWindowFlag(Qt::NoDropShadowWindowHint, true);   // Disable drop shadow

        connect(classDropdown, &QComboBox::currentIndexChanged, this, [this] {
            refreshTable();
        });

        manageTabLayout->addWidget(classDropdown);
        
        currentTable->setContextMenuPolicy(Qt::CustomContextMenu); // Uses custom context menu
        connect(currentTable, &QTableWidget::customContextMenuRequested, this, &PanelWindow::showScoreContextMenu);
        manageTabLayout->addWidget(currentTable);

        // Settings
        manageTab->setLayout(manageTabLayout);
        

        // +-------------+
        // | Judging tab |
        // +-------------+
        /*
         * Note: This part here is solely for the judging tab
         * They will have these options
         * 
         * - Wait Time (INPUT) OK
         * - Reload Time (INPUT) OK
         * - Show Tests (CHECKBOX) OK
         * 
         * Update: Please save me from this hellhole
         */
        QVBoxLayout *judgingTabLayout = new QVBoxLayout();

        // WAIT TIME
        QLabel *judgingWaitTimeLabel = new QLabel();
        judgingWaitTimeLabel->setText("Thời gian đợi quét");
        judgingWaitTimeLabel->setStyleSheet(STYLE_BIGLABEL);
        judgingTabLayout->addWidget(judgingWaitTimeLabel);

        QLabel *judgingWaitTimeDesc = new QLabel();
        judgingWaitTimeDesc->setText("Thời gian tạm dừng giữa các lần quét bài làm của học sinh. Hữu dụng khi cần chấm bài trục tuyến mà cần giảm lượng CPU sử dụng cho quá trình chấm bài, tuy nhiên sẽ làm giảm tốc độ chấm bài. (giây)");
        judgingWaitTimeDesc->setWordWrap(true);
        judgingTabLayout->addWidget(judgingWaitTimeDesc);

        QDoubleValidator *judgingWaitTimeInputValidator = new QDoubleValidator(0.0, 100.0, 2); // 0 -> 100 with 2 decimal places
        judgingWaitTimeInput->setValidator(judgingWaitTimeInputValidator);
        judgingTabLayout->addWidget(judgingWaitTimeInput);
        // Connecting action of changing text to a function (Using Lambda)
        connect(judgingWaitTimeInput, &QLineEdit::textChanged, this, [this](const QString text) {
            onInputChanges("wait_time", text);
        });


        // RELOAD TIME
        QLabel *judgingReloadTimeLabel = new QLabel();
        judgingReloadTimeLabel->setText("Thời gian làm mới");
        judgingReloadTimeLabel->setStyleSheet(STYLE_BIGLABEL);
        judgingTabLayout->addWidget(judgingReloadTimeLabel);

        QLabel *judgingReloadTimeDesc = new QLabel();
        judgingReloadTimeDesc->setText("Thời gian làm mới máy ảo để kiểm tra bài làm học sinh. Máy ảo được tạo ra để đảm bảo sự an toàn cho những tác vụ chạy trên file bài làm của học sinh mà không cần mất đi tính năng nào như iofstream hay freopen. Thời gian làm mới là thời gian để máy ảo được làm mới nhằm đảm bảo sự an toàn của máy tính giáo viên và xóa những tác vụ đã xảy ra trước đó. (giây)");
        judgingReloadTimeDesc->setWordWrap(true);
        judgingTabLayout->addWidget(judgingReloadTimeDesc);

        QDoubleValidator *judgingReloadTimeInputValidator = new QDoubleValidator(0.0, 100.0, 2); // Same as the one in wait time
        judgingReloadTimeInput->setValidator(judgingReloadTimeInputValidator);
        judgingTabLayout->addWidget(judgingReloadTimeInput);
        // Connecting to a function with lambda
        connect(judgingReloadTimeInput, &QLineEdit::textChanged, this, [this](const QString text) {
            onInputChanges("reload_time", text);
        });


        // SHOW TESTS
        QLabel *judgingShowTestLabel = new QLabel();
        judgingShowTestLabel->setText("Hiện đáp án (test)");
        judgingShowTestLabel->setStyleSheet(STYLE_BIGLABEL);
        judgingTabLayout->addWidget(judgingShowTestLabel);

        QLabel *judgingShowTestDesc = new QLabel();
        judgingShowTestDesc->setText("Khi kiểm tra bài thi, kết quả kiểm tra sẽ được ghi ra một tệp kết quả cùng với thông tin chi tiết về quá trình kiểm tra. Cài đặt này sẽ hiện thông tin chi tiết như đầu vào, đầu ra chương trình và đầu ra đáp án.");
        judgingShowTestDesc->setWordWrap(true);
        judgingTabLayout->addWidget(judgingShowTestDesc);

        judgingTabLayout->addWidget(judgingShowTestCheckbox);
        // Connecting to a function w/ lambda (again)
        connect(judgingShowTestCheckbox, &QCheckBox::stateChanged, this, [this](const int state) {
            onInputChanges("show_test", QString::fromStdString(intToString(state)));
        });


        // Setting judging tab's layout
        judgingTabLayout->setAlignment(Qt::AlignTop);
        judgingTab->setLayout(judgingTabLayout);


        // +---------------+
        // | Webserver Tab |
        // +---------------+
        /*
         * Like with judging tab, this will have the following options:
         * - Maxmimum not logged in seconds
         * - Aliases:
         *   + Website name
         *   + Slogan
         *   + Host name
         */
        QVBoxLayout *webserverTabLayout = new QVBoxLayout();
        webserverTabLayout->setAlignment(Qt::AlignTop);

        // MAXIMUM NOT LOGGED IN SECONDS
        QLabel *webserverLogInSecsLabel = new QLabel();
        webserverLogInSecsLabel->setText("Thời gian không hoạt động tối đa");
        webserverLogInSecsLabel->setWordWrap(true);
        webserverLogInSecsLabel->setStyleSheet(STYLE_BIGLABEL);
        webserverTabLayout->addWidget(webserverLogInSecsLabel);

        QLabel *webserverLogInSecsDesc = new QLabel();
        webserverLogInSecsDesc->setText("Thời gian tối đa từ lần cuối người dùng hoạt động trên thiết bị của họ cho đến bây giờ. Tính năng này sẽ tự động xóa các dữ liệu thiết bị (không phải dữ liệu người dùng/học sinh) nhằm tiết kiệm bộ nhớ và tăng tốc các hành động. (giây)");
        webserverLogInSecsDesc->setWordWrap(true);
        webserverTabLayout->addWidget(webserverLogInSecsDesc);
        
        QIntValidator *webserverLogInSecsValidator = new QIntValidator(0, 10000000); // Equivalent of nearly 116 days of not logged in (or 1158 if you count the variations)
        webserverLogInSecsInput->setValidator(webserverLogInSecsValidator);
        webserverTabLayout->addWidget(webserverLogInSecsInput);

        // Connect the action of changing settings to a function through connect and lambda
        connect(webserverLogInSecsInput, &QLineEdit::textChanged, this, [this](const QString &value) {
            onInputChanges("maximum_login_seconds", value);
        });

        // ALIASES
        QLabel *webserverAliasLabel = new QLabel();
        webserverAliasLabel->setText("Thông tin trang web");
        webserverAliasLabel->setStyleSheet(STYLE_BIGLABEL);
        webserverTabLayout->addWidget(webserverAliasLabel);

        // Alias: Website name
        QLabel *webserverAliasWebnameLabel = new QLabel();
        webserverAliasWebnameLabel->setText("Tên Website (hoặc lớp học. VD: Trang web chấm bài tự động, lớp học chuyên tin thầy N...):");
        webserverAliasWebnameLabel->setWordWrap(true);
        webserverAliasWebnameLabel->setStyleSheet(STYLE_BOLDLABEL);
        webserverTabLayout->addWidget(webserverAliasWebnameLabel);

        webserverTabLayout->addWidget(webserverAliasWebnameInput);

        // Connecting to function
        connect(webserverAliasWebnameInput, &QLineEdit::textChanged, this, [this](const QString &value) {
            onInputChanges("website_name", value);
        });

        // Alias: Slogan
        QLabel *webserverAliasSloganLabel = new QLabel();
        webserverAliasSloganLabel->setText("Khẩu hiệu (VD: Càng học càng vui; keep typing, keep loving; Trên bước đường thành công không có dấu chân của kẻ thất bại...):");
        webserverAliasSloganLabel->setWordWrap(true);
        webserverAliasSloganLabel->setStyleSheet(STYLE_BOLDLABEL);
        webserverTabLayout->addWidget(webserverAliasSloganLabel);

        webserverTabLayout->addWidget(webserverAliasSloganInput);

        connect(webserverAliasSloganInput, &QLineEdit::textChanged, this, [this](const QString &value) {
            onInputChanges("slogan", value);
        });

        // Alias: Host name
        QLabel *webserverAliasHostnameLabel = new QLabel();
        webserverAliasHostnameLabel->setText("Tên chủ sở hữu (hoặc quản trị viên. VD: Nguyễn Văn A, Lê Văn B, Lê Quang N...):");
        webserverAliasHostnameLabel->setWordWrap(true);
        webserverAliasHostnameLabel->setStyleSheet(STYLE_BOLDLABEL);
        webserverTabLayout->addWidget(webserverAliasHostnameLabel);

        webserverTabLayout->addWidget(webserverAliasHostnameInput);

        connect(webserverAliasHostnameInput, &QLineEdit::textChanged, this, [this](const QString &value) {
            onInputChanges("hostname", value);
        });

        // Applying neccessary layout
        webserverTab->setLayout(webserverTabLayout);


        ///////////////////////
        // Finishing touches //
        ///////////////////////

        // Settings for container
        container->setLayout(mainLayout);

        // Settings for QMainWindow
        setCentralWidget(container);
    }

    void initialize() {
        loadThemeColors();
        judgingEnabled = false;
        webserverEnabled = false;
    }

    void about() {
        QWidget *aboutFrame = new QWidget();
        aboutFrame->setFixedSize(500, 250);
        aboutFrame->show();
        aboutFrame->setWindowTitle("Về ATOMIC");
        aboutFrame->setWindowIcon(QIcon(iconPixmap));
        aboutFrame->setObjectName("about");
        
        aboutFrame->setStyleSheet(styleSheetResult); // Setting styles

        // Reading version info
        std::fstream versionFile(dirPath + VERSION_PATH, std::ios::in);
        if (versionFile.is_open()) {
            // If file is working as intended
            try {
                version = json::parse(versionFile);

                std::string stableText = "";
                if (version["stable"]) {
                    stableText = "Yes";
                } else {
                    stableText = "No";
                }
                std::string versionText = "Codename " + std::string(version["version"]) + '\n' +
                                        "API Version: " + std::string(version["api"]) + '\n' +
                                        "Branch: " + std::string(version["branch"]) + '\n' +
                                        "Sig: " + std::string(version["signature"]) + '\n' +
                                        "Stable: " + stableText;

                std::cout << versionText << '\n';

                // The rest of the operation stays here
                QLabel *versionLabel = new QLabel();
                versionLabel->setText(QString::fromStdString(versionText));
                // versionLabel->setWordWrap(true);

                // Adding Image
                QLabel *appImage = new QLabel();
                QPixmap iconPixmapScaled = iconPixmap.scaled(QSize(75, 75), Qt::KeepAspectRatio);
                appImage->setPixmap(iconPixmapScaled);
                appImage->setAlignment(Qt::AlignRight);

                // Adding in splitter to split between a huge-sized version of 
                // iconPixmap and version info
                QSplitter *splitter = new QSplitter();
                splitter->setOrientation(Qt::Orientation::Horizontal);
                splitter->setChildrenCollapsible(false);
                splitter->addWidget(versionLabel);
                splitter->addWidget(appImage);

                // Setting width for easy splitting
                splitter->setFixedHeight(75);

                // This text is like this because of some shit.
                // It's just cause of C's poor strings...
                QString licensingText = "====== Giấy phép ======\n\nPhần mềm này được phát hành dưới Giấy phép Mã nguồn mở MIT (MIT License) đã qua sửa đổi. Thông tin chi tiết truy cập trang dự án GitHub.\n\n====== Ghi nhận ======\n\nPhần mềm được phát triển dựa trên hoặc sử dụng các dự án mã nguồn mở sau:\n\n- Qt GUI Framework (https://qt.io)  \n- Python Interpreter (https://python.org)  \n- waitress (https://github.com/Pylons/waitress)  \n- Docker (https://docker.com)  \n- nlohmann/json - JSON for Modern C++ (https://github.com/nlohmann/json)  \n- boppreh/keyboard - Python keyboard library (https://github.com/boppreh/keyboard)";

                QSplitter *licensingSplitter = new QSplitter();
                licensingSplitter->setOrientation(Qt::Orientation::Vertical);
                licensingSplitter->setChildrenCollapsible(false);
                licensingSplitter->addWidget(splitter);
                
                QTextEdit *licensingInfo = new QTextEdit();
                licensingInfo->setText(licensingText);
                licensingInfo->setReadOnly(true);
                licensingInfo->setObjectName("aboutLicensing");
                licensingInfo->setFont(monospaceFont);
                licensingSplitter->addWidget(licensingInfo);

                QVBoxLayout *layout = new QVBoxLayout();
                layout->addWidget(licensingSplitter);
                aboutFrame->setLayout(layout);
            } catch (const json::parse_error& e) { 
                // If error got and it is JSON parsing error
                errorDialog("Tệp phiên bản đã bị hỏng. Vui lòng cập nhật để tự động sửa lỗi hoặc cài đặt lại chương trình.");
                close();
                exit(0);
            }
        }
        else {
            errorDialog("Tệp thông tin phiên bản đã bị hỏng. Vui lòng cập nhật để tự động sửa lỗi hoặc cài đặt lại chương trình.");
            aboutFrame->close();
        }
    }

    void loadThemeColors() {
        std::fstream themeColorsFile(dirPath + THEMECOLORS_PATH, std::ios::in);

        // Default values
        COLOR_CONSOLE_ERROR = QColor(185, 0, 0);
        COLOR_CONSOLE_WARNING = QColor(185, 153, 0);
        COLOR_CONSOLE_OK = QColor(0, 185, 0);
        COLOR_CONSOLE_DEFAULT = QColor(0, 0, 0);
        FORMATBTN_DEFAULT = "#ffffff";
        FORMATBTN_SELECTED = "#9cc2ff";

        if (themeColorsFile.is_open()) {
            std::stringstream stream;
            stream << themeColorsFile.rdbuf();
            QString content = QString::fromStdString(stream.str());
            std::cout << content.toStdString() << '\n';

            // Now, we processes the whole thing first by removing spaces
            content = content.remove(" ", Qt::CaseSensitive);
            // Next split them into LINES
            QStringList lines = content.split('\n');

            // Finally, browse through each of them
            for (QString &line : lines) {
                if (line[0] == '#') continue;

                QStringList parts = line.split('=');
                QString name = parts[0];
                QString value = parts[1];

                if (name == "COLOR_CONSOLE_ERROR") {
                    QStringList rgb = value.split(',');
                    bool ok;
                    int valR = rgb[0].toInt(&ok);
                    int valG = rgb[1].toInt(&ok);
                    int valB = rgb[2].toInt(&ok);

                    if (ok) COLOR_CONSOLE_ERROR = QColor(valR, valG, valB);
                    else continue;
                } else if (name == "COLOR_CONSOLE_WARNING") {
                    QStringList rgb = value.split(',');
                    bool ok;
                    int valR = rgb[0].toInt(&ok);
                    int valG = rgb[1].toInt(&ok);
                    int valB = rgb[2].toInt(&ok);

                    if (ok) COLOR_CONSOLE_WARNING = QColor(valR, valG, valB);
                    else continue;
                } else if (name == "COLOR_CONSOLE_OK") {
                    QStringList rgb = value.split(',');
                    bool ok;
                    int valR = rgb[0].toInt(&ok);
                    int valG = rgb[1].toInt(&ok);
                    int valB = rgb[2].toInt(&ok);

                    if (ok) COLOR_CONSOLE_OK = QColor(valR, valG, valB);
                    else continue;
                } else if (name == "COLOR_CONSOLE_DEFAULT") {
                    QStringList rgb = value.split(',');
                    bool ok;
                    int valR = rgb[0].toInt(&ok);
                    int valG = rgb[1].toInt(&ok);
                    int valB = rgb[2].toInt(&ok);

                    if (ok) COLOR_CONSOLE_DEFAULT = QColor(valR, valG, valB);
                    else continue;
                } else if (name == "FORMATBTN_DEFAULT") {
                    FORMATBTN_DEFAULT = value;
                } else if (name == "FORMATBTN_SELECTED") {
                    FORMATBTN_SELECTED = value;
                } else {
                    std::cout << "[THEMECOLOR OPTION PARSER] Unknown parameter: " << name.toStdString() << '\n';
                }
            }
        }
    }

    void gitHub() {
        // Executing the github repo into Windows
        ShellExecuteA(0, "open", GITHUB_PAGE.c_str(), 0, 0, SW_SHOWNORMAL); // Using the ANSI version and not Unicode
    }

    void openHttpWebsite() {
        // Opening the opened HTTP website
        ShellExecuteA(0, "open", "http://127.0.0.1/", 0, 0, SW_SHOWNORMAL);
    }

    void backUp() {

    }

    void loadBackUp() {

    }

    private: // PRIVATE FUNCTIONS. These cannot be connected to outside of whatever this object is.

    // -------------------------------------------------
    // Purpose: [GROUPED] (onSidebarFeatureButtonPushed, 
    //          readyJudgingOutput, stoppedJudging)
    //          For process-related functionalities
    // -------------------------------------------------
    void onSidebarFeatureButtonPushed(std::string type) {
        // When the sidebar's button is pushed on
        // Which are all features toggler

        if (type == "judging") {
            // If the button pushed is a judging button
            // => Enable the judging feature / system

            // Changing status
            judgingEnabled = !judgingEnabled;
            std::cout << "Current JUDGING status: " << (judgingEnabled ? "Enabled" : "Disabled") << "\n";
            
            if (judgingEnabled) {
                // If JUDGING is now enabled, performs the enable action

                judgingProcess->start();
                std::cout << "Started JUDGING process: " << judgingProcess->program().toStdString() << " " << judgingProcess->arguments().join(' ').toStdString() << '\n';

                if (judgingProcess->state() == QProcess::Running) {
                    // If judgingProcess ran without errors
                    std::cout << "Successfully ran JUDGING process with PID " << judgingProcess->processId() << "\n";
                    judgingProcessButton->setText("Dừng chấm bài");
                } else {
                    std::cout << "Process failed to start. Error: " << judgingProcess->error() << '\n'; 
                    errorDialog("Thất bại trong quá trình mở hệ thống chấm bài. Hãy thử cài đặt lại chương trình hoặc cập nhật lên bản cập nhật mới nhất");
                }
            }
            else {
                // Turn off JUDGING process via created socket HOLE
                SOCKET exitSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (exitSocket == INVALID_SOCKET) {
                    std::cout << "exitSocket: INVALID_SOCKET\n";
                    errorDialog("Lỗi tạo dựng SOCKET. Chương trình sẽ thoát và hãy mở lại chương trình. Thiết bị của bạn có thể không tương thích với ATOMIC.");
                    WSACleanup();
                    close();
                    // judgingProcess->terminate();
                    exit(0);
                }

                // Marking addresses for impending connection
                sockaddr_in targetAddr;
                targetAddr.sin_family = AF_INET;
                targetAddr.sin_port = htons(JUDGING_EXITPORT);
                targetAddr.sin_addr.s_addr = inet_addr(JUDGING_EXITADDR.c_str());

                // Connecting to target with Winsock's connect
                if (::connect(exitSocket, (SOCKADDR*)&targetAddr, sizeof(targetAddr)) == SOCKET_ERROR) {
                    // Connection failure:
                    // Connect to target returned SOCKET_ERROR

                    errorDialog("Kết nối đến hệ thống chấm bài không thành công. Vui lòng đợi một chút và thử lại sau. Nếu vẫn không thành công thì hãy thử khởi động lại hệ thống.");
                    closesocket(exitSocket);
                }
                else {
                    const char* exitMsg = "exit";
                    send(exitSocket, exitMsg, strlen(exitMsg), 0);

                    // No need for recieving

                    // Now, we have to set the button disabled because the user might just spam it up so...
                    judgingProcessButton->setEnabled(false);
                }
            }
        } else if (type == "webserver") {
            webserverEnabled = !webserverEnabled;
            std::cout << "Toggled webserver to: " << (webserverEnabled ? "Enabled" : "Disabled") << '\n';
            if (webserverEnabled) {
                // If we just turned on webserver. We do the actions of turning it up
                webserverProcess->start();
                std::cout << "Started WEBSERVER process: " << webserverProcess->program().toStdString() << " " << webserverProcess->arguments().join(' ').toStdString() << '\n';

                // Check if the process started normally
                if (webserverProcess->state() == QProcess::Running) {
                    // If webserverProcess ran without errors
                    std::cout << "Successfully ran WEBSERVER process with PID " << webserverProcess->processId() << "\n";
                    webserverProcessButton->setText("Tắt Website");
                } else {
                    std::cout << "Process failed to start. Error: " << webserverProcess->error() << '\n'; 
                    errorDialog("Thất bại trong quá trình mở hệ thống chấm bài. Hãy thử cài đặt lại chương trình hoặc cập nhật lên bản cập nhật mới nhất");
                }
            } else {
                // Turning off actions
                // Turn off JUDGING process via created socket HOLE
                SOCKET exitSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (exitSocket == INVALID_SOCKET) {
                    std::cout << "exitSocket: INVALID_SOCKET\n";
                    errorDialog("Lỗi tạo dựng SOCKET. Chương trình sẽ thoát và hãy mở lại chương trình. Thiết bị của bạn có thể không tương thích với ATOMIC.");
                    WSACleanup();
                    close();
                    // judgingProcess->terminate();
                    exit(0);
                }

                // Marking addresses for impending connection
                sockaddr_in targetAddr;
                targetAddr.sin_family = AF_INET;
                targetAddr.sin_port = htons(WEBSERVER_EXITPORT);
                targetAddr.sin_addr.s_addr = inet_addr(WEBSERVER_EXITADDR.c_str());

                // Connecting to target with Winsock's connect
                if (::connect(exitSocket, (SOCKADDR*)&targetAddr, sizeof(targetAddr)) == SOCKET_ERROR) {
                    // Connection failure:
                    // Connect to target returned SOCKET_ERROR

                    errorDialog("Kết nối đến hệ thống website không thành công. Vui lòng đợi một chút và thử lại sau. Nếu vẫn không thành công thì hãy thử khởi động lại hệ thống.");
                    closesocket(exitSocket);
                }
                else {
                    const char* exitMsg = "exit";
                    send(exitSocket, exitMsg, strlen(exitMsg), 0);

                    // No need for recieving

                    // Now, we have to set the button disabled because the user might just spam it up so...
                    webserverProcessButton->setEnabled(false);
                }
            }
        }
    }

    // Add these two methods:
    void judgingHandleStandardOutput() {
        QString output = QString::fromUtf8(judgingProcess->readAllStandardOutput());
        judgingProcessOutput(output);
    }

    void judgingHandleStandardError() {
        QString output = QString::fromUtf8(judgingProcess->readAllStandardError());
        judgingProcessOutput(output);
    }

    // Move your processing function to its own method:
    void judgingProcessOutput(const QString& source) {
        QStringList lines = source.split('\n'); // Shit is plain ol' magic

        for (const QString& originalLine : lines) {
            // Remove trailing '\r' while preserving Unicode characters
            QString line = originalLine;
            line.remove('\r');

            if (line.isEmpty())
                continue;

            // Check for starting ANSI escape code
            if (!line.isEmpty() && line[0] == QChar('\x1b')) {
                QString colorCode;
                int pos = 0;

                // Extract ANSI escape sequence
                while (pos < line.length() && line[pos] != QChar('m')) {
                    colorCode += line[pos];
                    pos++;
                }

                if (pos < line.length() && line[pos] == QChar('m')) {
                    colorCode += 'm';
                    pos++;
                }

                line = line.mid(pos); // Remove ANSI code prefix

                // Set text color based on ANSI code
                if (colorCode == QString::fromStdString(INFO_COL))
                    judgingProcessConsole->setTextColor(COLOR_CONSOLE_DEFAULT);
                else if (colorCode == QString::fromStdString(ERROR_COL))
                    judgingProcessConsole->setTextColor(COLOR_CONSOLE_ERROR);
                else if (colorCode == QString::fromStdString(WARN_COL))
                    judgingProcessConsole->setTextColor(COLOR_CONSOLE_WARNING);
                else if (colorCode == QString::fromStdString(OK_COL))
                    judgingProcessConsole->setTextColor(COLOR_CONSOLE_OK);
            }

            // Remove any ending ANSI color code
            int ansiPos = line.indexOf(QChar('\x1b'));
            if (ansiPos != -1) {
                line = line.left(ansiPos);
            }

            // Append line and reset color
            judgingProcessConsole->append(line);
            judgingProcessConsole->setTextColor(COLOR_CONSOLE_DEFAULT);
        }
    }

    void stoppedJudging(int exitCode, QProcess::ExitStatus exitStatus) {
        judgingProcessButton->setEnabled(true);
        judgingProcessButton->setText("Bật chấm bài");
        judgingEnabled = false;

        // RESET some stuffs
        judgingProcessConsole->setTextColor(COLOR_CONSOLE_DEFAULT);
        judgingProcessConsole->append("\n\n");

        std::cout << "Exitted with exit code: " << exitCode << '\n';

        // Exit code handling
        if (exitCode != 0) {
            // Preparing variables as default values
            QString windowTitle = "Lỗi trong quá trình chấm bài.";
            QString message = "Đã có lỗi xảy ra trong quá trình chấm bài. Hãy tra trong Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết. Mã lỗi: ";
            message += QString::fromStdString(intToString(exitCode));

            // Use switch/case for better handling of these jobs and more optimized
            // IF/ELSEs
            switch(exitCode) {
                // Error dictionary in `judge.py`'s first lines
                case -127:
                    // Docker environment error
                    message = "Lỗi -127: Thất bại trong quá trình kết nối đến môi trường Docker. Hãy chắc chắn rằng bạn đã mở Docker Desktop và bật Docker Environment trên thiết bị. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -126:
                    // Docker connection error
                    message = "Lỗi -126: Môi trường Docker không phản hồi. Xin đợi và thử lại sau. Nếu không thành công, hãy thử khởi động lại Docker hoặc tra trong Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -125:
                    // Docker compilation image building error
                    message = "Lỗi -125: Ảnh biên soạn không được xây dựng thành công. Hãy thử khởi động lại hệ thống chấm bài. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -124:
                    // Docker compile container failed to start
                    message = "Lỗi -124: Container biên soạn không được bật thành công. Hãy thử khởi động lại hệ thống chấm bài. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -123:
                    // Judging disabling socket failed to start
                    message = "Lỗi -123: Socket không được bật thành công. Hãy kiểm tra xem có phần mềm nào đang sử dụng cổng ";
                    message += QString::fromStdString(intToString(JUDGING_EXITPORT));
                    message += " hay không. Nếu có thì hãy tắt phần mềm đó. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -122:
                    // Execution image building error
                    message = "Lỗi -122: Ảnh thực hiện không được xây dựng thành công. Hãy thử khởi động lại hệ thống chấm bài. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
                case -121:
                    // Docker compile container failed to start
                    message = "Lỗi -121: Container thực hiện không được bật thành công. Hãy thử khởi động lại hệ thống chấm bài. Tra Trợ Giúp/Hướng dẫn để biết thêm thông tin chi tiết.";
                    break;
            }

            // Showing the error
            QMessageBox *msgBox = new QMessageBox();
            msgBox->setText(message);
            msgBox->setWindowTitle(windowTitle);
            msgBox->setIcon(QMessageBox::Critical);
            msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
            // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
            msgBox->setWindowIcon(QIcon(iconPixmap));
            
            msgBox->setStyleSheet(styleSheetResult);
            msgBox->show();
        } else {
            // If the software exitted gracefully, we do a little bit of windowin'
            // by simply showing a response to assure the user nothing is BAD
            QMessageBox *msgBox = new QMessageBox();
            msgBox->setText("Dừng hệ thống chấm bài thành công!");
            msgBox->setWindowTitle("Thành công");
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
            // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
            msgBox->setWindowIcon(QIcon(iconPixmap));

            msgBox->setStyleSheet(styleSheetResult);
            msgBox->show();
        }
    }

    void stoppedWebserver(int exitCode, QProcess::ExitStatus exitStatus) {
        // Handle webserver exitting, along with creating log files.
        webserverProcessButton->setText("Bật Website");
        webserverProcessButton->setEnabled(true);
        webserverEnabled = false;

        if (exitCode != 0) {
            // Error happened during execution
            QMessageBox::StandardButton reply;

            reply = QMessageBox::critical(this, "Lỗi trong web chấm bài trực tuyến", 
                                  "Đã có lỗi xảy ra trong khi đang mở website chấm bài trực tuyến. Mở nhật ký đầu ra chương trình (Tiếng Anh, Nâng cao)?",
                                  QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                // Open logfile
                std::cout << "Opened Webserver's log file.\n";
                ShellExecuteA(0, "open", (dirPath + LOG_PATH).c_str(), 0, 0, SW_SHOWNORMAL);
            }
        } else {
            // Alright its fine
            // QMessageBox::information(this, "Đã tắt trang web chấm bài trực tuyến");
        }
    }

    // --------------------------------------------------
    // Purpose: To refresh the classlist in the dropdown
    // -------------------------------------------------
    void refreshClassDropdown() {
        // classDropdown->addItem("Wassup");
        // classDropdown->addItem("Boys");

        // Getting amount of items in dropdown
        int amount = classDropdown->count();

        // 2 possibilities
        if (amount == 0) { // In case the dropdown has not been loaded
            std::cout << "[classDropdown] Dropdown EMPTY\n";
            for (auto& item: classes.items()) { // Iterate through EACH element
                classDropdown->addItem(QString::fromStdString(item.key()));
            }
        } else { // In case the dropdown still has something
            std::cout << "[classDropdown] Dropdown NOT EMPTY\n";
            
            const std::string oldCurrentValue = classDropdown->currentText().toStdString(); // For the sake of keeping the selection correct
            classDropdown->clear();

            bool currentTextStillExists = false;
            for (auto& item: classes.items()) { // Iterate through EACH element
                classDropdown->addItem(QString::fromStdString(item.key()));

                // Check if the current text still exists in the new texts
                if (oldCurrentValue == item.key()) currentTextStillExists = true;
            }

            if (currentTextStillExists)
                classDropdown->setCurrentText(QString::fromStdString(oldCurrentValue));
        }
    }

    // ----------------------------------------------------------------
    // Purpose: To display the information of a particular submission.
    // ----------------------------------------------------------------
    void displaySubmissionInfo(std::string username, std::string contestName) {
        // Prepare the name for future use
        const std::string filenameNoEXT = std::string("[") + username + std::string("][") + contestName + std::string("]");

        // Check if a log exists while also accounting for multiple log files OF multiple EXTENSIONS
        bool doesExist = false;
        std::vector<std::string> extensionsFound;
        for (std::string extension : supportedExtensions) {
            if (std::filesystem::exists(dirPath + SUBMITLOG_DIR + filenameNoEXT + extension + logExt)) {
                extensionsFound.push_back(extension);
                doesExist = true;
            }
        }

        if (!doesExist) {
            errorDialog("Người dùng không có tệp thông tin bài làm của bài thi này.");
            return; // Halt right away if continuing will only become a nuisance
        }

        // Use the same method as the replaceTestToQueue one when there are multiple
        // extensions' log file.
        std::string selectedExtension;
        if (extensionsFound.size() > 1) { // More than 1 language submitted that has generated an information file
            QStringList languages;
            for (std::string extension: extensionsFound) {
                languages << QString::fromStdString(extension);
            }

            std::cout << "[replaceTestIntoQueue()] Found more than 1 extension. Opening dialog...\n";

            CST_RadioButtonDialog *dialog = new CST_RadioButtonDialog(
                this, "Đuôi tệp", 
                QString::fromStdString(contestName + std::string(": Tìm thấy nhiều đuôi đã được nộp. Hãy chọn đuôi cần biết thông tin.")), languages
            );

            if (dialog->exec() == QDialog::Accepted) {
                selectedExtension = dialog->selectedOption().toStdString();
                std::cout << "Selected " << selectedExtension << '\n';
            }
        } else {
            // Assumes that the first and only language is the one the user would want to check out
            selectedExtension = extensionsFound[0];
        }

        // Now, we just run up the text editor?
        CST_TextEditorDialog *diag = new CST_TextEditorDialog(
            this, QString::fromStdString(filenameNoEXT),
            dirPath + SUBMITLOG_DIR + filenameNoEXT + selectedExtension + logExt,
            true
        );

        diag->setText("Thông tin bài làm học sinh:");
        diag->show();
    }

    // ------------------------------------------------------------
    // Purpose: To move submissions back into queue for retesting.
    // ------------------------------------------------------------
    void replaceTestToQueue(std::string username, std::string contestName, bool allTest) {
        if (allTest) { // In case we have to retestALL tests, use recursive
            // Get all the tests with the column's display
            int cAmount = currentTable->columnCount();
            for (int i = 0; i < cAmount; i++) {
                QString text = currentTable->horizontalHeaderItem(i)->text();
                replaceTestToQueue(username, text.toStdString(), false);
            }
        } else {
            // First, check if the file exists (the user's submission has already been tested once before)
            // We check for EACH extension
            bool doesExist = false;
            std::vector<std::string> extensionsFound;
            for (std::string extension : supportedExtensions) {
                if (std::filesystem::exists(dirPath + USERSUBHISTORY_DIR + username + std::string("/") + contestName + extension)) {
                    extensionsFound.push_back(extension);
                    doesExist = true;
                }
            }

            if (!doesExist) { // If the file does not exists
                // errorDialog("Người dùng không có bài làm nào đã chấm thuộc về bài thi này.");
                return; // HALT immediately
            }

            std::string selectedExtension;
            if (extensionsFound.size() > 1) { // More than 1 language submitted???
                QStringList languages;
                for (std::string extension: extensionsFound) {
                    languages << QString::fromStdString(extension);
                }

                std::cout << "[replaceTestIntoQueue()] Found more than 1 extension. Opening dialog...\n";
                CST_RadioButtonDialog *dialog = new CST_RadioButtonDialog(this, "Đuôi tệp", QString::fromStdString(contestName + std::string(": Hãy chọn đuôi cần chấm lại.")), languages);
                if (dialog->exec() == QDialog::Accepted) {
                    selectedExtension = dialog->selectedOption().toStdString();
                    std::cout << "Selected " << selectedExtension << '\n';
                }

            } else { // When there is only 1 language
                selectedExtension = extensionsFound[0];
            }

            // Prepare the name for future use
            const std::string filename = std::string("[") + username + std::string("][") + contestName + std::string("]") + selectedExtension;

            // Check if a submission awaiting testing exists for this same user
            if (std::filesystem::exists(dirPath + USERQUEUE_DIR + filename)) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Xác nhận vị trí", 
                    "Hiện đang có một bài nộp của bài thi này và của học sinh này chưa được chấm. Bạn có chắc muốn tiếp tục (ghi đè)?",
                    QMessageBox::Yes | QMessageBox::No);
                
                if (reply == QMessageBox::No) { // If user doesn't want to continue
                    return;
                }
            }

            // Actually doing the job
            std::fstream queueFile(dirPath + USERQUEUE_DIR + filename, std::ios::out | std::ios::trunc); // Truncates the file if already exists
            std::fstream orgFile(dirPath + USERSUBHISTORY_DIR + username + std::string("/") + contestName + selectedExtension, std::ios::in);

            if (queueFile.is_open()) {
                if (orgFile.is_open()) {
                    // Passed all loophole tests
                    // We do the job
                    queueFile << orgFile.rdbuf();
                } else {
                    errorDialog("Không thể mở tệp bài nộp (đã chấm) hiện tại của học sinh.");
                }
            } else {
                errorDialog("Thất bại trong quá trình tạo/chỉnh sửa tệp. Có lẽ tệp đang được sử dụng bởi một ứng dụng khác?");
            }

            queueFile.close();
            orgFile.close();
        }
    }

    // -----------------------------------------------------------------------------
    // Purpose: To show a custom context menu for each user's result score in order
    //          to help the user interface more intuitive
    // -----------------------------------------------------------------------------
    void showScoreContextMenu(const QPoint &pos) {
        QTableWidgetItem *item = currentTable->itemAt(pos);
        if (!item) return; // Only execute if the cell was clicked on

        QMenu contextMenu(this); // The context menu in which the options are built upon

        QAction *refresh = contextMenu.addAction("Làm mới");

        QAction *reTest = contextMenu.addAction("Chấm lại");
        
        // Getting information before checking anything
        QString username = currentTable->verticalHeaderItem(item->row())->text();
        if (item->column() == 0) {
            // So user wants to do this for ALL submissions
            QAction *selectedAction = contextMenu.exec(currentTable->viewport()->mapToGlobal(pos));

            if (selectedAction == reTest) {
                replaceTestToQueue(username.toStdString(), "", true);
                std::cout << "[showScoreContextMenu()] " << username.toStdString() << ": All test replaced into queue.\n";
            } else if (selectedAction == refresh) {
                refreshClassDropdown();
                refreshTable();
            }
        } else {
            QAction *showInfo = contextMenu.addAction("Thông tin bài làm");
            QAction *selectedAction = contextMenu.exec(currentTable->viewport()->mapToGlobal(pos));
            
            QString contestName = currentTable->horizontalHeaderItem(item->column())->text();
            if (selectedAction == reTest) {
                std::cout << "[showScoreContextMenu()] " << username.toStdString() << ": Test " << contestName.toStdString() << " replaced into queue.\n";
                replaceTestToQueue(username.toStdString(), contestName.toStdString(), false);
            } else if (selectedAction == showInfo) {
                std::cout << "[showScoreContextMenu()] " << username.toStdString() << ": Test " << contestName.toStdString() << " infomation shown.\n";
                displaySubmissionInfo(username.toStdString(), contestName.toStdString());
            } else if (selectedAction == refresh) {
                refreshClassDropdown();  // No need for refreshTable as this already generates a signal that will call
                                         // refreshTable
                // refreshTable();
            }
        }
    }

    // --------------------------------------------------------------------------
    // Purpose: To refresh the table to account for changes in the whole result. 
    //          Probably needs to be better made but I am running out of time.
    // --------------------------------------------------------------------------
    void refreshTable() {
        // To refresh the table for the results in the management tab
        // To achieve this. We have to clear the table first
        currentTable->clear();

        // Get the current class
        std::string currentClass = classDropdown->currentText().toStdString();

        // Okay. Fine. Let's finally get a vector<std::string> of contests
        std::vector<std::string> currentContests;

        for (const auto& item: contests.items()) {
            // std::cout << item.value() << '\n';  // Used for debugging only

            // Now: Read through and process one by one if the class we need and the class its
            // valid for is in the same place
            bool classExists = false;
            for (const std::string classValue: item.value()["Classes"]) {
                // std::cout << classValue << '\n'; // Debugging

                // So... it seems like the classes are in fact, valid in this way.
                // So we compare with the current class
                if (classValue == currentClass) {
                    classExists = true;
                    break;
                }
            }

            // If the class does exists in the list of valid classes of the contest we are pointing to
            // then we add it into the vector created previously
            if (classExists) currentContests.push_back(item.key());
        }

        // With the current contests of this tab processed through. We prepare the space needed for them
        // to occupy (set into). Also accounting the collumn for the names and the sum of points too
        currentTable->setColumnCount(currentContests.size() + 1);

        QTableWidgetItem *columnHeaderSumOfPointsText = new QTableWidgetItem();
        columnHeaderSumOfPointsText->setFlags(columnHeaderSumOfPointsText->flags() & ~Qt::ItemFlag::ItemIsEditable);
        columnHeaderSumOfPointsText->setText("Tổng");
        currentTable->setHorizontalHeaderItem(0, columnHeaderSumOfPointsText);

        // With the header for the users column done. We add in the columns for contests
        int index = 1;
        for (const std::string& contest : currentContests) {
            // For each contest we has for this class

            QTableWidgetItem *columnHeaderContestText = new QTableWidgetItem();
            columnHeaderContestText->setFlags(columnHeaderContestText->flags() & ~Qt::ItemFlag::ItemIsEditable);
            columnHeaderContestText->setText(QString::fromStdString(contest));
            currentTable->setHorizontalHeaderItem(index, columnHeaderContestText);
            index++;
        }

        // Now. We move onto gathering the required informations for the users
        std::vector<std::string> usersList;

        for (const auto& item : users.items()) {
            // Automatically account for difference in classes
            // usersList.push_back(item.key());
            bool classExists = false;

            for (const std::string cls : item.value()["class"]) {
                // Just like the contest purification.
                // We find the users with matching classes

                if (cls == currentClass) {
                    classExists = true;
                    break;
                }
            }

            if (classExists) {
                usersList.push_back(item.key());
                // std::cout << "[refreshTable() / DEBUGGING #182] " << item.key() << '\n';  // Should not exists to reserve I/O speed
            }
            
        }

        // Variable for contests' points
        // contestsPointsByOrder[username] -> {int contest1Point, int contest2Point...}
        // Order will go with the order of currentContests (std::vector<>)
        std::unordered_map<std::string, std::vector<int>> contestsPointsByOrder;
        std::vector<std::pair<int, std::string>> usersSums; // More sorting friendly

        // We go through EACH user
        for (const std::string user : usersList) {
            // Get the contests of EACH user (in JSON format)
            std::fstream userSubmitResultFile(dirPath + USERSTATS_DIR + user + std::string(".json"), std::ios::in);
            if (userSubmitResultFile.is_open()) {
                try {
                    const json submissions = json::parse(userSubmitResultFile);

                    // Success? Nice. Now we just need to do some checking through.
                    // Now we read the submissions relative to the contests
                    int sum = 0;
                    for (std::string contest : currentContests) {
                        // Okay. Maybe also calculate the sums along the way?
                        if (submissions.contains(contest)) {
                            const int relativePoints = submissions[contest];
                            sum += relativePoints;

                            // Now, we add the constest points
                            contestsPointsByOrder[user].push_back(relativePoints);
                        } else {
                            contestsPointsByOrder[user].push_back(NONE_PLACEHOLDER);
                        }
                    }

                    // Adding the user's sum into the destination vector
                    // to prepare for the sorting later on
                    usersSums.push_back({sum, user});
                    // std::cout << "[refreshTable() / DEBUGGING #412] " << user << " -> " << sum << "\n";  // Another I/O waste
                } catch (const json::parse_error& e) {
                    // If error caught. We just delete that file.
                    remove((dirPath + USERSTATS_DIR + user + std::string(".json")).c_str());

                    // Fill the value with NOTHING? Let's just put LOWEST INT VALUE as a placeholder
                    contestsPointsByOrder[user].push_back(NONE_PLACEHOLDER);

                    // Adding user's sum in for sorting
                    usersSums.push_back({0, user});
                }
            } else {
                // Fill the value with NOTHING? Let's just put LOWEST INT VALUE as a placeholder
                contestsPointsByOrder[user].push_back(NONE_PLACEHOLDER);

                // Adding user's sum in for sorting
                usersSums.push_back({0, user});
            }
        }

        sort(usersSums.begin(), usersSums.end(), [](const auto &a, const auto &b) { // Descending
            return a.first > b.first;
        });

        // Now? Sink it all in. Because... We are gonna INITIATE THIS SHIT
        // Prepare room for user values to sit in (putting those values into a table)
        currentTable->setRowCount(usersSums.size());

        // Add the users in one by one
        index = 0; // Reuse the variable
        for (const auto &userSorterValue : usersSums) {
            // Go through usersSums as we already sorted
            QTableWidgetItem *usernameItem = new QTableWidgetItem();
            usernameItem->setFlags(usernameItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
            usernameItem->setText(QString::fromStdString(userSorterValue.second)); // Second is name, actually.
            currentTable->setVerticalHeaderItem(index, usernameItem);

            // Set the sum amount of points (0 is hardcoded cus of the position of the sum column)
            QTableWidgetItem *pointsItem = new QTableWidgetItem();
            pointsItem->setFlags(pointsItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
            pointsItem->setText(QString::fromStdString(intToString(userSorterValue.first)));  // First is the actual index value
            currentTable->setItem(index, 0, pointsItem);

            // With this done, let's move onto the final nail - the contest points
            if (contestsPointsByOrder[userSorterValue.second].size() == 1 && contestsPointsByOrder[userSorterValue.second][0] == NONE_PLACEHOLDER) {
                // In case this contest does NOT exist (the user never done the contest)
                const int& amountOfDestBlankCells = currentTable->columnCount();

                for (int i = 0; i < amountOfDestBlankCells; i++) {
                    QTableWidgetItem *blankItem = new QTableWidgetItem();
                    blankItem->setFlags(blankItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
                    currentTable->setItem(index, i, blankItem);
                }
            } else {
                int cindex = 1; // Index of column
                for (const int &point : contestsPointsByOrder[userSorterValue.second]) {
                    // Yeah it looks like I cant reuse any QTableWidgetItem, actually.
                    QTableWidgetItem *contestPointsItem = new QTableWidgetItem();
                    contestPointsItem->setFlags(contestPointsItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
                    contestPointsItem->setText(QString::fromStdString(intToString(point)));
                    currentTable->setItem(index, cindex, contestPointsItem);

                    // Prepare for next loop
                    cindex++;
                }
            }

            // Prepare for next loop
            index++;
        }

        std::cout << "[currentTable] Table refreshed!\n";
    }

    // -----------------------------------------------------------------
    // Purpose: Showing the neccessary subwindow for EACH option on the
    //          weird bar above the table in the management tab
    // -----------------------------------------------------------------
    void showButtonInfoFromBarType(std::string type) {
        if (type == "contests") {
            WIN_ContestsSettings *cstWin = new WIN_ContestsSettings(this);
            cstWin->show();
        }
    }

    // ------------------------------------------------
    // Purpose: Refresh current configurations on each
    //          tab switch to update contents and also
    //          update variables.
    // ------------------------------------------------
    void onTabSwitches(int index) {
        // When user switches tabs. It's good to update all contents of ALL TABS.
        // This is just for the sake of accuracy. Future optimizations might
        // reduce the stress on refreshing.
        std::cout << "onTabSwitches(): Called. currentSwitched.\n";

        // Reading data into variables related to them
        std::fstream settingsFile(dirPath + SETTINGS_PATH, std::ios::in);
        if (settingsFile.is_open()) {
            // If file is open-able (Works as intended)
            try {
                settings = json::parse(settingsFile);

                // If file has successfully been parsed
                // std::cout << "settings[\"wait_time\"] = " << settings["wait_time"] << '\n';
                std::cout << "[JSON: settings] " << settings << '\n';

                // Applying current settings from fcking JSON into Qt Line Input elements
                judgingWaitTimeInput->setText(QString::fromStdString(doubleToString(settings["wait_time"])));
                judgingReloadTimeInput->setText(QString::fromStdString(doubleToString(settings["reload_time"])));
                judgingShowTestCheckbox->setCheckState(settings["show_test"] ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

                webserverLogInSecsInput->setText(QString::fromStdString(intToString(settings["max_not_logged_in_session_seconds"])));

            } catch (const json::parse_error& e) { 
                // If error got and it is JSON parsing error
                errorDialog("Tệp cài đặt đã bị hỏng. Hãy cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
            settingsFile.close();
        } else {
            errorDialog("Tệp cài đặt không tồn tại. Hãy cài đặt lại ứng dụng để sửa lỗi.");
            close();
            exit(0);
        }

        std::fstream aliasFile(dirPath + ALIAS_PATH, std::ios::in);
        if (aliasFile.is_open()) {
            try {
                aliases = json::parse(aliasFile);

                // If sucessfully parsed
                std::cout << "[JSON: aliases] " <<  aliases << '\n';

                std::string webname = aliases["website_name"];
                webserverAliasWebnameInput->setText(QString::fromUtf8(webname.c_str()));

                std::string slogan = aliases["slogan"];
                webserverAliasSloganInput->setText(QString::fromUtf8(slogan.c_str()));

                std::string hostname = aliases["hostname"];
                webserverAliasHostnameInput->setText(QString::fromUtf8(hostname.c_str()));

                aliasFile.close();
            } catch (const json::parse_error& e) { 
                // If error got and it is JSON parsing error
                errorDialog("Tệp dữ liệu hiển thị đã bị hỏng. Hãy cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
        } else {
            errorDialog("Tệp dữ liệu hiển thị không tồn tại. Hãy cài đặt lại ứng dụng để sửa lỗi.");
            close();
            exit(0);
        }

        std::fstream classesFile(dirPath + CLASSES_PATH, std::ios::in);
        if (classesFile.is_open()) {
            try {
                classes = json::parse(classesFile);

                // Sucessfully parsed?
                std::cout << "[JSON: classes] " <<  classes << '\n';
            } catch (const json::parse_error& e) {
                // If parsing the JSON returned to be a failure
                errorDialog("Tệp dữ liệu lớp học đã bị hỏng. Vui lòng cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }

            classesFile.close();
        } else {
            errorDialog("Tệp dữ liệu lớp học không tồn tại.");
            
            // Creating file
            std::fstream file(dirPath + CLASSES_PATH, std::ios::out | std::ios::trunc);

            if (!file.is_open()) {
                errorDialog("Tệp dữ liệu lớp học không thể được tạo. Sửa chữa không thành công. Vui lòng cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
            file << "{}";
            std::cout << "Repair successful\n";
            file.close();

            onTabSwitches(0); // Call it up!!! BRING IT ONN
        }

        std::fstream contestsFile(dirPath + CONTESTS_PATH, std::ios::in);
        if (contestsFile.is_open()) {
            try {
                contests = json::parse(contestsFile);

                // Successfully parsed. Prints logging
                std::cout << "[JSON: contests] " <<  contests << '\n';

                contestsFile.close();
            } catch (const json::parse_error& e) {
                errorDialog("Tệp dữ liệu bài thi đã bị hỏng. Vui lòng cài đặt lại ứng dụng để sửa lỗi");
                close();
                exit(0);
            }
        } else {
            errorDialog("Tệp dữ liệu bài thi không tồn tại");

            // Creating file
            std::fstream file(dirPath + CONTESTS_PATH, std::ios::out | std::ios::trunc);

            if (!file.is_open()) {
                errorDialog("Tệp dữ liệu bài thi không thể được tạo. Sửa chữa không thành công. Vui lòng cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
            file << "{}";
            std::cout << "Repair successful\n";
            file.close();

            onTabSwitches(0); // Call it up!!! BRING IT ONN
        }

        std::fstream usersFile(dirPath + USERDATA_PATH, std::ios::in);
        if (usersFile.is_open()) {
            try {
                users = json::parse(usersFile);

                // Successfully parsed w/o any error whatsoever
                std::cout << "[JSON: users] " << users << '\n';

                usersFile.close();
            } catch (const json::parse_error& e) {
                errorDialog("Tệp dữ liệu người dùng đã bị hỏng. Vui lòng cài đặt lại ứng dụng để sửa lỗi");
                close();
                exit(0);
            }
        } else {
            errorDialog("Tệp dữ liệu người dùng không tồn tại");

            // Creating file
            std::fstream file(dirPath + USERDATA_PATH, std::ios::out | std::ios::trunc);

            if (!file.is_open()) {
                errorDialog("Tệp dữ liệu người dùng không thể được tạo. Sửa chữa không thành công. Vui lòng cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
            file << "{}";
            std::cout << "Repair successful\n";
            file.close();

            onTabSwitches(0); // Call it up!!! BRING IT ONN
        }

        refreshClassDropdown();
        refreshTable();
    }

    // -------------------------------------------------------------------------
    // Purpose: Showing errors faster than having to type an actual long command
    // -------------------------------------------------------------------------
    void errorDialog(std::string error) {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText(QString::fromStdString("Đã có lỗi xảy ra: " + error));
        msgBox->setWindowTitle("Lỗi");
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStandardButtons(QMessageBox::StandardButton::Ok);
        // msgBox->addButton("OK", QMessageBox::ButtonRole::AcceptRole);
        msgBox->setWindowIcon(QIcon(iconPixmap));
        msgBox->setStyleSheet(styleSheetResult);

        msgBox->show();
    }

    // -------------------------------------------
    // Purpose: Change settings for inputs from a
    //          predetermined 'type' value
    // -------------------------------------------
    void onInputChanges(std::string type, QString value) {
        std::cout << "onInputChanges(std::string type, std::string value): " << type << ": -> " << value.toStdString() << '\n';

        // Checking which type the input belongs to
        if (type == "wait_time" || type == "reload_time" || type == "show_test" || type == "maximum_login_seconds") {
            std::fstream settingsFile(dirPath + SETTINGS_PATH, std::ios::out);

            // Checking which value does the input correspond to
            if (type == "wait_time") {
                // Applying changes
                settings["wait_time"] = stringToDouble(value.toStdString());
            } else if (type == "reload_time") {
                // This is reload time. So we will change the reload_time key
                settings["reload_time"] = stringToDouble(value.toStdString());
            } else if (type == "show_test") {
                // Alright. This is showing tests, so we save into show_test key
                // std::cout << "!!!";
                settings["show_test"] = value == "2" ? true : false;
            } else if (type == "maximum_login_seconds") {
                // Now, save into the correct key.
                // NOTICE: the key is not the same as type this time. Might happen in the future tho
                settings["max_not_logged_in_session_seconds"] = stringToInt(value.toStdString());
            }

            if (settingsFile.is_open()) {
                // Write the result
                settingsFile << settings;

                // Flush I/O
                settingsFile.close();
            } else {
                errorDialog("Tệp cài đặt không tồn tại hoặc bị lỗi. Hãy cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
        } else if (type == "website_name" || type == "slogan" || type == "hostname") {
            std::fstream aliasFile(dirPath + ALIAS_PATH, std::ios::out);

            // Checking which type?
            if (type == "website_name") {
                aliases["website_name"] = value.toUtf8().toStdString();
            } else if (type == "slogan") {
                aliases["slogan"] = value.toUtf8().toStdString();
            } else if (type == "hostname") {
                aliases["hostname"] = value.toUtf8().toStdString();
            }

            if (aliasFile.is_open()) {
                aliasFile << aliases;
                aliasFile.close();
            } else {
                errorDialog("Tệp thông tin hiển thị không tồn tại hoặc bị lỗi. Hãy cài đặt lại ứng dụng để sửa lỗi.");
                close();
                exit(0);
            }
        }
    }

    protected: // Events in which are native Qt events
    void closeEvent(QCloseEvent *event) override {
        if (judgingProcess->state() == QProcess::Running || webserverProcess->state() == QProcess::Running) {
            // If these processes are still running
            QMessageBox::information(this, "Không thể hoàn thành yêu cầu", 
                "Không thể thoát vì hệ thống chấm bài hoặc website chấm bài trực tuyến vẫn còn đang chạy hoặc đang trong quá trình tắt. Vui lòng đợi và thử lại sau.",
                QMessageBox::Ok);

            event->ignore(); // Prevent event from closing
        } else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Xác nhận", "Bạn có chắc muốn thoát?",
                                        QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                event->accept();  // Allow closing
                exit(0);
            } else {
                event->ignore();  // Prevent closing
            }
        }
    }
};

/* PROPOSED BY CHATGPT. IDK HOW THIS WORKS BUT LOOKS LIKE ITS WAY BETTER */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1) Grab the wide‐char argv list from Windows
    int argc_w;
    LPWSTR* argv_w = CommandLineToArgvW(GetCommandLineW(), &argc_w);
    if (!argv_w) {
        // If for some reason this fails, bail out.
        return -1;
    }

    // 2) Convert each wide‐string to UTF-8 std::string exactly once
    std::vector<std::string> utf8Args;
    utf8Args.reserve(argc_w);

    for (int i = 0; i < argc_w; ++i) {
        // Determine how many bytes are needed
        int bytesNeeded = WideCharToMultiByte(
            CP_UTF8,
            0,
            argv_w[i],
            -1,
            nullptr,
            0,
            nullptr,
            nullptr
        );
        if (bytesNeeded <= 0) {
            // Conversion failure—fallback to empty string
            utf8Args.push_back(std::string());
            continue;
        }

        // Do the actual conversion into a std::string
        std::string s;
        s.resize(bytesNeeded - 1); // WideCharToMultiByte writes a trailing '\0'
        WideCharToMultiByte(
            CP_UTF8,
            0,
            argv_w[i],
            -1,
            &s[0],
            bytesNeeded,
            nullptr,
            nullptr
        );
        // At this point, 's' contains a null-terminated UTF-8 string.
        utf8Args.push_back(std::move(s));
    }

    // We no longer need the wide‐char array
    LocalFree(argv_w);

    // 3) Check for "--console" among utf8Args
    bool hideConsole = true;
    for (int i = 1; i < argc_w; ++i) {
        if (utf8Args[i] == "--console") {
            hideConsole = false;
            break;
        }
    }

    // 4) Build the char* array that Qt wants, pointing at each std::string's internal buffer
    std::vector<char*> qt_argv;
    qt_argv.reserve(argc_w);
    for (int i = 0; i < argc_w; ++i) {
        qt_argv.push_back( const_cast<char*>(utf8Args[i].c_str()) );
    }
    int qt_argc = argc_w;
    char** qt_argv_ptr = qt_argv.data();

    // 5) Optionally allocate a console
    if (!hideConsole) {
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$",  "r", stdin);

        std::cout << "Console attached!\n";
        std::cout << "Application running with console support.\n";
        std::cout << R"(
      ::::    :::     :::     :::    ::: ::::::::::: ::::::::::: :::       :::    :::  ::::::::      :::     :::    ::: 
     :+:+:   :+:   :+: :+:   :+:    :+:     :+:         :+:     :+:       :+:    :+: :+:    :+:    :+:      :+:   :+:   
    :+:+:+  +:+  +:+   +:+  +:+    +:+     +:+         +:+     +:+       +:+    +:+ +:+          +:+ +:+   +:+  +:+     
   +#+ +:+ +#+ +#++:++#++: +#+    +:+     +#+         +#+     +#+       +#+    +:+ +#++:++#++  +#+  +:+   +#++:++       
  +#+  +#+#+# +#+     +#+ +#+    +#+     +#+         +#+     +#+       +#+    +#+        +#+ +#+#+#+#+#+ +#+  +#+       
 #+#   #+#+# #+#     #+# #+#    #+#     #+#         #+#     #+#       #+#    #+# #+#    #+#       #+#   #+#   #+#       
###    #### ###     ###  ########      ###     ########### ########## ########   ########        ###   ###    ###  
        )" << "\n";
    }

    // 6) Initialize Winsock
    std::cout << "Initializing Winsock\n";
    WSADATA wsaData;
    int wsResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (wsResult != 0) {
        std::cout << "WSAStartup failed: " << wsResult << "\n";
        QMessageBox::critical(
            nullptr,
            "Lỗi mở WINSOCK",
            "Đã gặp lỗi thiết lập WINSOCK. Thiết bị của bạn có thể không tương thích với phần mềm này",
            QMessageBox::StandardButton::Ok
        );
        return 1;
    }

    // 7) Launch Qt application
    QApplication app(qt_argc, qt_argv_ptr);
    Q_INIT_RESOURCE(qres);

    PanelWindow panel;
    panel.initialize();

    if (!hideConsole) {
        QMessageBox::warning(nullptr, "Hiện bảng lịch sử hoạt động chương trình (logging)", "Bạn đang bắt đầu ATOMIC bằng tham số --console, điều này không được khuyến nghị bởi vì sẽ có thể xảy ra các lỗi liên quan đến bộ nhớ khi I/O chưa hoàn thiện. Nếu bạn không có ý định gỡ rối chương trình, hãy tránh sử dụng tham số --console khi khởi động chương trình", QMessageBox::Ok);
    }

    panel.show();
    int ret = app.exec();

    // 8) Cleanup Winsock (optional, since process is exiting)
    WSACleanup();

    return ret;
}