#include "ecodetext.h"
#include "ui_ecodetext.h"

EcoDeText::EcoDeText(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EcoDeText)
{
    ui->setupUi(this);
}

EcoDeText::~EcoDeText()
{
    delete ui;
}

