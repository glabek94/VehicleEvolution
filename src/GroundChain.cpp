//
// Created by ccc on 5/13/18.
//

#include "GroundChain.h"
#include <iostream>

GroundChain::GroundChain(std::vector<sf::Vector2f>& verts)
: lineStrip{sf::LineStrip, verts.size()}
{
    body = World::getInstance().createStaticBody(0,0);

    b2Vec2 tmp[verts.size()];
    for(int i=0; i<verts.size(); ++i){
        tmp[i] = b2Vec2(verts[i].x/SCALE,verts[i].y/SCALE);
    }
    b2ChainShape chainShape;
    chainShape.CreateChain(tmp, verts.size());

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    //FixtureDef.friction = 0.7f;
    FixtureDef.friction = 30.0f;
    FixtureDef.shape = &chainShape;
    body->CreateFixture(&FixtureDef);


    for(int i=0; i<verts.size(); ++i){
        lineStrip[i].position = verts[i];
        lineStrip[i].color = sf::Color::Black;
    }

}
const sf::VertexArray& GroundChain::getShapes() const {
    return lineStrip;
}
b2ChainShape* GroundChain::getChainShape() const {

    // static cast - it's always b2ChainShape (gotta go fast)
    return static_cast<b2ChainShape*>(body->GetFixtureList()->GetShape());
}
