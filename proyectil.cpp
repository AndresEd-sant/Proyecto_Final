// proyectil.cpp
#include "proyectil.h"
#include <QPixmap>
#include <QtMath>
#include "jugador.h"

Proyectil::Proyectil(float velocidad, bool reflejadoPorJugador)
    : vel(velocidad), reflejado(reflejadoPorJugador)
{
    QPixmap imagen(":/proyectil.png");
    imagen = imagen.scaled(50, 50);  // Ajusta el tamaño según tu sprite
    setPixmap(imagen);
}



void Proyectil::avanzar() {
    t += 0.1f;

    float radio = 60.0f;
    float dx = reflejado ? vel : -vel;
    float dy = radio * qCos(t);  // Movimiento vertical circular

    moveBy(dx, dy * 0.05f);

    QList<QGraphicsItem*> colisiones = collidingItems();
    for (QGraphicsItem* item : colisiones) {
        Jugador* j = dynamic_cast<Jugador*>(item);
        if (j && !reflejado) {
            if (j->estaEnModoContraataque()) {
                reflejado = true;
                vel = 8;
                break;
            }
        }
    }
}


bool Proyectil::fueraDePantalla() const {
    auto r = boundingRect();
    return x() + r.width() < 0 || x() > 1200;
}
