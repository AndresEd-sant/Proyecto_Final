#include "item.h"

Item::Item(const QString& rutaSprite, int ancho, int alto, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    QPixmap sprite(rutaSprite);
    setPixmap(sprite.scaled(ancho, alto));
}

void Item::mover() {
    moveBy(-2, 0);
}

bool Item::estaFueraDePantalla() const {
    return x() + boundingRect().width() < 0;
}

