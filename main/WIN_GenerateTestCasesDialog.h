#ifndef WIN_GENERATETESTCASESDIALOG_H
#define WIN_GENERATETESTCASESDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>

// #include <QtGui/QIntValidator>

#include <QtCore/Qt>

#include "WIN_ContestsSettings.h"
#include "utilities.h"

class WIN_GenerateTestCasesDialog: public QWidget {
    Q_OBJECT

    public:
    // Only WIN_ContestsSettings can be the parent here.
    // busyState is the bool that says "HEY AI GENERATION IS BUSY"
    WIN_GenerateTestCasesDialog(WIN_ContestsSettings *parent = nullptr, bool *busyState = nullptr);
};

#endif