#include "mylineedit.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyLineEdit w;
    w.show();
    return a.exec();
}
