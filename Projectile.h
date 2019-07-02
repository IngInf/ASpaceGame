//
// Created by gianmarco on 01/07/19.
//

#ifndef ASPACEGAME_PROJECTILE_H
#define ASPACEGAME_PROJECTILE_H


#include <SFML/Graphics.hpp>

class Cannon;

class Projectile {
public:
    Projectile();

    Projectile(sf::Vector2f size, float speed, Cannon *cannonPtr, sf::Vector2f movement, int damage);
    void move();

private:
    sf::Vector2f size;
    float speed;
    sf::Sprite sprite;
    Cannon *cannonPtr; //TODO
    sf::Vector2f movement;
    int damage;
};


#endif //ASPACEGAME_PROJECTILE_H
