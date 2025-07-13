#include "pausa_panel.h"
#include <QLabel>

PausaPanel::PausaPanel(QWidget* parent) : QWidget(parent) {
    setFixedSize(400, 300);
    setStyleSheet("background-color: rgba(0, 0, 0, 180); color: white; border-radius: 10px;");

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titulo = new QLabel("PAUSA");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 24px; font-weight: bold;");
    layout->addWidget(titulo);

    QPushButton* btnReanudar = new QPushButton("Reanudar");
    QPushButton* btnReiniciar = new QPushButton("Reiniciar nivel");
    QPushButton* btnSalir = new QPushButton("Salir al menú");

    layout->addWidget(btnReanudar);
    layout->addWidget(btnReiniciar);
    layout->addWidget(btnSalir);

    connect(btnReanudar, &QPushButton::clicked, this, &PausaPanel::reanudarJuego);
    connect(btnReiniciar, &QPushButton::clicked, this, &PausaPanel::reiniciarNivel);
    connect(btnSalir, &QPushButton::clicked, this, &PausaPanel::salirAlMenu);
}

void PausaPanel::mostrarConAnimacion() {
    setWindowOpacity(0.0);  // inicial
    show();

    auto* efecto = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(efecto);

    QPropertyAnimation* anim = new QPropertyAnimation(efecto, "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
