#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>

class Button : public QGraphicsPixmapItem {

public:
    // constructor
    Button(QGraphicsItem *parent = NULL);

    // methods
    void drawButton(QString buttonType = "", QString buttonPos = "right");

protected:
    // variables
    QString type, pos;

    // events
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // BUTTON_H
