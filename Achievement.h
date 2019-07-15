//
// Created by gianmarco on 12/07/19.
//

#ifndef ASPACEGAME_ACHIEVEMENT_H
#define ASPACEGAME_ACHIEVEMENT_H


#include "Observer.h"
#include "EntityManager.h"

class Achievement : public Observer {
public:
    void attach() override;

    void detach() override;

    void update() override;

    std::list<sf::Sprite> &getSprites();

    explicit Achievement(EntityManager *subject);

private:
    EntityManager *subject;
    unsigned int asteroidsBadges;
    unsigned int scoreBadges;
    unsigned int spaceshipsBadges;
    unsigned int bossesBadges;

    void checkForAsteroids();

    void checkForSpaceships();

    void checkForBosses();

    void checkForScore();

    std::list<sf::Sprite> sprites;
    sf::Sprite sprite;

};


#endif //ASPACEGAME_ACHIEVEMENT_H
