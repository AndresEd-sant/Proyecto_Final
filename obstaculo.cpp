#include "obstaculo.h"

Obstaculo::Obstaculo(const QString& rutaSprite, int ancho, int alto, const QString& tipo, int velocidad, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), tipoMovimiento(tipo), velocidadX(velocidad)
{
    QPixmap sprite(rutaSprite);

    if (tipo != "") {
        int frameWidth = sprite.width() / 2;
        int frameHeight = sprite.height() / 2;

        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 2; ++col) {
                QPixmap frame = sprite.copy(col * frameWidth, row * frameHeight, frameWidth, frameHeight)
                .scaled(ancho, alto).transformed(QTransform().scale(-1, 1));
                frames.append(frame);
            }
        }

        setPixmap(frames[0]);

        animTimer = new QTimer(this);
        connect(animTimer, &QTimer::timeout, this, &Obstaculo::animar);
        animTimer->start(150);
    } else {
        setPixmap(sprite.scaled(ancho, alto));
    }

    if (tipo == "parabolico") {
        posYInicial = y();
        velocidadY = -10;
        gravedad = 0.3;
    }
}

void Obstaculo::animar() {
    if (frames.isEmpty()) return;
    frameActual = (frameActual + 1) % frames.size();
    setPixmap(frames[frameActual]);
}

void Obstaculo::mover() {
    if (tipoMovimiento == "lineal") {
        moveBy(-velocidadX, 0);
    } else if (tipoMovimiento == "parabolico") {
        velocidadY += gravedad;
        moveBy(-velocidadX, velocidadY);

        const qreal sueloY = 450;
        if (y() >= sueloY) {
            setY(sueloY);
            velocidadY = -velocidadY * 0.9;
            if (std::abs(velocidadY) < 1.5) {
                velocidadY = 0;
            }
        }
    } else {
        moveBy(-velocidadX, 0);
    }
}


bool Obstaculo::estaFueraDePantalla() const {
    return x() + boundingRect().width() < 0;
}
