#include "oni.h"
#include "table.h"
#include <QBrush>
#include <QImage>

oni::oni(QWidget *parent) {
    int height = 700, width = 1200;

    // set up the screen
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width,height);

    // set up the scene
    scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, width, height);
    scene->setBackgroundBrush(QBrush(QImage(":/pics/wood.svg")));
    setScene(scene);
}

void oni::start() {
    // initilize & draw board
    table = new Table;
    table->drawBoard();

    // draw cardslots
    table->drawCardSlots();
}

void oni::load_game() {

}

void oni::new_game() {
    this->start();
}

void oni::save_game() {

}
