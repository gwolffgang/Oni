#include "table.h"
#include "oni.h"
#include <QPainter>

extern Oni *game;

int myrandom(int i) { return std::rand()%i; }

void Table::drawBoard(double border_x, double border_y, int numOfCols, int numOfRows) {
    // scaling the board to windowsize
    double size = (game->scene->height()-2*border_y)/numOfRows;

    // drawing the board
    board = new QList<QList<Field*>>;
    QList<Field*> row;
    for (int i = 0; i < numOfRows; i++) {
        row.empty();
        for (int j = 0; j < numOfCols; j++) {
            Field *field = new Field(size);
            field->setCol(i);
            field->setRow(j);
            field->setPos(border_x + size * j, border_y + size * (4-i));
            if (game->pieces)
                for (int k = 0; k < game->pieces->size(); k++)
                    if (game->pieces->at(k)->getRow() == i && game->pieces->at(k)->getCol() == j)
                        field->addPiece(game->pieces->at(k)->getType(), size);
            game->scene->addItem(field);
            row.append(field);
        }
        board->append(row);
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
    slot_p1_c1->setOwner(1);
    slot_p1_c1->addCard(int_array[0], slot_p1_c1->rect().width(), slot_p1_c1->rect().height());
    game->scene->addItem(slot_p1_c1);

    CardSlot *slot_p1_c2 = new CardSlot(size_y);
    slot_p1_c2->setPos(game->scene->height() + size_x + 2*border_x, 2*size_y + 3*border_y);
    slot_p1_c2->setOwner(1);
    slot_p1_c2->addCard(int_array[1], slot_p1_c2->rect().width(), slot_p1_c2->rect().height());
    game->scene->addItem(slot_p1_c2);

    CardSlot *slot_neutral = new CardSlot(size_y);
    slot_neutral->setPos(game->scene->height() + 0.5*(size_x + 3*border_x), size_y + 2*border_y);
    slot_neutral->setOwner(0);
    slot_neutral->addCard(int_array[2], slot_neutral->rect().width(), slot_neutral->rect().height());
    game->scene->addItem(slot_neutral);

    CardSlot *slot_p2_c1 = new CardSlot(size_y);
    slot_p2_c1->setPos(game->scene->height() + border_x, border_y);
    slot_p2_c1->setOwner(2);
    slot_p2_c1->addCard(int_array[3], slot_p2_c1->rect().width(), slot_p2_c1->rect().height());
    game->scene->addItem(slot_p2_c1);

    CardSlot *slot_p2_c2 = new CardSlot(size_y);
    slot_p2_c2->setPos(game->scene->height() + size_x + 2*border_x, border_y);
    slot_p2_c2->setOwner(2);
    slot_p2_c2->addCard(int_array[4], slot_p2_c2->rect().width(), slot_p2_c2->rect().height());
    game->scene->addItem(slot_p2_c2);
}

void Table::setUpPieces() {
    // game->pieces
    // char piece = game->pieces[1][1];
    // if (piece != ' ') board[1].at(1).field.addPiece(piece, board[1].at(1).   .field->rect().width());

}
