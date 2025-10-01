// WIN_GenerateTestCasesDialog.cpp
// All headers are in the headers file
// This is just the implementation to be able to generate .o files alongside moc's .o files
#include <WIN_GenerateTestCasesDialog.h>

WIN_GenerateTestCasesDialog::WIN_GenerateTestCasesDialog(WIN_ContestsSettings *parent, bool *busyState, const std::string& contest) : QWidget(parent, Qt::Window) {
    setObjectName("container");
    setWindowTitle("Tạo bộ test bằng trí tuệ nhân tạo");
    setWindowIcon(parent->windowIcon());
    setStyleSheet(parent->styleSheet());
    setAttribute(Qt::WA_DeleteOnClose); // Cleaning

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

    generateButton = new QPushButton(optionsArea);
    generateButton->setText("Tạo");
    optionsAreaLayout->addStretch(1); // Area so that the button will always be at the end
    optionsAreaLayout->addWidget(generateButton);

    connect(generateButton, &QPushButton::clicked, this, [this, parent, contest, testCasesAmount]() {
        if (!allowForGeneration) {
            // Probably not generating
            generateButton->setText("Hủy");

            // Let's construct the string here
            json contestData = parent->contests[contest];
            std::string query = testCasesAmount->cleanText().toStdString() + "TEST CASES\n" + 
                                contestData["Desc"].template get<std::string>()
            ;

            // Run &WIN_GenerateTestCasesDialog::beginGeneration on the `this` object
            // std::thread generationThread(&WIN_GenerateTestCasesDialog::beginGeneration, this)
            allowForGeneration = true;
            QThread *generateThread = QThread::create([this, query]() {
                beginGeneration(query);
            });

            // Cleanup
            connect(generateThread, &QThread::finished, generateThread, &QObject::deleteLater);

            generateThread->start();
        } else {
            // Probably generating
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

    std::cout << "[//*//] Initialized WIN_GenerateTestCasesDialog.\n";

    if (contest == "") {
        std::cout << "[//*//] So its not a real contest...";
        close();
    }
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
    aiResponseFrame->setPlainText(aiResponseFrame->toPlainText() + "\n\nProcessing...");

    aiResponseFrame->moveCursor(QTextCursor::End);
    aiResponseFrame->ensureCursorVisible();

    // generateButton->setEnabled(true);
    generateButton->setText("Tạo");
}

void WIN_GenerateTestCasesDialog::beginGeneration(std::string query) {
    // Alr
    std::cout << "[beginGeneration()] Beginning generation of test\n";

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