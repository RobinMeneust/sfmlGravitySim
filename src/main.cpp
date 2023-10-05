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
        particles.push_back(Particle(30 + i*30 % (windowRes.x-60), 30 + 10*(i*30 / (windowRes.y-60)), -0.3, 0.2 + (0.1 / nbParticles) * i, windowBoundingBox, std::to_string(i)));
        // particles.push_back(Particle(30 + i*30 % (windowRes.x-60), 30 + 10*(i*30 / (windowRes.y-60)), 2, 0.2 + (0.1 / nbParticles) * i), std::to_string(i));

        float val = (float)i / (float)nbParticles;
        sf::Color col = mapValToColor(val);

        particles[i].setColor(col);
    }


    while (window.isOpen()) {
        std::cout << "________________step________________" <<std::endl;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        window.clear();
        
        // Update acceleration vectors
        for (int i = 0; i < particles.size(); i++) {
            for (int j = 0; j < sources.size(); j++) {
                particles[i].updateAcceleration(sources[j]);
            }
        }

        // Check if there are collisions a particle and a border
        for (int i = 0; i < particles.size(); i++) {
            float collisionTime = particles[i].bordersCollisionUpdate();
            if(collisionTime != -1.0f) {
                // There is a collision
                for (int k = 0; k < particles.size(); k++) {
                    if(k != i) {
                        particles[i].turnBackTime(collisionTime);
                    }
                }
            }
        }

        // Check if there are collisions between two particles
        for (int i = 0; i < particles.size(); i++) {
            for (int j = 0; j < particles.size(); j++) {
                if(i!=j) {
                    float collisionTime = particles[i].collisionUpdate(particles[j]);
                    if(collisionTime != -1.0f) {
                        // There is a collision
                        for (int k = 0; k < particles.size(); k++) {
                            if(k != i && k != j) {
                                particles[i].turnBackTime(collisionTime);
                            }
                        }
                    }
                }
            }
        }

        // Update positions and render the particles
        for (int i = 0; i < particles.size(); i++) {
            particles[i].updatePhysics();
            particles[i].render(window);
        }

        // Render the gravity sources
        for (int i = 0; i < sources.size(); i++) {
            sources[i].render(window);
        }

        window.display();
    }
    return 0;
}