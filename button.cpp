#include "button.h"
#include "oni.h"
#include <QTimer>

extern Oni *game;

Button::Button(QGraphicsItem *parent) : QGraphicsPixmapItem(parent), type(""), pos("") {
    setAcceptHoverEvents(true);
}
    //button drawing
void Button::drawButton(QString buttonType, QString buttonPos) {
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

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if (type == "flipButton") setCursor(Qt::PointingHandCursor);
    if (type == "turnBlue" || type == "turnRed" ) {
        drawButton("whiteFlag","right");
        setCursor((Qt::PointingHandCursor));
    }
    event->ignore();
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    if (type == "flipButton")
        setCursor(Qt::ArrowCursor);
    if (type == "whiteFlag") {
        if (game->getFirstPlayersTurn())
            drawButton("turnRed", "right");
        else
            drawButton("turnBlue", "right");
        setCursor((Qt::ArrowCursor));
    }
    event->ignore();
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (type == "flipButton")
        QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionFlipOnce_triggered()) );
    if (type == "whiteFlag")
        QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionResign_triggered()) );
    event->ignore();
}
