#include "ecodetext.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EcoDeText w;
    w.show();
    return a.exec();
}
