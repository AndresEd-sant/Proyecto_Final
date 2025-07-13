#include "enemigo.h"
#include <QTimer>
#include <QtMath>
#include <QRandomGenerator>

// Constructor: enemigo de disparo (nivel 3, como Roshi)
Enemigo::Enemigo(int x, int y, int ancho, int alto, int velocidad, int vidas, const QString& rutaSprite)
    : QObject(nullptr)
    , Personaje(x, y, ancho, alto, velocidad, vidas)
    , anchoSprite(ancho)
    , altoSprite(alto)
{
    QPixmap sheet(rutaSprite);
    int cols = 6;
    float w = sheet.width() / cols;
    float h = sheet.height() / 4;

    for (int i = 0; i < 4; ++i)
        framesDisparo.append(sheet.copy(i * w, 0, w, h).scaled(anchoSprite, altoSprite));

    frameFinal = sheet.copy(3 * w, 0, w, h).scaled(anchoSprite, altoSprite);
    setPixmap(framesDisparo[3]);

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Enemigo::actualizarAnimacion);
}

// Constructor: enemigo móvil como tiburón (nivel 2)
Enemigo::Enemigo(QVector<QPixmap> sprites, int x, int y, int velocidad)
    : QObject(nullptr)
    , Personaje(x, y, sprites[0].width(), sprites[0].height(), velocidad, 1)
    , spritesMovimiento(sprites)
    , esTiburon(true)
    , paso(0)
    , baseY(y)
{
    if (!spritesMovimiento.isEmpty()) {
        setPixmap(spritesMovimiento[0]);
    }
    setPos(x, y);
}

// Movimiento principal
void Enemigo::mover() {
    if (esTiburon) {
        // Movimiento lineal + oscilación
        moveBy(-velocidad, 0);
        paso++;
        int yOffset = 20 * qSin(paso * 0.1);
        setY(baseY + yOffset);

        // Animación de sprites
        contadorFramesMovimiento++;
        if (contadorFramesMovimiento % 5 == 0 && !spritesMovimiento.isEmpty()) {
            frameMovimiento = (frameMovimiento + 1) % spritesMovimiento.size();
            setPixmap(spritesMovimiento[frameMovimiento]);
        }

    } else {
        // Enemigos que no se mueven directamente (Roshi se anima)
        animarMovimiento();
    }
}

// Animación de movimiento para enemigos tipo Roshi
void Enemigo::animarMovimiento() {
    static float t = 0.0f;
    t += 0.03f;

    const float centerX = 900.0f;
    const float amplitude = 200.0f;
    float x = centerX + amplitude * qSin(t);
    setX(static_cast<int>(x));

    static bool saltando = false;
    static float tSalto = 0.0f;
    const float yBase = 430.0f;
    const float v0 = 20.0f;
    const float g = 1.5f;

    if (!saltando && QRandomGenerator::global()->bounded(500) == 0) {
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

// Recibir daño con parpadeo
void Enemigo::recibirDano(int cantidad) {
    vidas -= cantidad;
    if (vidas < 0) vidas = 0;

    if (timerParpadeo) timerParpadeo->stop();
    parpadeosRestantes = 6;
    timerParpadeo = new QTimer(this);
    connect(timerParpadeo, &QTimer::timeout, this, &Enemigo::finParpadeo);
    timerParpadeo->start(100);
}

void Enemigo::finParpadeo() {
    setOpacity(opacity() < 1.0 ? 1.0 : 0.3);
    parpadeosRestantes--;
    if (parpadeosRestantes <= 0) {
        timerParpadeo->stop();
        setOpacity(1.0);
    }
}

// Animación de disparo (nivel 3)
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

