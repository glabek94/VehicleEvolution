//
// Created by dawid on 27.04.18.
//

#ifndef VEHICLEEVOLUTION_VEHICLE_H
#define VEHICLEEVOLUTION_VEHICLE_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <utility>
#include <memory>
#include <iostream>

class Vehicle
{
private:

    /*
     * SCALE should be deleted from this class and moved to another, "global" header - it has to be
     * the same for every class in the project.
     */
    float SCALE;

    std::vector<std::pair<float, float>> vertices;

    /*
     * Naked pointer since b2Bodies are allocated by b2World.
     */
    b2Body* body;

    /*
     * Maybe in future it can be changed to the shared_ptr - performance tests needed.
     */
    b2World* world;

    /*
     * In this case, it actually represents concave shape, but since sf::ConvexShape is implemented
     * as triangle fan[1], we can use it to represent vehicle's body since it's also a triangle fan.
     * [1] https://www.sfml-dev.org/tutorials/2.4/graphics-shape.php
     */
    sf::ConvexShape shape;

public:
    /*
     * vertices_ is vector of vertices representing the body in the Box2D-scale (meters).
     */
    Vehicle( std::vector<std::pair<float, float>> &vertices_, float SCALE_, float x_, float y_, b2World *world_);

    sf::ConvexShape getShape() const;
    void updateShape();
    b2Body* getBody() const;


};


#endif //VEHICLEEVOLUTION_VEHICLE_H
