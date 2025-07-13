#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QVector>

#include "jugador.h"
#include "obstaculo.h"
#include "item.h"
#include "enemigo.h"
#include "proyectil.h"

class Nivel : public QObject {
    Q_OBJECT

private:
    // Escena y vista
    QGraphicsScene* escena = nullptr;
    QGraphicsView* vista = nullptr;

    // Jugador y enemigo
    Jugador* jugador = nullptr;
    Enemigo* enemigo = nullptr;

    // Fondos
    QPixmap fondo;
    QGraphicsPixmapItem *fondoItem1 = nullptr, *fondoItem2 = nullptr;
    int desplazamientoFondo = 0;

    // Timers
    QTimer* temporizador= nullptr;
    QTimer* generadorObstaculos = nullptr;
    QTimer* generadorItems = nullptr;
    QTimer* timerDisparo = nullptr;

    // Vidas
    QPixmap spriteVidas;
    QGraphicsPixmapItem* barraVidas= nullptr;

    // Tarros de leche
    QGraphicsPixmapItem* iconoLeche = nullptr;
    QGraphicsTextItem* textoLecheRestante= nullptr;
    int lecheRecolectada = 0;

    // Listas dinámicas
    QVector<Obstaculo*> obstaculos;
    QVector<Item*> items;
    QVector<Proyectil*> proyectiles;

    // Utilidades internas
    void actualizarBarraVidas();
    void actualizarContadorLeche();
    void mostrarResultado(const QString& mensaje);

public:
    // Constructores para cada nivel
    Nivel(QWidget* parent);                  // Nivel 1
    Nivel(QWidget* parent, int ID);          // Nivel 2
    Nivel(QWidget* parent, int ID, int ID2); // Nivel 3
    void limpiarTodo();

    virtual ~Nivel();

    QGraphicsPixmapItem* barraVidasRoshi = nullptr;
    void actualizarBarraVidasRoshi();

    QGraphicsView* obtenerVista() const;
    void generarObstaculo();
    void generarItem();
    void lanzarProyectil();

private slots:
    void actualizar(int);        // Nivel 1
    void actualizar(float);      // Nivel 2
    void actualizar(double);     // Nivel 3

    void verificarVictoria();

signals:
    void regresarAlMenu();
};

#endif // NIVEL_H
