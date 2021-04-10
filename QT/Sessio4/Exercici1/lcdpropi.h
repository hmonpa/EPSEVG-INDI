#ifndef LCDPROPI_H
#define LCDPROPI_H

#include <QLCDNumber>
class LCDpropi: public QLCDNumber
{
    Q_OBJECT
    public:
        LCDpropi(QWidget *parent);

    public slots:       // Los slots van a parte, se implementarán en el .cpp
        void LCDzero();
        void colors(int);


};

#endif // LCDPROPI_H
