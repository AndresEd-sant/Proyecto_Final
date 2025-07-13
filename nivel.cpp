#include "nivel.h"
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include <QPushButton>
#include <QRandomGenerator>
#include <algorithm>


// ---------------------- CONSTRUCTOR UNICO ----------------------
Nivel::Nivel(QWidget* parent, int tipo, int modo)
    : QGraphicsView(parent), tipo(tipo), modo(modo)  // No heredes QGraphicsView
{
    escena = new QGraphicsScene();
    vista = new QGraphicsView(escena, (QWidget*)parent);
    vista->setFixedSize(1200, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocus();

    tiempoNivel = 0;
    velocidadObstaculos = 8;
    oxigenoActual = 100;

    // Punteros a nullptr
    jugador = nullptr;
    enemigo = nullptr;
    fondoItem1 = nullptr;
    fondoItem2 = nullptr;
    barraVidas = nullptr;
    barraOxigeno = nullptr;
    barraVidasRoshi = nullptr;
    iconoLeche = nullptr;
    textoLecheRestante = nullptr;
    textoOxigeno = nullptr;
    textoPuntaje = nullptr;

    generadorEnemigos = nullptr;
    generadorItems = nullptr;
    generadorObstaculos = nullptr;
    temporizador = nullptr;
    timerDisparo = nullptr;
    timerDificultad = nullptr;

    // Llama al nivel correcto solo al final, cuando todo está listo
    if (tipo == 1 && modo == 0)
        iniciarNivelCarrera();
    else if (tipo == 2 && modo == 1)
        iniciarNivelLago();
    else if (tipo == 3)
        iniciarNivelRoshi();
}

// ---------------------- NIVEL 1 ----------------------
void Nivel::iniciarNivelCarrera() {
    tipo = 1;
    modo=0;
    pausaActiva = false;

    fondo.load(":/fondo_nv1.png");
    fondo = fondo.scaled(1200, 600);
    fondoItem1 = new QGraphicsPixmapItem(fondo);
    fondoItem2 = new QGraphicsPixmapItem(fondo.transformed(QTransform().scale(-1, 1)));
    fondoItem1->setPos(0, 0);
    fondoItem2->setPos(1200, 0);
    escena->addItem(fondoItem1);
    escena->addItem(fondoItem2);

    iniciarMusica("qrc:/sonidos/MFondo_lv1.mp3");

    jugador = new Jugador(50, 500, 72, 74, 8, 5, ":/Goku_16.png", true);
    escena->addItem(jugador);
    jugador->setFocus();
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);

    connect(jugador, &Jugador::pausarJuego, this, &Nivel::pausarJuego);

    spriteVidas.load(":/vidas.png");
    barraVidas = new QGraphicsPixmapItem();
    actualizarBarraVidas();
    barraVidas->setPos(20, 20);
    escena->addItem(barraVidas);

    QPixmap icono(":/leche.PNG");
    iconoLeche = new QGraphicsPixmapItem(icono.scaled(90, 90));
    iconoLeche->setPos(1100, 14);
    escena->addItem(iconoLeche);

    textoLecheRestante = new QGraphicsTextItem("5");
    textoLecheRestante->setDefaultTextColor(Qt::white);
    textoLecheRestante->setFont(QFont("Arial", 23, QFont::Bold));
    textoLecheRestante->setPos(1090, 36);
    escena->addItem(textoLecheRestante);

    textoPuntaje = new QGraphicsTextItem("Puntaje: 0");
    textoPuntaje->setDefaultTextColor(Qt::white);
    textoPuntaje->setFont(QFont("Arial", 20, QFont::Bold));
    textoPuntaje->setPos(800, 30);
    escena->addItem(textoPuntaje);

    generadorObstaculos = new QTimer(this);
    connect(generadorObstaculos, &QTimer::timeout, this, &Nivel::generarObstaculo);
    generadorObstaculos->start(4000);

    generadorItems = new QTimer(this);
    connect(generadorItems, &QTimer::timeout, this, &Nivel::generarItem);
    generadorItems->start(6000);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, [=]() {
        actualizar(0);
    });
    temporizador->start(30);

    vista->setFocus();

    timerDificultad = new QTimer(this);
    connect(timerDificultad, &QTimer::timeout, this, [=]() {
        tiempoNivel++;
        if (tiempoNivel % 10 == 0 && velocidadObstaculos < 20) {
            velocidadObstaculos += 2;
            int nuevoIntervalo = std::max(1000, 4000 - tiempoNivel * 100);
            generadorObstaculos->start(nuevoIntervalo);
        }
    });
    timerDificultad->start(4000);

    connect(jugador, &Jugador::disparar, this, [=](int x, int y) {
        Proyectil* nuevo = new Proyectil(8, true);
        nuevo->setPos(x, y);
        escena->addItem(nuevo);
        proyectiles.append(nuevo);
        jugador->proyectilesActivos.append(nuevo);
    });
}

