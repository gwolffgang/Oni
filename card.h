#ifndef CARD_H
#define CARD_H

#include <QGraphicsPixmapItem>

enum CardID {
    BOAR = 1, COBRA, CRAB, CRANE, DRAGON, EEL, ELEPHANT, FROG, GOOSE, HORSE, MANTIS, MONKEY, OX, RABBIT, ROOSTER, TIGER,
    BEAR, DOG, FOX, GIRAFFE, IGUANA, KIRIN, MOUSE, OTTER, PANDA, PHOENIX, RAT, SABLE, SEASNAKE, TANUKI, TURTLE, VIPER,
    GOAT, SHEEP
};

class Card : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    // constructor
    Card(QGraphicsItem *parent = nullptr);

    // operators
    inline bool operator==(Card *card) { if (card->id == id) return true; else return false; }

    // getter
    inline QString getName() { return name; }
    inline int getID() { return id; }
    inline int getColFromChoice(const int number) { return choice[number][0]; }
    inline int getRowFromChoice(const int number) { return choice[number][1]; }

    // methods
    void setCardValues(int cardID);
    void draw(int player);
    void drawYinYang(QPainter *painter, int x, int y);

private:
    // variables
    QString name;
    int choice[4][2], id;
    QPixmap card;
};

#endif // CARD_H
