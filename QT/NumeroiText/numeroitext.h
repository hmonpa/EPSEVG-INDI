#ifndef NUMEROITEXT_H
#define NUMEROITEXT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class numeroiText; }
QT_END_NAMESPACE

class numeroiText : public QWidget
{
    Q_OBJECT

public:
    numeroiText(QWidget *parent = nullptr);
    ~numeroiText();

private:
    Ui::numeroiText *ui;
};
#endif // NUMEROITEXT_H
