#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QObject>
#include <QVector>
#include <QPixmap>
#include <QTimer>
#include "personaje.h"

class Enemigo : public QObject, public Personaje {
    Q_OBJECT

public:
    // Constructor para enemigos de disparo (nivel 3)
    Enemigo(int x, int y, int ancho, int alto, int velocidad, int vidas, const QString& rutaSprite);

    // Constructor para enemigos móviles como tiburones (nivel 2)
    Enemigo(QVector<QPixmap> spritesMovimiento, int x, int y, int velocidad);

    void mover() override;
    void recibirDano(int cantidad) override;
    void iniciarAnimacionDisparo();

    // Solo para enemigos tipo Roshi
    void animarMovimiento();

signals:
    void disparar();

private slots:
    void actualizarAnimacion();
    void finParpadeo();

private:
    // ------- Para animación de disparo (nivel 3) -------
    QVector<QPixmap> framesDisparo;
    QPixmap frameFinal;
    int frameActual = 0;
    QTimer* timerAnimacion = nullptr;

    // ------- Para animación de movimiento (nivel 2) -------
    QVector<QPixmap> spritesMovimiento;
    int frameMovimiento = 0;
    int contadorFramesMovimiento = 0;
    int paso = 0;

    // ------- Parpadeo por daño -------
    QTimer* timerParpadeo = nullptr;
    int parpadeosRestantes = 0;

    // ------- Datos generales -------
    int anchoSprite = 0;
    int altoSprite = 0;
    int baseY = 0;
    bool esTiburon = false;
};

#endif // ENEMIGO_H

