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
#include <EvoAlgo/Chromosome.h>
#include "Constants.h"

class Vehicle
{
private:



    std::vector<std::pair<float, float>> vertices;
    Chromosome chromo;
    /*
     * Naked pointer since b2Bodies are allocated by b2World.
     */
    b2Body* chassisBody;

    std::vector<b2Body*> wheelBodies;
    b2Body* leftWheelBody;
    b2Body* rightWheelBody;


    /*
     * Naked pointer since b2Joints are allocated by b2World.
     */
    std::vector<b2WheelJoint*> springs;
    b2WheelJoint* springLeft;
    b2WheelJoint* springRight;


    /*
     * In this case, it actually represents concave shape, but since sf::ConvexShape is implemented
     * as triangle fan[1], we can use it to represent vehicle's body since it's also a triangle fan.
     * [1] https://www.sfml-dev.org/tutorials/2.4/graphics-shape.php
     */
    sf::ConvexShape chassisShape;

    //temporary code duplication, left/right will be changed to pair or vector;
    std::vector<sf::CircleShape> wheelShapes;
public:
    const std::vector<sf::CircleShape>& getWheelShapes() const;

private:
    sf::CircleShape leftWheelShape;
    sf::CircleShape rightWheelShape;


    void createShapes(float leftWheelSize, float rightWheelSize, float x_,  float y_);

    void createShapes(const std::vector<float>& wheelSizes, float x_, float y_);

    void createBody(float leftWheelSize, float rightWheelSize,  float x_,  float y_);

    void createBody(const std::vector<float>& wheelSizes, const std::vector<int>& wheelVertices, float x_, float y_);

public:
    /*
     * vertices_ is vector of vertices representing the body in the Box2D-scale (meters).
     */
    Vehicle( std::vector<std::pair<float, float>> &vertices_, float leftWheelSize, float rightWheelSize, float x_, float y__);

    Vehicle(const Chromosome& chromosome, float x_, float y_);
    sf::ConvexShape getChassisShape() const;
    sf::CircleShape getLeftWheelShape() const;
    sf::CircleShape getRightWheelShape() const;
    void updateShape();
    b2Body* getBody() const;



};


#endif //VEHICLEEVOLUTION_VEHICLE_H
