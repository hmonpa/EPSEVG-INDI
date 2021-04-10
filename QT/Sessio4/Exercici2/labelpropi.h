#ifndef LABELPROPI_H
#define LABELPROPI_H

#include <QLabel>                   // Añadimos la clase QLABEL

class Labelpropi: public QLabel     // .. y la extenderemos
{
    Q_OBJECT
public:
    Labelpropi(QWidget *parent);
    int R;
    int G;
    int B;

public slots:
    void numR(int n);
    void numG(int n);
    void numB(int n);
    void Ok();
};

#endif // LABELPROPI_H
