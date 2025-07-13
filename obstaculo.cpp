#include "obstaculo.h"

Obstaculo::Obstaculo(const QString& rutaSprite, int ancho, int alto, const QString& tipo, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), tipoMovimiento(tipo)
{
    QPixmap sprite(rutaSprite);

    if (tipo != "") {
        int frameWidth = sprite.width() / 2;
        int frameHeight = sprite.height() / 2;

        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 2; ++col) {
                QPixmap frame = sprite.copy(col * frameWidth, row * frameHeight, frameWidth, frameHeight)
                .scaled(ancho, alto).transformed(QTransform().scale(-1, 1));;
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

    // Inicializar física para rebote
    if (tipo == "parabolico") {
        posYInicial = y();
        velocidadY = -10;   // Salto inicial hacia arriba
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
        moveBy(-3, 0);  // Abeja
    }
    else if (tipoMovimiento == "parabolico") {
        // Movimiento parabólico con rebote fijo
        velocidadY += gravedad;
        moveBy(-4, velocidadY);

        const qreal sueloY = 450;  // <-- Rebota en esta posición fija

        if (y() >= sueloY) {
            setY(sueloY);
            velocidadY = -velocidadY * 0.9;  // Rebote con pérdida de energía
            if (std::abs(velocidadY) < 1.5) {
                velocidadY = 0; // Detener si es muy pequeño
            }
        }
    }
    else {
        moveBy(-2, 0);  // Piedra
    }
}


bool Obstaculo::estaFueraDePantalla() const {
    return x() + boundingRect().width() < 0;
}
