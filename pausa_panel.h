#ifndef PAUSA_PANEL_H
#define PAUSA_PANEL_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class PausaPanel : public QWidget {
    Q_OBJECT

public:
    explicit PausaPanel(QWidget* parent = nullptr);
    void mostrarConAnimacion();

signals:
    void reanudarJuego();
    void reiniciarNivel();
    void salirAlMenu();
};

#endif // PAUSA_PANEL_H
