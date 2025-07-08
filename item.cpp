#include "item.h"

Item::Item(const QString& rutaSprite, int ancho, int alto)
    : ancho(ancho), alto(alto)
{
    QPixmap sprite(rutaSprite);
    setPixmap(sprite.scaled(ancho, alto));
}


bool Item::estaFueraDePantalla() const {
    return x() + ancho < 0;
}


void Item::mover() {
    moveBy(-2, 0); // Se mueve igual que los obstáculos
}

