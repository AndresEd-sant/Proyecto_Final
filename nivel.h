#ifndef NIVEL_H
#define NIVEL_H


#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "jugador.h"
#include "obstaculo.h"
#include <QVector>
#include "item.h"

class Nivel : public QObject {
    Q_OBJECT
private:
    QTimer* generadorObstaculos;
    QTimer* generadorItems;
    int lecheRecolectada = 0;
    QGraphicsPixmapItem* barraVidas;
    QPixmap spriteVidas;
    void actualizarBarraVidas();
    int indice = 0;
    QGraphicsPixmapItem* iconoLeche;
    QGraphicsTextItem* textoLecheRestante;

protected:
    QGraphicsScene* escena;
    QGraphicsView* vista;
    QTimer* temporizador;

    QPixmap fondo;
    QGraphicsPixmapItem* fondoItem1;
    QGraphicsPixmapItem* fondoItem2;
    int desplazamientoFondo;

    Jugador* jugador;
    QVector<Obstaculo*> obstaculos;
    QVector<Item*> items;

    void verificarVictoria(); // NUEVO

public:
    Nivel(QWidget* parent = nullptr);
    virtual ~Nivel();

    void generarObstaculo();
    void generarItem(); // NUEVO
    void actualizarContadorLeche();
    QGraphicsView* obtenerVista() const;
    virtual void actualizar();
    void mostrarResultado(const QString& mensaje);
};

#endif // NIVEL_H
