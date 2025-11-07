// WIN_GenerateTestCasesDialog.cpp
// All headers are in the headers file
// This is just the implementation to be able to generate .o files alongside moc's .o files
#include "WIN_GenerateTestCasesDialog.h"

WIN_GenerateTestCasesDialog::WIN_GenerateTestCasesDialog(WIN_ContestsSettings *parent, bool *busyState, const std::string& contest) : QWidget(parent, Qt::Window) {
    setObjectName("container");
    setWindowTitle("Tạo bộ test bằng trí tuệ nhân tạo");
    setWindowIcon(parent->windowIcon());
    setStyleSheet(parent->styleSheet());
    // setAttribute(Qt::WA_DeleteOnClose); // Cleaning

    setMinimumSize(700, 400);

    // Okay now let's work towards the layout
    // It should be something like this?
    // +----------+-----------------------+
    // | Options  | AI response           |
    // |          |                       |
    // |          |                       |
    // |          |                       |
    // |          |                       |
    // +----------+-----------------------+
    // Options: Things like test count, use what? idk man. Custom prompt shit. Customize ollama's system prompt too
    // AI responses: Where it will show the result
    // Yeah split with QSplitter

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    // Its fine guys We can fix this!!!
    QSplitter *mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Orientation::Horizontal);
    layout->addWidget(mainSplitter);

    // Good now we just add the much needed QWidgets
    // OPTIONS
    QWidget *optionsArea = new QWidget(mainSplitter);
    optionsArea->setObjectName("con_sec");
    optionsArea->setMinimumWidth(250);

    QVBoxLayout *optionsAreaLayout = new QVBoxLayout(optionsArea);
    optionsArea->setLayout(optionsAreaLayout);

    QLabel *testCasesAmountLabel = new QLabel(optionsArea);
    testCasesAmountLabel->setText("Số lượng bộ test");
    optionsAreaLayout->addWidget(testCasesAmountLabel);

    QSpinBox *testCasesAmount = new QSpinBox(optionsArea);
    testCasesAmount->setMaximum(1000);
    optionsAreaLayout->addWidget(testCasesAmount);

    QWidget *overrideAllCurrentCasesCheckArea = new QWidget(optionsArea);
    overrideAllCurrentCasesCheckArea->setToolTip("Xóa tất cả những trường hợp test đã có của bài thi này và thay thế bằng các trường hợp test được sáng tạo bởi AI.");

    QHBoxLayout *overrideAllCurrentCasesCheckAreaLayout = new QHBoxLayout(overrideAllCurrentCasesCheckArea); // Long ass names
    overrideAllCurrentCasesCheckAreaLayout->setAlignment(Qt::AlignLeft);
    overrideAllCurrentCasesCheckAreaLayout->setSpacing(0); // Thank god
    overrideAllCurrentCasesCheckAreaLayout->setContentsMargins(0, 0, 0, 0);

    // Initialize the overrideAllCurrentCases checkbox
    overrideAllCurrentCases = new QCheckBox(overrideAllCurrentCasesCheckArea);
    overrideAllCurrentCases->setTristate(false); // Ensure no partially checked state
    overrideAllCurrentCasesCheckAreaLayout->addWidget(overrideAllCurrentCases);

    QLabel *overrideAllCurrentCasesLabel = new QLabel(overrideAllCurrentCasesCheckArea);
    overrideAllCurrentCasesLabel->setText("Ghi đè lên bộ test đã có");
    // overrideAllCurrentCasesLabel->setWordWrap(true); // Stop this! Its way unneeded cuz it will never overflow anyways
    overrideAllCurrentCasesCheckAreaLayout->addWidget(overrideAllCurrentCasesLabel);

    // ALRIGHT let me engage the checkbox with the function
    connect(overrideAllCurrentCases, &QCheckBox::checkStateChanged, this, &WIN_GenerateTestCasesDialog::overrideCheckBoxStateChanged);

    // Add the override area in
    optionsAreaLayout->addWidget(overrideAllCurrentCasesCheckArea);

    // Now we add a QTextEdit that will pose as additional input here
    QTextEdit *additionalQuery = new QTextEdit(optionsArea);
    additionalQuery->setPlaceholderText("Yêu cầu thêm cho AI...");
    optionsAreaLayout->addWidget(additionalQuery, 1);

    // optionsAreaLayout->addStretch(1); // Area so that the button will always be at the end

    // Add the busy indicator
    workingIndicator = new QProgressBar(optionsArea);
    optionsAreaLayout->addWidget(workingIndicator);

    QWidget *genexpArea = new QWidget(optionsArea);
    QHBoxLayout *genexpAreaLayout = new QHBoxLayout(genexpArea);
    genexpAreaLayout->setContentsMargins(0, 0, 0, 0);

    generateButton = new QPushButton(genexpArea);
    generateButton->setText("Tạo");
    genexpAreaLayout->addWidget(generateButton);

    exportButton = new QPushButton(genexpArea);
    exportButton->setText("Xuất");
    exportButton->setEnabled(false);
    genexpAreaLayout->addWidget(exportButton);

    connect(exportButton, &QPushButton::clicked, this, &WIN_GenerateTestCasesDialog::exportResults);

    optionsAreaLayout->addWidget(genexpArea);

    connect(generateButton, &QPushButton::clicked, this, [this, parent, contest, testCasesAmount, additionalQuery]() {
        if (!allowForGeneration) {
            // Probably not generating => We gonna generate
            generateButton->setText("Hủy");
            exportButton->setEnabled(false); // Prepare to clear all that stuff i guess? Yea...
            workingIndicator->setRange(0, 0);

            // Let's construct the string here
            json contestData = parent->contests[contest];
            // Lemme just extract the text into a QTextDocument to turn it into plain text
            QTextDocument doc;
            doc.setHtml(QString::fromStdString(contestData["Desc"].template get<std::string>()));

            std::string query = testCasesAmount->cleanText().toStdString() + " TEST CASES\nAddtional request: " + 
                                additionalQuery->toPlainText().toStdString() + "\nProblem description: " +
                                doc.toPlainText().toStdString()
            ;

            // Stop any existing thread first
            if (generateThread && generateThread->isRunning()) {
                allowForGeneration = false;
                generateThread->wait(1000);
            }

            generateThread = QThread::create([this, query]() {
                beginGeneration(query);
            });

            // Cleanup
            connect(generateThread, &QThread::finished, generateThread, &QObject::deleteLater);
            connect(generateThread, &QThread::finished, this, [this]() {
                generateThread = nullptr;  // Clear the pointer when done
            });

            allowForGeneration = true;

            generateThread->start();
        } else {
            // Probably generating => We gotta abort
            generateButton->setEnabled(false); // Stop the user from spamming alright
            allowForGeneration = false;
        }
    });

    // Connect the relevant signals to their appropriate functions
    connect(this, &WIN_GenerateTestCasesDialog::clearResponseEvent, this, &WIN_GenerateTestCasesDialog::clearResponse, Qt::QueuedConnection);
    connect(this, &WIN_GenerateTestCasesDialog::newTokenEvent, this, &WIN_GenerateTestCasesDialog::newToken, Qt::QueuedConnection);
    connect(this, &WIN_GenerateTestCasesDialog::finishedGenerationEvent, this, &WIN_GenerateTestCasesDialog::finishedGeneration, Qt::QueuedConnection);

    // AI RESPONSE
    QWidget *resultArea = new QWidget(mainSplitter);
    resultArea->setObjectName("con_sec");
    resultArea->setMinimumWidth(400); // Needed bcuz yk

    QVBoxLayout *resultAreaLayout = new QVBoxLayout(resultArea);
    resultArea->setLayout(resultAreaLayout);

    aiResponseFrame = new QTextEdit(resultArea);
    aiResponseFrame->setReadOnly(true);
    aiResponseFrame->setFont(parent->fonts["monospace"]);
    aiResponseFrame->setText("Artificial Intelligence.");
    resultAreaLayout->addWidget(aiResponseFrame);

    // Add them into the splitter
    mainSplitter->addWidget(optionsArea);
    mainSplitter->addWidget(resultArea);

    targetContest = contest;
    allowForGeneration = false;

    busy = busyState;
    *busy = false;

    std::cout << "[//*//] Initialized WIN_GenerateTestCasesDialog.\n";

    if (contest == "") {
        std::cout << "[//*//] So its not a real contest...";
        close();
    }

    // Qt config
    testProcess = new QProcess(this);
    testProcess->setProgram(QString::fromStdString(dirPath + PYDIR));  // Good

    connect(testProcess, &QProcess::finished, this, &WIN_GenerateTestCasesDialog::processFinished);
}

