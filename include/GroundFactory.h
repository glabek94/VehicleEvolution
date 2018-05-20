//
// Created by ccc on 5/13/18.
//

#ifndef PROJECT_GROUNDFACTORY_H
#define PROJECT_GROUNDFACTORY_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <random>
#include <memory>
#include <vector>
#include <stdexcept>
#include "GroundChain.h"
#include "Constants.h"

/*! \class GroundFactory
 *  \brief Class creating ground objects
 *
 *  Uses factory and singleton design patterns.
 *
 */

class GroundFactory {
private:

    /*
     * Singleton - constructors are private
     */
    GroundFactory(b2World *world_);
    GroundFactory(const GroundFactory&) = delete;
    GroundFactory&operator=(const GroundFactory& ) = delete;


    std::unique_ptr<std::mt19937> rng;
    std::unique_ptr<std::uniform_real_distribution<>> distribution;



    b2World *world;


    /*
     * Edge length in px
     */
    int edgeLength = 150;

    /*
     * Number of edges in chain
     */
    int numberOfEdges = 20;

    /*
     * Maximum degree in rads
     */
    float maxDegree = 0.6f;

    b2Vec2 previousChainEnd;

public:
    static GroundFactory& getInstance(b2World *world_);
    GroundChain createGround();

};


#endif //PROJECT_GROUNDFACTORY_H
