#ifndef ESCULLCOLOR_H
#define ESCULLCOLOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class EscullColor; }
QT_END_NAMESPACE

class EscullColor : public QWidget
{
    Q_OBJECT

public:
    EscullColor(QWidget *parent = nullptr);
    ~EscullColor();

private:
    Ui::EscullColor *ui;
};
#endif // ESCULLCOLOR_H
