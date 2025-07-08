#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Obstaculo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Obstaculo(const QString& rutaSprite, int ancho, int alto, QGraphicsItem* parent = nullptr);
    void mover();     // Se moverá con el fondo
    bool estaFueraDePantalla() const;
};

#endif // OBSTACULO_H
