#ifndef CST_LISTING_H
#define CST_LISTING_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

#include <QtCore/QStringListModel>

// ---------------------------------------------------------------
// Feature: A list view that cans be modified in terms of entries
// ---------------------------------------------------------------
class CST_Listing: public QWidget {
    Q_OBJECT

    public:
    CST_Listing(QWidget *parent = (QWidget *)nullptr);
    void setEntries(QStringList entList);
    QStringList getEntries() const;
    void setReadOnly(bool b);

    QListView *listView = new QListView(this);
    QStringListModel *model = new QStringListModel(this); // NULL AT FIRST, I GUESS
    QPushButton *addBtn = new QPushButton(this); // It's shit, i know, its inefficient, i know
    QPushButton *subBtn = new QPushButton(this); // what do you honestly expect from a fucking 14 years old?

    private:
    void addEntry();
    void removeEntry();
};

#endif