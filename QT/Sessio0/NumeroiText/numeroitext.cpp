#include "numeroitext.h"
#include "ui_numeroitext.h"

numeroiText::numeroiText(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::numeroiText)
{
    ui->setupUi(this);
}

numeroiText::~numeroiText()
{
    delete ui;
}