// ----------------------  NIVEL 2 ----------------------
void Nivel::iniciarNivelLago() {
    nivelActual = 2;

    fondo.load(":/Fondo_nv2.png");
    fondo = fondo.scaled(1200, 600);
    fondoItem1 = new QGraphicsPixmapItem(fondo);
    fondoItem2 = new QGraphicsPixmapItem(fondo);
    fondoItem1->setPos(0, 0);
    fondoItem2->setPos(1200, 0);
    escena->addItem(fondoItem1);
    escena->addItem(fondoItem2);

    iniciarMusica("qrc:/sonidos/MFondo_lv2.mp3");

    jugador = new Jugador(200, 300, 72, 74, 8, 5, ":/Goku_1.png", false);
    escena->addItem(jugador);
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();
    oxigenoActual = 100;

    connect(jugador, &Jugador::pausarJuego, this, &Nivel::pausarJuego);

    QTimer::singleShot(100, jugador, [=]() {
        jugador->setFocus();
    });

    spritesTiburon = {
        QPixmap(":/tiburon_1.png").scaled(100, 100),
        QPixmap(":/tiburon_2.png").scaled(100, 100),
        QPixmap(":/tiburon_3.png").scaled(100, 100),
    };

    spriteVidas.load(":/vidas.png");
    spriteBurbujas.load(":/oxigeno.png");

    barraVidas = new QGraphicsPixmapItem();
    barraVidas->setPos(20, 20);
    actualizarBarraVidas();
    escena->addItem(barraVidas);

    barraOxigeno = new QGraphicsPixmapItem();
    barraOxigeno->setPos(250, 20);
    actualizarBarraOxigeno();
    escena->addItem(barraOxigeno);

    textoOxigeno = new QGraphicsTextItem();
    textoOxigeno->setDefaultTextColor(Qt::white);
    textoOxigeno->setFont(QFont("Arial", 18, QFont::Bold));
    textoOxigeno->setPos(520, 20);
    textoOxigeno->setPlainText("Oxígeno: 100");
    escena->addItem(textoOxigeno);

    textoPuntaje = new QGraphicsTextItem("Puntaje: 0");
    textoPuntaje->setDefaultTextColor(Qt::white);
    textoPuntaje->setFont(QFont("Arial", 20, QFont::Bold));
    textoPuntaje->setPos(950, 20);
    escena->addItem(textoPuntaje);

    generadorEnemigos = new QTimer(this);
    connect(generadorEnemigos, &QTimer::timeout, this, &Nivel::generarTiburon);
    generadorEnemigos->start(4000);

    generadorItems = new QTimer(this);
    connect(generadorItems, &QTimer::timeout, this, &Nivel::generarItem);
    generadorItems->start(6000);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, [=]() {
        actualizar(0.0f);
    });
    temporizador->start(30);

    vista->setFocus();

    connect(jugador, &Jugador::disparar, this, [=](float x, float y) {
        Proyectil* p = new Proyectil(8, true);
        p->setPos(x, y);
        escena->addItem(p);
        proyectiles.append(p);
        jugador->proyectilesActivos.append(p);
    });
}

