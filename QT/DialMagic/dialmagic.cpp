#include "dialmagic.h"
#include "ui_dialmagic.h"

DialMagic::DialMagic(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DialMagic)
{
    ui->setupUi(this);
}

DialMagic::~DialMagic()
{
    delete ui;
}

