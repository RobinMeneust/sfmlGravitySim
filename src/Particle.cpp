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
    if(boundingBox.width != 0 && boundingBox.height !=0) {
        sf::Vector2f tmpPos = pos + velocity;
        if (tmpPos.x - radius < boundingBox.left || tmpPos.x + radius > boundingBox.left + boundingBox.width) {
            velocity.x = -velocity.x;
        }
        if (tmpPos.y - radius < boundingBox.top || tmpPos.y + radius > boundingBox.top + boundingBox.height) {
            velocity.y = -velocity.y;
        }
    }
    pos += velocity;
}

float Particle::distanceTo(Particle p2) {
    return distance(this->radius, p2.radius, this->pos, p2.pos);
}

float Particle::distance(float r1, float r2, sf::Vector2f pos1, sf::Vector2f pos2) {
    // std::cout << "pos1: " << pos1.x << " " << pos1.y << " pos2: " << pos2.x << " " << pos2.y << std::endl;
    float dx = pos1.x-pos2.x;
    float dy = pos1.y-pos2.y;
    // std::cout << "dx: " << dx << " dy: " << dy << std::endl;
    float dist = std::sqrt(dx * dx + dy * dy) - r1 - r2;
    // std::cout << "d: " << dist << std::endl;
    return dist;
}

void Particle::findCollisionPoint(sf::Vector2f *collisionPointP1, sf::Vector2f *collisionPointP2, sf::Vector2f prevPos1, sf::Vector2f prevPos2, sf::Vector2f pos1, sf::Vector2f pos2, float r1, float r2) {
    float cx1 = prevPos1.x - prevPos2.x;
    float cy1 = prevPos1.x - prevPos2.x;
    
    float cx2 = pos1.x - prevPos1.x - (pos2.x - prevPos2.x);
    float cy2 = pos1.y - prevPos1.y - (pos2.y - prevPos2.y);

    float a = cx2*cx2 + cy2*cy2;
    float b = 2*(cx1*cx2 + cy1*cy2);
    float c = cy1*cy1 + cx1*cx1 - (r1+r2)*(r1+r2);

    float delta = b*b - 4*a*c;
    float t1 = -1;
    float t2 = -1;
    float t = -1;

    if(delta >= 0) {
        t1 = (-b-sqrt(delta))/(2*a);
        t2 = (-b+sqrt(delta))/(2*a);
    }

    std::cout << "PREV: p1: " << prevPos1.x << " " << prevPos1.y << " p2: " << prevPos2.x << " " << prevPos2.y << std::endl;
    std::cout << "NEXT: p1: " << pos1.x << " " << pos1.y << " p2: " << pos2.x << " " << pos2.y << std::endl;
    std::cout << "a: " << a << " b: " << b << " c: " << c << std::endl;
    std::cout << "b^2: " << b*b << " 4ac: " << 4*a*c << std::endl;
    std::cout << "delta" << delta << std::endl;

    if(t1 >=0 && t1 <=1) {
        if(t1 < t2 || t2<0 || t2>1) {
            t = t1;
        } else {
            t = t2;
        }
    } else if(t2 >=0 && t2 <=1) {
        t = t2;
    } else {
        std::cerr << "Root could not be found" << std::endl;
        exit(EXIT_FAILURE);
    }

    *collisionPointP1 = prevPos1 + t*(pos1 - prevPos1);
    *collisionPointP2 = prevPos2 + t*(pos2 - prevPos2);
}

void Particle::collisionUpdate(Particle p2) {
    float dist = distanceTo(p2);
    if(dist <= 0.0) {
        if(dist < 0.1) {
            // It's overlapping so we fix the positions
            sf::Vector2f collisionPointP1;
            sf::Vector2f collisionPointP2;
        
            findCollisionPoint(&collisionPointP1, &collisionPointP2, pos, p2.pos, pos + velocity, p2.pos + p2.velocity, radius, p2.radius);
            pos = collisionPointP1;
            p2.pos = collisionPointP2;
        }
        float invMassSum = 1/(mass + p2.mass);
        float dOrientation = dotProduct(velocity - p2.velocity, pos - p2.pos) / ((pos.x-p2.pos.x)*(pos.x-p2.pos.x) + (pos.y-p2.pos.y)*(pos.y-p2.pos.y));

        // <-u1,-u2> = <u1,u2>
        velocity -= 2*p2.mass * invMassSum * dOrientation * (pos - p2.pos);
        p2.velocity -= 2*mass * invMassSum * dOrientation * (p2.pos - pos);
    }
}

void Particle::setColor(sf::Color col) {
    shape.setFillColor(col);
}

float Particle::dotProduct(sf::Vector2f u1, sf::Vector2f u2) {
    return u1.x * u2.x + u1.y + u2.y;
}
