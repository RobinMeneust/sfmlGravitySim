#include "Particle.h"
#define FONTDIR "C:\\Windows\\Fonts\\"

Particle::Particle(float posX, float posY, float velX, float velY, sf::Rect<float> boundingBox, std::string name) {
    pos.x = posX;
    pos.y = posY;
    velocity.x = velX;
    velocity.y = velY;
    radius = 10;
    mass = 1;

    this->boundingBox = boundingBox;

    this->name = name;

    //TO DO: use a relative path

    std::cout << (font.loadFromFile(FONTDIR "Calibri.ttf") ? "success" : "failure") << std::endl;
    nameTextBox.setFont(font);
    nameTextBox.setString(name);
    // nameTextBox.setCharacterSize(10);
    // nameTextBox.setFillColor(sf::Color::Black);
    // nameTextBox.setOutlineColor(sf::Color::White);
    nameTextBox.setPosition(pos);
    
    shape.setPosition(pos);
    shape.setOrigin(radius,radius);
    shape.setFillColor(sf::Color::White);
    shape.setRadius(radius);
}

Particle::Particle(float posX, float posY, float velX, float velY, sf::Rect<float> boundingBox) : Particle::Particle(posX, posY, velX, velY, boundingBox, "") {}

Particle::Particle(float posX, float posY, float velX, float velY, std::string name) : Particle::Particle(posX, posY, velX, velY, sf::FloatRect(0,0,0,0), name) {}

Particle::Particle(float posX, float posY, float velX, float velY) : Particle::Particle(posX, posY, velX, velY, "") {}

void Particle::render(sf::RenderWindow& window) {
    shape.setPosition(pos);
    nameTextBox.setPosition(pos);
    nameTextBox.setFont(font);
    window.draw(shape);
    window.draw(nameTextBox);
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
    pos += velocity;
}

void Particle::updatePhysics(float framePercentage) {
    pos += velocity * framePercentage;
}

float Particle::findBorderCollisionTime(sf::Vector2f prevPos, sf::Vector2f pos, float radius, bool isHorizontal, float coord) {
    std::cout << "root border search for nb " << name << std::endl;
    float c1;
    float c2;
    if(isHorizontal) {
        c1 = pos.y - prevPos.y;
        c2 = prevPos.y - coord;
    } else {
        c1 = pos.x - prevPos.x;
        c2 = prevPos.x - coord;
    }

    float a = c1*c1;
    float b = 2*c1*c2;
    float c = c2*c2 - radius*radius;

    float delta = b*b - 4*a*c;
    float t1 = -1;
    float t2 = -1;
    float t = -1;


    //TODO: put the following code in a function to avoid repetition
    if(a == 0) {
        // if b == 0 and c is too large then there is no collision
        if(b == 0 && c<0.001f) {
            t1 = 0;
        } else {
            t1 = -c/b;
        }
    } else if(delta >= 0) {
        t1 = (-b-sqrt(delta))/(2*a);
        t2 = (-b+sqrt(delta))/(2*a);
    }

    std::cout << "PREV: p: " << prevPos.x << " " << prevPos.y << std::endl;
    std::cout << "NEXT: p: " << pos.x << " " << pos.y << std::endl;
    std::cout << "isBorderHorizontal: " << (isHorizontal ? "true" : "false") << std::endl;
    std::cout << "a: " << a << " b: " << b << " c: " << c << std::endl;
    std::cout << "b^2: " << b*b << " 4ac: " << 4*a*c << std::endl;
    std::cout << "delta: " << delta << std::endl;
    std::cout << "t1: " << t1 << " t2: " << t2 << std::endl;

    if(t1 >=0 && t1 <=1) {
        if(t1 < t2 || t2<0 || t2>1) {
            t = t1;
        } else {
            t = t2;
        }
    } else if(t2 >=0 && t2 <=1) {
        t = t2;
    } else {
        std::cerr << "(border collision) Root could not be found for nb " << name << std::endl;
        exit(EXIT_FAILURE);
    }

    return t;
}

void Particle::collideBorder() {
    if (pos.x - radius <= boundingBox.left || pos.x + radius >= boundingBox.left + boundingBox.width) {
        velocity.x = -velocity.x;
    }
    if (pos.y - radius <= boundingBox.top || pos.y + radius >= boundingBox.top + boundingBox.height) {
        velocity.y = -velocity.y;
    }
}

