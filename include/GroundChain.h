//
// Created by ccc on 5/13/18.
//

#ifndef PROJECT_GROUND_H
#define PROJECT_GROUND_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Constants.h"

/*! \class GroundChain
 *  \brief Class representing ground
 *
 *  Objects of this class are created by GroundFactory
 *
 */

class GroundChain {
private:

    /*
     * Naked pointer since it's allocated by b2World
     */
    b2Body* body;

    b2World *world;

    sf::VertexArray lineStrip;


public:

    GroundChain( std::vector<sf::Vector2f>& verts, b2World *world_);


    const sf::VertexArray& getShapes() const;

};


#endif //PROJECT_GROUND_H
