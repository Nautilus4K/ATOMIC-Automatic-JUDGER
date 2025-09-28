// WIN_GenerateTestCasesDialog.cpp
// All headers are in the headers file
// This is just the implementation to be able to generate .o files alongside moc's .o files
#include <WIN_GenerateTestCasesDialog.h>

WIN_GenerateTestCasesDialog::WIN_GenerateTestCasesDialog(WIN_ContestsSettings *parent, bool *busyState) : QWidget(parent, Qt::Window) {
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
    testCasesAmount->setMaximum(100);
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

    QPushButton *generateButton = new QPushButton(optionsArea);
    generateButton->setText("Tạo");
    optionsAreaLayout->addStretch(1); // Area so that the button will always be at the end
    optionsAreaLayout->addWidget(generateButton);

    connect(generateButton, &QPushButton::clicked, this, &WIN_GenerateTestCasesDialog::beginGeneration);

    // AI RESPONSE
    QWidget *resultArea = new QWidget(mainSplitter);
    resultArea->setObjectName("con_sec");
    resultArea->setMinimumWidth(400); // Needed bcuz yk

    // Add them into the splitter
    mainSplitter->addWidget(optionsArea);
    mainSplitter->addWidget(resultArea);

    std::cout << "[//*//] Initialized WIN_GenerateTestCasesDialog.\n";
}

void WIN_GenerateTestCasesDialog::overrideCheckBoxStateChanged() {
    // Here it is
    std::cout << "[overrideCheckBoxStateChanged()] State changed -> " << overrideAllCurrentCases->checkState() << '\n';
}

void WIN_GenerateTestCasesDialog::beginGeneration() {
    // Alr
    std::cout << "[beginGeneration()] Beginning generation of test\n";
}