#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class GravitySource {
private:
    sf::Vector2f pos;
    float strength;
    sf::CircleShape shape;

public:
    GravitySource(float posX, float posY, float strength) {
        pos.x = posX;
        pos.y = posY;
        this->strength = strength;

        shape.setPosition(pos);
        shape.setFillColor(sf::Color::White);
        shape.setRadius(15);
    }

    void render(sf::RenderWindow& window) {
        window.draw(shape);
    }


    sf::Vector2f getPos() {
        return pos;
    }

    float getStrength() {
        return strength;
    }
};

class Particle {
private:
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::CircleShape shape;
    //sf::Rect<float> boundingBox;

public:
    Particle(float posX, float posY, float velX, float velY/*, sf::Rect<float> boundingBox*/) {
        pos.x = posX;
        pos.y = posY;
        velocity.x = velX;
        velocity.y = velY;

        /*this->boundingBox = boundingBox;*/

        shape.setPosition(pos);
        shape.setFillColor(sf::Color::White);
        shape.setRadius(3);
    }

    void render(sf::RenderWindow& window) {
        shape.setPosition(pos);
        window.draw(shape);
    }

    void updatePhysics(GravitySource& src) {
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

       /* sf::Vector2f tmpPos = pos + velocity;

        if (tmpPos.x < boundingBox.left || tmpPos.x > boundingBox.left + boundingBox.width) {
            velocity.x = -velocity.x;
        }
        if (tmpPos.y < boundingBox.top || tmpPos.y > boundingBox.top + boundingBox.height) {
            velocity.y = -velocity.y;
        }
        */
        pos += velocity;
    }

    void setColor(sf::Color col) {
        shape.setFillColor(col);
    }
};


sf::Color mapValToColor(float value) {
    if (value < 0.0f) {
        value = 0;
    }
    else if (value > 1.0f) {
        value = 1;
    }

    int r = 0, g = 0, b = 0;

    if (value < 0.25f) {
        r = 255 * (value*4);
    }
    else if (value < 0.25f) {
        r = 255 * (0.5f - value) * 4;
        b = 255 * ((value-0.25f) * 4);
    }
    else if (value < 0.5f) {
        b = 255 * (0.75f - value) * 4;
        g = 255 * ((value - 0.5f) * 4);
    }
    else {
        g = 255 * (1.0f - value) * 4;
    }

    return sf::Color(r, g, b);
}


int main()
{
    sf::Vector2i windowRes(1600, 1000);
    sf::RenderWindow window(sf::VideoMode(windowRes.x, windowRes.y), "My Program");
    window.setFramerateLimit(60);

    std::vector<GravitySource> sources;

    sources.push_back(GravitySource(500, 500, 7000));
    sources.push_back(GravitySource(1200, 500, 7000));

    int nbParticles = 1000;

    std::vector<Particle> particles;

    //sf::Rect<float> windowBoundingBox(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(windowRes.x, windowRes.y));

    for (int i = 0; i < nbParticles; i++) {
        particles.push_back(Particle(600, 700, 4, 0.2 + (0.1 / nbParticles) * i/*, windowBoundingBox*/));

        float val = (float)i / (float)nbParticles;
       sf::Color col = mapValToColor(val);

       particles[i].setColor(col);
    }


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        window.clear();
        for (int i = 0; i < particles.size(); i++) {
            for (int j = 0; j < sources.size(); j++) {
                particles[i].updatePhysics(sources[j]);
            }
            particles[i].render(window);
        }
        for (int i = 0; i < sources.size(); i++) {
            sources[i].render(window);
        }
        window.display();
    }
    return 0;
}