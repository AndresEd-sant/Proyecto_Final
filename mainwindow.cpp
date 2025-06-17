#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1200, 600); // o 800x600
}



void MainWindow::on_nivel_1_clicked() {
    // Preparar Nivel 1
    qDebug() << "Nivel 1 – Carrera con Leche";
    // Aquí podrías mostrar una nueva vista o iniciar lógica del nivel
}

void MainWindow::on_nivel_2_clicked() {
    qDebug() << "Nivel 2 – Lago con Piedra";
    // Lógica del nivel 2
}

void MainWindow::on_nivel_3_clicked() {
    qDebug() << "Nivel 3 – Entrenamiento Roshi";
    // Lógica del nivel 3
}



MainWindow::~MainWindow()
{
    delete ui;
}
