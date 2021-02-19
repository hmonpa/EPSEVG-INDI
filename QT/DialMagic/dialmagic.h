#ifndef DIALMAGIC_H
#define DIALMAGIC_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class DialMagic; }
QT_END_NAMESPACE

class DialMagic : public QWidget
{
    Q_OBJECT

public:
    DialMagic(QWidget *parent = nullptr);
    ~DialMagic();

private:
    Ui::DialMagic *ui;
};
#endif // DIALMAGIC_H
