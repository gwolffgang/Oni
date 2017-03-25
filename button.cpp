#include "button.h"
#include "oni.h"
#include <QTimer>

extern Oni *game;

Button::Button(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    //allow responding to hover events
    setAcceptHoverEvents(true);
}
    //button drawing
void Button::drawButton(QString buttonType, QString buttonPos) {
    type = buttonType;
    pos = buttonPos;
    if (type != "") {
        QPixmap img(":/pics/buttons/" + type + ".png");
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
    // possible button-events
void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if (type == "flipButton") setCursor(Qt::PointingHandCursor);
    event->ignore();
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    if (type == "flipButton") setCursor(Qt::ArrowCursor);
    event->ignore();
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (type == "flipButton") QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionFlipOnce_triggered()) );
    event->ignore();
}
