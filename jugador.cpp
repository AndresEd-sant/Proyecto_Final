#include "jugador.h"
#include <QDebug>
#include <qmath.h>
#include <QTimer>

Jugador::Jugador(int x, int y, int ancho, int alto, int velocidad, int vidas, const QString& rutaSprite)
    :QObject(), Personaje(x, y, ancho, alto, velocidad, vidas),
    puntaje(0), oxigeno(100.0),
    saltando(false), tiempoSalto(0),
    velocidadSalto(20.0), gravedad(0.6),
    posicionInicialSalto(0)
{
    sprite.load(rutaSprite);
    sprite = sprite.scaled(ancho, alto);
    setPixmap(sprite);
    spritesCorrer.append(QPixmap(":/Goku_15.png").scaled(ancho, alto));
    spritesCorrer.append(QPixmap(":/Goku_16.png").scaled(ancho, alto));
    spriteSaltar = QPixmap(":/Goku_14.png").scaled(ancho, alto);

    // Iniciar con primer sprite
    setPixmap(spritesCorrer[0]);
    frameActualCorrer = 0;

    // Temporizador para animar correr
    animacionCorrerTimer = new QTimer(this);
    connect(animacionCorrerTimer, &QTimer::timeout, [this]() {
        if (!saltando) {
            frameActualCorrer = (frameActualCorrer + 1) % spritesCorrer.size();
            setPixmap(spritesCorrer[frameActualCorrer]);
        }
    });
    animacionCorrerTimer->start(200);  // cambia cada 200ms

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    saltoTimer = new QTimer(this);
    connect(saltoTimer, &QTimer::timeout, this, &Jugador::actualizarSalto);
}

// --------- Nivel 1: Carrera con Leche ---------

void Jugador::moverCarrera() {
    // Movimiento solo vertical si está saltando
    if (saltando) return; // el salto es animado por el QTimer
}

void Jugador::iniciarSalto() {
    if (!saltando) {
        saltando = true;
        tiempoSalto = 0;
        posicionInicialSalto = getY();
        setPixmap(spriteSaltar);
        saltoTimer->start(16); // ~60 FPS
    }
}

void Jugador::actualizarSalto() {
    float t = (++tiempoSalto) * 0.1f; // Aumentar antes de usarlo
    float nuevaY = posicionInicialSalto - (velocidadSalto * t - 0.5f * gravedad * t * t);

    qDebug() << "Actualizando salto. t =" << t << " nuevaY =" << nuevaY;

    if (nuevaY >= posicionInicialSalto) {
        setY(posicionInicialSalto);
        saltando = false;
        saltoTimer->stop();
        qDebug() << "Salto terminado";
    } else {
        setY(nuevaY);
    }
}



// --------- Nivel 2: Lago con Piedra ---------

void Jugador::moverAcuatico() {
    float nuevaX = getX() + velocidad * 0.5;
    float nuevaY = getY() + qSin(getX() * 0.1) * 2;

    velocidad *= 0.98;
    setX(nuevaX);
    setY(nuevaY);

    oxigeno -= 0.2;
}

// --------- Nivel 3: Entrenamiento Roshi ---------

void Jugador::moverEntrenamiento() {
    qDebug() << "Movimiento de entrenamiento aún en desarrollo.";
}

// --------- Entrada por teclado ---------

void Jugador::keyPressEvent(QKeyEvent* event) {

    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Space:
        iniciarSalto();
        break;
    case Qt::Key_A:
        setX(getX() - velocidad);
        break;
    case Qt::Key_D:
        setX(getX() + velocidad);
        break;
    case Qt::Key_S:
        qDebug() << "Tecla S presionada (acción opcional)";
        break;
    default:
        break;
    }
}

// --------- Estado del jugador ---------

void Jugador::perderVida() {
    if (invulnerable) return;

    vidas--;
    if (vidas < 0) vidas = 0;

    invulnerable = true;
    contadorParpadeo = 0;
    setOpacity(0.5);

    timerInvulnerabilidad = new QTimer(this);
    connect(timerInvulnerabilidad, &QTimer::timeout, this, [=]() {
        contadorParpadeo++;
        setOpacity((opacity() == 1.0) ? 0.3 : 1.0);

        if (contadorParpadeo > 10) {
            timerInvulnerabilidad->stop();
            delete timerInvulnerabilidad;
            timerInvulnerabilidad = nullptr;
            setOpacity(1.0);
            invulnerable = false;
        }
    });
    timerInvulnerabilidad->start(100);
}

void Jugador::reiniciar() {
    vidas = 3;
    oxigeno = 100.0f;
    puntaje = 0;
    setX(0);
    setY(0);
    velocidad = 5;
}

void Jugador::recibirDano(int cantidad) {
    perderVida();
}

// --------- Getters y Setters ---------

int Jugador::getVidas() const { return vidas; }
void Jugador::setVidas(int v) { vidas = v; }

float Jugador::getOxigeno() const { return oxigeno; }
void Jugador::setOxigeno(float o) { oxigeno = o; }

int Jugador::getPuntaje() const { return puntaje; }
void Jugador::setPuntaje(int p) { puntaje = p; }
