#ifndef PARTICULE_H
#define PARTICULE_H

#include <SFML/Graphics.hpp>
#include "GravitySource.h"
#include <iostream>
#include <functional>
// #include <filesystem>

class Particle {
private:
    sf::Vector2f pos;
    sf::Vector2f velocity; // m.s-1
    sf::CircleShape shape;
    float mass; // kg
    float radius; // m
    sf::Rect<float> boundingBox;
    std::string name;
    sf::Text nameTextBox;

public:
    Particle(float posX, float posY, float velX, float velY, sf::Rect<float> boundingBox, std::string name);
    Particle(float posX, float posY, float velX, float velY, sf::Rect<float> boundingBox);
    Particle(float posX, float posY, float velX, float velY, std::string name);
    Particle(float posX, float posY, float velX, float velY);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPos();
    void updateAcceleration(GravitySource& src);
    void updatePhysics();
    float findBorderCollisionTime(sf::Vector2f prevPos, sf::Vector2f pos, float radius, bool isHorizontal, float coord);
    float bordersCollisionUpdate();
    float distanceTo(Particle p2);
    static float distance(float r1, float r2, sf::Vector2f pos1, sf::Vector2f pos2);
    float findCollisionTime(sf::Vector2f prevPos1, sf::Vector2f prevPos2, sf::Vector2f pos1, sf::Vector2f pos2, float r1, float r2);
    float collisionUpdate(Particle p2);
    void setColor(sf::Color col);
    float dotProduct(sf::Vector2f u1, sf::Vector2f u2);
    void turnBackTime(float time);
    std::string getName();
};

#endif