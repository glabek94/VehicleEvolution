//
// Created by dawid on 27.04.18.
//

#ifndef VEHICLEEVOLUTION_VEHICLE_H
#define VEHICLEEVOLUTION_VEHICLE_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <EvoAlgo/Chromosome.h>
#include "Constants.h"

class Vehicle
{
private:

    std::vector<std::pair<float, float>> vertices;
    Chromosome chromo;

    //Naked pointer since b2Bodies are allocated by b2World.
    b2Body* chassisBody;

    //Naked pointer since b2Bodies are allocated by b2World.
    std::vector<b2Body*> wheelBodies;

    //Naked pointer since b2Joints are allocated by b2World.
    std::vector<b2WheelJoint*> springs;


    /*
     * In this case, it actually represents concave shape, but since sf::ConvexShape is implemented
     * as triangle fan[1], we can use it to represent vehicle's body since it's also a triangle fan.
     * [1] https://www.sfml-dev.org/tutorials/2.4/graphics-shape.php
     */
    sf::ConvexShape chassisShape;

    //temporary code duplication, left/right will be changed to pair or vector;
    std::vector<sf::CircleShape> wheelShapes;


    //used in isMoving()
    float timeStopped;

    void createShapes(const std::vector<float>& wheelSizes, float x_, float y_);

    void createBody(const std::vector<float>& wheelSizes, const std::vector<int>& wheelVertices, float x_, float y_);

public:

    Vehicle(const Chromosome& chromosome, float x_ = 250, float y_ = -300);
    const std::vector<sf::CircleShape>& getWheelShapes() const;
    const sf::ConvexShape& getChassisShape() const;
    void updateShape();
    b2Body* getBody() const;
    bool isMoving();
    void deleteBody();

};


#endif //VEHICLEEVOLUTION_VEHICLE_H
