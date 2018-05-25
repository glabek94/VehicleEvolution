//
// Created by ccc on 5/13/18.
//

#include "GroundFactory.h"

GroundFactory::GroundFactory() : previousChainEnd{0,0} {

}

/*
 * At the first use world can't be null.
 */
GroundFactory& GroundFactory::getInstance() {
    static GroundFactory instance;
    return instance;
}

GroundChain GroundFactory::createGround() {
    std::vector<sf::Vector2f> verts;
    verts.push_back(sf::Vector2f(previousChainEnd.x*SCALE, previousChainEnd.y*SCALE));
    for(int i=1; i<numberOfEdges; ++i){
        float deg = RandomNumberGenerator::Instance().GetDoubleFromUniformDist(-maxDegree, maxDegree);
        verts.push_back(sf::Vector2f(verts[i-1].x+edgeLength*std::cos(deg),verts[i-1].y+edgeLength*std:: sin(deg)));
    }
    return GroundChain(verts);
}