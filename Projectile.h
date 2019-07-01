//
// Created by gianmarco on 01/07/19.
//

#ifndef ASPACEGAME_PROJECTILE_H
#define ASPACEGAME_PROJECTILE_H


#include <SFML/Graphics.hpp>
#include "Cannon.h"

class Projectile {
public:
    void move();

private:
    sf::Vector2f size;
    float speed;
    sf::Sprite sprite;
    Cannon *cannonPtr;
    sf::Vector2f movement;
};


#endif //ASPACEGAME_PROJECTILE_H
