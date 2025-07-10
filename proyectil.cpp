// proyectil.cpp
#include "proyectil.h"
#include <QPixmap>
#include <QtMath>
#include "jugador.h"

Proyectil::Proyectil(float velocidad, bool reflejadoPorJugador)
    : vel(velocidad), reflejado(reflejadoPorJugador)
{
    QPixmap sheet(":/proyectil.png");
    int cols = 5, rows = 4;
    int w = sheet.width() / cols;
    int h = sheet.height() / rows;

    int filaDeseada = 1; // Segunda fila (indexada desde 0)

    for (int x = 0; x < cols; ++x) {
        QPixmap frame = sheet.copy(x * w, filaDeseada * h, w, h).scaled(80, 80);
        animacion.append(frame);
    }

    setPixmap(animacion[0]);

    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, [this]() {
        frameActual = (frameActual + 1) % animacion.size();
        setPixmap(animacion[frameActual]);
    });
    animTimer->start(100);
}


void Proyectil::avanzar() {
    static float t = 0.0f;
    t += 0.1f;

    float radio = 60.0f;
    float dx = reflejado ? vel : -vel;
    float dy = radio * qCos(t);  // Movimiento vertical circular

    moveBy(dx, dy * 0.05f);  // Control vertical más suave

    QList<QGraphicsItem*> colisiones = collidingItems();
    for (QGraphicsItem* item : colisiones) {
        Jugador* j = dynamic_cast<Jugador*>(item);
        if (j && !reflejado) {
            if (j->estaEnModoContraataque()) {  // accede directamente
                reflejado = true;
                vel = 8;  // invertir la dirección
                break;
            }
        }
    }
}

bool Proyectil::fueraDePantalla() const {
    auto r = boundingRect();
    return x() + r.width() < 0 || x() > 1200;
}
