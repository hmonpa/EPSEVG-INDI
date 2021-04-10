#ifndef EXERCICI1_H
#define EXERCICI1_H

#include <QWidget>      // Queremos crear nuestros propios widgets de QT
                        // crearemos clases derivadas de QT para programar los slots que queramos
                        // en este caso, derivamos de QWidget para diseñar nuevas componentes gráficas

#include <QLabel>       // Se añade manualmente
#include <QLCDNumber>   // Se añade manualmente

QT_BEGIN_NAMESPACE
namespace Ui { class Exercici1; }
QT_END_NAMESPACE

class Exercici1 : public QWidget // Cambiamos el public QWidget por public QLCDNumber
{
    Q_OBJECT
    // Importante

public:
    Exercici1(QWidget *parent = nullptr);
    ~Exercici1();
/*
public slots:       // Los slots van a parte, se implementarán en el .cpp
    void LCDzero();
    void colors(int);*/

private:
    Ui::Exercici1 *ui;
};
#endif // EXERCICI1_H
