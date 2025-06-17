#ifndef JUGADOR_H
#define JUGADOR_H

#include "personaje.h"
#include <QString>
#include <QPixmap>

class Jugador : public Personaje {
private:
    int vidas;
    float oxigeno;
    int puntaje;

    // Para salto parabólico Nivel 1
    bool saltando;
    int tiempoSalto;
    float velocidadSalto;
    float gravedad;
    float posicionInicialSalto;

public:
    Jugador(int x, int y, int ancho, int alto, int velocidad, int vidas);

    // Movimiento específico por nivel
    void moverCarrera();       // Nivel 1 – scroll automático + salto parabólico
    void iniciarSalto();
    void moverAcuatico();      // Nivel 2 – nado con resistencia
    void moverEntrenamiento(); // Nivel 3 – esquivar y contraatacar


    // Gestión de estado
    void perderVida();
    void reiniciar();

    // Getters / Setters
    int getVidas() const;
    void setVidas(int v);

    float getOxigeno() const;
    void setOxigeno(float o);

    int getPuntaje() const;
    void setPuntaje(int p);
};

#endif // JUGADOR_H
