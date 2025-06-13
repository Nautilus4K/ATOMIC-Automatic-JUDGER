#ifndef WIN_USERSSETTINGS_H
#define WIN_USERSSETTINGS_H

#include "CST_Listing.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>

#include <nlohmann/json.hpp>

#include <vector>

using nlohmann::json;

class WIN_UsersSettings: public QWidget {
    Q_OBJECT

    public:
    WIN_UsersSettings(QWidget *parent = (QWidget *)nullptr);

    private:
    // Widgets pointers
    QListWidget  *listView = new QListWidget(this);
    CST_Listing  *classListing = new CST_Listing(this);
    QLabel       *passwdLabel = new QLabel(this);

    // Data variables
    std::vector<std::string>   userByRowOrder;
    std::string                currentUser;
    std::string                currentPasswdHash;
    json                       users;

    // ----------------------------------
    // Purpose: To initialize the window.
    // ----------------------------------
    // This was made to make things simpler, less cramped, and wtv panel.cpp isn't. That bitch is
    // a pain in the ass to work with. Like, dude, it's just fcking bad.
    void initAct();

    // -------------------------------------------------------------------------
    // Purpose: To load the users on the sidebar, can be used to LOAD or RELOAD.
    // -------------------------------------------------------------------------
    // This is an equivalent of `fetchContests()` on `WIN_ContestsSettings`, which is very bad. But, it works.
    // so I mean, it's fine. This is just spaghetti code to be honest.
    void loadUsers();

    // -------------------------------------------------------------
    // Purpose: To reload variables. Kinda easy, right? Maybe. Idk.
    // -------------------------------------------------------------
    // This is created to make the commands easier to read, and reloading variables will be quite a constant show up.
    void reloadVars();

    // -------------------------------------------------------------------------------
    // Purpose: To refresh the details page accordingly to the username we changed to
    // -------------------------------------------------------------------------------
    void toUser(std::string username);

    // ------------------------------------------------------------
    // Purpose: Simulate the action of selecting a user from a row
    //          number
    // ------------------------------------------------------------
    // What's the difference from `toUser()` is, you may ask. The difference is clear: selectUserFromRow will programmatically
    // simulate the action of clicking an entry, while `toUser()` only takes care of details page. So in a way, this is more
    // complicated.
    void selectUserFromRow(int row);

    // ------------------------------------------
    // Purpose: To save the information of user.
    // ------------------------------------------
    void saveInfo(std::string username);

    // --------------------------------------------------------------------
    // Purpose: To change the current password...? Just the hash actually.
    // --------------------------------------------------------------------
    void changePassword();

    // ---------------------------------------------------------------------------
    // Purpose: Showing errors faster than having to type an actual long command.
    //          Can't believe I have to make a duplicate
    // ---------------------------------------------------------------------------
    void errorDialog(std::string error);

    signals:
    void closed(); // For when the user closes this settings page

    protected: // Events in which are native Qt events
    void closeEvent(QCloseEvent *event) override;
};

#endif