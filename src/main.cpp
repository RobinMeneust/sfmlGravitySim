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
    sf::Vector2i windowRes(200, 200);
    // sf::Vector2i windowRes(1600, 800);
    sf::RenderWindow window(sf::VideoMode(windowRes.x, windowRes.y), "My Program");
    window.setFramerateLimit(60);

    std::vector<GravitySource> sources;

    // sources.push_back(GravitySource(500, 500, 3000));
    // sources.push_back(GravitySource(1200, 500, 7000));

    int nbParticles = 20;

    std::vector<Particle> particles;

    sf::Rect<float> windowBoundingBox(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(windowRes.x, windowRes.y));

    int x = 0;
    int y = 30;

    for (int i = 0; i < nbParticles; i++) {
        x += 30;
        if(x+15>windowRes.x) {
            x = 30;
            y += 30;
        }

        particles.push_back(Particle(x, y, -0.3, 0.2 + (0.1 / nbParticles) * i, windowBoundingBox, std::to_string(i)));
        // particles.push_back(Particle(30 + i*30 % (windowRes.x-60), 30 + 10*(i*30 / (windowRes.y-60)), 2, 0.2 + (0.1 / nbParticles) * i, std::to_string(i)));

        float val = (float)i / (float)nbParticles;
        sf::Color col = mapValToColor(val);

        particles[i].setColor(col);
    }

    float remainingFrameTime = 1.0f;


    while (window.isOpen()) {
        std::cout << "________________step________________" <<std::endl;
        sf::Event event;
        
        sf::Vector2f collisionParticles;
        float firstCollisionTime = INFINITY;
        Particle* particlesColliding[2] = {NULL,NULL};
        bool isBorderCollision = false;
        

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
        std::cout << "UPDATED ACCELERATION VECTORS" << std::endl;
        // Check if there are collisions between a particle and a border
        for (int i = 0; i < particles.size(); i++) {
            float collisionTime = particles[i].getNextBorderCollisionTime();
            if(collisionTime != -1.0f && collisionTime < firstCollisionTime) {
                // There is a collision
                isBorderCollision = true;
                firstCollisionTime = collisionTime;
                particlesColliding[0] = &(particles[i]);
            }
        }

        std::cout << "CHECKED BORDERS COLLISION" << std::endl;

        // Check if there are collisions between two particles
        for (int i = 0; i < particles.size(); i++) {
            for (int j = 0; j < particles.size(); j++) {
                if(i!=j) {
                    float collisionTime = particles[i].getNextParticleCollisionTime(&(particles[j]));
                    if(collisionTime != -1.0f && collisionTime < firstCollisionTime) {
                        // There is a collision
                        isBorderCollision = false;
                        firstCollisionTime = collisionTime;
                        particlesColliding[0] = &(particles[i]);
                        particlesColliding[1] = &(particles[j]);
                    }
                }
            }
        }

        std::cout << "CHECKED PARTICLES COLLISION" << std::endl;
        
        if(firstCollisionTime >= 0.0f && firstCollisionTime <= 1.0f) {
            if(firstCollisionTime != 0.0f) {
                for(int i=0; i<particles.size(); i++) {
                    // Go to the collision time
                    particles[i].forwardTime(std::min(firstCollisionTime,remainingFrameTime));
                }
            }

            if(isBorderCollision) {
                particlesColliding[0]->collideBorder();
            } else {
                particlesColliding[0]->collideParticle(particlesColliding[1]);
            }

            remainingFrameTime = std::max(remainingFrameTime - firstCollisionTime,0.0f);
        } else {
            for(int i=0; i<particles.size(); i++) {
                particles[i].forwardTime(remainingFrameTime);
            }
            remainingFrameTime = 0.0f;
        }

        if(remainingFrameTime <= 0) {
            remainingFrameTime = 1.0f;
            // Render the particles
            for (int i = 0; i < particles.size(); i++) {
                particles[i].render(window);
            }

            // Render the gravity sources
            for (int i = 0; i < sources.size(); i++) {
                sources[i].render(window);
            }
            window.display();
            std::cout << "________________RENDER________________" <<std::endl;
            std::cout << "______________________________________" <<std::endl;
        }
    }
    return 0;
}