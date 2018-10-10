#include "button.h"
#include "oni.h"
#include <QTimer>

extern Oni *game;

Button::Button(QGraphicsItem *parent) : QGraphicsPixmapItem(parent), type(""), pos("") {
    setAcceptHoverEvents(true);
}

void Button::draw(QString buttonType, QString buttonPos) {
    type = buttonType;
    pos = buttonPos;
    double sideBarSize = game->getWindow()->getSideBarSize();

    if (type != "") {
        QPixmap button(":/pics/buttons/" + type + ".png");
        double width = 0;
        double height = 0;
        if (pos == "right") {
            width = sideBarSize;
            height = width / button.width() * button.height();
        } else if (pos == "bottom") {
            height = sideBarSize;
            width = height / button.height() * button.width();
        }
        button = button.scaled(static_cast<int>(width), static_cast<int>(height));
        setPixmap(button);
    }
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    if (type == "flipButton") setCursor(Qt::PointingHandCursor);
    if (type == "turnBlue" || type == "turnRed" ) {
        draw("whiteFlag","right");
        setCursor((Qt::PointingHandCursor));
    }
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    if (type == "flipButton")
        setCursor(Qt::ArrowCursor);
    if (type == "whiteFlag") {
        if (game->getFirstPlayersTurn())
            draw("turnRed", "right");
        else
            draw("turnBlue", "right");
        setCursor((Qt::ArrowCursor));
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *) {
    if (type == "flipButton")
        QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionFlipOnce_triggered()) );
    if (type == "whiteFlag")
        QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionResign_triggered()) );
}
