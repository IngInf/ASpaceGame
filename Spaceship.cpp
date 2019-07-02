//
// Created by gianmarco on 30/06/19.
//
#include "ResourceManager.h"
#include "Spaceship.h"

#define scale 0.35f

Spaceship::Spaceship(int hp, int strength, float speed) : hp(hp), strength(strength), speed(speed) {
    sprite.setScale(scale, scale);
}

int Spaceship::getHp() const {
    return hp;
}

void Spaceship::setHp(int hp) {
    Spaceship::hp = hp;
}

int Spaceship::getStrength() const {
    return strength;
}

void Spaceship::setStrength(int strength) {
    Spaceship::strength = strength;
}

float Spaceship::getSpeed() const {
    return speed;
}

void Spaceship::setSpeed(float speed) {
    Spaceship::speed = speed;
}

sf::Sprite Spaceship::getSprite() {
    return sprite;
}

