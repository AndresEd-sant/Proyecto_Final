#include "enemigo.h"
#include <QTimer>
#include <QtMath>
#include <QRandomGenerator>
#include <QGraphicsOpacityEffect>

Enemigo::Enemigo(int x, int y, int ancho, int alto, int velocidad, int vidas, const QString& rutaSprite)
    : QObject(nullptr)
    , Personaje(x, y, ancho, alto, velocidad, vidas)
    , anchoSprite(ancho)
    , altoSprite(alto)
{
    QPixmap sheet(rutaSprite);
    int cols = 6;  // Hay 6 columnas en la primera fila, pero solo usamos las primeras 4 (índices 0-3)
    int w = 611 / cols;
    int h = 409 / 3;

    for (int i = 0; i < 4; ++i)  // Solo los primeros 4 cuadros de la fila 0
        framesDisparo.append(sheet.copy(i * w, 0, w, h).scaled(anchoSprite, altoSprite));

    // El último frame útil (el 4to) será el de disparo
    frameFinal = sheet.copy(3 * w, 0, w, h).scaled(anchoSprite, altoSprite);
    setPixmap(framesDisparo[0]);

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Enemigo::actualizarAnimacion);
}



void Enemigo::animarMovimiento() {
    static float t = 0.0f;
    t += 0.03f;

    const float centerX = 900.0f;
    const float amplitude = 100.0f;
    float x = centerX + amplitude * qCos(t); // cambio de sin a cos
    setX(static_cast<int>(x));

    // Salto parabólico discreto
    static bool saltando = false;
    static float tSalto = 0.0f;
    const float yBase = 430.0f;
    const float v0 = 12.0f;
    const float g = 9.8f;


    if (!saltando && QRandomGenerator::global()->bounded(200) == 0) {
        saltando = true;
        tSalto = 0.0f;
    }

    if (saltando) {
        tSalto += 0.1f;
        float dy = v0 * tSalto - 0.5f * g * tSalto * tSalto;
        setY(static_cast<int>(yBase - dy));

        if (dy <= 0.0f) {
            setY(static_cast<int>(yBase));
            saltando = false;
        }
    }
}


void Enemigo::mover() {
    // Su movimiento lo gestiona animarMovimiento()
}

void Enemigo::recibirDano(int cantidad) {
    vidas -= cantidad;
    if (vidas < 0) vidas = 0;

    // Parpadeo simple: alternar opacidad 6 veces
    if (timerParpadeo) timerParpadeo->stop();
    parpadeosRestantes = 6;
    timerParpadeo = new QTimer(this);
    connect(timerParpadeo, &QTimer::timeout, this, &Enemigo::finParpadeo);
    timerParpadeo->start(100);
}


void Enemigo::finParpadeo() {
    // Alternar opacidad
    setOpacity(opacity() < 1.0 ? 1.0 : 0.3);
    --parpadeosRestantes;
    if (parpadeosRestantes <= 0) {
        timerParpadeo->stop();
        setOpacity(1.0);
    }
}

void Enemigo::iniciarAnimacionDisparo() {
    frameActual = 0;
    timerAnimacion->start(100);
}

void Enemigo::actualizarAnimacion() {
    if (frameActual < framesDisparo.size()) {
        setPixmap(framesDisparo[frameActual++]);
    } else {
        timerAnimacion->stop();
        setPixmap(frameFinal);
        emit disparar();
    }
}

