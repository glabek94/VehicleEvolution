//
// Created by dawid on 27.04.18.
//

#ifndef VEHICLEEVOLUTION_APPLICATION_H
#define VEHICLEEVOLUTION_APPLICATION_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Constants.h"

class Application {
public:
    //const float SCALE = 30.f;
    void run();
    void CreateGround(b2World& World, float X, float Y);
    void CreateBox(b2World& World, int MouseX, int MouseY);
    std::vector<b2Body*> boxes;

};


#endif //VEHICLEEVOLUTION_APPLICATION_H
