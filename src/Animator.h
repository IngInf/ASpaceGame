//
// Created by Fabio Luccioletti on 2019-07-03.
//

#ifndef ASPACEGAME_ANIMATOR_H
#define ASPACEGAME_ANIMATOR_H

#include <string>
#include <SFML/Graphics.hpp>
#include <list>


class Animator {

public:
    struct Animation {
        std::string animName;
        std::string animTextureName;
        std::vector<sf::IntRect> animFrames;
        sf::Time animDuration;
        bool animLooping;

        Animation(std::string name, std::string textureName, sf::Time const &duration, bool looping) :
                animName(std::move(name)), animTextureName(std::move(textureName)), animDuration(duration),
                animLooping(looping) {}

        void addFrames(sf::Vector2i const &startFrom, sf::Vector2i const &frameSize, unsigned int frames,
                       unsigned int rows = 1) {
            sf::Vector2i current = startFrom;
            for (unsigned int j = 0; j < rows; j++) {
                for (unsigned int i = 0; i < frames; i++) {
                    animFrames.emplace_back(current.x, current.y, frameSize.x, frameSize.y);
                    current.x += frameSize.x;
                }
                current.x = startFrom.x;
                current.y += frameSize.y;
            }
        }
    };

    explicit Animator(sf::Sprite &sprite);

    virtual ~Animator();

    Animator::Animation &createAnimation(std::string const &name, std::string const &textureName,
                                         sf::Time duration, bool loop = false);

    void update(float dt, float delay = 0);

private:
    void switchAnimation(Animator::Animation *animation);

    sf::Sprite &sprite;
    float currentTime;
    std::list<Animator::Animation> animations;
    Animator::Animation *currentAnimation;

};


#endif //ASPACEGAME_ANIMATOR_H
