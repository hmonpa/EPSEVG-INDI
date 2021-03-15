#ifndef CONVERSIONUMERICA_H
#define CONVERSIONUMERICA_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ConversioNumerica; }
QT_END_NAMESPACE

class ConversioNumerica : public QWidget
{
    Q_OBJECT

public:
    ConversioNumerica(QWidget *parent = nullptr);
    ~ConversioNumerica();

private:
    Ui::ConversioNumerica *ui;
};
#endif // CONVERSIONUMERICA_H
