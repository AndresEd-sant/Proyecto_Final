#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nivel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_nivel_1_clicked();
    void on_nivel_2_clicked();
    void on_nivel_3_clicked();

private:
    Nivel* nivelActual;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
