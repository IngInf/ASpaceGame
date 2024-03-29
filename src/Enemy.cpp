//
// Created by gianmarco on 30/06/19.
//

#include "Enemy.h"
#include "ResourceManager.h"
#include "Functions.h"
#include "Game.h"

void Enemy::move(float time) {
    if (!(Game::isLegalMove(sprite.getPosition().x, sprite.getScale().x * sprite.getOrigin().x, direction)))
        direction = -direction;
    sprite.move(direction * speed * time, 0);
    boundingBox.setPosition(sprite.getPosition());
}

Enemy::Enemy(float hp, float strength, float speed, float fireRate, const Cannon &cannon, int explosionNum)
        : Spaceship(hp, strength, speed, fireRate, cannon, explosionNum) {
    direction = right;
    sprite.setRotation(180.f);
    elapsedTime = -enemySpawnDuration;
    sprite.setScale(0, 0);
}

void Enemy::setPosition(float x, float y) {
    sprite.setPosition(sf::Vector2f(x, y));
}

void Enemy::setPosition(const sf::Vector2f &pos) {
    sprite.setPosition(pos);
}

void Enemy::blink(float time) {
    blinkingTime += time;
    if (blinkingTime <= enemyBlinkDuration)
        sprite.setColor(sf::Color(230, 130, 130));
    if (blinkingTime > enemyBlinkDuration) {
        blinkingTime = 0;
        setReceivingDamage(false);
        sprite.setColor(sf::Color::White);
    }
}

bool Enemy::die(float time) {
    if (dyingTime == 0) {
        explosionSound.play();
        for (auto &explosion : explosions)
            explosion.setPosition(getRandomPosition(
                    sprite.getGlobalBounds().left, sprite.getGlobalBounds().left + sprite.getGlobalBounds().width,
                    sprite.getGlobalBounds().top, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height));
        boundingBox.setSize(sf::Vector2f(0, 0));
    }
    int i = 0;
    for (auto &animator : animators) {
        animator->update(time, ((dyingDuration - explosionDuration) / explosionNum) * i++);
    }
    dyingTime += time;
    sprite.setColor(sf::Color(255, 255, 255, 255 - static_cast<sf::Uint8>(255. * dyingTime / dyingDuration)));
    return dyingTime >= dyingDuration;
}

void Enemy::spawn(float time) {
    sprite.setScale(sprite.getScale() + sf::Vector2f(1, 1) * std::min(maxScale, maxScale / enemySpawnDuration * time));
    boundingBox.setScale(sprite.getScale());
}

Enemy::~Enemy() = default;
