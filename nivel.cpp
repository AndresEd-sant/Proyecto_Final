#include "nivel.h"
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include <QPushButton>

Nivel::Nivel(QWidget* parent) : QObject(parent), desplazamientoFondo(0) {
    escena = new QGraphicsScene();
    vista = new QGraphicsView(escena, (QWidget*)parent);
    vista->setFixedSize(1200, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    fondo.load(":/fondo_nv1.png");
    fondo = fondo.scaled(1200, 600);  // Escala al tamaño de la vista

    fondoItem1 = new QGraphicsPixmapItem(fondo);

    QPixmap fondoInvertido = fondo.transformed(QTransform().scale(-1, 1));
    fondoItem2 = new QGraphicsPixmapItem(fondoInvertido);

    fondoItem1->setPos(0, 0);
    fondoItem2->setPos(1200,0);


    escena->addItem(fondoItem1);
    escena->addItem(fondoItem2);

    jugador = new Jugador(50, 500, 72, 74, 8, 5, ":/Goku_16.png");
    escena->addItem(jugador);
    jugador->setFocus();

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Nivel::actualizar);
    temporizador->start(30);

    generadorObstaculos = new QTimer(this);
    connect(generadorObstaculos, &QTimer::timeout, this, &Nivel::generarObstaculo);
    generadorObstaculos->start(2000);

    generadorItems = new QTimer(this);
    connect(generadorItems, &QTimer::timeout, this, &Nivel::generarItem);
    generadorItems->start(4000); // cada 4 segundos aparece un tarro

    spriteVidas.load(":/vidas.png");
    barraVidas = new QGraphicsPixmapItem();
    actualizarBarraVidas();
    barraVidas->setPos(20, 20); // Puedes ajustar la posición
    escena->addItem(barraVidas);

    jugador->setFlag(QGraphicsItem::ItemIsFocusable);


    vista->setFocus();

    // Crear ícono de leche
    QPixmap icono(":/leche.PNG");
    iconoLeche = new QGraphicsPixmapItem(icono.scaled(90, 90)); // Ajusta el tamaño si quieres
    iconoLeche->setPos(1100, 14);  // Posición cerca de la esquina superior derecha
    escena->addItem(iconoLeche);

    // Crear texto de contador
    textoLecheRestante = new QGraphicsTextItem();
    textoLecheRestante->setPlainText("5"); // Inicialmente faltan 5
    textoLecheRestante->setDefaultTextColor(Qt::white);
    textoLecheRestante->setFont(QFont("Arial", 23, QFont::Bold));
    textoLecheRestante->setPos(1090, 36); // A la derecha del ícono
    escena->addItem(textoLecheRestante);
}

Nivel::~Nivel() {
    delete temporizador;
    delete vista;
    delete escena;
}

QGraphicsView* Nivel::obtenerVista() const {
    return vista;
}

void Nivel::actualizar() {
    jugador->moverCarrera();

    // Mover ambos fondos
    fondoItem1->moveBy(-2, 0);
    fondoItem2->moveBy(-2, 0);

    // Si uno se sale por la izquierda, reposicionarlo a la derecha del otro
    if (fondoItem1->x() + 1200 <= 0) {
        fondoItem1->setX(fondoItem2->x() + 1200);
    }
    if (fondoItem2->x() + 1200 <= 0) {
        fondoItem2->setX(fondoItem1->x() + 1200);
    }

    for (int i = 0; i < obstaculos.size(); ++i) {
        Obstaculo* obs = obstaculos[i];
        obs->mover();

        // Colisión con el jugador
        if (obs->collidesWithItem(jugador)) {
            jugador->perderVida();
            actualizarBarraVidas();
            escena->removeItem(obs);
            delete obs;
            obstaculos.removeAt(i);
            i--;
            continue;
        }


        // Fuera de pantalla
        if (obs->estaFueraDePantalla()) {
            escena->removeItem(obs);
            delete obs;
            obstaculos.removeAt(i);
            i--;
        }
    }

    for (int i = 0; i < items.size(); ++i) {
        Item* item = items[i];
        item->mover();

        if (item->collidesWithItem(jugador)) {
            lecheRecolectada++;
            escena->removeItem(item);
            delete item;
            items.removeAt(i);
            i--;

            actualizarContadorLeche();

            verificarVictoria();
            continue;
        }

        if (item->estaFueraDePantalla()) {
            escena->removeItem(item);
            delete item;
            items.removeAt(i);
            i--;
        }
    }

    if (jugador->getVidas() <= 0) {
        mostrarResultado("¡Perdiste!");
        return;
    }
}

void Nivel::generarObstaculo() {

    Obstaculo* obs = new Obstaculo(":/piedra.png", 50, 50); // Cambia el tamaño según la piedra real
    obs->setPos(1200, 520);  // Suelo (ajusta Y si es necesario)
    escena->addItem(obs);
    obstaculos.append(obs);
}

void Nivel::generarItem() {
    Item* leche = new Item(":/leche.PNG", 60, 60);
    leche->setPos(1200, 300);  // Altura similar al suelo
    escena->addItem(leche);
    items.append(leche);
}

void Nivel::verificarVictoria() {
    if (lecheRecolectada >= 5) {
        qDebug() << "¡Ganaste el Nivel!";
        temporizador->stop();
        generadorObstaculos->stop();
        generadorItems->stop();
        mostrarResultado("¡Ganaste!");
    }
}

void Nivel::actualizarBarraVidas() {
    int vidas = jugador->getVidas(); // Máximo 5
    if (vidas < 0) vidas = 0;
    if (vidas > 5) vidas = 5;

    int alto = 491 / 5;  // 5 filas
    int ancho = 508;     // ancho total

    int fila = 5 - vidas;  // 0: lleno, 4: casi vacío, 5: vacío

    QPixmap seccion = spriteVidas.copy(0, alto * fila, ancho, alto);
    barraVidas->setPixmap(seccion.scaled(250, 50)); // Ajusta el tamaño si quieres
}

void Nivel::mostrarResultado(const QString& mensaje) {
    temporizador->stop();
    generadorObstaculos->stop();

    QMessageBox msgBox;
    msgBox.setWindowTitle("Fin del nivel");
    msgBox.setText(mensaje);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    // Volver al menú (MainWindow)
    QMainWindow* ventana = qobject_cast<QMainWindow*>(vista->parent());
    if (ventana) {
        vista->hide();
        delete this; // opcional si quieres liberar
        ventana->findChild<QPushButton*>("nivel_1")->show();
        ventana->findChild<QPushButton*>("nivel_2")->show();
        ventana->findChild<QPushButton*>("nivel_3")->show();
    }

    this->deleteLater();
}

void Nivel::actualizarContadorLeche() {
    int faltan = 5 - lecheRecolectada;
    if (faltan < 0) faltan = 0;
    textoLecheRestante->setPlainText(QString::number(faltan));
}
