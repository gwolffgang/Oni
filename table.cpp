#include "table.h"
#include "oni.h"

extern Oni *game;

int myrandom(int i) { return std::rand()%i; }

void Table::drawBoard() {
    // drawing the board
    board = new QList<QList<Field*>>;
    QList<Field*> fieldsRow;
    for (int row = 0; row < game->getRows(); row++) {
        fieldsRow.clear();
        for (int col = 0; col < game->getCols(); col++) {
            // create field
            Field *field = new Field(game->getFieldHeight());
            field->setRow(row);
            field->setCol(col);
            field->setPos(game->getBorderX() + game->getFieldHeight() * col, game->getBorderY() + game->getFieldHeight() * (4-row));
            // add piece to field
            if (game->getPieces()) {
                int pieceNumber = field->identifyPiece();
                if (pieceNumber != -1) {
                    field->linkPiece(game->getPieces()->at(pieceNumber));
                    field->getPiece()->drawPiece();
                }
            }
            // add field to scene
            game->getScene()->addItem(field);
            // add field to board
            fieldsRow.append(field);
        }
        board->append(fieldsRow);
    }
}

void Table::drawCardSlots() {
    // scaling the cardslots to windowsize
    double sizeY = (game->getScene()->height() - 4*game->getBorderY()) / 3;
    double sizeX = sizeY;

    // determine the cards
    int intArray[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    srand(unsigned(time(NULL)));
    std::random_shuffle(&intArray[0], &intArray[16], myrandom);

    // drawing the slotsGrid
    slotsGrid = new QList<QList<CardSlot*>>;
    QList<CardSlot*> slotsRow;
    int elem = 0;
    for (int player = 0; player < 3; player++) {
        slotsRow.clear();
        int maxSlots = game->getCardsPerPlayer();
        if (player == 0) maxSlots = game->getNeutralCardsPerPlayer();
        for (int number = 0; number < maxSlots; number++) {
            CardSlot *slot = new CardSlot(sizeY);
            slot->setOwner(player);
            double posX = game->getScene()->height() + number * sizeX + (number + 1) * game->getBorderX();
            double posY = ((-1.5 * player + 2.5) * player + 1) * sizeY + ((-1.5 * player + 2.5) * player + 2) * game->getBorderY();
            slot->setPos(posX, posY);
            slot->addCard(intArray[elem++], slot->rect().width(), slot->rect().height(), player);
            game->getScene()->addItem(slot);
            slotsRow.append(slot);
        }
        slotsGrid->append(slotsRow);
    }
}
