//
// Created by ccc on 5/13/18.
//

#include "GroundChain.h"

GroundChain::GroundChain(std::vector<sf::Vector2f>& verts)
: lineStrip{sf::LineStrip, verts.size()}
{
    b2Body* body = World::getInstance().createStaticBody(0,0);
    b2ChainShape chainShape;
    b2Vec2 tmp[verts.size()];
    for(int i=0; i<verts.size(); ++i){
        tmp[i] = b2Vec2(verts[i].x/SCALE,verts[i].y/SCALE);
    }
    chainShape.CreateChain(tmp, verts.size());

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
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