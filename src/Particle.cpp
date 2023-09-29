#include "Particle.h"

Particle::Particle(float posX, float posY, float velX, float velY, sf::Rect<float> boundingBox) {
    pos.x = posX;
    pos.y = posY;
    velocity.x = velX;
    velocity.y = velY;
    radius = 10;
    mass = 1;

    this->boundingBox = boundingBox;

    shape.setPosition(pos);
    shape.setOrigin(radius,radius);
    shape.setFillColor(sf::Color::White);
    shape.setRadius(radius);
}

Particle::Particle(float posX, float posY, float velX, float velY) : Particle::Particle(posX, posY, velX, velY, sf::FloatRect(0,0,0,0)) {}

void Particle::render(sf::RenderWindow& window) {
    shape.setPosition(pos);
    window.draw(shape);
}

sf::Vector2f Particle::getPos() {
    return sf::Vector2f(pos);
}

void Particle::updateAcceleration(GravitySource& src) {
    // std::cout << "SPEED : " << velocity.x << " " << velocity.y << std::endl;
    sf::Vector2f distanceVect = src.getPos() - this->pos;
    float squareDistance = distanceVect.x * distanceVect.x + distanceVect.y * distanceVect.y;
    float distance = sqrt(squareDistance);
    
    //normalize acceleration vector & deduce acceleration vector
    sf::Vector2f normalizedDistance = distanceVect / distance;

    float forceDivMass = src.getStrength() / squareDistance;
    sf::Vector2f accelerationVect;

    accelerationVect.x = normalizedDistance.x * forceDivMass;
    accelerationVect.y = normalizedDistance.y * forceDivMass;

    velocity += accelerationVect;
}

void Particle::updatePhysics() {
    sf::Vector2f tmpPos = pos + velocity;
    if(boundingBox.width != 0 && boundingBox.height !=0) {
        if (tmpPos.x < boundingBox.left || tmpPos.x + radius > boundingBox.left + boundingBox.width) {
            velocity.x = -velocity.x;
        }
        if (tmpPos.y < boundingBox.top || tmpPos.y + radius > boundingBox.top + boundingBox.height) {
            velocity.y = -velocity.y;
        }
    }

    pos += velocity;
}

bool Particle::isColliding(Particle p2) {
    float dx = pos.x-p2.pos.x;
    float dy = pos.y-p2.pos.y;
    float dist = std::sqrt(dx * dx + dy * dy) - radius - p2.radius;
    return dist >=-0.1 && dist <= 0.0;
}

void Particle::collisionVelocityUpdate(Particle p2) {
    float invMassSum = 1/(mass + p2.mass);
    float dOrientation = dotProduct(velocity - p2.velocity, pos - p2.pos) / ((pos.x-p2.pos.x)*(pos.x-p2.pos.x) + (pos.y-p2.pos.y)*(pos.y-p2.pos.y));

    // <-u1,-u2> = <u1,u2>
    velocity -= 2*p2.mass * invMassSum * dOrientation * (pos - p2.pos);
    p2.velocity -= 2*mass * invMassSum * dOrientation * (p2.pos - pos);
}

void Particle::setColor(sf::Color col) {
    shape.setFillColor(col);
}

float Particle::dotProduct(sf::Vector2f u1, sf::Vector2f u2) {
    return u1.x * u2.x + u1.y + u2.y;
}
