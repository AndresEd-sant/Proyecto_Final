#include "nivel.h"
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include <QPushButton>
#include <QRandomGenerator>


// ------------------------ CONSTRUCTOR NIVEL 1 ------------------------
Nivel::Nivel(QWidget* parent) : QObject(parent), desplazamientoFondo(0) {
    escena = new QGraphicsScene();
    vista = new QGraphicsView(escena, (QWidget*)parent);
    vista->setFixedSize(1200, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    fondo.load(":/fondo_nv1.png");
    fondo = fondo.scaled(1200, 600);

    fondoItem1 = new QGraphicsPixmapItem(fondo);
    QPixmap fondoInvertido = fondo.transformed(QTransform().scale(-1, 1));
    fondoItem2 = new QGraphicsPixmapItem(fondoInvertido);
    fondoItem1->setPos(0, 0);
    fondoItem2->setPos(1200, 0);

    escena->addItem(fondoItem1);
    escena->addItem(fondoItem2);

    jugador = new Jugador(50, 500, 72, 74, 8, 5, ":/Goku_16.png");
    escena->addItem(jugador);
    jugador->setFocus();
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);

    spriteVidas.load(":/vidas.png");
    barraVidas = new QGraphicsPixmapItem();
    actualizarBarraVidas();
    barraVidas->setPos(20, 20);
    escena->addItem(barraVidas);

    QPixmap icono(":/leche.PNG");
    iconoLeche = new QGraphicsPixmapItem(icono.scaled(90, 90));
    iconoLeche->setPos(1100, 14);
    escena->addItem(iconoLeche);

    textoLecheRestante = new QGraphicsTextItem();
    textoLecheRestante->setPlainText("5");
    textoLecheRestante->setDefaultTextColor(Qt::white);
    textoLecheRestante->setFont(QFont("Arial", 23, QFont::Bold));
    textoLecheRestante->setPos(1090, 36);
    escena->addItem(textoLecheRestante);

    generadorObstaculos = new QTimer(this);
    connect(generadorObstaculos, &QTimer::timeout, this, &Nivel::generarObstaculo);
    generadorObstaculos->start(5000);

    generadorItems = new QTimer(this);
    connect(generadorItems, &QTimer::timeout, this, &Nivel::generarItem);
    generadorItems->start(8000);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, [=]() {
        actualizar(1);
    });
    temporizador->start(30);

    vista->setFocus();
}

// ------------------------ CONSTRUCTOR NIVEL 2 ------------------------
Nivel::Nivel(QWidget* parent, int ID) {
    // (Pendiente de implementar si es necesario)
}

// ------------------------ CONSTRUCTOR NIVEL 3 ------------------------
Nivel::Nivel(QWidget* parent, int ID, int ID2) : QObject(parent) {
    escena = new QGraphicsScene();
    vista = new QGraphicsView(escena, (QWidget*)parent);
    vista->setFixedSize(1200, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    fondo.load(":/fondo_nv3.png");
    fondo = fondo.scaled(1200, 600);
    fondoItem1 = new QGraphicsPixmapItem(fondo);
    fondoItem2 = new QGraphicsPixmapItem(fondo);
    fondoItem1->setPos(0, 0);
    fondoItem2->setPos(1200, 0);
    escena->addItem(fondoItem1);
    escena->addItem(fondoItem2);

    jugador = new Jugador(50, 500, 72, 74, 8, 5, ":/Goku_16.png");
    escena->addItem(jugador);
    jugador->setFocus();
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);

    spriteVidas.load(":/vidas.png");
    barraVidas = new QGraphicsPixmapItem();
    actualizarBarraVidas();
    barraVidas->setPos(20, 20);
    escena->addItem(barraVidas);

    enemigo = new Enemigo(900, 430, 120, 160, 10, 5, ":/roshi.png");
    escena->addItem(enemigo);

    barraVidasRoshi = new QGraphicsPixmapItem();
    actualizarBarraVidasRoshi();
    barraVidasRoshi->setPos(930, 20);  // Del otro lado de la pantalla
    escena->addItem(barraVidasRoshi);

    QTimer* moveRoshi = new QTimer(this);
    connect(moveRoshi, &QTimer::timeout, enemigo, &Enemigo::animarMovimiento);
    moveRoshi->start(30);

    connect(enemigo, &Enemigo::disparar, this, &Nivel::lanzarProyectil);

    connect(jugador, &Jugador::contraatacar, this, [=]() {
        auto p = new Proyectil(8, true);
        p->setPos(jugador->x()+50, jugador->y()+20);
        escena->addItem(p);
        proyectiles.append(p);
    });

    // <<<<< AQUÍ estaba el error antes: usabas temporizador antes de inicializarlo >>>>>
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, [=]() {
        actualizar(3.0);
    });
    temporizador->start(30);

    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, enemigo, &Enemigo::iniciarAnimacionDisparo);
    timerDisparo->start(3000);

    vista->setFocus();
}


