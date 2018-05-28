//
// Created by ccc on 5/13/18.
//

#ifndef PROJECT_GROUNDFACTORY_H
#define PROJECT_GROUNDFACTORY_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <memory>

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

    GroundFactory(const GroundFactory &) = delete;

    GroundFactory &operator=(const GroundFactory &) = delete;


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
    const float maxDegree = 0.78f;

    float difficultyCoeff = 0.95f;

    b2Vec2 previousChainEnd;

    //shared_ptrs are slow, maybe change to something else in future?
    std::shared_ptr<GroundChain> previousChain;

public:
    static GroundFactory &getInstance();

    std::shared_ptr<GroundChain> createGround();

    b2Vec2 getPreviousChainEnd() const;
};


#endif //PROJECT_GROUNDFACTORY_H
