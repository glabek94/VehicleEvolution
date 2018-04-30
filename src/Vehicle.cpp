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