//
// Created by gianmarco on 30/06/19.
//

#ifndef ASPACEGAME_SPACESHIP_H
#define ASPACEGAME_SPACESHIP_H

static const float maxScale = 0.35f;

#include <SFML/Graphics.hpp>
#include "Cannon.h"
#include <memory>

class Spaceship {
public:
    Spaceship(int hp, int strength, float speed);

    virtual ~Spaceship();

    virtual void move(float time) = 0;

    virtual std::unique_ptr<Projectile> useCannon(float dt, Cannon *cannon);

    bool receiveDamage(int damage);

    int getHp() const;

    void setHp(int hp);

    int getStrength() const;

    void setStrength(int strength);

    float getSpeed() const;

    void setSpeed(float speed);

    sf::Sprite &getSprite();

    Cannon &getPrimaryCannon();

protected:
    int hp;
    int strength;
    float speed;
    sf::Sprite sprite;
    Cannon primaryCannon;
    float elapsedTime = 0;

};


#endif //ASPACEGAME_SPACESHIP_H
