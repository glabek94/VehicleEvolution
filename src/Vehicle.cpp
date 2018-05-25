//
// Created by dawid on 27.04.18.
//

#include <numeric>
#include <utility>
#include "Vehicle.h"

sf::ConvexShape Vehicle::getChassisShape() const
{
    return chassisShape;
}

sf::CircleShape Vehicle::getLeftWheelShape() const
{
    return leftWheelShape;
}

sf::CircleShape Vehicle::getRightWheelShape() const
{
    return rightWheelShape;
}

b2Body* Vehicle::getBody() const
{
    return chassisBody;
}

void Vehicle::updateShape()
{
    //std::cout<<"position: x:"<<body->GetPosition().x*SCALE <<" y:"<< body->GetPosition().y*SCALE<<std::endl;
    chassisShape.setPosition(chassisBody->GetPosition().x * SCALE, chassisBody->GetPosition().y * SCALE);
    chassisShape.setRotation(chassisBody->GetAngle() * 180 / b2_pi);

    //temporary code duplication, left/right will be changed to pair or vector;
    leftWheelShape.setPosition(
            chassisShape.getTransform().transformPoint(chassisShape.getPoint(0)).x,
            chassisShape.getTransform().transformPoint(chassisShape.getPoint(0)).y);
    rightWheelShape.setPosition(
            chassisShape.getTransform().transformPoint(chassisShape.getPoint(2)).x,
            chassisShape.getTransform().transformPoint(chassisShape.getPoint(2)).y);
}

Vehicle::Vehicle(std::vector<std::pair<float, float>>& vertices_, float leftWheelSize, float rightWheelSize, float x_,
                 float y_, b2World* world_) :
        vertices{vertices_},
        world{world_}
{

    createShapes(leftWheelSize, rightWheelSize, x_, y_);

    createBody(leftWheelSize, rightWheelSize, x_, y_);


}

Vehicle::Vehicle(const Chromosome& chromosome, float x_, float y_, b2World* world_) : vertices{
        chromosome.getBodyVertices()}, world{world_}
{
    float centerX = std::accumulate(vertices.begin(), vertices.end(), 0.0,
                                     [](float first, const std::pair<float, float>& second)
                                     {
                                         return first += std::get<0>(second);
                                     }) / vertices.size();

    float centerY = std::accumulate(vertices.begin(), vertices.end(), 0.0,
                                     [](float first, const std::pair<float, float>& second)
                                     {
                                         return first += std::get<1>(second);
                                     }) / vertices.size();

    for (const auto& v : vertices)
    {
        std::cout << std::get<0>(v) << ' ' << std::get<1>(v) << std::endl;
    }
    std::cout << std::endl;

    std::sort(vertices.begin(), vertices.end(),
              [&](const auto& left, const auto& right)
              {
                  return atan2(std::get<1>(left) - centerY, std::get<0>(left) - centerX) <
                         atan2(std::get<1>(right) - centerY, std::get<0>(right) - centerX);
              });

    for (const auto& v : vertices)
    {
        std::cout << std::get<0>(v) << ' ' << std::get<1>(v) << std::endl;
    }
    std::cout << std::endl;

    auto firstWheelSize = chromosome.getWheelSizes().at(0);
    auto secondWheelSize = chromosome.getWheelSizes().at(1);
    createShapes(firstWheelSize, secondWheelSize, x_, y_);
    createBody(firstWheelSize, secondWheelSize, x_, y_);

}

/*
 * Box2D body creation
 */
