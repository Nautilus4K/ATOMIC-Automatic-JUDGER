#ifndef CST_SEPARATOR_H
#define CST_SEPARATOR_H

#include <QtWidgets/QWidget>

class CST_Separator: public QWidget {
    Q_OBJECT

    public:
    // Vertical separator for horizontal layout with padding on each side
    CST_Separator(QWidget *parent = (QWidget *)nullptr, int padding = 10);
};

#endif