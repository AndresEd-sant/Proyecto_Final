#include "jugador.h"
#include "proyectil.h"
#include <QGraphicsScene>
#include <QTransform>
#include <QDebug>
#include <QTimer>
#include <qmath.h>

Jugador::Jugador(int x, int y, int ancho, int alto, int velocidad, int vidas, const QString& rutaSprite, bool modoCarrera)
    : QObject(nullptr), Personaje(x, y, ancho, alto, velocidad, vidas), enModoCarrera(modoCarrera),
    puntaje(0), oxigeno(100), saltando(false), tiempoSalto(0),
    velocidadSalto(20.0f), gravedad(0.6f), posicionInicialSalto(0),
    disparoActivo(false), invulnerable(false), contadorParpadeo(0)
{
    setPos(x, y);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    QPixmap sprite(rutaSprite);
    sprite = sprite.scaled(ancho, alto);
    setPixmap(sprite);

    saltoTimer = new QTimer(this);
    connect(saltoTimer, &QTimer::timeout, this, &Jugador::actualizarSalto);

    timerInvulnerabilidad = new QTimer(this);
    connect(timerInvulnerabilidad, &QTimer::timeout, this, &Jugador::actualizarParpadeo);

    timerContraataque = new QTimer(this);
    connect(timerContraataque, &QTimer::timeout, this, [=]() {
        modoContraataque = false;
        qDebug() << "Contraataque terminado";
    });

    timerCooldown = new QTimer(this);
    connect(timerCooldown, &QTimer::timeout, this, [=]() {
        puedeContraatacar = true;
        qDebug() << "Contraataque recargado";
    });

    if (enModoCarrera) {
        spritesCorrer.append(QPixmap(":/Goku_esc1.png").scaled(ancho, alto));
        spritesCorrer.append(QPixmap(":/Goku_esc2.png").scaled(ancho, alto));
        spritesCorrer.append(QPixmap(":/Goku_esc3.png").scaled(ancho, alto));
        spritesCorrer.append(QPixmap(":/Goku_esc_6.png").scaled(ancho, alto));
        spriteSaltar = QPixmap(":/Goku_esc_6.png").scaled(ancho, alto);

        frameActualCorrer = 0;
        animacionCorrerTimer = new QTimer(this);
        connect(animacionCorrerTimer, &QTimer::timeout, this, &Jugador::actualizarSpriteCorrer);
        animacionCorrerTimer->start(200);

        timerDisparo = new QTimer(this);
        timerDisparo->setInterval(300);  // Control de velocidad de disparo
        connect(timerDisparo, &QTimer::timeout, this, [=]() {
            disparoActivo = false;
            timerDisparo->stop();
        });

    } else {
        spritesAcuaticos.append(QPixmap(":/Goku_1.png").scaled(ancho, alto));
        spritesAcuaticos.append(QPixmap(":/Goku_2.png").scaled(ancho, alto));
        spritesAcuaticos.append(QPixmap(":/Goku_arriba.png").scaled(ancho, alto));
        spritesAcuaticos.append(QPixmap(":/Goku_abajo.png").scaled(ancho, alto));

        frameActualAcuatico = 0;
        animacionAcuaticaTimer = new QTimer(this);
        connect(animacionAcuaticaTimer, &QTimer::timeout, this, &Jugador::actualizarSpriteAcuatico);
        animacionAcuaticaTimer->start(200);
    }
}

void Jugador::moverCarrera() {
    if (!saltando && y() < 500) {
        setY(y() + 10);
    }
}
void Jugador::moverAcuatico() {
    if (!enModoCarrera) { // es decir, estamos en modo acuático
            float nuevaY = y() + 2.5f;  // simula que se hunde levemente por la gravedad
            nuevaY = std::clamp(nuevaY, 200.0f, 520.0f);  // evita que se salga del agua
            setY(nuevaY);
    }
}

void Jugador::moverEntrenamiento() {
    qDebug() << "Movimiento entrenamiento aún no implementado";
}


void Jugador::lanzarProyectil() {
    if (proyectilesActivos.size() < cantidadProyectiles) {
        emit disparar(x() + 50, y() + 20);  // solo 1 proyectil por Space
    }
}

void Jugador::iniciarSalto() {
    if (!saltando) {
        saltando = true;
        enElAire = true;
        dobleSaltoDisponible = true;
        tiempoSalto = 0;
        posicionInicialSalto = y();
        velocidadSalto = 60.0f;
        gravedad = 9.8f;
        setPixmap(spriteSaltar);
        saltoTimer->start(16);
    } else if (dobleSaltoDisponible) {
        dobleSaltoDisponible = false;
        tiempoSalto = 0;
        posicionInicialSalto = y();
        velocidadSalto = 60.0f;
        gravedad = 9.8f;
        setPixmap(spriteSaltar);
    }
}


void Jugador::actualizarSalto() {
    tiempoSalto++;
    float t = tiempoSalto * 0.1f;
    float desplazamiento = (velocidadSalto * t - 0.5f * gravedad * t * t);
    float nuevaY = posicionInicialSalto - desplazamiento;

    if (descensoRapido) gravedad = 11.2f;

    if (nuevaY >= 500) {
        setY(500);
        saltoTimer->stop();
        saltando = false;
        enElAire = false;
        dobleSaltoDisponible = true;
        descensoRapido = false;
        velocidadSalto = 18.0f;
        gravedad = 9.8f;
        if (!agachado && animacionCorrerTimer && !animacionCorrerTimer->isActive()) {
            animacionCorrerTimer->start(200);
        }
    } else {
        setY(nuevaY);
    }
}

