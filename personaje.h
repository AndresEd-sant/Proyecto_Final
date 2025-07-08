#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>

class Personaje : public QGraphicsPixmapItem {
protected:
    int ancho, alto;
    int velocidad;
    int vidas;
    QPixmap sprite;

public:
    Personaje(int x, int y, int ancho, int alto, int velocidad, int vidas);
    virtual ~Personaje();

    virtual void mover() = 0;
    virtual void recibirDano(int cantidad) = 0;

    int getX() const;
    int getY() const;
    int getVidas() const;
    QRectF obtenerHitbox() const;

    void setX(int nuevoX);
    void setY(int nuevoY);
};

#endif // PERSONAJE_H
