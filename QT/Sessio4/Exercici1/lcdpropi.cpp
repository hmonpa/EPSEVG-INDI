#include "lcdpropi.h"

LCDpropi::LCDpropi(QWidget *parent):QLCDNumber(parent)
{

}

void LCDpropi::LCDzero(){
    display(0);
    colors(0);
}

void LCDpropi::colors(int n){
    if (n == 0){
        setSegmentStyle(Flat);
        setStyleSheet("color:green");
        display(n);
    }
    else if (n % 2 == 0){
        setSegmentStyle(Flat);
        setStyleSheet("color:blue");
        display(n);
    }
    else if (n % 2 != 0){
        setSegmentStyle(Flat);
        setStyleSheet("color:red");
        display(n);
    }
}
