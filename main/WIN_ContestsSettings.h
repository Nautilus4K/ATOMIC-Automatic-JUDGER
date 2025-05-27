#ifndef WIN_CONTESTSSETTINGS_H
#define WIN_CONTESTSSETTINGS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>

#include "CST_RichTextEdit.h"
#include "CST_Listing.h"

#include <nlohmann/json.hpp>
using nlohmann::json;

// ------------------------------------------------
// Functionality: To edit/change contests settings
// ------------------------------------------------
class WIN_ContestsSettings: public QWidget {
    Q_OBJECT

    public:
    WIN_ContestsSettings(QWidget *parent = (QWidget *)nullptr);

    // Sidebar widget
    QListWidget *listView = new QListWidget(this);
    CST_RichTextEdit *descEdit = new CST_RichTextEdit(this);
    CST_Listing *classList = new CST_Listing(this);
    QLabel *contestNameLabel = new QLabel(); // Give it nullptr since I am a lazy bitch
    QPushButton *saveBtn = new QPushButton();
    QTableWidget *testTable = new QTableWidget(this);

    json contests; // Contest data in JSON. Would allow for faster reading since the whole thing is already based around JSON
    std::string currentCnts = "";

    private:
    // --------------------------
    // Purpose: Look at the name
    // --------------------------
    void reloadContestsVar();

    // ---------------------------------------------------------------
    // Purpose: A way to fetch new contests when I just opened it up.
    // ---------------------------------------------------------------
    void fetchContests(bool selectEntryAutomatically);
    
    // --------------------------------------------------------------------
    // Purpose: To save the information to the FILE. Alright? Not the JSON
    //          variable but the FILE ITSELF. Kinda like syncing
    // --------------------------------------------------------------------
    void saveInfo();

    // --------------------------------------------------------
    // Purpose: To refresh currently clicked on contest with a
    //          compact function
    // --------------------------------------------------------
    void toCnts(std::string contestName);

    // ---------------------------------------------------------------------------
    // Purpose: Showing errors faster than having to type an actual long command.
    //          Can't believe I have to make a duplicate
    // ---------------------------------------------------------------------------
    void errorDialog(std::string error);

    std::vector<std::string> contestByRowOrder; // A variable to get the order of contests in the sidebar

    signals:
    void closed(); // For when the user closes this settings page

    protected: // Events in which are native Qt events
    void closeEvent(QCloseEvent *event) override;
};

#endif