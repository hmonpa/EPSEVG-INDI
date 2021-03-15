#ifndef ECODETEXT_H
#define ECODETEXT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class EcoDeText; }
QT_END_NAMESPACE

class EcoDeText : public QWidget
{
    Q_OBJECT

public:
    EcoDeText(QWidget *parent = nullptr);
    ~EcoDeText();

private:
    Ui::EcoDeText *ui;
};
#endif // ECODETEXT_H
