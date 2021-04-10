#include "exercici2.h"
#include "ui_exercici2.h"

Exercici2::Exercici2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Exercici2)
{
    ui->setupUi(this);
}

Exercici2::~Exercici2()
{
    delete ui;
}

