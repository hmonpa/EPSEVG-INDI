#ifndef EXERCICI2_H
#define EXERCICI2_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Exercici2; }
QT_END_NAMESPACE

class Exercici2 : public QWidget
{
    Q_OBJECT

public:
    Exercici2(QWidget *parent = nullptr);
    ~Exercici2();

private:
    Ui::Exercici2 *ui;
};
#endif // EXERCICI2_H
