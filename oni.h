#ifndef ONI_H
#define ONI_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "table.h"
#include "card.h"

class oni : public QGraphicsView {
private:
public:
    //variables
    QGraphicsScene *scene;
    Table *table;

    int game_result = 0;            // game_result: 0 = ongoing game; 1 = player 1 has won; -1 = player 2 has won
    int students_left[2] = {4, 4};  // students_left: [0] = first player, [1] = second player
    bool move_first_player = true;  // move_first_player: true = turn of player 1, false = turn of player 2

    //constructors
    oni(QWidget *parent = NULL);

    //functions
    void start();

public slots:
    void load_game();
    void new_game();
    void save_game();
};

#endif // ONI_H
