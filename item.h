#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Item : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Item(const QString& spritePath, int ancho, int alto, QGraphicsItem* parent = nullptr);
    void mover();
    bool estaFueraDePantalla() const;
    QString getruta();
private:
    QString rutasprite;
};

#endif // ITEM_H
