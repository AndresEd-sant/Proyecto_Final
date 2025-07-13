#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>

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
    // Inicializa audio
    musicaMenu = new QMediaPlayer(this);
    salidaAudioMenu = new QAudioOutput(this);

    musicaMenu->setAudioOutput(salidaAudioMenu);
    musicaMenu->setSource(QUrl("qrc:/sonidos/Intro_menu.mp3"));
    salidaAudioMenu->setVolume(34); // volumen 0-100
    musicaMenu->play();
    musicaMenu->setLoops(QMediaPlayer::Infinite);


    // Asegúrate de que los botones estén visibles encima
    ui->nivel_1->raise();
    ui->nivel_2->raise();
    ui->nivel_3->raise();

}



void MainWindow::on_nivel_1_clicked() {
    qDebug() << "Nivel 1 – Carrera con Leche";
    if (musicaMenu && musicaMenu->playbackState() == QMediaPlayer::PlayingState)
        musicaMenu->stop();
    ui->nivel_1->hide();
    ui->nivel_2->hide();
    ui->nivel_3->hide();


    nivelActualTipo = 1;   // Guardamos tipo y modo para reinicio
    nivelActualModo = 0;

    nivelActual = new Nivel(this, nivelActualTipo, nivelActualModo);

    connect(nivelActual, &Nivel::regresarAlMenu, this, [=]() {
        if (nivelActual) {
            nivelActual->limpiarTodo(); // 🔴 LIMPIAR TODO PRIMERO
            nivelActual->obtenerVista()->hide();
            nivelActual->obtenerVista()->setParent(nullptr);
            nivelActual->deleteLater();
            nivelActual = nullptr;
            if (musicaMenu) musicaMenu->play();

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
    if (musicaMenu && musicaMenu->playbackState() == QMediaPlayer::PlayingState)
        musicaMenu->stop();
    qDebug() << "Nivel 2 – Lago con Piedra";

    ui->nivel_1->hide();
    ui->nivel_2->hide();
    ui->nivel_3->hide();

    nivelActualTipo = 2;   // Guardamos tipo y modo para reinicio
    nivelActualModo = 1;

    nivelActual = new Nivel(this, nivelActualTipo, nivelActualModo);

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

void MainWindow::on_nivel_3_clicked() {
    if (musicaMenu && musicaMenu->playbackState() == QMediaPlayer::PlayingState)
        musicaMenu->stop();
    qDebug() << "Nivel 3 – Entrenamiento Roshi";
    ui->nivel_1->hide();
    ui->nivel_2->hide();
    ui->nivel_3->hide();

    nivelActualTipo = 3;   // Guardamos tipo y modo para reinicio
    nivelActualModo = 0;

    nivelActual = new Nivel(this, nivelActualTipo, nivelActualModo);

    connect(nivelActual, &Nivel::regresarAlMenu, this, [=]() {
        if (nivelActual) {
            nivelActual->limpiarTodo();
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

void MainWindow::reiniciarNivelActual() {
    if (nivelActual) {
        nivelActual->limpiarTodo();
        nivelActual->obtenerVista()->hide();
        nivelActual->obtenerVista()->setParent(nullptr);
        nivelActual->deleteLater();
        nivelActual = nullptr;
    }

    QTimer::singleShot(100, this, [=]() {
        nivelActual = new Nivel(this, nivelActualTipo, nivelActualModo);

        // 👉 Solo estos connect (nada de volverAlMenu si no existe)
        connect(nivelActual, &Nivel::regresarAlMenu, this, [=]() {
            if (nivelActual) {
                nivelActual->limpiarTodo();
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

        connect(nivelActual, &Nivel::reiniciarNivel, this, &MainWindow::reiniciarNivelActual);

        nivelActual->obtenerVista()->setParent(this);
        nivelActual->obtenerVista()->show();
        nivelActual->obtenerVista()->setFocus();
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}
