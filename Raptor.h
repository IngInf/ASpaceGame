//
// Created by gianmarco on 30/06/19.
//

#ifndef ASPACEGAME_RAPTOR_H
#define ASPACEGAME_RAPTOR_H

static const int rechargeTime = 5;

#include "Player.h"

class Raptor: public Player{
public:
    Raptor();

    sf::CircleShape &getShield();

    void move(float time, short int movement) override;

    void useShield(float dt);

    void recharge(float dt);

private:
    sf::CircleShape shield;
    float shieldDuration;
public:
    bool isCharging1() const;

private:
    bool isCharging;
};


#endif //ASPACEGAME_RAPTOR_H
