#include "CST_Separator.h"

#include <QtWidgets/QHBoxLayout>

//                                                         This will act as a widget
//                                                                   v
CST_Separator::CST_Separator(QWidget *parent, int padding) : QWidget(parent, Qt::Widget) {
    setStyleSheet(parent->styleSheet());

    setFixedWidth(padding * 2);
    QHBoxLayout *layout = new QHBoxLayout(this);
    setLayout(layout);

    // Getting a line in the middle
    QWidget *line = new QWidget(this);
    line->setObjectName("separatorLine");
    line->setFixedWidth(1);
    layout->addWidget(line);

    // Making it centered so that the fixed width is nice
    layout->setAlignment(Qt::AlignCenter);
}