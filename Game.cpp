//
// Created by Fabio Luccioletti on 2019-06-30.
//

#include <iostream>
#include "Game.h"
#include "ResourceManager.h"
#include "Bomber.h"
#include "Kamikaze.h"
#include "Fighter.h"
#include "Boss.h"
#include "Minion.h"
#include "Assaulter.h"
#include "Raptor.h"
#include "Background.h"
#include "FireRate.h"
#include "Speed.h"
#include "Asteroid.h"
#include "Player.h"
#include "Spaceship.h"
#include "EnhanceSpecial.h"
#include "AuxiliaryCannon.h"
#include <memory>
#include <math.h>

Game::Game() : window(sf::VideoMode(windowWidth, windowHeight), "A Space Game"), isPaused(false),
               isMovingLeft(false), isMovingRight(false), isShooting(false), isUsingSpecial(false),
               view((sf::FloatRect(0, 0, window.getSize().x, window.getSize().y))) {
    specialHud.setSize(sf::Vector2f(15, 100));
    specialHud.setPosition(400, 500);
    specialHud.rotate(-90.f);
    specialHud.setFillColor(sf::Color::Red);
    specialHudOutline.setSize(sf::Vector2f(15, 100));
    specialHudOutline.setPosition(400, 500);
    specialHudOutline.rotate(-90.f);
    specialHudOutline.setFillColor(sf::Color(255, 255, 255, 0));
    specialHudOutline.setOutlineThickness(2);
    specialHudOutline.setOutlineColor(sf::Color::White);

    //Player's spaceship creation
    /*for (int i = 0; i < 5; i++) {
        auto *boss = new Boss;
        boss->setPosition(50 * i, 200);
        enemyManager.insert(enemyManager.begin(), boss);
    }*/

    auto *boss = new Boss;
    auto *fighter = new Fighter;
    auto *kamikaze = new Kamikaze;
    auto *minion = new Minion;
    auto *assaulter = new Assaulter;

    kamikaze->setPosition(20, 300);
    fighter->setPosition(700, 200);
    enemyManager.insert(enemyManager.begin(), fighter);
    enemyManager.insert(enemyManager.begin(), kamikaze);
    enemyManager.insert(enemyManager.begin(), assaulter);
    enemyManager.insert(enemyManager.begin(), minion);
    enemyManager.insert(enemyManager.begin(), boss);


    player = new Raptor;

    powerUp = std::unique_ptr<PowerUp>(new AuxiliaryCannon);

    //Background creation
    background = new Background;

    //Limitation of the framerate
    window.setFramerateLimit(60);

    for (int i = 0; i < 100; i++) {
        std::unique_ptr<Asteroid> asteroid(new Asteroid);
        asteroidManager.insert(asteroidManager.begin(), std::move(asteroid));
    }
}

void Game::run() {
    sf::Clock clock;
    sf::Time deltaTime;
    while (window.isOpen()) {
        deltaTime = clock.restart();
        processEvents();
        if (!isPaused)
            update(deltaTime);
        render();

    }

}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::GainedFocus:
                isPaused = false;
                break;
            case sf::Event::LostFocus:
                isPaused = true;
                break;
            default:
                break;
        }
    }
}

void Game::update(sf::Time dt) {

    //Player movement
    float time = dt.asSeconds();
    updateAsteroids(dt); //todo all take delta time
    updateEnemies(time);
    updatePlayer(time);
    updateProjectiles(time);
    updatePowerUp(dt);
    background->scroll(time);
    //View updating
    view.setCenter(static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2);
    window.setView(view);
}

void Game::render() {
    window.clear(sf::Color::Black);

    drawBackground();
    drawAsteroids();
    drawEnemies();
    drawPlayer();
    drawProjectiles();
    drawPowerUp();
    window.draw(specialHud);
    window.draw(specialHudOutline);


    window.display();
}

void Game::drawPowerUp() {
    if (powerUp != nullptr)
        window.draw(powerUp->getSprite());
}

void Game::drawProjectiles() {
    for (auto &i : projectileManager) {
        window.draw(i->getSprite());
    }
}

void Game::drawPlayer() {
    window.draw(player->getSprite());

    if (isUsingSpecial) {
        if (typeid(*player) == typeid(Raptor)) {
            window.draw(dynamic_cast<Raptor &>(*player).getShield());
        }
    }
}

void Game::drawEnemies() {
    for (auto &i : enemyManager) {
        window.draw(i->getSprite());
    }
}

void Game::drawBackground() {
    window.draw(background->getSprite1());
    window.draw(background->getSprite2());
}

void Game::drawAsteroids() {
    for (auto &i : asteroidManager) {
        window.draw(i->getSprite());
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::Left)
        isMovingLeft = isPressed;
    else if (key == sf::Keyboard::Right)
        isMovingRight = isPressed;
    else if (key == sf::Keyboard::Z)
        isShooting = isPressed;
    else if (key == sf::Keyboard::Q)
        isUsingSpecial = isPressed;
}

