//
// Created by ccc on 5/13/18.
//

#ifndef PROJECT_GROUNDFACTORY_H
#define PROJECT_GROUNDFACTORY_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include "GroundChain.h"
#include "Constants.h"
#include "World.h"
#include "RandomNumberGenerator.h"

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
    GroundFactory();
    GroundFactory(const GroundFactory&) = delete;
    GroundFactory&operator=(const GroundFactory& ) = delete;


    std::unique_ptr<std::mt19937> rng;
    std::unique_ptr<std::uniform_real_distribution<>> distribution;


    /*
     * Edge length in px
     */
    const int edgeLength = 150;

    /*
     * Number of edges in chain
     */
    const int numberOfEdges = 20;

    /*
     * Maximum degree in rads
     */
    const float maxDegree = 0.6f;

    b2Vec2 previousChainEnd;

public:
    static GroundFactory& getInstance();
    GroundChain createGround();

};


#endif //PROJECT_GROUNDFACTORY_H
