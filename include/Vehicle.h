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
    Vehicle( std::vector<std::pair<float, float>> &vertices_, float SCALE_, float x_, float y_, b2World *world_) :
            SCALE{SCALE_},
            vertices{vertices_},
            world{world_}{


        /*
         *  SFML shape creation
         */

        shape.setPointCount(vertices_.size());

        int i=0;
        for(std::pair<float, float> v : vertices_){
            shape.setPoint(i++, sf::Vector2f(v.first*SCALE, v.second*SCALE));
        }

        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(x_, y_);


        /*
         * Box2D body creation
         */

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = b2Vec2(x_/SCALE, y_/SCALE);
        body = world->CreateBody(&bodyDef);

        /*
         * Iterate over every vertex and create a triangle fan centered at [0,0]
         */
        for(std::vector<std::pair<float, float>>::iterator v = vertices_.begin(); v!=vertices_.end(); ++v){
            std::vector<std::pair<float, float>>::iterator next;
            if(v+1!=vertices_.end())
                next = v+1;
            else
                next = vertices_.begin();
            //std::cout<<"A: "<<v->first<<" : "<<v->second<<std::endl;
            //std::cout<<"B: "<<next->first<<" : "<<next->second<<std::endl;
            b2FixtureDef triangleFixture;
            triangleFixture.density = 1;
            triangleFixture.friction = 0.7;
            b2PolygonShape triangleShape;

            b2Vec2 triangle[3];
            triangle[0] = {0.f, 0.f};
            triangle[1] = {v->first, v->second};

            triangle[2] = {next->first, next->second};
            triangleShape.Set(triangle, 3);
            triangleFixture.shape = &triangleShape;
            body->CreateFixture(&triangleFixture);
        }

    }

    sf::ConvexShape getShape() const;
    void updateShape();
    b2Body* getBody() const;


};


#endif //VEHICLEEVOLUTION_VEHICLE_H