bool Game::isLegalMove(float x, float origin, short int direction) {
    return !((x <= origin && direction == left) || (x >= windowWidth - origin && direction == right));
}

void Game::updatePowerUp(sf::Time dt) {
    if (powerUp != nullptr) {
        powerUp->getAnimator()->update(dt);
        powerUp->move(dt.asSeconds());
        if (isOutOfSigth(powerUp->getSprite())) {
            powerUp.reset();
        } else if (powerUp->getSprite().getGlobalBounds().intersects(player->getBoundingBox().getGlobalBounds())) {
            powerUp->powerUp(player);
            powerUp.reset(new AuxiliaryCannon);
        }
    }
}

void Game::updatePlayer(float time) {
    if (player->isReceivingDamage()) {
        player->blink(time);
    }
    if (isMovingRight)
        player->move(time, right);

    if (isMovingLeft)
        player->move(time, left);

    if (isShooting) {
        std::unique_ptr<Projectile> projectile = player->useCannon(time, &(player->getPrimaryCannon()));
        emplaceProj(std::move(projectile));
        for (auto &j : player->getAuxiliaryCannons()) {
            Cannon *cannon = &j;
            projectile = player->useCannon(time, cannon);
            emplaceProj(std::move(projectile));
        }
    }
    if (isUsingSpecial) {
        if (typeid(*player) == typeid(Bomber)) {
            if (!dynamic_cast<Bomber &>(*player).isCharging()) {
                std::unique_ptr<Projectile> projectile;
                projectile = dynamic_cast<Bomber &>(*player).useBomb(time, specialHud);
                if (projectile != nullptr)
                    projectileManager.emplace_back(new Projectile(*projectile));
            }
        }
        if (typeid(*player) == typeid(Raptor)) {
            if (!dynamic_cast<Raptor &>(*player).isCharging())
                dynamic_cast<Raptor &>(*player).useShield(time, specialHud);
        }
    } else {
        if (typeid(*player) == typeid(Bomber)) {
            if (dynamic_cast<Bomber &>(*player).isCharging())
                dynamic_cast<Bomber &>(*player).recharge(time, specialHud);
        }
        if (typeid(*player) == typeid(Raptor)) {
            if (dynamic_cast<Raptor &>(*player).isCharging())
                dynamic_cast<Raptor &>(*player).recharge(time, specialHud);
        }
    }
}

void Game::updateEnemies(float time) {
    for (auto enemyIter = enemyManager.begin(); enemyIter != enemyManager.end();) {
        auto enemy = *enemyIter;
        if ((enemy)->getHp() <= 0) {
            if ((enemy)->die(time)) {
                enemyManager.erase(enemyIter++);
            } else
                enemyIter++;
        } else {
            enemyIter++;
            if ((enemy)->isReceivingDamage()) {
                (enemy)->blink(time);
            }
            std::unique_ptr<Projectile> projectile;

            if (typeid(*enemy) == typeid(Minion)) {
                dynamic_cast<Minion &>(*(enemy)).move(time);
                projectile = dynamic_cast<Minion &>(*(enemy)).useCannon(time, &((enemy)->getPrimaryCannon()));
                emplaceProj(std::move(projectile));

            } else if (typeid(*enemy) == typeid(Boss)) {
                dynamic_cast<Boss &>(*(enemy)).move(time);
                projectile = dynamic_cast<Boss &>(*(enemy)).useCannon(time, &((enemy)->getPrimaryCannon()));
                emplaceProj(std::move(projectile));

            } else if (typeid(*enemy) == typeid(Kamikaze)) {
                dynamic_cast<Kamikaze &>(*(enemy)).move(time);
                projectile = dynamic_cast<Kamikaze &>(*(enemy)).useCannon(time, &((enemy)->getPrimaryCannon()));
                emplaceProj(std::move(projectile));

            } else if (typeid(*enemy) == typeid(Fighter)) {
                dynamic_cast<Fighter &>(*(enemy)).move(time);
                projectile = dynamic_cast<Fighter &>(*(enemy)).useCannon(time, &((enemy)->getPrimaryCannon()));
                emplaceProj(std::move(projectile));
                for (auto &j : dynamic_cast<Fighter &>(*(enemy)).getExternalCannons()) {
                    Cannon *cannon = &j;
                    projectile = dynamic_cast<Fighter &>(*(enemy)).useCannon(time, cannon);
                    emplaceProj(std::move(projectile));
                }
            } else if (typeid(*enemy) == typeid(Assaulter)) {
                dynamic_cast<Assaulter &>(*(enemy)).move(time);
                projectile = dynamic_cast<Assaulter &>(*(enemy)).useCannon(time, &((enemy)->getPrimaryCannon()),
                                                                           player->getSprite().getPosition());
                emplaceProj(std::move(projectile));
            }

        }
    }
}