WIN_GenerateTestCasesDialog::~WIN_GenerateTestCasesDialog() {
    std::cout << "[~WIN_GenerateTestCasesDialog()] Destructor called\n";

    currentTestCases.clear();
    targetContest.clear();
}

void WIN_GenerateTestCasesDialog::exportResults() {
    if (overrideAllCurrentCases->checkState() == Qt::CheckState::Checked) {
        std::cout << "[exportResults()] Confirming to override all current cases...\n";
        QMessageBox::StandardButton rep = QMessageBox::question(this, 
            "Bạn có chắc muốn tiếp tục?", 
            "Bạn đang ghi đè lên bộ test đã có. Đây là một hành động RẤT NGUY HIỂM và sẽ không thể nào được đưa về như cũ sau khi bạn xuất (dữ liệu sẽ tự động lưu). Bạn có chắc?", 
            QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No
        );

        if (rep == QMessageBox::StandardButton::No) {
            return;
        }
    }

    std::cout << "[exportResults()] Preparing...\n";
    // std::vector<std::vector<std::string>> result;
    // for (const testCase& c : currentTestCases) {
    //     std::vector<std::string> caseVector(2);
    //     caseVector[0] = c.input;
    //     caseVector[1] = c.output;

    //     result.push_back(caseVector);
    // }

    emit exportResultsEvent(currentTestCases, (overrideAllCurrentCases->checkState() == Qt::CheckState::Checked));
}

