#ifndef ZARA_MAINWINDOW_H
#define ZARA_MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class ZARAMainWindow; }
QT_END_NAMESPACE

class ZARAMainWindow : public QMainWindow
{
Q_OBJECT

public:

    ZARAMainWindow(QWidget *parent = nullptr);

private:

    Ui::ZARAMainWindow *ui;

    void init();

};
#endif // ZARA_MAINWINDOW_H
