#include "table.h"
#include "oni.h"

extern Oni *game;

int myrandom(int i) { return std::rand()%i; }

void Table::drawBoard() {
    // scaling the board to windowsize
    double size = (game->getScene()->height() - 2*game->getBorderY()) / game->getRows();

    // drawing the board
    board = new QList<QList<Field*>>;
    QList<Field*> fieldsRow;
    for (int row = 0; row < game->getRows(); row++) {
        fieldsRow.empty();
        for (int col = 0; col < game->getCols(); col++) {
            Field *field = new Field(size);
            field->setRow(row);
            field->setCol(col);
            field->setPos(game->getBorderX() + size * col, game->getBorderY() + size * (4-row));
            if (game->getPieces())
                for (int i = 0; i < game->getPieces()->size(); i++)
                    if (game->getPieces()->at(i)->getRow() == row && game->getPieces()->at(i)->getCol() == col) {
                        field->addPiece(game->getPieces()->at(i)->getType(), size);
                        field->setPiecetype(game->getPieces()->at(i)->getType());
                        field->setPiece(game->getPieces()->at(i));
                    }
            game->getScene()->addItem(field);
            fieldsRow.append(field);
        }
        board->append(fieldsRow);
    }
}

void Table::drawCardSlots(double border_x, double border_y) {
    // scaling the cardslots to windowsize
    double size_y = (game->getScene()->height() - 4*border_y) / 3;
    double size_x = size_y/74*52;

    // determine the cards
    int int_array[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    srand(unsigned(time(NULL)));
    std::random_shuffle(&int_array[0], &int_array[16], myrandom);

    // drawing the cardGrid
    CardSlot *slot_p1_c1 = new CardSlot(size_y);
    slot_p1_c1->setPos(game->getScene()->height() + border_x, 2*size_y + 3*border_y);
    slot_p1_c1->setOwner(1);
    slot_p1_c1->addCard(int_array[0], slot_p1_c1->rect().width(), slot_p1_c1->rect().height());
    game->getScene()->addItem(slot_p1_c1);

    CardSlot *slot_p1_c2 = new CardSlot(size_y);
    slot_p1_c2->setPos(game->getScene()->height() + size_x + 2*border_x, 2*size_y + 3*border_y);
    slot_p1_c2->setOwner(1);
    slot_p1_c2->addCard(int_array[1], slot_p1_c2->rect().width(), slot_p1_c2->rect().height());
    game->getScene()->addItem(slot_p1_c2);

    CardSlot *slot_neutral = new CardSlot(size_y);
    slot_neutral->setPos(game->getScene()->height() + 0.5*(size_x + 3*border_x), size_y + 2*border_y);
    slot_neutral->setOwner(0);
    slot_neutral->addCard(int_array[2], slot_neutral->rect().width(), slot_neutral->rect().height());
    game->getScene()->addItem(slot_neutral);

    CardSlot *slot_p2_c1 = new CardSlot(size_y);
    slot_p2_c1->setPos(game->getScene()->height() + border_x, border_y);
    slot_p2_c1->setOwner(2);
    slot_p2_c1->addCard(int_array[3], slot_p2_c1->rect().width(), slot_p2_c1->rect().height());
    game->getScene()->addItem(slot_p2_c1);

    CardSlot *slot_p2_c2 = new CardSlot(size_y);
    slot_p2_c2->setPos(game->getScene()->height() + size_x + 2*border_x, border_y);
    slot_p2_c2->setOwner(2);
    slot_p2_c2->addCard(int_array[4], slot_p2_c2->rect().width(), slot_p2_c2->rect().height());
    game->getScene()->addItem(slot_p2_c2);
}