// ---------------------- NIVEL 3 ----------------------
void Nivel::iniciarNivelRoshi() {
    nivelActual = 3;

    fondo.load(":/fondo_nv3.png");
    fondo = fondo.scaled(1200, 600);
    fondoItem1 = new QGraphicsPixmapItem(fondo);
    fondoItem2 = new QGraphicsPixmapItem(fondo);
    fondoItem1->setPos(0, 0);
    fondoItem2->setPos(1200, 0);
    escena->addItem(fondoItem1);
    escena->addItem(fondoItem2);

    iniciarMusica("qrc:/sonidos/MFondo_lv3.mp3");

    jugador = new Jugador(50, 500, 80, 85, 10, 5, ":/Goku_esc1.png", true);
    escena->addItem(jugador);
    jugador->setFocus();
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);

    connect(jugador, &Jugador::pausarJuego, this, &Nivel::pausarJuego);

    spriteVidas.load(":/vidas.png");
    barraVidas = new QGraphicsPixmapItem();
    actualizarBarraVidas();
    barraVidas->setPos(20, 20);
    escena->addItem(barraVidas);

    enemigo = new Enemigo(900, 430, 120, 160, 10, 5, ":/roshi.png");
    escena->addItem(enemigo);

    barraVidasRoshi = new QGraphicsPixmapItem();
    actualizarBarraVidasRoshi();
    barraVidasRoshi->setPos(930, 20);
    escena->addItem(barraVidasRoshi);

    QTimer* moveRoshi = new QTimer(this);
    connect(moveRoshi, &QTimer::timeout, enemigo, &Enemigo::animarMovimiento);
    moveRoshi->start(30);

    connect(enemigo, &Enemigo::disparar, this, [=]() {
        auto p = new Proyectil(5.5f, false);  // disparo de Roshi
        p->setPos(enemigo->x(), enemigo->y() + enemigo->boundingRect().height() / 2);
        escena->addItem(p);
        proyectiles.append(p);
    });


    connect(jugador, &Jugador::contraatacar, this, [=]() {
        auto p = new Proyectil(8, true);
        p->setPos(jugador->x() + 50, jugador->y() + 20);
        escena->addItem(p);
        proyectiles.append(p);
    });

    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, enemigo, &Enemigo::iniciarAnimacionDisparo);
    timerDisparo->start(3000);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, [=]() {
        actualizar(0.0);
    });
    temporizador->start(30);

    vista->setFocus();
}

// ------------------------ DESTRUCTOR ------------------------
Nivel::~Nivel() {
    detenerMusica();

    if (temporizador) temporizador->stop();
    if (generadorObstaculos) generadorObstaculos->stop();
    if (generadorItems) generadorItems->stop();
    if (generadorEnemigos) generadorEnemigos->stop();
    if (timerDisparo) timerDisparo->stop();
    if (timerDificultad) timerDificultad->stop();

    // Eliminar obstáculos
    for (auto o : obstaculos) {
        escena->removeItem(o);
        delete o;
    }
    obstaculos.clear();

    // Eliminar items
    for (auto i : items) {
        escena->removeItem(i);
        delete i;
    }
    items.clear();

    // Eliminar proyectiles
    for (auto p : proyectiles) {
        escena->removeItem(p);
        delete p;
    }
    proyectiles.clear();

    // Eliminar enemigos
    for (auto e : enemigos) {
        escena->removeItem(e);
        delete e;
    }
    enemigos.clear();

    // Eliminar jugador
    if (jugador) {
        escena->removeItem(jugador);
        delete jugador;
        jugador = nullptr;
    }

    // Eliminar enemigo tipo Roshi
    if (enemigo) {
        escena->removeItem(enemigo);
        delete enemigo;
        enemigo = nullptr;
    }

    // Eliminar roca final (nivel 2)
    if (rocaFinal) {
        escena->removeItem(rocaFinal);
        delete rocaFinal;
        rocaFinal = nullptr;
    }

    // Eliminar barras y textos
    if (barraVidas) escena->removeItem(barraVidas);
    if (barraOxigeno) escena->removeItem(barraOxigeno);
    if (barraVidasRoshi) escena->removeItem(barraVidasRoshi);
    if (textoLecheRestante) escena->removeItem(textoLecheRestante);
    if (textoOxigeno) escena->removeItem(textoOxigeno);
    if (textoPuntaje) escena->removeItem(textoPuntaje);
    if (iconoLeche) escena->removeItem(iconoLeche);

    // Eliminar panel de pausa si estaba creado
    if (panelPausa) {
        panelPausa->hide();
        delete panelPausa;
        panelPausa = nullptr;
    }

    // Eliminar escena
    if (escena) {
        escena->clear();
        delete escena;
        escena = nullptr;
    }

    // Eliminar vista si no era this (por si usabas QGraphicsView externo)
    if (vista && vista != this) {
        vista->hide();
        delete vista;
        vista = nullptr;
    }
}

