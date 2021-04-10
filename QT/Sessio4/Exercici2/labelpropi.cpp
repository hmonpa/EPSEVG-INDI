#include "labelpropi.h"

Labelpropi::Labelpropi(QWidget *parent): QLabel(parent)
{

}

void Labelpropi::numR(int n){
    R = n;
}

void Labelpropi::numG(int n){
    G = n;
}

void Labelpropi::numB(int n){
    B = n;
}

void Labelpropi::Ok(){
    QString a = "background-color: rgb(";
    QString b = QString::number(R, 10);
    QString c = QString::number(G, 10);
    QString d = QString::number(B, 10);
    QString e = ")";
    QString f = ",";
    a+=b;
    a+=f;
    a+=c;
    a+=f;
    a+=d;
    a+=e;
    setStyleSheet(a);
}
