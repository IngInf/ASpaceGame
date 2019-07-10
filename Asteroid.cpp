//
// Created by gianmarco on 01/07/19.
//
#include <cmath>
#include "ResourceManager.h"
#include "Asteroid.h"
#include "Randomizer.h"
#include "Animator.h"
#include "Game.h"


void Asteroid::move(float dt) {
    elapsedTime += dt;
    if (elapsedTime <= ASTEROID_APPEARANCE_DURATION) {
        sf::Vector2f deltaScale(size / ASTEROID_APPEARANCE_DURATION * dt,
                                size / ASTEROID_APPEARANCE_DURATION * dt);
        sprite.setScale(sprite.getScale() + deltaScale);
        if (elapsedTime >= ASTEROID_FREEZE_DURATION) {
            float t = elapsedTime - ASTEROID_FREEZE_DURATION;
            float deltaT = ASTEROID_APPEARANCE_DURATION - ASTEROID_FREEZE_DURATION;
            sprite.setPosition(sprite.getPosition().x,
                               initialPosition.y + 0.5 * (speed / (deltaT)) * pow(t, 2));
        }
    } else
        sprite.move(0, speed * dt);
}

Asteroid::Asteroid() : speed(Randomizer::getRandomReal(asteroidMinSpeed, asteroidMaxSpeed)),
                       size(Randomizer::getRandomReal(asteroidMinSize, asteroidMaxSize)) {
    hp = maxHp / asteroidMaxSize * size;
    startingHp = hp;
    auto &rotation = animator->createAnimation("Rotation", "../Texture/Asteroid.png", sf::seconds(1), true);
    unsigned int frames = 8, rows = 4, animInFile = 2;
    int startAnim = Randomizer::getRandomInt(0, 1) ? 0 : 128 * (rows);
    rotation.addFrames(sf::Vector2i(0, startAnim), sf::Vector2i(128, 128), frames, rows);

    sprite.setScale(0, 0);
    sf::Vector2f distOrigin(sprite.getLocalBounds().width / (2 * frames),
                            sprite.getLocalBounds().height / (2 * rows * animInFile));
    initialPosition = Randomizer::getRandomPosition(
            distOrigin.x * size, windowWidth - distOrigin.x * size, distOrigin.y * size,
            distOrigin.y * size + asteroidMaxSpawnHeight);
    sprite.setOrigin(distOrigin);
    sprite.setPosition(initialPosition);
}

void Asteroid::receiveDamage(float damageReceived) {
    receivingDamage = true;
    this->hp -= damageReceived;
}

void Asteroid::blink(float time) {
    blinkingTime += time;
    if (blinkingTime <= ENEMY_BLINK_DURATION)
        sprite.setColor(sf::Color(230, 130, 130));
    if (blinkingTime > ENEMY_BLINK_DURATION) {
        blinkingTime = 0;
        setReceivingDamage(false);
        sprite.setColor(sf::Color::White);
    }
}

bool Asteroid::die(float time) {
    dyingTime += time;
    sprite.setColor(sf::Color(255, 255, 255, 255 - static_cast<int>(255. * dyingTime / DYING_DURATION)));
    return dyingTime >= DYING_DURATION;
}

sf::Sprite &Asteroid::getSprite() {
    return sprite;
}

Animator *Asteroid::getAnimator() {
    return animator;
}

float Asteroid::getDamage() const {
    return damage * size;
}

float Asteroid::getSize() const {
    return size;
}

bool Asteroid::isReceivingDamage() const {
    return receivingDamage;
}

void Asteroid::setReceivingDamage(bool receiveDamage) {
    Asteroid::receivingDamage = receiveDamage;
}

float Asteroid::getHp() const {
    return hp;
}

void Asteroid::setHp(float newHp) {
    Asteroid::hp = newHp;
}

float Asteroid::getStartingHp() const {
    return startingHp;
};
