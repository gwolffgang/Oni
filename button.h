#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>

class Button : public QGraphicsPixmapItem {

public:
    // constructor
    Button(QGraphicsItem *parent = nullptr);

    // methods
    void draw(QString buttonType = "", QString buttonPos = "right");

protected:
    // variables
    QString type, pos;

    // events
    void hoverEnterEvent(QGraphicsSceneHoverEvent *) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
};

#endif // BUTTON_H