// ------------------------ DESTRUCTOR ------------------------
Nivel::~Nivel() {
    temporizador->stop();
    if (generadorObstaculos) generadorObstaculos->stop();
    if (generadorItems) generadorItems->stop();
    if (timerDisparo) timerDisparo->stop();

    for (auto o : obstaculos) delete o;
    for (auto i : items) delete i;
    for (auto p : proyectiles) delete p;

    delete vista;
    delete escena;
}

// ------------------------ NIVEL 1 - ACTUALIZAR ------------------------
void Nivel::actualizar(int) {
    jugador->moverCarrera();

    fondoItem1->moveBy(-2, 0);
    fondoItem2->moveBy(-2, 0);

    if (fondoItem1->x() + 1200 <= 0)
        fondoItem1->setX(fondoItem2->x() + 1200);
    if (fondoItem2->x() + 1200 <= 0)
        fondoItem2->setX(fondoItem1->x() + 1200);

    for (int i = 0; i < obstaculos.size(); ++i) {
        Obstaculo* obs = obstaculos[i];
        obs->mover();

        if (obs->collidesWithItem(jugador)) {
            jugador->perderVida();
            actualizarBarraVidas();
            escena->removeItem(obs);
            delete obs;
            obstaculos.removeAt(i--);
            continue;
        }

        if (obs->estaFueraDePantalla()) {
            escena->removeItem(obs);
            delete obs;
            obstaculos.removeAt(i--);
        }
    }

    for (int i = 0; i < items.size(); ++i) {
        Item* item = items[i];
        item->mover();

        if (item->collidesWithItem(jugador)) {
            lecheRecolectada++;
            actualizarContadorLeche();
            escena->removeItem(item);
            delete item;
            items.removeAt(i--);
            if (lecheRecolectada >= 5) {
                mostrarResultado("¡Recolectaste toda la leche!");
            }
            continue;
        }

        if (item->estaFueraDePantalla()) {
            escena->removeItem(item);
            delete item;
            items.removeAt(i--);
        }
    }

    if (jugador->getVidas() <= 0)
        mostrarResultado("¡Perdiste!");
}

// ------------------------ NIVEL 2 - ACTUALIZAR ------------------------
void Nivel::actualizar(float){}
// ------------------------ NIVEL 3 - ACTUALIZAR ------------------------
void Nivel::actualizar(double) {
    // Mover y procesar todos los proyectiles
    for (int i = 0; i < proyectiles.size(); ++i) {
        Proyectil* p = proyectiles[i];
        p->avanzar();

        // 1. Si viene de Roshi y colisiona con el jugador
        if (!p->esReflejado() && p->collidesWithItem(jugador)) {
            // Verificar si está en modo contraataque
            if (jugador->estaEnModoContraataque()) {
                p->setReflejado(true); // marcar como reflejado
                continue;
            } else {
                jugador->perderVida();
                actualizarBarraVidas();
                escena->removeItem(p);
                delete p;
                proyectiles.removeAt(i--);
                continue;
            }
        }

        // 2. Si es reflejado y choca con Roshi
        if (p->esReflejado() && p->collidesWithItem(enemigo)) {
            enemigo->recibirDano(1);
            actualizarBarraVidasRoshi();
            escena->removeItem(p);
            delete p;
            proyectiles.removeAt(i--);

            if (enemigo->getVidas() == 0) {
                mostrarResultado("¡Derrotaste al Maestro Roshi!");
            }
            continue;
        }

        // 3. Si sale de la pantalla
        if (p->fueraDePantalla()) {
            escena->removeItem(p);
            delete p;
            proyectiles.removeAt(i--);
        }
    }

    // Verificar si el jugador perdió
    if (jugador->getVidas() == 0) {
        mostrarResultado("¡Has sido derrotado!");
    }
}




