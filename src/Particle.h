#ifndef PARTICULE_H
#define PARTICULE_H

#include <SFML/Graphics.hpp>
#include "GravitySource.h"
#include <iostream>

class Particle {
private:
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::CircleShape shape;
    float mass;
    float radius;
    sf::Rect<float> boundingBox;

public:
    Particle(float posX, float posY, float velX, float velY, sf::Rect<float> boundingBox);
    Particle(float posX, float posY, float velX, float velY);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPos();
    void updateAcceleration(GravitySource& src);
    void updatePhysics();
    bool isColliding(Particle p2);
    void collisionVelocityUpdate(Particle p2);
    void setColor(sf::Color col);
};

#endif