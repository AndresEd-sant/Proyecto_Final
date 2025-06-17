#include "jugador.h"
#include <QDebug>
#include <qmath.h>

Jugador::Jugador(int x, int y, int ancho, int alto, int velocidad, int vidas)
    : Personaje(x, y, ancho, alto, velocidad, vidas),
    oxigeno(100.0f), puntaje(0),
    saltando(false), tiempoSalto(0),
    velocidadSalto(10.0f), gravedad(0.5f),
    posicionInicialSalto(0) {}


// -------------------- MOVIMIENTOS POR NIVEL --------------------

void Jugador::moverCarrera() {
    // Movimiento horizontal automático
    setX(getX() + velocidad); // `velocidad` heredada de Personaje

    // Lógica de salto parabólico (puede mejorar)
    if (saltando) {
        // actualizar posición Y por física básica
        float t = tiempoSalto++;
        setY(getY() - (velocidadSalto * t - 0.5 * gravedad * t * t));
    }
}

void Jugador::iniciarSalto() {
    if (!saltando) {
        saltando = true;
        tiempoSalto = 0;
        posicionInicialSalto = getY();
    }
}

void Jugador::moverAcuatico() {
    // Movimiento con oscilación y resistencia
    float nuevaX = getX() + velocidad * 0.5;
    float nuevaY = getY() + qSin(getX() * 0.1) * 2; // oscilación suave

    // Resistencia del agua: reduce velocidad con el tiempo
    velocidad *= 0.98;

    setX(nuevaX);
    setY(nuevaY);

    oxigeno -= 0.2;
}

void Jugador::moverEntrenamiento() {
    // Aquí podrías chequear si hay proyectiles cerca para esquivar
    // o si acaba de esquivar para contraatacar
    qDebug() << "Movimiento de entrenamiento aún en construcción";
}

// -------------------- ESTADO --------------------

void Jugador::perderVida() {
    vidas--;
    if (vidas < 0) vidas = 0;
}

void Jugador::reiniciar() {
    vidas = 3;
    oxigeno = 100.0f;
    puntaje = 0;
    setX(0);
    setY(0);
    velocidad = 5;
}

// -------------------- GETTERS Y SETTERS --------------------

int Jugador::getVidas() const { return vidas; }
void Jugador::setVidas(int v) { vidas = v; }

float Jugador::getOxigeno() const { return oxigeno; }
void Jugador::setOxigeno(float o) { oxigeno = o; }

int Jugador::getPuntaje() const { return puntaje; }
void Jugador::setPuntaje(int p) { puntaje = p; }
