//
// Created by Fabio Luccioletti on 2019-07-02.
//

#ifndef ASPACEGAME_MINION_H
#define ASPACEGAME_MINION_H


#include "Enemy.h"

class Minion : public Enemy {
public:
    Minion();

    void move() override;

};

#endif //ASPACEGAME_MINION_H