void Jugador::keyPressEvent(QKeyEvent* event) {
    if (enModoCarrera) {
        switch (event->key()) {
        case Qt::Key_W:
            if (!saltando) {
                iniciarSalto();  // primer salto
            } else if (dobleSaltoDisponible) {
                iniciarSalto();  // doble salto
            }
            break;
        case Qt::Key_Escape:
            emit pausarJuego();  // Esto se conecta con Nivel
            break;
        case Qt::Key_S:
            if (saltando) {
                descensoRapido = true;
            } else {
                agachado = true;
                setPixmap(QPixmap(":/Goku_agachado.png").scaled(85, 65));
                if (animacionCorrerTimer) animacionCorrerTimer->stop();
            }
            break;
        case Qt::Key_Space:
            lanzarProyectil();
            break;

        case Qt::Key_A:
        case Qt::Key_D:
            if (!saltando && !agachado) {
                int dx = (event->key() == Qt::Key_D ? velocidad : -velocidad);
                int nuevaX = std::clamp(static_cast<int>(x()) + dx, 0, 1200 - ancho);
                setX(nuevaX);
            }
            break;

        case Qt::Key_F:
            if (!saltando && puedeContraatacar) {
                modoContraataque = true;
                puedeContraatacar = false;
                timerContraataque->start(2000);
                timerCooldown->start(10000);
                emit contraatacar();
            }
            break;

        default:
            break;
        }
    } else {
        // Lógica para modo acuático
            if (!enModoCarrera) {
                int dx = 0, dy = 0;
                bool vertical = false;

                switch (event->key()) {
                case Qt::Key_W:
                case Qt::Key_Up:
                    dy = -10;
                    setPixmap(spritesAcuaticos[2]);  // Sprite subir
                    vertical = true;
                    break;
                case Qt::Key_S:
                case Qt::Key_Down:
                    dy = 10;
                    setPixmap(spritesAcuaticos[3]);  // Sprite bajar
                    vertical = true;
                    break;
                case Qt::Key_Escape:
                    emit pausarJuego();  // Esto se conecta con Nivel
                    break;
                }

                // Si no se está moviendo verticalmente, revisamos horizontal
                if (!vertical) {
                    frameActualAcuatico = (frameActualAcuatico + 1) % 2; // Alternar entre nadar1 y nadar2

                    if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
                        dx = 10;
                        setPixmap(spritesAcuaticos[frameActualAcuatico]); // hacia la derecha normal
                    }
                    else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
                        dx = -10;
                        setPixmap(spritesAcuaticos[frameActualAcuatico].transformed(QTransform().scale(-1, 1))); // reflejado hacia izquierda
                    }
                }

                // Movimiento final con límites
                int nuevoX = std::clamp(static_cast<int>(x()) + dx, 0, 1200 - ancho);
                int nuevoY = std::clamp(static_cast<int>(y()) + dy, 100, 520);
                setPos(nuevoX, nuevoY);
        }
}   }


void Jugador::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_S && agachado) {
        agachado = false;

        // Volver a la animación de correr si no está en el aire
        if (!saltando && animacionCorrerTimer && !animacionCorrerTimer->isActive())
            animacionCorrerTimer->start(200);
    }
}

void Jugador::actualizarParpadeo() {
    setOpacity(opacity() < 1.0 ? 1.0 : 0.3);
    contadorParpadeo++;

    if (contadorParpadeo > 10) {
        timerInvulnerabilidad->stop();
        setOpacity(1.0);
        invulnerable = false;
        contadorParpadeo = 0;
    }
}

void Jugador::actualizarSpriteCorrer() {
    if (!saltando && !spritesCorrer.isEmpty()) {
        frameActualCorrer = (frameActualCorrer + 1) % spritesCorrer.size();
        setPixmap(spritesCorrer[frameActualCorrer]);
    }
}

void Jugador::actualizarSpriteAcuatico() {
    static bool alternar = false;
    setPixmap(alternar ? spritesAcuaticos[0] : spritesAcuaticos[1]);
    alternar = !alternar;
}

void Jugador::perderVida() {
    if (invulnerable) return;

    vidas--;
    if (vidas < 0) vidas = 0;

    invulnerable = true;
    contadorParpadeo = 0;
    setOpacity(0.5);
    timerInvulnerabilidad->start(100);
}

void Jugador::reiniciar() {
    vidas = 5;
    oxigeno = 100;
    puntaje = 0;
}

void Jugador::recibirDano(int cantidad) {
    perderVida();
}

void Jugador::setPuntaje(int p) {
        puntaje = p;
        verificarMejoraProyectiles();
}
void Jugador::verificarMejoraProyectiles() {
    static int umbral = 30;
    if (puntaje >= umbral) {
        cantidadProyectiles++;
        umbral += 20;  // siguiente mejora
        qDebug() << "¡Cantidad de proyectiles aumentada a:" << cantidadProyectiles;
    }
}
void Jugador::verificarProyectilesActivos() {
    // Eliminar referencias de proyectiles que ya no están en la escena
    for (int i = 0; i < proyectilesActivos.size(); ++i) {
        if (!proyectilesActivos[i]->scene()) {
            proyectilesActivos.removeAt(i--);
        }
    }

    if (proyectilesActivos.isEmpty()) {
        disparoActivo = false;
    }
}


int Jugador::getVidas() const { return vidas; }
void Jugador::setVidas(int v) { vidas = v; }

float Jugador::getOxigeno() const { return oxigeno; }
void Jugador::setOxigeno(float o) { oxigeno = o; }

int Jugador::getPuntaje() const { return puntaje; }

bool Jugador::esModoCarrera() const { return enModoCarrera; }