// ------------------------ NIVEL 1 - ACTUALIZAR ------------------------
void Nivel::actualizar(int) {
    if (nivelFinalizado) return;

    jugador->moverCarrera();

    fondoItem1->moveBy(-2, 0);
    fondoItem2->moveBy(-2, 0);
    if (fondoItem1->x() + 1200 <= 0) fondoItem1->setX(fondoItem2->x() + 1200);
    if (fondoItem2->x() + 1200 <= 0) fondoItem2->setX(fondoItem1->x() + 1200);

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
            if (lecheRecolectada >= 5)
                mostrarResultado("¡Recolectaste toda la leche!");
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
    for (int i = 0; i < proyectiles.size(); ++i) {
        Proyectil* p = proyectiles[i];
        p->avanzar();

        if (p->fueraDePantalla()) {
            escena->removeItem(p);
            delete p;
            proyectiles.removeAt(i--);
            jugador->proyectilesActivos.removeOne(p);
            jugador->verificarProyectilesActivos();

            continue;
        }

        // Colisión con obstáculos
        for (int j = 0; j < obstaculos.size(); ++j) {
            Obstaculo* obs = obstaculos[j];
            if (p->esReflejado() && p->collidesWithItem(obs)) {
                escena->removeItem(obs);
                delete obs;
                obstaculos.removeAt(j--);

                escena->removeItem(p);
                delete p;
                proyectiles.removeAt(i--);
                jugador->proyectilesActivos.removeOne(p);
                jugador->verificarProyectilesActivos();
                jugador->setPuntaje(jugador->getPuntaje() + 5);
                actualizarPuntaje();  // Esta función actualiza el texto en pantalla

                break;
            }
        }
    }

}

// ------------------------ NIVEL 2 - ACTUALIZAR ------------------------
void Nivel::actualizar(float) {
    if (pausaActiva || nivelFinalizado) return;

    jugador->moverAcuatico();

    fondoItem1->moveBy(-1, 0);
    fondoItem2->moveBy(-1, 0);
    if (fondoItem1->x() + 1200 <= 0) fondoItem1->setX(fondoItem2->x() + 1200);
    if (fondoItem2->x() + 1200 <= 0) fondoItem2->setX(fondoItem1->x() + 1200);

    // ------------------- MANEJO EXCLUSIVO DE LA ROCA GANADORA -------------------
    if (rocaFinal && jugador && rocaFinal->scene()) {
        rocaFinal->mover();

        if (jugador->scene() && rocaFinal->collidesWithItem(jugador)) {
            if (temporizador) temporizador->stop();
            mostrarResultado("¡Encontraste la roca del maestro Roshi!");

            escena->removeItem(rocaFinal);
            delete rocaFinal;
            rocaFinal = nullptr;
        } else if (rocaFinal->x() + rocaFinal->boundingRect().width() < 0) {
            escena->removeItem(rocaFinal);
            delete rocaFinal;
            rocaFinal = nullptr;
        }
    }



    // Solo se reduce si está en la parte profunda (ej. y > 400)
    if (jugador->y() > 200) {
        oxigenoActual -= 0.3;
        if (oxigenoActual < 0) oxigenoActual = 0;
    } else {
        oxigenoActual += 0.5; // Se recupera lentamente si está arriba
        if (oxigenoActual > 100) oxigenoActual = 100;
    }

    actualizarBarraOxigeno();

    if (oxigenoActual <= 0) {
        jugador->perderVida();
        oxigenoActual = 100;
        actualizarBarraVidas();
        actualizarBarraOxigeno();
    }


    for (int i = 0; i < enemigos.size(); ++i) {
        Enemigo* tiburon = enemigos[i];
        tiburon->mover();

        if (tiburon->collidesWithItem(jugador)) {
            jugador->perderVida();
            actualizarBarraVidas();
            escena->removeItem(tiburon);
            delete tiburon;
            enemigos.removeAt(i--);

        } else if (tiburon->x() + tiburon->boundingRect().width() < 0) {
            escena->removeItem(tiburon);
            delete tiburon;
            enemigos.removeAt(i--);
        }
    }
    for (int i = 0; i < items.size(); ++i) {
        Item* item = items[i];
        item->mover();

        if (item->collidesWithItem(jugador)) {
            if (item->getruta() == ":/roca_agi.png") {
                jugador->perderVida();
                actualizarBarraVidas();
            } else {
                oxigenoActual = 100;
                actualizarBarraOxigeno();
            }

            escena->removeItem(item);
            delete item;
            items.removeAt(i--);
        }
    }



    if (jugador->getVidas() <= 0)
        mostrarResultado("¡Has sido derrotado!");
}

