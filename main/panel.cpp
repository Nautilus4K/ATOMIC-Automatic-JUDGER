/**********************************

Compilation commandline:
g++ -I"C:\Qt\6.5.3\mingw_64\include" -L"C:\Qt\6.5.3\mingw_64\lib" panel.cpp -lQt6Widgets -lQt6Core -lQt6Gui -o panel.exe -mwindows

**********************************/

// Importing Qt stuffs
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    w.show();
    return a.exec();
}
