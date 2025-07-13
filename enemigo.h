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
    Enemigo(int x, int y, int ancho, int alto, int velocidad, int vidas, const QString& rutaSprite);
    ~Enemigo() override = default;

    // Implementación de los métodos puros
    void mover() override;
    void recibirDano(int cantidad) override;

    void iniciarAnimacionDisparo();
    void animarMovimiento();

signals:
    void disparar();

private slots:
    void actualizarAnimacion();
    void finParpadeo();

private:
    QVector<QPixmap> framesDisparo;
    QPixmap frameFinal;
    int frameActual = 0;
    QTimer* timerAnimacion = nullptr;

    // Para el parpadeo al recibir daño
    QTimer* timerParpadeo = nullptr;
    int parpadeosRestantes = 0;

    int anchoSprite, altoSprite;
};

#endif // ENEMIGO_H