void Game::updateAsteroids(sf::Time dt) {
    for (auto asteroidIter = asteroidManager.begin(); asteroidIter != asteroidManager.end();) {
        (*asteroidIter)->getAnimator()->update(dt);
        (*asteroidIter)->move(dt.asSeconds());
        checkForAsteroidsCollisions(asteroidIter++);
    }
}

void Game::updateProjectiles(float time) {
    for (auto projectileIter = projectileManager.begin(); projectileIter != projectileManager.end();) {
        (*projectileIter)->move(time);
        checkForProjectileCollisions(projectileIter++);
    }
}

void Game::checkForProjectileCollisions(std::list<std::unique_ptr<Projectile>>::iterator projectileIter) {
    //todo simplify
    sf::Sprite &projSprite = (*projectileIter)->getSprite();
    if (isOutOfSigth(projSprite))
        projectileManager.erase(projectileIter);
    else {
        bool iteratorDeleted = false;
        if ((*projectileIter)->isEvil()) {
            if (typeid(*player) == typeid(Raptor)) {
                if (isUsingSpecial && !dynamic_cast<Raptor &>(*player).isCharging()) {
                    auto shield = dynamic_cast<Raptor &>(*player).getShield();
                    if (dist(projSprite.getPosition(), shield.getPosition()) <=
                        (shield.getRadius() + projSprite.getGlobalBounds().height / 2)) {
                        projectileManager.erase(projectileIter);
                        iteratorDeleted = true;
                    }
                }
            }
            if (!iteratorDeleted &&
                player->getBoundingBox().getGlobalBounds().intersects((projSprite.getGlobalBounds()))) {
                player->receiveDamage((*projectileIter)->getDamage()); //todo handle death in one position
                projectileManager.erase(projectileIter);
            }
        } else { //todo bounding box or circle collision if projectile is a circle
            for (auto asteroidIter = asteroidManager.begin(); asteroidIter != asteroidManager.end(); asteroidIter++) {
                if ((*asteroidIter)->getSprite().getGlobalBounds().intersects(projSprite.getGlobalBounds())) {
                    asteroidManager.erase(asteroidIter);
                    projectileManager.erase(projectileIter);
                    iteratorDeleted = true;
                    break;
                }
            }
            if (!iteratorDeleted) {
                for (auto &enemy : enemyManager) {
                    if (enemy->getBoundingBox().getGlobalBounds().intersects((projSprite.getGlobalBounds()))) {
                        enemy->receiveDamage((*projectileIter)->getDamage());
                        projectileManager.erase(projectileIter);
                        break;
                    }
                }
            }
        }
    }
}

bool Game::isOutOfSigth(const sf::Sprite &sprite) const {
    return sprite.getPosition().y + sprite.getOrigin().y < 0 ||
           sprite.getPosition().y - sprite.getOrigin().y > windowHeight ||
           sprite.getPosition().x + sprite.getOrigin().x < 0 ||
           sprite.getPosition().x - sprite.getOrigin().x > windowWidth;
}

void Game::emplaceProj(std::unique_ptr<Projectile> projectile) {
    if (projectile != nullptr)
        projectileManager.emplace_back(new Projectile(*projectile));
}

void Game::checkForAsteroidsCollisions(std::list<std::unique_ptr<Asteroid>>::iterator asteroidIter) {
    sf::Sprite &asteroidSprite = (*asteroidIter)->getSprite(); //todo simplify
    if (isOutOfSigth(asteroidSprite))
        asteroidManager.erase(asteroidIter);
    else {
        bool iteratorDeleted = false;
        if (typeid(*player) == typeid(Raptor)) {
            if (isUsingSpecial && !dynamic_cast<Raptor &>(*player).isCharging()) {
                auto shield = dynamic_cast<Raptor &>(*player).getShield();
                if (dist(asteroidSprite.getPosition(), shield.getPosition()) <=
                    (shield.getRadius() + localRadiusPixels * (*asteroidIter)->getSize())) {
                    asteroidManager.erase(asteroidIter);
                    iteratorDeleted = true;
                }
            }
        }
        if (!iteratorDeleted &&
            player->getBoundingBox().getGlobalBounds().intersects((asteroidSprite.getGlobalBounds()))) {
            player->receiveDamage(static_cast<int>((*asteroidIter)->getDamage()));
            asteroidManager.erase(asteroidIter);
        }
    }

}

float Game::dist(const sf::Vector2f &pointA, const sf::Vector2f &pointB) {
    return sqrt(pow(pointB.x - pointA.x, 2) + pow(pointB.y - pointA.y, 2));
}
