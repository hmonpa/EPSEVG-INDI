#include "escullcolor.h"
#include "ui_escullcolor.h"

EscullColor::EscullColor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EscullColor)
{
    ui->setupUi(this);
}

EscullColor::~EscullColor()
{
    delete ui;
}

