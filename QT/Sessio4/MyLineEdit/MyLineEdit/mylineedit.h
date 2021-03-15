#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QWidget>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MyLineEdit; }
QT_END_NAMESPACE

class MyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    MyLineEdit(QWidget *parent);
    ~MyLineEdit();

public slots:
    void tractaReturn();

signals:
    void enviaText(const QString &);

private:
    Ui::MyLineEdit *ui;
};
#endif // MYLINEEDIT_H
