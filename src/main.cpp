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

    int r = 50, g = 50, b = 20;

    if (value < 0.25f) {
        r = 50 + 205 * (value*4);
    }
    else if (value < 0.25f) {
        r = 50 + 205 * (0.5f - value) * 4;
        b = 50 + 205 * ((value-0.25f) * 4);
    }
    else if (value < 0.5f) {
        b = 50 + 205 * (0.75f - value) * 4;
        g = 50 + 205 * ((value - 0.5f) * 4);
    }
    else {
        g = 50 + 205 * (1.0f - value) * 4;
    }

    return sf::Color(r, g, b);
}


int main()
{
    sf::Vector2i windowRes(1600, 800);
    sf::RenderWindow window(sf::VideoMode(windowRes.x, windowRes.y), "My Program");
    window.setFramerateLimit(60);

    std::vector<GravitySource> sources;

    // sources.push_back(GravitySource(500, 500, 3000));
    // sources.push_back(GravitySource(1200, 500, 3000));

    int nbParticles = 10;

    std::vector<Particle> particles;

    sf::Rect<float> windowBoundingBox(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(windowRes.x, windowRes.y));

    for (int i = 0; i < nbParticles; i++) {
        particles.push_back(Particle(30 + i*30 % (windowRes.x-60), 30 + 10*(i*30 / (windowRes.y-60)), -0.3, 0.2 + (0.1 / nbParticles) * i, windowBoundingBox));
        // particles.push_back(Particle(30 + i*30 % (windowRes.x-60), 30 + 10*(i*30 / (windowRes.y-60)), 2, 0.2 + (0.1 / nbParticles) * i));

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

            for (int j = 0; j < particles.size(); j++) {
                if(i!=j) {
                    particles[i].collisionUpdate(particles[j]);
                }
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