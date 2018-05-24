//
// Created by ccc on 5/24/18.
//

#ifndef PROJECT_WORLD_H
#define PROJECT_WORLD_H

#include <Box2D/Box2D.h>
#include <memory>

/*! \class World
 *  \brief Class representing b2World
 *
 *  Uses singleton and adapter design patterns.
 */

class World {

private:

     //Singleton - constructors are private

    World();
    World(const World& ) = delete;
    World&operator=(const World& ) = delete;

    b2Vec2 gravity;
    b2World world;

    //b2WheelJoint constants
    const float32 defaultMotorSpeed = 16.f;
    const float32 defaultMaxMotorTorque = 50.f;
    const bool    defaultEnableMotor = true;
    const float32 defaultFrequencyHz = 16.f;
    const float32 defaultDampingRatio = 0.7f;

public:
    static World& getInstance();
    b2Body* createStaticBody(float32 x=0.f, float32 y=0.f);
    b2Body* createDynamicBody(float32 x=0.f, float32 y=0.f);
    b2WheelJoint* createWheelJoint(b2Body* bodyA,
                                   b2Body* bodyB,
                                   const b2Vec2& anchor,
                                   const b2Vec2& axis = b2Vec2(0.f, 1.f));
    void step(float timeStep, int velocityIterations, int positionIterations);
};


#endif //PROJECT_WORLD_H
