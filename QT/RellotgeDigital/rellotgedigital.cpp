#include "rellotgedigital.h"
#include "ui_rellotgedigital.h"

RellotgeDigital::RellotgeDigital(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RellotgeDigital)
{
    ui->setupUi(this);
}

RellotgeDigital::~RellotgeDigital()
{
    delete ui;
}

