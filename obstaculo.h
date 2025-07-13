#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QPixmap>

class Obstaculo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Obstaculo(const QString& rutaSprite, int ancho, int alto, const QString& tipo = "", int velocidad = 3, QGraphicsItem* parent = nullptr);

    void mover();
    void animar();
    bool estaFueraDePantalla() const;
    int velocidadX = 3;


private:
    QString tipoMovimiento;
    QVector<QPixmap> frames;
    int frameActual = 0;
    int tiempo = 0; // tiempo acumulado (solo para parabolico)
    QTimer* animTimer = nullptr;
    qreal velocidadY = 0;
    qreal gravedad = 0.5;
    qreal posYInicial = 0;
};

#endif // OBSTACULO_H
