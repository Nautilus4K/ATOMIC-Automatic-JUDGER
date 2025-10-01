#ifndef WIN_GENERATETESTCASESDIALOG_H
#define WIN_GENERATETESTCASESDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMessageBox>

// #include <QtGui/QIntValidator>

#include <QtCore/Qt>

#include <iostream>

// Socket
#include <QtCore/QThread> // ACTUAL THREADING WITH QT

// Ollama
#include "ollama.hpp"

#include "WIN_ContestsSettings.h"
#include "utilities.h"

class WIN_GenerateTestCasesDialog: public QWidget {
    Q_OBJECT

    public:
    // We gotta add an option to automatically clear all old test for these new ones
    QCheckBox *overrideAllCurrentCases;
    QPushButton *generateButton;
    QTextEdit *aiResponseFrame;
    std::string targetContest;
    bool allowForGeneration;

    // Only WIN_ContestsSettings can be the parent here.
    // busyState is the bool that says "HEY AI GENERATION IS BUSY"
    WIN_GenerateTestCasesDialog(WIN_ContestsSettings *parent = nullptr, bool *busyState = nullptr, const std::string& contest = "");

    private slots:
    // Event when the overrideAllCurrentCases checkbox changed states
    void overrideCheckBoxStateChanged();

    // Event when the user clicks on the generate button
    void beginGeneration(std::string query);

    // Event when response is cleared to make way for the new responses
    void clearResponse();

    // Event when there is a new token
    void newToken(QString token);

    // Event when the generation is finished
    void finishedGeneration();

    signals:
    // These signals are for the AI Generation functionality here yk.
    void clearResponseEvent();
    void newTokenEvent(QString token);
    void finishedGenerationEvent();
};

#endif