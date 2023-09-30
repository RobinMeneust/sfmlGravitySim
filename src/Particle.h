#ifndef PARTICULE_H
#define PARTICULE_H

#include <SFML/Graphics.hpp>
#include "GravitySource.h"
#include <iostream>
#include <functional>

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
    float distanceTo(Particle p2);
    static float distance(float r1, float r2, sf::Vector2f pos1, sf::Vector2f pos2);
    void findCollisionPoint(sf::Vector2f *collisionPointP1, sf::Vector2f *collisionPointP2, sf::Vector2f prevPos1, sf::Vector2f prevPos2, sf::Vector2f pos1, sf::Vector2f pos2, float r1, float r2);
    void collisionUpdate(Particle p2);
    void setColor(sf::Color col);
    float dotProduct(sf::Vector2f u1, sf::Vector2f u2);
};

#endif