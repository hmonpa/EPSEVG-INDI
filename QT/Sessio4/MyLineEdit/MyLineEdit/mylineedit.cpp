#include "mylineedit.h"
#include "ui_mylineedit.h"

MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit(parent)
    // El constructor siempre ha de llamar al constructor de la clase base
{


}

MyLineEdit::~MyLineEdit()
{
}

void MyLineEdit::tractaReturn(){
    // Este slot sólo produce un nuevo signal que es quien envia el texto
    emit enviaText(text());
}