void WIN_GenerateTestCasesDialog::overrideCheckBoxStateChanged() {
    // Here it is
    std::cout << "[overrideCheckBoxStateChanged()] State changed -> " << overrideAllCurrentCases->checkState() << '\n';

    if (overrideAllCurrentCases->checkState() == Qt::CheckState::Checked) {
        QMessageBox::warning(this, "Ghi đè lên bộ test đã có", "Hành động này RẤT NGUY HIỂM vì nó sẽ xóa toàn bộ các trường hợp test bạn đã có hiện tại.", QMessageBox::StandardButton::Ok);
    }
}

void WIN_GenerateTestCasesDialog::newToken(QString token) {
    // Do the thing (this time in main GUI thread)
    aiResponseFrame->setPlainText(aiResponseFrame->toPlainText() + token);
    aiResponseFrame->moveCursor(QTextCursor::End);
    aiResponseFrame->ensureCursorVisible();
}

void WIN_GenerateTestCasesDialog::clearResponse() {
    aiResponseFrame->setPlainText(""); // No need for full clear()
}

void WIN_GenerateTestCasesDialog::finishedGeneration() {
    // Do the OTHER thing (this time also in main GUI thread)
    if (allowForGeneration) { // In case the generation was allowed through
        workingIndicator->setRange(0, 2);
        workingIndicator->setValue(1);
        // aiResponseFrame->setPlainText(aiResponseFrame->toPlainText() + "\n\nĐang bắt đầu xử lí...");

        // Now we extract the code block(s)
        std::string code = extractCodeBlocks(aiResponseFrame->toPlainText().toStdString());
        std::cout << "Extracted code:\n" << code << '\n';

        // Let's set the code
        aiResponseFrame->setPlainText(QString::fromStdString(code));

        // Good. Now we should try running this
        // Yeah even if the generation is finished we shouldn't be so fast
        std::cout << "[finishedGeneration()] Creating temporary file...\n";
        std::string path = "/aiexec.py";
        std::fstream file(TEMP_FOLDER + path, std::ios::trunc | std::ios::out);
        if (file.is_open()) {
            std::cout << "[finishedGeneration()] Writing into temporary file...\n";
            file << code;
        } else {
            std::cerr << "ffs\n"; // Here we skip the whole thing and act as if nothing ever happened
            aiResponseFrame->setText("Lỗi: Không thể tạo tệp và thử nghiệm.");
            allowForGeneration = false;
            generateButton->setEnabled(true);
            generateButton->setText("Tạo");

            return;
        }

        // Now we execute it i guess?
        // Yeah we need to execute it
        // Let's get it running
        std::cout << "[finishedGeneration()] Setting arguments...\n";
        std::cout << "> Initializing args...\n" << std::flush;
        QStringList args;
        std::cout << "> Converting...\n" << std::flush;
        args << QString::fromStdString(TEMP_FOLDER + path);
        std::cout << "> Setting args...\n" << std::flush;
        testProcess->setArguments(args); // WHY IS IT FREEZING HERE

        std::cout << "[finishedGeneration()] Starting testProcess with the following command: " << testProcess->program().toStdString() << " " << TEMP_FOLDER + path << '\n' << std::flush;
        testProcess->start(); // Now we wait for this to be finished

        allowForGeneration = false;
        generateButton->setEnabled(false);
    } else { // In case it was forcefully shut
        std::cout << "Aborted generation forcefully.\n";
        workingIndicator->setValue(0);

        generateButton->setEnabled(true);
        generateButton->setText("Tạo");
    }
}

