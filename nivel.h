#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "jugador.h"
#include "enemigo.h"
#include "obstaculo.h"
#include "item.h"
#include "pausa_panel.h"

class Nivel : public QGraphicsView
 {
    Q_OBJECT

public:
    explicit Nivel(QWidget* parent, int tipo, int modo);
    ~Nivel();

    QGraphicsView* obtenerVista() const;
    void limpiarTodo();
    void reiniciarNivel();

signals:
    void regresarAlMenu();
public slots:
    void pausarJuego();


private:
    // ----------- General -----------

    int tipo;
    int modo;
    bool pausaActiva;
    QGraphicsScene* escena = nullptr;
    QGraphicsView* vista = nullptr;
    QPixmap fondo;
    QGraphicsPixmapItem* fondoItem1 = nullptr;
    QGraphicsPixmapItem* fondoItem2 = nullptr;
    QTimer* temporizador = nullptr;
    int nivelActual = 0;
    bool nivelFinalizado = false;

    // ----------- Jugador y enemigo -----------
    Jugador* jugador = nullptr;
    Enemigo* enemigo = nullptr; // Solo para nivel 3
    QList<Obstaculo*> obstaculos;
    QList<Item*> items;
    QList<Proyectil*> proyectiles;
    QList<Enemigo*> enemigos;  // tiburones en nivel 2

    // ----------- Vidas y UI -----------
    QPixmap spriteVidas;
    QPixmap spriteBurbujas;
    QGraphicsPixmapItem* barraVidas = nullptr;
    QGraphicsPixmapItem* barraVidasRoshi = nullptr;
    QGraphicsPixmapItem* barraOxigeno = nullptr;
    QGraphicsTextItem* textoLecheRestante = nullptr;
    QGraphicsTextItem* textoOxigeno = nullptr;
    QGraphicsTextItem* textoPuntaje = nullptr;
    QGraphicsPixmapItem* iconoLeche = nullptr;

    // ----------- Música -----------
    QMediaPlayer* musica = nullptr;
    QAudioOutput* salidaAudio = nullptr;

    // ----------- Nivel 1 -----------
    QTimer* generadorObstaculos = nullptr;
    QTimer* timerDificultad = nullptr;
    int velocidadObstaculos = 6;
    int tiempoNivel = 0;
    int lecheRecolectada = 0;

    // ----------- Nivel 2 -----------
    QTimer* generadorEnemigos = nullptr;
    float oxigenoActual = 100;
    QTimer* generadorItems = nullptr;
    Item* rocaFinal = nullptr;
    QVector<QPixmap> spritesTiburon;

    // ----------- Nivel 3 -----------
    QTimer* timerDisparo = nullptr;

    // ----------- Panel Pausa -----------
    PausaPanel* panelPausa = nullptr;

    // ----------- Métodos privados -----------
    void iniciarNivelRoshi();
    void iniciarNivelCarrera();
    void iniciarNivelLago();

    void actualizar(int);      // Nivel 1
    void actualizar(float);    // Nivel 2
    void actualizar(double);   // Nivel 3

    void actualizarBarraVidas();
    void actualizarBarraVidasRoshi();
    void actualizarBarraOxigeno();
    void actualizarContadorLeche();
    void actualizarPuntaje();

    void mostrarResultado(const QString& mensaje);
    void verificarVictoria();

    void generarObstaculo();
    void generarItem();
    void generarTiburon();
    void generarRoca();

    void iniciarMusica(const QString& ruta);
    void detenerMusica();
    void volverAlMenu() ;

};

#endif // NIVEL_H

