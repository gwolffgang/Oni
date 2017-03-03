#ifndef FIELD_H
#define FIELD_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Field : public QGraphicsRectItem {
private:
    //variables
    // int row, col;
    // char piece = ' ';

public:
    //constructors
    Field(double size, QGraphicsItem *parent = NULL);

    //events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void clicked() {}
};

#endif // FIELD_H
