#include "personaje.h"

Personaje::Personaje(int x, int y, int ancho, int alto, int velocidad, int vidas)
    : x(x), y(y), ancho(ancho), alto(alto), velocidad(velocidad), vidas(vidas) {}

Personaje::~Personaje() {}

int Personaje::getX() const { return x; }
int Personaje::getY() const { return y; }
int Personaje::getVidas() const { return vidas; }

QRect Personaje::obtenerHitbox() const {
    return QRect(x, y, ancho, alto);
}

void Personaje::setX(int nuevoX) { x = nuevoX; }
void Personaje::setY(int nuevoY) { y = nuevoY; }