void Vehicle::createBody(float leftWheelSize, float rightWheelSize, float x_, float y_)
{

    float centerX = std::accumulate(vertices.begin(), vertices.end(), 0.0,
                                     [](float first, const std::pair<float, float>& second)
                                     {
                                         return first += std::get<0>(second);
                                     }) / vertices.size();

    float centerY = std::accumulate(vertices.begin(), vertices.end(), 0.0,
                                     [](float first, const std::pair<float, float>& second)
                                     {
                                         return first += std::get<1>(second);
                                     }) / vertices.size();

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(x_ / SCALE, y_ / SCALE);
    chassisBody = world->CreateBody(&bodyDef);

    /*
     * Iterate over every vertex and create a triangle fan centered at [centerX,centerY]
     */
    for (auto v = vertices.begin(); v != vertices.end(); ++v)
    {
        std::vector<std::pair<float, float>>::iterator next;
        if (v + 1 != vertices.end())
        {
            next = v + 1;
        }
        else
        {
            next = vertices.begin();
        }
        //std::cout<<"A: "<<v->first<<" : "<<v->second<<std::endl;
        //std::cout<<"B: "<<next->first<<" : "<<next->second<<std::endl;
        b2FixtureDef triangleFixture;
        triangleFixture.density = 1;
        triangleFixture.friction = 0.7;
        b2PolygonShape triangleShape;

        b2Vec2 triangle[3];
        triangle[0] = {centerX, centerY};
        triangle[1] = {v->first, v->second};

        triangle[2] = {next->first, next->second};
        triangleShape.Set(triangle, 3);
        triangleFixture.shape = &triangleShape;
        chassisBody->CreateFixture(&triangleFixture);
    }


    //temporary code duplication, left/right will be changed to pair or vector;
    b2CircleShape leftWheelCircle;
    leftWheelCircle.m_radius = leftWheelSize;
    b2FixtureDef leftWheelDef;
    leftWheelDef.shape = &leftWheelCircle;
    leftWheelDef.density = 1.0f;
    leftWheelDef.friction = 0.9f;
    bodyDef.position.Set(x_ / SCALE + vertices[0].first, y_ / SCALE + vertices[0].second);
    leftWheelBody = world->CreateBody(&bodyDef);
    leftWheelBody->CreateFixture(&leftWheelDef);

    b2CircleShape rightWheelCircle;
    rightWheelCircle.m_radius = rightWheelSize;
    b2FixtureDef rightWheelDef;
    rightWheelDef.shape = &rightWheelCircle;
    rightWheelDef.density = 0.9f;
    rightWheelDef.friction = 0.9f;
    bodyDef.position.Set(x_ / SCALE + vertices[2].first, y_ / SCALE + vertices[2].second);
    rightWheelBody = world->CreateBody(&bodyDef);
    rightWheelBody->CreateFixture(&rightWheelDef);

    b2WheelJointDef jointDef;
    b2Vec2 axis(0.0f, 1.0f);

    jointDef.Initialize(chassisBody, leftWheelBody, leftWheelBody->GetPosition(), axis);
    jointDef.motorSpeed = 16.0f;
    jointDef.maxMotorTorque = 30.0f;
    jointDef.enableMotor = true;
    jointDef.frequencyHz = 16.0f;
    jointDef.dampingRatio = 0.7f;
    springLeft = (b2WheelJoint*) world->CreateJoint(&jointDef);

    jointDef.Initialize(chassisBody, rightWheelBody, rightWheelBody->GetPosition(), axis);
    springRight = (b2WheelJoint*) world->CreateJoint(&jointDef);
}

void Vehicle::createShapes(float leftWheelSize, float rightWheelSize, float x_, float y_)
{/*
     *  SFML shape creation
     */

    chassisShape.setPointCount(vertices.size());

    int i = 0;
    for (std::pair<float, float> v : vertices)
    {
        chassisShape.setPoint(i++, sf::Vector2f(v.first * SCALE, v.second * SCALE));
    }
    chassisShape.setFillColor(sf::Color::Blue);
    chassisShape.setPosition(x_, y_);


    //temporary code duplication left/right will be changed to pair or vector;
    leftWheelShape.setRadius(leftWheelSize * SCALE);
    leftWheelShape.setOrigin(leftWheelSize * SCALE, leftWheelSize * SCALE);
    leftWheelShape.setFillColor(sf::Color::Green);
    leftWheelShape.setOutlineThickness(1.0f);
    leftWheelShape.setOutlineColor(sf::Color::Black);
    //leftWheelShape.setPosition(bodyworkShape.getPosition()+sf::Vector2f(vertices[0].first*SCALE, vertices[0].second*SCALE));

    rightWheelShape.setRadius(rightWheelSize * SCALE);
    rightWheelShape.setOrigin(rightWheelSize * SCALE, rightWheelSize * SCALE);
    rightWheelShape.setFillColor(sf::Color::Red);
    rightWheelShape.setOutlineThickness(1.0f);
    rightWheelShape.setOutlineColor(sf::Color::Black);
    //rightWheelShape.setPosition(bodyworkShape.getPosition()+sf::Vector2f(vertices[2].first*SCALE, vertices[2].second*SCALE));


}


