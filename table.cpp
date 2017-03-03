#include "table.h"
#include "oni.h"
#include <QPainter>

extern oni *game;

int myrandom(int i) { return std::rand()%i; }

void Table::drawBoard(double border_x, double border_y, int numOfCols, int numOfRows) {
    // scaling the board to windowsize
    double size = (game->scene->height()-2*border_y)/numOfRows;

    // drawing the board
    for (int i = 0; i < numOfRows; i++)
        for (int j = 0; j < numOfCols; j++) {
            Field *field = new Field(size);
            field->setPos(border_x + size * j, border_y + size * i);
            board.append(field);
            game->scene->addItem(field);
        }
}

void Table::drawCardSlots(double border_x, double border_y) {
    // scaling the cardslots to windowsize
    double size_y = (game->scene->height() - 4*border_y) / 3;
    double size_x = size_y/74*52;

    // determine the cards
    int int_array[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    srand(unsigned(time(NULL)));
    std::random_shuffle(&int_array[0], &int_array[16], myrandom);

    // drawing the cardGrid
    CardSlot *slot_p1_c1 = new CardSlot(size_y);
    slot_p1_c1->setPos(game->scene->height() + border_x, 2*size_y + 3*border_y);
    slot_p1_c1->addCard(int_array[0], slot_p1_c1->rect().width(), slot_p1_c1->rect().height(), border_x, border_y);
    cardGrid.append(slot_p1_c1);
    game->scene->addItem(slot_p1_c1);

    CardSlot *slot_p1_c2 = new CardSlot(size_y);
    slot_p1_c2->setPos(game->scene->height() + size_x + 2*border_x, 2*size_y + 3*border_y);
    slot_p1_c2->addCard(int_array[1], slot_p1_c2->rect().width(), slot_p1_c2->rect().height(), border_x, border_y);
    cardGrid.append(slot_p1_c2);
    game->scene->addItem(slot_p1_c2);

    CardSlot *slot_neutral = new CardSlot(size_y);
    slot_neutral->setPos(game->scene->height() + 0.5*(size_x + 3*border_x), size_y + 2*border_y);
    slot_neutral->addCard(int_array[2], slot_neutral->rect().width(), slot_neutral->rect().height(), border_x, border_y);
    cardGrid.append(slot_neutral);
    game->scene->addItem(slot_neutral);

    CardSlot *slot_p2_c1 = new CardSlot(size_y);
    slot_p2_c1->setPos(game->scene->height() + border_x, border_y);
    slot_p2_c1->addCard(int_array[3], slot_p2_c1->rect().width(), slot_p2_c1->rect().height(), border_x, border_y);
    cardGrid.append(slot_p2_c1);
    game->scene->addItem(slot_p2_c1);

    CardSlot *slot_p2_c2 = new CardSlot(size_y);
    slot_p2_c2->setPos(game->scene->height() + size_x + 2*border_x, border_y);
    slot_p2_c2->addCard(int_array[4], slot_p2_c2->rect().width(), slot_p2_c2->rect().height(), border_x, border_y);
    cardGrid.append(slot_p2_c2);
    game->scene->addItem(slot_p2_c2);

    //drawing the cards
}
