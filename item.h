#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Item : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
private:
private:
    int ancho, alto;
public:
    Item(const QString& spritePath, int ancho, int alto);
    void mover();
    bool estaFueraDePantalla() const;
};

#endif // ITEM_H