float Particle::getNextBorderCollisionTime() {
    if(name == "7") {
        std::cout << "pos7 = " << pos.x << " " << pos.y << std::endl;
    }
    if(boundingBox.width != 0 && boundingBox.height !=0) {
        // If there is a bounding box
        sf::Vector2f nextStepPos = pos + velocity;

        if (nextStepPos.x - radius <= boundingBox.left) {
            return findBorderCollisionTime(pos,nextStepPos, radius,false, boundingBox.left);
        }        
        if(nextStepPos.x + radius >= boundingBox.left + boundingBox.width) {
            return findBorderCollisionTime(pos,nextStepPos, radius,false, boundingBox.left + boundingBox.width);
        }
        if (nextStepPos.y - radius <= boundingBox.top) {
            return findBorderCollisionTime(pos,nextStepPos, radius,true, boundingBox.top);
        }
        if (nextStepPos.y + radius >= boundingBox.top + boundingBox.height) {
            return findBorderCollisionTime(pos,nextStepPos, radius,true, boundingBox.top + boundingBox.height);
        }
    }
    return -1.0f;
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

float Particle::findCollisionTime(sf::Vector2f prevPos1, sf::Vector2f prevPos2, sf::Vector2f pos1, sf::Vector2f pos2, float r1, float r2) {
    float cx1 = prevPos1.x - prevPos2.x;
    float cy1 = prevPos1.y - prevPos2.y;
    
    float cx2 = pos1.x - prevPos1.x - (pos2.x - prevPos2.x);
    float cy2 = pos1.y - prevPos1.y - (pos2.y - prevPos2.y);

    float a = cx2*cx2 + cy2*cy2;
    float b = 2*(cx1*cx2 + cy1*cy2);
    float c = cy1*cy1 + cx1*cx1 - (r1+r2)*(r1+r2);

    float delta = b*b - 4*a*c;
    float t1 = -1;
    float t2 = -1;
    float t = -1;

    if(a == 0) {
        // if b == 0 and c is too large then there is no collision
        if(b == 0 && c<0.001f) {
            t1 = 0;
        } else {
            t1 = -c/b;
        }
    } else if(delta >= 0) {
        t1 = (-b-sqrt(delta))/(2*a);
        t2 = (-b+sqrt(delta))/(2*a);
    }

    std::cout << "PREV: p1: " << prevPos1.x << " " << prevPos1.y << " p2: " << prevPos2.x << " " << prevPos2.y << std::endl;
    std::cout << "NEXT: p1: " << pos1.x << " " << pos1.y << " p2: " << pos2.x << " " << pos2.y << std::endl;
    std::cout << "a: " << a << " b: " << b << " c: " << c << std::endl;
    std::cout << "b^2: " << b*b << " 4ac: " << 4*a*c << std::endl;
    std::cout << "delta: " << delta << std::endl;
    std::cout << "t1: " << t1 << " t2: " << t2 << std::endl;

    if(t1 >=0 && t1 <=1) {
        if(t1 < t2 || t2<0 || t2>1) {
            t = t1;
        } else {
            t = t2;
        }
    } else if(t2 >=0 && t2 <=1) {
        t = t2;
    } else {
        std::cerr << "(2 particles collision) Root could not be found" << std::endl;
        exit(EXIT_FAILURE);
    }

    return t;
}

void Particle::collideParticle(Particle* p2) {
    std::cout << "collide particle" << std::endl;

    sf::Vector2f nextPosP1 = pos + velocity;
    sf::Vector2f nextPosP2 = p2->pos + p2->velocity;
    float invMassSum = 1/(mass + p2->mass);
    float dOrientation = dotProduct(velocity - p2->velocity, pos - p2->pos) / ((pos.x-p2->pos.x)*(pos.x-p2->pos.x) + (pos.y-p2->pos.y)*(pos.y-p2->pos.y));
    float dist = distance(radius, p2->radius, nextPosP1, nextPosP2);

    if(dist <= 0.001) {
        // <-u1,-u2> = <u1,u2>
        std::cout << "PREV VELOCITY: p1: (" << name << ") " << velocity.x << " " << velocity.y << " p2: (" << p2->name << ") " << p2->velocity.x << " " << p2->velocity.y << std::endl;
        velocity -= 2*p2->mass * invMassSum * dOrientation * (pos - p2->pos);
        p2->velocity -= 2*mass * invMassSum * dOrientation * (p2->pos - pos);
        std::cout << "PREV VELOCITY: p1: (" << name << ") " << velocity.x << " " << velocity.y << " p2: (" << p2->name << ") " << p2->velocity.x << " " << p2->velocity.y << std::endl;
    }
}

float Particle::getNextParticleCollisionTime(Particle* p2) {
    sf::Vector2f nextPosP1 = pos + velocity;
    sf::Vector2f nextPosP2 = p2->pos + p2->velocity;
    float dist = distance(radius, p2->radius, nextPosP1, nextPosP2);
    float collisionTime = -1.0f;

    if(dist <= 0.001) {
        // collision (and eventually overlap)    
        std::cout << "COLLISION BETWEEN " << name << " & " << p2->name << std::endl;
        return findCollisionTime(pos, p2->pos, nextPosP1, nextPosP2, radius, p2->radius);
    }

    return collisionTime;
}

/* TODO: We should :
- Put in a temp var the next step position
- Check if there is a collision

1. If there is a collision :
- Go to the collision point (function below) = UPDATE POSITION
- Update velocity (for the next step of the main loop)

2. If there is no collision :
- Update position (pos2 = pos next step)

*/
void Particle::forwardTime(float time) {
    std::cout << "next step "<< time << std::endl;
    sf::Vector2f nextStepPos = pos + velocity;
    pos = pos + time*(nextStepPos - pos);
}

void Particle::setColor(sf::Color col) {
    shape.setFillColor(col);
}

float Particle::dotProduct(sf::Vector2f u1, sf::Vector2f u2) {
    return u1.x * u2.x + u1.y + u2.y;
}

std::string Particle::getName() {
    return name;
}