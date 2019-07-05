//
// Created by Fabio Luccioletti on 2019-07-02.
//

#include "Minion.h"
#include "ResourceManager.h"
#include "Randomizer.h"

Minion::Minion() : Enemy(50, 10, 100) { //todo adjust values
    sprite.setTexture(ResourceManager::getTexture("../Texture/Minion.png"));
    sprite.setPosition(Randomizer::getRandomPosition(100, windowWidth - 100, 150, 150));
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    primaryCannon.setFireRate(0.4);
    primaryCannon.setSpaceshipPtr(this);
    Projectile projectilePrototype(sf::Vector2f(0.5, 0.5), 200, sf::Vector2f(0, 1), strength);
    primaryCannon.setProjectilePrototype(projectilePrototype);
}

void Minion::move(float time) {
    elapsedTime += time;
    if (elapsedTime < 1) {
        Enemy::move(time);
    }
    else if (elapsedTime > 2)
        elapsedTime = 0;
}