void Nivel::actualizarBarraVidas() {
    int vidas = jugador->getVidas();
    vidas = std::clamp(vidas, 0, 5);

    int alto = 491 / 5;
    int ancho = 508;
    int fila = 5 - vidas;

    QPixmap seccion = spriteVidas.copy(0, alto * fila, ancho, alto);
    barraVidas->setPixmap(seccion.scaled(250, 50));
}

void Nivel::actualizarContadorLeche() {
    int faltan = std::max(0, 5 - lecheRecolectada);
    textoLecheRestante->setPlainText(QString::number(faltan));
}

void Nivel::lanzarProyectil() {
    auto p = new Proyectil(8, false); // false si viene de Roshi
    p->setPos(enemigo->x() - 50, enemigo->y() + 50);
    escena->addItem(p);
    proyectiles.append(p);
}

void Nivel::mostrarResultado(const QString& mensaje) {
    // Crear el mensaje con 'this' como parent
    QMessageBox* msgBox = new QMessageBox(QMessageBox::Information,
                                          "Fin del nivel", mensaje,
                                          QMessageBox::Ok, vista); // ✅ usa vista como parent

    // Conecta la señal 'accepted' para regresar al menú solo cuando el usuario cierre el mensaje
    connect(msgBox, &QMessageBox::accepted, this, [=]() {
        emit regresarAlMenu(); // ahora sí limpiamos todo
    });

    msgBox->show();
}




QGraphicsView* Nivel::obtenerVista() const {
    return vista;
}

void Nivel::generarObstaculo() {
    int tipo = QRandomGenerator::global()->bounded(3);  // 0: piedra, 1: tronco, 2: abeja
    Obstaculo* obs = nullptr;

    int posY = QRandomGenerator::global()->bounded(350, 501);  // [350, 500]

    if (tipo == 0) {
        obs = new Obstaculo(":/piedra.png", 90, 90, "");
        obs->setPos(1200, 400);
    } else if (tipo == 1) {
        obs = new Obstaculo(":/tronco.png", 100, 100, "parabolico");
        obs->setPos(1200, posY);
    } else {
        obs = new Obstaculo(":/abeja.png", 100, 100, "lineal");
        obs->setPos(1200, posY);
    }

    escena->addItem(obs);
    obstaculos.append(obs);
}


void Nivel::generarItem() {
    int posY = QRandomGenerator::global()->bounded(350, 501);  // [350, 500]

    Item* leche = new Item(":/leche.PNG", 50, 50);
    leche->setPos(1200, posY);
    escena->addItem(leche);
    items.append(leche);
}


void Nivel::verificarVictoria() {
    if (lecheRecolectada >= 5) {
        mostrarResultado("¡Recolectaste toda la leche!");
    }
}

void Nivel::actualizarBarraVidasRoshi() {
    int vidas = enemigo->getVidas();
    vidas = std::clamp(vidas, 0, 5);

    int alto = 491 / 5;
    int ancho = 508;
    int fila = 5 - vidas;

    QPixmap seccion = spriteVidas.copy(0, alto * fila, ancho, alto);
    barraVidasRoshi->setPixmap(seccion.scaled(250, 50));
}

void Nivel::limpiarTodo() {
    if (temporizador) temporizador->stop();
    if (generadorObstaculos) generadorObstaculos->stop();
    if (generadorItems) generadorItems->stop();
    if (timerDisparo) timerDisparo->stop();

    for (auto o : obstaculos) {
        escena->removeItem(o);
        delete o;
    }
    obstaculos.clear();

    for (auto i : items) {
        escena->removeItem(i);
        delete i;
    }
    items.clear();

    for (auto p : proyectiles) {
        escena->removeItem(p);
        delete p;
    }
    proyectiles.clear();

    if (jugador) {
        escena->removeItem(jugador);
        delete jugador;
        jugador = nullptr;
    }

    if (enemigo) {
        escena->removeItem(enemigo);
        delete enemigo;
        enemigo = nullptr;
    }

    if (barraVidas) escena->removeItem(barraVidas);
    if (barraVidasRoshi) escena->removeItem(barraVidasRoshi);
    if (iconoLeche) escena->removeItem(iconoLeche);
    if (textoLecheRestante) escena->removeItem(textoLecheRestante);

    escena->clear();  // Borra cualquier otro item restante
}

