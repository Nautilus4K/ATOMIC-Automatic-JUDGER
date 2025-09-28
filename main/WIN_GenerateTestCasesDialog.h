#ifndef WIN_GENERATETESTCASESDIALOG_H
#define WIN_GENERATETESTCASESDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>

// #include <QtGui/QIntValidator>

#include <QtCore/Qt>

#include <iostream>

#include "WIN_ContestsSettings.h"
#include "utilities.h"

class WIN_GenerateTestCasesDialog: public QWidget {
    Q_OBJECT

    public:
    // We gotta add an option to automatically clear all old test for these new ones
    QCheckBox *overrideAllCurrentCases;

    // Only WIN_ContestsSettings can be the parent here.
    // busyState is the bool that says "HEY AI GENERATION IS BUSY"
    WIN_GenerateTestCasesDialog(WIN_ContestsSettings *parent = nullptr, bool *busyState = nullptr);

    private slots:
    // Event when the overrideAllCurrentCases checkbox changed states
    void overrideCheckBoxStateChanged();

    // Event when the user clicks on the generate button
    void beginGeneration();
};

#endif