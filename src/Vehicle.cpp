//
// Created by dawid on 27.04.18.
//

#include <numeric>
#include <utility>
#include <World.h>
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
                 float y_) :
        vertices{vertices_}
{

    createShapes(leftWheelSize, rightWheelSize, x_, y_);

    createBody(leftWheelSize, rightWheelSize, x_, y_);
}

Vehicle::Vehicle(const Chromosome& chromosome, float x_, float y_) : vertices{ chromosome.getBodyVertices()}
{
    float centerX = std::accumulate(vertices.begin(), vertices.end(), 0.0f,
                                     [](float acc, const std::pair<float, float>& vrt)
                                     {
                                         return acc += vrt.first;
                                     }) / vertices.size();

    float centerY = std::accumulate(vertices.begin(), vertices.end(), 0.0f,
                                     [](float acc, const std::pair<float, float>& vrt)
                                     {
                                         return acc += vrt.second;
                                     }) / vertices.size();

    std::sort(vertices.begin(), vertices.end(),
              [&](const std::pair<float, float>& left, const std::pair<float, float>& right)
              {
                  return atan2(left.second - centerY, left.first - centerX) <
                         atan2(right.second - centerY, right.first - centerX);
              });


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


    chassisBody = World::getInstance().createDynamicBody(x_ / SCALE, y_ / SCALE);

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
    leftWheelBody = World::getInstance().createDynamicBody(x_ / SCALE + vertices[0].first, y_ / SCALE + vertices[0].second);
    leftWheelBody->CreateFixture(&leftWheelDef);

    b2CircleShape rightWheelCircle;
    rightWheelCircle.m_radius = rightWheelSize;
    b2FixtureDef rightWheelDef;
    rightWheelDef.shape = &rightWheelCircle;
    rightWheelDef.density = 0.9f;
    rightWheelDef.friction = 0.9f;
    rightWheelBody = World::getInstance().createDynamicBody(x_ / SCALE + vertices[2].first, y_ / SCALE + vertices[2].second);
    rightWheelBody->CreateFixture(&rightWheelDef);

    //b2Vec2 axis(0.0f, 1.0f);
    springLeft = World::getInstance().createWheelJoint(chassisBody, leftWheelBody, leftWheelBody->GetPosition());
    springLeft = World::getInstance().createWheelJoint(chassisBody, rightWheelBody, rightWheelBody->GetPosition());
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


