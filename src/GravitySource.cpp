#include "GravitySource.h"

GravitySource::GravitySource(float posX, float posY, float strength) {
    pos.x = posX;
    pos.y = posY;
    this->strength = strength;

    shape.setPosition(pos);
    shape.setFillColor(sf::Color::White);
    shape.setRadius(15);
}

void GravitySource::render(sf::RenderWindow& window) {
    window.draw(shape);
}


sf::Vector2f GravitySource::getPos() {
    return pos;
}

float GravitySource::getStrength() {
    return strength;
}