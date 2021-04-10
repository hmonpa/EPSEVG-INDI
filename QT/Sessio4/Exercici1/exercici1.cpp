#include "exercici1.h"
#include "ui_exercici1.h"

Exercici1::Exercici1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Exercici1)
{
    ui->setupUi(this);
}

Exercici1::~Exercici1()
{
    delete ui;
}


/*void Exercici1::colors(int n){
    if (intValue() == 0){
        setStyleSheet("color:green");
        display(n);
    }
    else if (intValue() % 2 == 0){
        setStyleSheet("color:blue");
        display(n);
    }
    else if (intValue() % 2 != 0){
        setStyleSheet("color:red");
        display(n);
    }
}

void Exercici1::LCDzero(){
    display(0);
}*/
