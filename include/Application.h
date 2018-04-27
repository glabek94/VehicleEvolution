//
// Created by dawid on 27.04.18.
//

#ifndef VEHICLEEVOLUTION_APPLICATION_H
#define VEHICLEEVOLUTION_APPLICATION_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Application {
private:
    const float SCALE = 30.f;
    sf::RenderWindow window;
    b2Vec2 gravity;
    b2World world;
public:
    Application() :window{sf::VideoMode(800, 600, 32), "SFML window"},
                   gravity{0.f, 9.8f},
                   world(gravity) {
        window.setFramerateLimit(60);
    };


    void run();
    void CreateGround(b2World& World, float X, float Y);
    void CreateBox(b2World& World, int MouseX, int MouseY);
};


#endif //VEHICLEEVOLUTION_APPLICATION_H
