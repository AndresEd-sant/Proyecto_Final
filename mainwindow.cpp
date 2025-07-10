#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1200, 600);

    QLabel* fondoMenu = new QLabel(this);
    fondoMenu->setPixmap(QPixmap(":/menu.png").scaled(size()));
    fondoMenu->setGeometry(0, 0, 1200, 600);
    fondoMenu->lower(); // Manda al fondo

    // Asegúrate de que los botones estén visibles encima
    ui->nivel_1->raise();
    ui->nivel_2->raise();
    ui->nivel_3->raise();
}



void MainWindow::on_nivel_1_clicked() {
    qDebug() << "Nivel 1 – Carrera con Leche";
    ui->nivel_1->hide();
    ui->nivel_2->hide();
    ui->nivel_3->hide();

    nivelActual = new Nivel(this);
    connect(nivelActual, &Nivel::regresarAlMenu, this, [=]() {
        if (nivelActual) {
            nivelActual->limpiarTodo(); // 🔴 LIMPIAR TODO PRIMERO
            nivelActual->obtenerVista()->hide();
            nivelActual->obtenerVista()->setParent(nullptr);
            nivelActual->deleteLater();
            nivelActual = nullptr;
        }

        QTimer::singleShot(50, this, [=]() {
            ui->nivel_1->show();
            ui->nivel_2->show();
            ui->nivel_3->show();
        });
    });



    nivelActual->obtenerVista()->setParent(this);
    nivelActual->obtenerVista()->show();
    nivelActual->obtenerVista()->setFocus();
}

void MainWindow::on_nivel_2_clicked() {
    qDebug() << "Nivel 2 – Lago con Piedra";
    // Lógica del nivel 2
}

void MainWindow::on_nivel_3_clicked() {
    qDebug() << "Nivel 3 – Entrenamiento Roshi";
    ui->nivel_1->hide();
    ui->nivel_2->hide();
    ui->nivel_3->hide();
    nivelActual = new Nivel(this,1,2);

    connect(nivelActual, &Nivel::regresarAlMenu, this, [=]() {
        if (nivelActual) {
            nivelActual->limpiarTodo(); // 🔴 LIMPIAR TODO PRIMERO
            nivelActual->obtenerVista()->hide();
            nivelActual->obtenerVista()->setParent(nullptr);
            nivelActual->deleteLater();
            nivelActual = nullptr;
        }

        QTimer::singleShot(50, this, [=]() {
            ui->nivel_1->show();
            ui->nivel_2->show();
            ui->nivel_3->show();
        });
    });



    nivelActual->obtenerVista()->setParent(this);
    nivelActual->obtenerVista()->show();
    nivelActual->obtenerVista()->setFocus();
}



MainWindow::~MainWindow()
{
    delete ui;
}
