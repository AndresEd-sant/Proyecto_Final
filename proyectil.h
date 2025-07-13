// proyectil.h
#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QTimer>

class Proyectil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    // velocidad y si es reflejado por el jugador
    Proyectil(float velocidad, bool reflejadoPorJugador);

    void avanzar();
    bool fueraDePantalla() const;
    bool esReflejado() const { return reflejado; }
    void setReflejado(bool tipo) {reflejado = tipo;}

private:
    QVector<QPixmap> animacion;
    int frameActual = 0;
    QTimer* animTimer;
    float vel;
    bool reflejado;
};

#endif // PROYECTIL_H