// ------------------------ NIVEL 3 - ACTUALIZAR ------------------------
void Nivel::actualizar(double) {
    if (nivelFinalizado) return;


    if (enemigo && jugador && enemigo->collidesWithItem(jugador)) {
        jugador->perderVida();
        actualizarBarraVidas();
    }

    for (int i = 0; i < proyectiles.size(); ++i) {
        Proyectil* p = proyectiles[i];
        p->avanzar();

        if (!p->esReflejado() && p->collidesWithItem(jugador)) {
            if (jugador->estaEnModoContraataque()) {
                p->setReflejado(true);
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

        if (enemigo && p->esReflejado() && p->collidesWithItem(enemigo)) {
            enemigo->recibirDano(1);
            actualizarBarraVidasRoshi();
            escena->removeItem(p);
            delete p;
            proyectiles.removeAt(i--);

            if (enemigo && enemigo->getVidas() <= 0)
                mostrarResultado("¡Derrotaste al Maestro Roshi!");
            continue;
        }

        if (p->fueraDePantalla()) {
            escena->removeItem(p);
            delete p;
            proyectiles.removeAt(i--);
        }
    }

    if (jugador->getVidas() <= 0)
        mostrarResultado("¡Has sido derrotado!");
}


void Nivel::actualizarBarraVidas() {
    int vidas = jugador->getVidas();
    vidas = std::clamp(vidas, 0, 5);
    int alto = 499 / 5;  // Dividir sprite en 5 burbujas
    QPixmap seccion = spriteVidas.copy(0, alto * (5 - vidas), 499, alto);
    barraVidas->setPixmap(seccion.scaled(200, 40));
}


void Nivel::actualizarContadorLeche() {
    int faltan = std::max(0, 5 - lecheRecolectada);
    textoLecheRestante->setPlainText(QString::number(faltan));
}

void Nivel::actualizarBarraVidasRoshi() {
    int vidas = enemigo->getVidas();
    vidas = std::clamp(vidas, 0, 5);
    int alto = 491 / 5;
    QPixmap seccion = spriteVidas.copy(0, alto * (5 - vidas), 508, alto);
    barraVidasRoshi->setPixmap(seccion.scaled(250, 50));
}

void Nivel::actualizarBarraOxigeno() {
    int fila = std::clamp(5 - int(oxigenoActual / 20), 0, 4);
    int alto = 499 / 5;
    QPixmap seccion = spriteBurbujas.copy(0, alto * fila, 499, alto);
    barraOxigeno->setPixmap(seccion.scaled(200, 35));
    barraOxigeno->setPos(220, 30);

    if (textoOxigeno) {
        textoOxigeno->setPlainText("Oxígeno: " + QString::number(int(oxigenoActual)));
    }
}
void Nivel::actualizarPuntaje() {
    if (textoPuntaje)
        textoPuntaje->setPlainText("Puntaje: " + QString::number(jugador->getPuntaje()));
}

void Nivel::mostrarResultado(const QString& mensaje) {
    if (nivelFinalizado) return; // <- prevenir múltiples ejecuciones
    temporizador->stop();
    nivelFinalizado = true;

    QMessageBox* msgBox = new QMessageBox(QMessageBox::Information,
                                          "Fin del nivel", mensaje,
                                          QMessageBox::Ok, vista);
    connect(msgBox, &QMessageBox::accepted, this, [=]() {
        emit regresarAlMenu();
    });
    msgBox->show();
}

void Nivel::verificarVictoria() {
    if (lecheRecolectada >= 5)

    mostrarResultado("¡Recolectaste toda la leche!");
}

QGraphicsView* Nivel::obtenerVista() const {
    return vista;
}

void Nivel::limpiarTodo() {
    if (temporizador) temporizador->stop();
    if (generadorObstaculos) generadorObstaculos->stop();
    if (generadorItems) generadorItems->stop();
    if (generadorEnemigos) generadorEnemigos->stop();
    if (timerDisparo) timerDisparo->stop();

    for (auto o : obstaculos) { escena->removeItem(o); delete o; }
    for (auto i : items) { escena->removeItem(i); delete i; }
    for (auto p : proyectiles) { escena->removeItem(p); delete p; }
    for (auto e : enemigos) { escena->removeItem(e); delete e; }

    obstaculos.clear();
    items.clear();
    proyectiles.clear();
    enemigos.clear();

    if (jugador) { escena->removeItem(jugador); delete jugador; jugador = nullptr; }
    if (enemigo) { escena->removeItem(enemigo); delete enemigo; enemigo = nullptr; }
    if (rocaFinal) {
        escena->removeItem(rocaFinal);
        delete rocaFinal;
        rocaFinal = nullptr;
    }


    escena->clear();
}

void Nivel::generarItem() {
    if (nivelActual == 2) {
        // Generar piedra común
        int x = QRandomGenerator::global()->bounded(1200, 1400);
        Item* piedra = new Item(":/roca_agi.png", 80, 80);
        piedra->setPos(x, 535);
        escena->addItem(piedra);
        items.append(piedra);

        // Verificar y generar roca ganadora si no existe o ya se fue
        if (!rocaFinal || !rocaFinal->scene() || rocaFinal->x() + rocaFinal->boundingRect().width() < 0) {
            if (rocaFinal && rocaFinal->scene()) {
                escena->removeItem(rocaFinal);
                delete rocaFinal;
                rocaFinal = nullptr;
            }
            rocaFinal = new Item(":/piedra.png", 50, 50);  // roca ganadora
            rocaFinal->setZValue(2);
            rocaFinal->setPos(1650, 530);
            escena->addItem(rocaFinal);  // NO se agrega a items
        }
    } else {
        // Nivel 1: generar leche normalmente
        int y = QRandomGenerator::global()->bounded(200, 500);
        Item* leche = new Item(":/leche.PNG", 50, 50);
        leche->setPos(1200, y);
        escena->addItem(leche);
        items.append(leche);
    }
}

void Nivel::generarObstaculo() {
    int tipo = QRandomGenerator::global()->bounded(3); // 0: piedra, 1: tronco, 2: abeja
    Obstaculo* obs = nullptr;

    if (tipo == 0) {
        // Roca: posición fija en el suelo (ajústala si tu suelo está más arriba)
        int posY = 500; // <-- Coordenada del suelo
        obs = new Obstaculo(":/piedra.png", 90, 90, "", velocidadObstaculos);
        obs->setPos(1200, posY);
    }
    else if (tipo == 1) {
        int posY = QRandomGenerator::global()->bounded(400, 470);  // rebote medio
        obs = new Obstaculo(":/tronco.png", 100, 100, "parabolico", velocidadObstaculos);
        obs->setPos(1200, posY);
    }
    else {
        int posY = QRandomGenerator::global()->bounded(200, 300);  // altura para abejas
        obs = new Obstaculo(":/abeja.png", 100, 100, "lineal", velocidadObstaculos);
        obs->setPos(1200, posY);
    }

    escena->addItem(obs);
    obstaculos.append(obs);
}

void Nivel::generarTiburon() {
    int y = QRandomGenerator::global()->bounded(200, 500);
    Enemigo* tiburon = new Enemigo(spritesTiburon, 1200, y, 4);  // velocidad 4
    escena->addItem(tiburon);
    enemigos.append(tiburon);
}

void Nivel::generarRoca() {
    if (!rocaFinal) {
        rocaFinal = new Item(":/piedra.png", 80, 80);
        rocaFinal->setZValue(2);
        rocaFinal->setPos(1600, 520);
        escena->addItem(rocaFinal);
        items.append(rocaFinal);
    } else if (!rocaFinal->scene()) {
        // Si fue removida de la escena, volver a agregarla
        rocaFinal->setPos(1600, 520);
        escena->addItem(rocaFinal);
        items.append(rocaFinal);
    } else if (rocaFinal->x() + rocaFinal->boundingRect().width() < 0) {
        // Si salió completamente de la pantalla
        rocaFinal->setPos(1600, 520);
    }
}

void Nivel::iniciarMusica(const QString& ruta) {
    if (musica) {
        musica->stop();
        delete musica;
        musica = nullptr;
    }

    if (salidaAudio) {
        delete salidaAudio;
        salidaAudio = nullptr;
    }

    musica = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);
    musica->setAudioOutput(salidaAudio);

    musica->setSource(QUrl(ruta));
    musica->setLoops(QMediaPlayer::Infinite);
    salidaAudio->setVolume(40);  // 0 a 100
    musica->play();
}

void Nivel::detenerMusica() {
    if (musica) {
        musica->stop();
        delete musica;
        musica = nullptr;
    }
    if (salidaAudio) {
        delete salidaAudio;
        salidaAudio = nullptr;
    }
}
void Nivel::reiniciarNivel() {
    pausaActiva = true;  // Evita que actualizar() siga corriendo
    nivelFinalizado = true;
    if (rocaFinal) {
        if (rocaFinal->scene()) escena->removeItem(rocaFinal);
        delete rocaFinal;
        rocaFinal = nullptr;
    }

    nivelFinalizado = false;

    if (temporizador) temporizador->start(30);
    if (timerDificultad) timerDificultad->start(4000);
    if (timerDisparo) timerDisparo->start(3000);
    if (generadorObstaculos) generadorObstaculos->start(4000);
    if (generadorItems) generadorItems->start(6000);
    if (generadorEnemigos) generadorEnemigos->start(4000);

    limpiarTodo();
    if (tipo == 1 && modo == 0) iniciarNivelCarrera();
    else if (tipo == 2 && modo == 1) iniciarNivelLago();
    else if (tipo == 3) iniciarNivelRoshi();

    pausaActiva = false;
    if (panelPausa) panelPausa->hide();  // 👈 evita el crash si no existe aún
}


void Nivel::volverAlMenu() {
    emit regresarAlMenu(); // Puedes conectar esta señal al MainWindow para mostrar el menú
}
void Nivel::pausarJuego() {
    pausaActiva = !pausaActiva;

    if (pausaActiva) {
        // Crear el panel si no existe aún
        if (!panelPausa) {
            panelPausa = new PausaPanel(vista);
            panelPausa->move((vista->width() - panelPausa->width()) / 2,
                             (vista->height() - panelPausa->height()) / 2);

            // --- SLOT: Reanudar juego
            connect(panelPausa, &PausaPanel::reanudarJuego, this, [=]() {
                pausaActiva = false;
                if (panelPausa) panelPausa->hide();
                if (temporizador) temporizador->start(30);
                if (timerDisparo) timerDisparo->start(3000);
                if (timerDificultad) timerDificultad->start(4000);
                if (generadorItems) generadorItems->start(6000);
                if (generadorObstaculos) generadorObstaculos->start(4000);
                if (generadorEnemigos) generadorEnemigos->start(4000);
            });

            // --- SLOT: Reiniciar nivel
            connect(panelPausa, &PausaPanel::reiniciarNivel, this, [=]() {
                pausaActiva = false;
                if (panelPausa) panelPausa->hide();
                if (temporizador) temporizador->stop();
                if (timerDisparo) timerDisparo->stop();
                if (timerDificultad) timerDificultad->stop();
                if (generadorItems) generadorItems->stop();
                if (generadorObstaculos) generadorObstaculos->stop();
                if (generadorEnemigos) generadorEnemigos->stop();
                reiniciarNivel();
            });

            // --- SLOT: Salir al menú
            connect(panelPausa, &PausaPanel::salirAlMenu, this, [=]() {
                pausaActiva = false;
                if (panelPausa) panelPausa->hide();
                emit regresarAlMenu();
            });

            escena->addWidget(panelPausa);
        }

        // Detener todo
        if (temporizador) temporizador->stop();
        if (timerDisparo) timerDisparo->stop();
        if (timerDificultad) timerDificultad->stop();
        if (generadorItems) generadorItems->stop();
        if (generadorObstaculos) generadorObstaculos->stop();
        if (generadorEnemigos) generadorEnemigos->stop();

        if (panelPausa) panelPausa->setVisible(true);

    } else {
        pausaActiva = false;
        if (temporizador) temporizador->start(30);
        if (timerDisparo) timerDisparo->start(3000);
        if (timerDificultad) timerDificultad->start(4000);
        if (generadorItems) generadorItems->start(6000);
        if (generadorObstaculos) generadorObstaculos->start(4000);
        if (generadorEnemigos) generadorEnemigos->start(4000);

        if (panelPausa) panelPausa->setVisible(false);
    }
}

