#ifndef PARTICULE_H
#define PARTICULE_H

#include <SFML/Graphics.hpp>
#include "GravitySource.h"
#include <iostream>

class Particle {
private:
    sf::Vector2f pos;
    sf::Vector2f velocity; // m.s-1
    sf::CircleShape shape;
    float mass; // kg
    float radius; // m
    sf::Rect<float> boundingBox;

public:
    Particle(float posX, float posY, float velX, float velY, sf::Rect<float> boundingBox);
    Particle(float posX, float posY, float velX, float velY);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPos();
    void updateAcceleration(GravitySource& src);
    void updatePhysics();
    bool areColliding(Particle p2);
    void collisionUpdate(Particle p2);
    void setColor(sf::Color col);
    float dotProduct(sf::Vector2f u1, sf::Vector2f u2);
};

#endif