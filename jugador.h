#ifndef JUGADOR_H
#define JUGADOR_H

#include "personaje.h"
#include <QString>
#include <QTimer>
#include <QKeyEvent>
#include <QObject>
#include <QVector>

class Jugador : public QObject, public Personaje {
    Q_OBJECT

private:
    int puntaje;
    float oxigeno;

    // Para salto parabólico (Nivel 1)
    bool saltando;
    int tiempoSalto;
    float velocidadSalto;
    float gravedad;
    float posicionInicialSalto;

    QTimer* saltoTimer;
    QTimer* timerInvulnerabilidad;
    bool invulnerable = false;
    int contadorParpadeo = 0;
    QVector<QPixmap> spritesCorrer;
    QPixmap spriteSaltar;
    int frameActualCorrer;
    QTimer* animacionCorrerTimer;

public:
    // Constructor
    Jugador(int x, int y, int ancho, int alto, int velocidad, int vidas, const QString& rutaSprite);

    // Movimientos por nivel
    void moverCarrera();       // Nivel 1
    void moverAcuatico();      // Nivel 2
    void moverEntrenamiento(); // Nivel 3
    void iniciarSalto();

    // Estado y lógica de juego
    void actualizarSalto();
    void perderVida();
    void reiniciar();
    void recibirDano(int cantidad) override;
    void mover() override {} // no se usa directamente

    // Getters / Setters
    int getVidas() const;
    void setVidas(int v);
    float getOxigeno() const;
    void setOxigeno(float o);
    int getPuntaje() const;
    void setPuntaje(int p);

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // JUGADOR_H
