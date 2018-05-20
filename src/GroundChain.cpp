//
// Created by ccc on 5/13/18.
//

#include "GroundChain.h"

GroundChain::GroundChain(std::vector<sf::Vector2f>& verts, b2World *world_)
: world{world_}, lineStrip{sf::LineStrip, verts.size()}
{
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(0,0);
    bodyDef.type = b2_staticBody;
    b2Body* body = world_->CreateBody(&bodyDef);

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