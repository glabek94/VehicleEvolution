//
// Created by ccc on 5/24/18.
//

#include "World.h"

World::World() : gravity{0.f, 9.8f}, world{gravity} {}

//singleton
World& World::getInstance() {
    static  World instance;
    return instance;
}

b2Body* World::createStaticBody(float32 x, float32 y) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(x, y);
    return world.CreateBody(&bodyDef);
}


b2Body* World::createDynamicBody(float32 x, float32 y) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(x, y);
    return world.CreateBody(&bodyDef);
}

b2WheelJoint* World::createWheelJoint(b2Body* bodyA,
                                      b2Body* bodyB,
                                      const b2Vec2& anchor,
                                      const b2Vec2& axis){

    b2WheelJointDef wheelJointDef;
    wheelJointDef.Initialize(bodyA, bodyB, anchor, axis);
    //fill with deafault values defined in header
    wheelJointDef.motorSpeed =      defaultMotorSpeed;
    wheelJointDef.maxMotorTorque =  defaultMaxMotorTorque;
    wheelJointDef.enableMotor =     defaultEnableMotor;
    wheelJointDef.frequencyHz =     defaultFrequencyHz;
    wheelJointDef.dampingRatio =    defaultDampingRatio;
    return dynamic_cast<b2WheelJoint*> (world.CreateJoint(&wheelJointDef));
}

void World::step(float timeStep, int velocityIterations, int positionIterations) {
    world.Step(timeStep, velocityIterations, positionIterations);
}

