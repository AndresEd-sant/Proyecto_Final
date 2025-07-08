#include "personaje.h"

Personaje::Personaje(int x, int y, int ancho, int alto, int velocidad, int vidas)
    : ancho(ancho), alto(alto), velocidad(velocidad), vidas(vidas) {
    setPos(x, y);
}

Personaje::~Personaje() {}

int Personaje::getX() const { return pos().x(); }
int Personaje::getY() const { return pos().y(); }
int Personaje::getVidas() const { return vidas; }

QRectF Personaje::obtenerHitbox() const {
    return boundingRect();
}

void Personaje::setX(int nuevoX) { setPos(nuevoX, pos().y()); }
void Personaje::setY(int nuevoY) { setPos(pos().x(), nuevoY); }
