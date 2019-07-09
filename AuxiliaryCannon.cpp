//
// Created by Fabio Luccioletti on 2019-07-01.
//

#include "AuxiliaryCannon.h"
#include "ResourceManager.h"
#include "Game.h"
#include "Randomizer.h"
#include "Raptor.h"

AuxiliaryCannon::AuxiliaryCannon() : PowerUp(true) {}

void AuxiliaryCannon::powerUp(Player *player) { //todo prendi reference
    Cannon cannon(player->getPrimaryCannon());
    if (player->getAuxiliaryCannons().empty()) {
        sf::Vector2f relativePosition;
        if (typeid(*player) == typeid(Raptor)) {
            player->getSprite().setTexture(ResourceManager::getTexture("../Texture/RaptorCannon.png"));
            relativePosition = sf::Vector2f(76, 0);
        } else {
            player->getSprite().setTexture(ResourceManager::getTexture("../Texture/BomberCannon.png"));
            relativePosition = sf::Vector2f(106, 0);
        }

        cannon.setLocalRelativePosition(relativePosition);
        player->getPrimaryCannon().setLocalRelativePosition(sf::Vector2f(-relativePosition.x, 0));
        player->getAuxiliaryCannons().push_back(cannon);
    } else if (player->getAuxiliaryCannons().size() < 2) {
        sf::Vector2f relativePosition(0, 0);
        cannon.setLocalRelativePosition(relativePosition);
        player->getAuxiliaryCannons().push_back(cannon);
    }
}
