//
// Created by ccc on 5/13/18.
//

#include "GroundFactory.h"

GroundFactory::GroundFactory(b2World *world_) : world{world_}, previousChainEnd{0,0} {
    if(world == nullptr)
        throw std::invalid_argument("world pointer can't be null.");

    rng = std::make_unique<std::mt19937>();
    rng->seed(std::random_device()());
    distribution = std::make_unique<std::uniform_real_distribution<>>(-maxDegree, maxDegree);

}

/*
 * At the first use world can't be null.
 */
GroundFactory& GroundFactory::getInstance(b2World *world_ = nullptr) {
    static GroundFactory instance(world_);
    return instance;
}

GroundChain GroundFactory::createGround() {
    std::vector<sf::Vector2f> verts;
    verts.push_back(sf::Vector2f(previousChainEnd.x*SCALE, previousChainEnd.y*SCALE));
    for(int i=1; i<numberOfEdges; ++i){
        float deg = (*distribution)(*rng);
        verts.push_back(sf::Vector2f(verts[i-1].x+edgeLength*std::cos(deg),verts[i-1].y+edgeLength*std:: sin(deg)));
    }
    return GroundChain(verts, world);
}