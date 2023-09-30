#include "Particle.h"
#include <windows.h>

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

void Particle::findRoot2D(std::function<float(sf::Vector2f pos1, sf::Vector2f pos2)> f, sf::Vector2f root[2], sf::Vector2f input1, sf::Vector2f input2, sf::Vector2f stepVect1, sf::Vector2f stepVect2) {
    int iter = 0;
	float step = 0.1;
	sf::Vector2f bestApprox[2] = {input1, input2};
    float eps;
	float epsMin = INFINITY;
    float prevEps = fabs(f(input1,input2));
    float prevF;

    // std::cout << "INPUTS: (1:) " << input1.x << " " << input1.y << " (2:) " << input2.x << " " << input2.y << std::endl;
    // std::cout << "STEPS: (1:) " << stepVect1.x << " " << stepVect1.y << " (2:) " << stepVect2.x << " " << stepVect2.y << std::endl;

    sf::Vector2f x1 = input1;
    sf::Vector2f x2 = input2;

    /*
    g'(u) = lim_{h->0}((g(u+h) - g(u))/h)
    Here:
    h = step
    g(u+h) = eps
    g(u) = prevEps

    We will use Euler's method
    */

    do{
        prevF = fabs(f(x1,x2));
        x1 += stepVect1*step*prevF;
        x2 += stepVect2*step*prevF;
        // std::cout << "X: (1:) " << x1.x << " " << x1.y << " (2:) " << x2.x << " " << x2.y << std::endl;

        eps = fabs(f(x1,x2));
        if(eps < epsMin){
            epsMin = eps;
            bestApprox[0] = x1;
            bestApprox[0] = x2;
        }

        iter++;
        std::cout << "eps: " << eps << std::endl;
    }while(eps>0.1 && iter<100);
    root[0] = bestApprox[0];
    root[1] = bestApprox[1];
    // std::cout << "ROOTS: (x:) " << root[0].x << " " << root[0].y << " (y:) " << root[1].x << " " << root[1].y << std::endl;
    // std::cout << "___" << std::endl << "____" << std::endl;
}

void Particle::collisionUpdate(Particle p2) {
    float dist = distanceTo(p2);
    if(dist <= 0.0) {
        if(dist < 0.1) {
            // It's overlapping so we fix the positions
            sf::Vector2f root[2];
            
            auto f = std::bind(distance, this->radius, p2.radius, std::placeholders::_1, std::placeholders::_2);
            findRoot2D(f, root,pos,p2.pos,-velocity,-p2.velocity);
            pos = root[0];
            p2.pos = root[1];
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
