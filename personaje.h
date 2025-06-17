#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QPixmap>
#include <QPainter>

class Personaje {
protected:
    int x, y;               // Posición
    int ancho, alto;        // Dimensiones para hitbox
    int velocidad;
    int vidas;
    QPixmap sprite;

public:
    Personaje(int x, int y, int ancho, int alto, int velocidad, int vidas);
    virtual ~Personaje();

    // Métodos abstractos
    virtual void mover() = 0;
    virtual void dibujar(QPainter* painter) = 0;
    virtual void recibirDano(int cantidad) = 0;

    // Getters y Setters básicos
    int getX() const;
    int getY() const;
    int getVidas() const;
    QRect obtenerHitbox() const;
    void setX(int nuevoX);
    void setY(int nuevoY);
};

#endif // PERSONAJE_H
