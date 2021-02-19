#include "conversionumerica.h"
#include "ui_conversionumerica.h"

ConversioNumerica::ConversioNumerica(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConversioNumerica)
{
    ui->setupUi(this);
}

ConversioNumerica::~ConversioNumerica()
{
    delete ui;
}