void WIN_GenerateTestCasesDialog::processFinished() {
    // Now we work to finish this? TL;DR: This is the final step thank you for waiting
    // First lets just remove that temporary file first
    try {
        std::filesystem::remove(TEMP_FOLDER + "/aiexec.py");
    } catch (...) {
        // ffs
        std::cerr << "Error in processFinished(): TEMP/aiexec.py cant be deleted? wut\n";
    }

    // Alr let's get the result dammit
    QString stdoutData = testProcess->readAllStandardOutput();
    QString stderrData = testProcess->readAllStandardError();
    int returnCode = testProcess->exitCode();

    if (returnCode != 0) {
        // NOOOO FOR FUCK'S SAKE
        aiResponseFrame->setText("Lỗi trong quá trình thực thi mã Python:\n" + stdoutData + "\n" + stderrData);
    } else {
        // Yea its fine
        // aiResponseFrame->setText(stdoutData.replace("\2", " | ", Qt::CaseSensitive).replace(QChar(char(0)), "\n"));
        // Yeah let's interpret that stdoutData
        char sep_in_out = 2; // ASCII 2
        char sep_tests = 0;  // ASCII 0

        std::string data = stdoutData.toStdString();

        size_t start = 0;
        int test_id = 1;

        // std::vector<std::vector<std::string>> testCases; // Quick to embed into json cuz this is the same goddamn format
        // currentTestCases.inputs.clear(); // Clear first.
        // currentTestCases.outputs.clear();
        currentTestCases.clear();

        QString displayTestText = "";

        // std::vector<std::string> testCase(2); // reusable
        while (true) {
            size_t mid = data.find(sep_in_out, start);
            if (mid == std::string::npos) break;

            size_t end = data.find(sep_tests, mid + 1);
            if (end == std::string::npos) break;

            std::string input = cleanTrails(data.substr(start, mid - start));
            std::string output = cleanTrails(data.substr(mid + 1, end - mid - 1));

            // testCase[0] = input;
            // testCase[1] = output;

            // currentTestCases.push_back(testCase);
            // currentTestCases.inputs.push_back(input);
            // currentTestCases.outputs.push_back(output);
            // currentTestCases.push_back(c);

            // std::cout << "Test " << test_id++ << ":\n";
            // std::cout << "INPUT:\n" << input << "\n";
            // std::cout << "OUTPUT:\n" << output << "\n\n";
            testCase c;
            c.input = input;
            c.output = output;

            currentTestCases.push_back(c);

            displayTestText += "\n\nTest " + QString::fromStdString(intToString(test_id++)) + 
                               ":\nINPUT:\n" + QString::fromStdString(input) +
                               "\n\nOUTPUT:\n" + QString::fromStdString(output)
            ;

            start = end + 1;
        }

        aiResponseFrame->setText(displayTestText);

        // Save some memory
        currentTestCases.shrink_to_fit();

        exportButton->setEnabled(true);
    }

    allowForGeneration = false;
    generateButton->setEnabled(true);
    generateButton->setText("Tạo");

    workingIndicator->setValue(2);
}

void WIN_GenerateTestCasesDialog::beginGeneration(std::string query) {
    // Alr
    std::cout << "[beginGeneration()] Beginning generation of test with the following query: " << query << "\n";

    // Oh we dont need any external scripts
    // We could just call ollama ourselves
    // Yeah cuz its the job of the client
    Ollama agent("http://localhost:11434");
    
    // aiResponseFrame->setText("");
    emit clearResponseEvent();
    // This function will be called every token
    std::function<bool(const ollama::response&)> response_callback = [this](const ollama::response& response) -> bool {
        // Print the token received
        std::cout << response << std::flush;
        // Also mark it in, too
        // aiResponseFrame->setPlainText(aiResponseFrame->toPlainText() + QString::fromStdString(response));
        // Yeah we cant be accessing protected variables from another thread
        // Let's just use signals instead
        emit newTokenEvent(QString::fromStdString(response));

        // If the server marks this as done
        if (response.as_json()["done"] == true || !allowForGeneration) {
            std::cout << std::endl;
            // aiResponseFrame->setPlainText(aiResponseFrame->toPlainText() + "\n\nProcessing...");
            emit finishedGenerationEvent();
        }

        return allowForGeneration; // Return true to continue streaming
    };
    
    // Now we just have to do some stupid shit
    ollama::request request(ollama::message_type::generation);
    request["model"] = OLLAMA_TARGET_MODEL;
    request["prompt"] = query;
    request["system"] = OLLAMA_SYSTEM_PROMPT;

    ollama::generate(request, response_callback);
}

void WIN_GenerateTestCasesDialog::closeEvent(QCloseEvent *event) {
    // Ask worker to stop
    // allowForGeneration.store(false);
    allowForGeneration = false;

    if (generateThread) {
        if (generateThread->isRunning()) {
            // give the worker time to stop (it checks allowForGeneration)
            generateThread->wait(1500);
            // If still running, we don't call terminate(); best effort only.
        }
    }

    if (busy) {
        *busy = false;
    }

    this->deleteLater();
    event->accept();
}