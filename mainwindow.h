#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nivel.h"
#include <QMediaPlayer>
#include <QAudioOutput>


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
    int nivelActualTipo = 1;
    int nivelActualModo = 0;
    void reiniciarNivelActual();

private slots:
    void on_nivel_1_clicked();
    void on_nivel_2_clicked();
    void on_nivel_3_clicked();

private:
    Nivel* nivelActual;
    Ui::MainWindow *ui;
    QMediaPlayer* musicaMenu;
    QAudioOutput* salidaAudioMenu;

};
#endif // MAINWINDOW_H
