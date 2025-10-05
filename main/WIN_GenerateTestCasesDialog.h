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
#include <QtWidgets/QProgressBar>

// #include <QtGui/QIntValidator>

#include <QtGui/QTextDocument>
#include <QtGui/QCloseEvent>
#include <QtCore/QProcess>
#include <QtCore/Qt>

#include <iostream>
#include <filesystem>
#include <vector>
#include <atomic>

// Socket
#include <QtCore/QThread> // ACTUAL THREADING WITH QT

// Ollama
#include "ollama.hpp"

#include "WIN_ContestsSettings.h"
#include "utilities.h"

struct testCase {
    // std::vector<std::string> inputs, outputs;
    std::string input, output;
};

class WIN_GenerateTestCasesDialog: public QWidget {
    Q_OBJECT

    public:
    // We gotta add an option to automatically clear all old test for these new ones
    QCheckBox *overrideAllCurrentCases;
    QPushButton *generateButton;
    QPushButton *exportButton;
    QTextEdit *aiResponseFrame;
    QProgressBar *workingIndicator;
    QProcess *testProcess;
    std::string targetContest;
    // std::string currentTestOutput;
    std::vector<testCase> currentTestCases;
    // testCases currentTestCases;
    QThread *generateThread = nullptr;
    bool *busy;
    bool allowForGeneration;

    // Only WIN_ContestsSettings can be the parent here.
    // busyState is the bool that says "HEY AI GENERATION IS BUSY"
    WIN_GenerateTestCasesDialog(WIN_ContestsSettings *parent = nullptr, bool *busyState = nullptr, const std::string& contest = "");
    ~WIN_GenerateTestCasesDialog();

    // Destruct
    // ~WIN_GenerateTestCasesDialog();

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

    // Event when the goddamn process is done running (The final step)
    void processFinished();

    // Export the results into a signal?
    void exportResults();

    signals:
    // These signals are for the AI Generation functionality here yk.
    void clearResponseEvent();
    void newTokenEvent(QString token);
    void finishedGenerationEvent();
    void exportResultsEvent(std::vector<testCase> result, bool override);
    // void close();

    protected:
    void closeEvent(QCloseEvent *event);
};

#endif