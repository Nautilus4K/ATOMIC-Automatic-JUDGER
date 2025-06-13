#ifndef WIN_CONTESTSSETTINGS_H
#define WIN_CONTESTSSETTINGS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>

#include <QtGui/QDoubleValidator>

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

    // The rest of the widget
    CST_RichTextEdit *descEdit = new CST_RichTextEdit(this);
    CST_Listing *classList = new CST_Listing(this);
    QLabel *contestNameLabel = new QLabel(this);
    QPushButton *saveBtn = new QPushButton(this);
    QListWidget *testCasesList = new QListWidget(this);
    QCheckBox *fileInputCheck = new QCheckBox(this);
    QCheckBox *fileOutputCheck = new QCheckBox(this);
    QLineEdit *fileInputName = new QLineEdit(this);
    QLineEdit *fileOutputName = new QLineEdit(this);
    QLineEdit *timeLimit = new QLineEdit(this);

    json contests; // Contest data in JSON. Would allow for faster reading since the whole thing is already based around JSON
    std::string currentCnts = "";

    private:
    std::vector<int> indexesToBeRemoved; // List of indexes that will be removed
    std::vector<std::pair<int, std::pair<std::string, std::string>>> indexesToBeModified; // List of indexes that will be modified
    std::vector<std::pair<std::string, std::string>> indexesToBeAdded; // List of indexes to be added

    // -----------------------------------------------------------------
    // Purpose: To remove a contest from its ROW. Which originates from
    //          the fetchContests() functionality
    // -----------------------------------------------------------------
    // This will manage the removal of contests, clearing out caches related to the old contest
    // call refresh functions and also change selection of these dudes to the closest one.
    void remContest(QListWidgetItem *item);
    
    // ---------------------------------------------
    // Purpose: To select a new contest in the list
    // ---------------------------------------------
    void selectNewContest(std::string contestName);

    // --------------------------------------------------------------------
    // Purpose: To do a set of actions when the user clicks on adding new
    //          contest button
    // --------------------------------------------------------------------
    void newContest(bool forced);

    // --------------------------------------------------------------------
    // Purpose: To do actions when checkbox of the test case functionality
    //          was toggled
    // --------------------------------------------------------------------
    // Note: the `which` parameter have 3 possible values:
    // - `in`
    // - `out`
    // - `both`
    void onTestCaseCheckBoxToggled(std::string which);

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

    // -----------------------------------------------------------------------
    // Purpose: Add test cases. Just that. Yes. I am not lying. This function
    //          will show up a dialog to enter the case's data and then
    //          add it.
    // -----------------------------------------------------------------------
    void addCase();

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