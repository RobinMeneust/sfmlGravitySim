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
    void findRoot2D(std::function<float(sf::Vector2f pos1, sf::Vector2f pos2)> f, sf::Vector2f root[2], sf::Vector2f input1, sf::Vector2f input2, sf::Vector2f stepVect1, sf::Vector2f stepVect2);
    void collisionUpdate(Particle p2);
    void setColor(sf::Color col);
    float dotProduct(sf::Vector2f u1, sf::Vector2f u2);
};

#endif