#ifndef JUGADOR_H
#define JUGADOR_H

#include "personaje.h"
#include "proyectil.h"
#include <QString>
#include <QTimer>
#include <QKeyEvent>
#include <QObject>
#include <QVector>
#include <QList>

class Jugador : public QObject, public Personaje {
    Q_OBJECT

private:
    int puntaje;
    float oxigeno;
    bool descensoRapido = false;
    bool puedeContraatacar = true;
    bool agachado = false;

    // Estado general
    bool enModoCarrera;
    bool saltando =false;
    int tiempoSalto=0;
    float velocidadSalto;
    float gravedad;
    float posicionInicialSalto=0;
    bool dobleSaltoDisponible = true;
    bool enElAire = false;
    bool pausaActiva;

    bool modoContraataque = false;
    bool disparoActivo = false;
    int cantidadProyectiles = 1;  // Proyectiles que puede lanzar
    QTimer* timerDisparo = nullptr;


    QTimer* saltoTimer;
    QTimer* timerInvulnerabilidad;
    QTimer* timerContraataque;
    QTimer* timerCooldown;

    int contadorParpadeo;
    bool invulnerable;

    // Sprites carrera
    QVector<QPixmap> spritesCorrer;
    QPixmap spriteSaltar;
    int frameActualCorrer;
    QTimer* animacionCorrerTimer;

    // Sprites acuático
    QVector<QPixmap> spritesAcuaticos;
    int frameActualAcuatico;
    QTimer* animacionAcuaticaTimer;

    QGraphicsPixmapItem* rocaFinal;


public:
    Jugador(int x, int y, int ancho, int alto, int velocidad, int vidas, const QString& rutaSprite, bool modoCarrera);

    // Movimientos por nivel
    void moverCarrera();
    void moverAcuatico();
    void moverEntrenamiento();

    void iniciarSalto();
    void actualizarSalto();

    void perderVida();
    void reiniciar();
    void recibirDano(int cantidad) override;
    void mover() override {} // no se usa directamente
    void verificarMejoraProyectiles();
    void verificarProyectilesActivos();
    void lanzarProyectil();
    QList<Proyectil*> proyectilesActivos;




    // Getters y estado
    int getVidas() const;
    void setVidas(int v);
    float getOxigeno() const;
    void setOxigeno(float o);
    int getPuntaje() const;
    void setPuntaje(int p);
    bool estaEnModoContraataque() const { return modoContraataque; }
    bool esModoCarrera() const;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event);


private slots:
    void actualizarParpadeo();
    void actualizarSpriteCorrer();
    void actualizarSpriteAcuatico();

signals:
    void contraatacar();
    void disparar(float x, float y);
    void pausarJuego();

};

#endif // JUGADOR_H

