#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "GravitySource.h"
#include "Particle.h"

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
    sf::Vector2i windowRes(1600, 800);
    sf::RenderWindow window(sf::VideoMode(windowRes.x, windowRes.y), "My Program");
    window.setFramerateLimit(60);

    std::vector<GravitySource> sources;

    // sources.push_back(GravitySource(500, 500, 7000));
    // sources.push_back(GravitySource(1200, 500, 3000));

    int nbParticles = 10;

    std::vector<Particle> particles;

    sf::Rect<float> windowBoundingBox(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(windowRes.x, windowRes.y));

    for (int i = 0; i < nbParticles; i++) {
        particles.push_back(Particle(600+10*i, 700+10*i, 2, 0.2 + (0.1 / nbParticles) * i, windowBoundingBox));
        // particles.push_back(Particle(600+10*i, 700+10*i, 2, 0.2 + (0.1 / nbParticles) * i));

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
                particles[i].updateAcceleration(sources[j]);
            }
            
            particles[i].updatePhysics();
            particles[i].render(window);

            for (int j = 0; j < particles.size(); j++) {
                if(particles[i].isColliding(particles[j])) {
                    particles[i].collisionVelocityUpdate(particles[j]);
                }
            }
        }
        for (int i = 0; i < sources.size(); i++) {
            sources[i].render(window);
        }
        window.display();
    }
    return 0;
}