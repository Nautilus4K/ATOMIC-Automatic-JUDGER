/* WIN_ClassesSettings.h */
/* ------------------------------------
 * Header file to declare WIN_ClassesSettings' methods.
 * ------------------------------------ */

#ifndef WIN_CLASSESSETTINGS_H
#define WIN_CLASSESSETTINGS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>

#include <nlohmann/json.hpp>

using nlohmann::json;

class WIN_ClassesSettings: public QWidget {
    Q_OBJECT

    public:
    // Widgets variables (global)
    QTableWidget *table = new QTableWidget(this);

    // Data variables (global)
    json classes;

    // Structor and functions
    WIN_ClassesSettings(QWidget *parent = (QWidget *)nullptr);

    private:
    void init();
    void loadClasses();
    void updateInfo(int row, int column);
    void newClass();
    void showContextMenu(const QPoint& pos);

    signals:
    void closed();

    protected:
    void closeEvent(QCloseEvent *event);
};

#endif