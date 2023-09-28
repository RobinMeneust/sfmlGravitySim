#ifndef GRAVITY_SOURCE_H
#define GRAVITY_SOURCE_H

#include <SFML/Graphics.hpp>


class GravitySource {
private:
    sf::Vector2f pos;
    float strength;
    sf::CircleShape shape;

public:
    GravitySource(float posX, float posY, float strength);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPos();
    float getStrength();
};

#endif