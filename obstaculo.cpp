#include "obstaculo.h"

Obstaculo::Obstaculo(const QString& rutaSprite, int ancho, int alto, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    QPixmap sprite(rutaSprite);
    sprite = sprite.scaled(ancho, alto);
    setPixmap(sprite);
}

void Obstaculo::mover() {
    moveBy(-2, 0);  // Se mueve igual que el fondo
}

bool Obstaculo::estaFueraDePantalla() const {
    return x() + boundingRect().width() < 0;
}
