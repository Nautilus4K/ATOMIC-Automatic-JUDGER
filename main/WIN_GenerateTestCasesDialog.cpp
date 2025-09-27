// WIN_GenerateTestCasesDialog.cpp
// All headers are in the headers file
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

    QPushButton *generateButton = new QPushButton(optionsArea);
    generateButton->setText("Tạo");
    optionsAreaLayout->addStretch(1); // Area so that the button will always be at the end
    optionsAreaLayout->addWidget(generateButton);

    // AI RESPONSE
    QWidget *resultArea = new QWidget(mainSplitter);
    resultArea->setObjectName("con_sec");
    resultArea->setMinimumWidth(400); // Needed bcuz yk

    // Add them into the splitter
    mainSplitter->addWidget(optionsArea);
    mainSplitter->addWidget(resultArea);
}