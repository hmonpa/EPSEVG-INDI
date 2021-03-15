#include "escullcolor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EscullColor w;
    w.show();
    return a.exec();
}
