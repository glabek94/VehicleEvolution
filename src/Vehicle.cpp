//
// Created by dawid on 27.04.18.
//

#include "Vehicle.h"

sf::ConvexShape Vehicle::getShape() const{
    return shape;
}

b2Body* Vehicle::getBody() const {
    return body;
}

void Vehicle::updateShape(){
    //std::cout<<"position: x:"<<body->GetPosition().x*SCALE <<" y:"<< body->GetPosition().y*SCALE<<std::endl;
    shape.setPosition(body->GetPosition().x*SCALE , body->GetPosition().y*SCALE );
    shape.setRotation(body->GetAngle() * 180/b2_pi);
}

Vehicle::Vehicle(std::vector<std::pair<float, float>>& vertices_, float SCALE_, float x_, float y_, b2World* world_) :
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
