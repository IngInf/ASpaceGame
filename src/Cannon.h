//
// Created by Fabio Luccioletti on 2019-06-30.
//

#ifndef ASPACEGAME_CANNON_H
#define ASPACEGAME_CANNON_H


#include "Projectile.h"
#include <memory>

class Cannon {
public:
    explicit Cannon(const Projectile &projectilePrototype = Projectile(), float fireRateMultiplier = 1,
                    float strengthMultiplier = 1, bool tracker = false,
                    const sf::Vector2f &relativePosition = sf::Vector2f(0, 0));

    std::unique_ptr<Projectile> shoot(const sf::Vector2f &position, float dt, float fireRate);

    void setFireRateMultiplier(float multiplier);

    float getFireRateMultiplier() const;

    Projectile &getProjectilePrototype();

    bool isTracker() const;

    void setElapsedTime(float time);

    float getStrengthMultiplier() const;

    const sf::Vector2f &getLocalRelativePosition() const;

    void setLocalRelativePosition(const sf::Vector2f &position);

private:
    float fireRateMultiplier = 1;
    float strengthMultiplier = 1;
    Projectile projectilePrototype;
    bool tracker = false;
    float elapsedTime = 0;
    sf::Vector2f localRelativePosition = sf::Vector2f(0, 0);

    ///TEST
public:
    float getElapsedTimeTest() const {
        return elapsedTime;
    }
};
#endif //ASPACEGAME_CANNON_H
