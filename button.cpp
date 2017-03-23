#include "button.h"
#include "oni.h"
#include <QTimer>

extern Oni *game;

Button::Button(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    //allow responding to hover events
    setAcceptHoverEvents(true);
}

void Button::drawButton(QString type, QString pos) {
    if (type != "") {
        QPixmap img(":/pics/" + type + ".png");
        double width, height;
        if (pos == "right") {
            width = game->getWindow()->getSideBarSize();
            height = width / img.width() * img.height();
        }
        if (pos == "bottom") {
            height = game->getWindow()->getSideBarSize();
            width = height / img.height() * img.width();
        }
        img = img.scaled(width, height);
        setPixmap(img);
    }
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::PointingHandCursor);
    event->ignore();
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::ArrowCursor);
    event->ignore();
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionFlipOnce_triggered()) );
    event->ignore();
}
