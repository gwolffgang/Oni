#include "button.h"
#include "oni.h"

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
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::ArrowCursor);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    game->flipBoard();
}
