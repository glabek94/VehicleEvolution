//
// Created by dawid on 27.04.18.
//

#include <numeric>
#include <utility>
#include <World.h>
#include "Vehicle.h"

const sf::ConvexShape& Vehicle::getChassisShape() const {
    return chassisShape;
}

b2Body *Vehicle::getBody() const {
    return chassisBody;
}

void Vehicle::updateShape() {
    //std::cout<<"position: x:"<<body->GetPosition().x*SCALE <<" y:"<< body->GetPosition().y*SCALE<<std::endl;
    chassisShape.setPosition(chassisBody->GetPosition().x * SCALE, chassisBody->GetPosition().y * SCALE);
    chassisShape.setRotation(chassisBody->GetAngle() * 180 / b2_pi);

    int i = 0;
    for (auto &wheelShape : wheelShapes) {
        wheelShape.setPosition(
                chassisShape.getTransform().transformPoint(chassisShape.getPoint(chromo.getWheelVertices()[i])).x,
                chassisShape.getTransform().transformPoint(chassisShape.getPoint(chromo.getWheelVertices()[i])).y);

        ++i;
    }
}


Vehicle::Vehicle(const Chromosome &chromosome, float x_, float y_) : vertices{chromosome.getBodyVertices()},
                                                                     chromo(chromosome),
                                                                     wheelBodies(Chromosome::WHEELS_NUMBER),
                                                                     springs(Chromosome::WHEELS_NUMBER),
                                                                     wheelShapes(Chromosome::WHEELS_NUMBER),
                                                                     timeStopped(0.f)
{
    float centerX = std::accumulate(vertices.begin(), vertices.end(), 0.0f,
                                    [](float acc, const std::pair<float, float> &vrt) -> float {
                                        return acc += vrt.first;
                                    }) / vertices.size();

    float centerY = std::accumulate(vertices.begin(), vertices.end(), 0.0f,
                                    [](float acc, const std::pair<float, float> &vrt) -> float {
                                        return acc += vrt.second;
                                    }) / vertices.size();

    std::sort(vertices.begin(), vertices.end(),
              [&](const std::pair<float, float> &left, const std::pair<float, float> &right) {
                  return atan2(left.second - centerY, left.first - centerX) <
                         atan2(right.second - centerY, right.first - centerX);
              });


    createShapes(chromosome.getWheelSizes(), x_, y_);
    createBody(chromosome.getWheelSizes(), chromosome.getWheelVertices(), x_, y_);
}


void Vehicle::createShapes(const std::vector<float> &wheelSizes, float x_, float y_) {
/*
     *  SFML shape creation
     */

    chassisShape.setPointCount(vertices.size());

    int i = 0;
    for (std::pair<float, float> v : vertices) {
        chassisShape.setPoint(i++, sf::Vector2f(v.first * SCALE, v.second * SCALE));
    }


    //generate color according to the fenotype
    //make proper scale later
    //dangerous!! needs to be changed to something without overflows..
    sf::Color c(0, 0, 0);
    for (const auto &v : vertices) {
        c.r += v.first * 15;
        c.g += v.second * 15;
        c.b += (v.first + v.second) * 7;
    }

    chassisShape.setFillColor(c);
    chassisShape.setPosition(x_, y_);

    i = 0;
    for (auto const &wheel:wheelSizes) {
        wheelShapes[i].setRadius(wheel * SCALE);
        wheelShapes[i].setOrigin(wheel * SCALE, wheel * SCALE);
        //set color according to the genotype
        //make proper scale later
        //dangerous! Might overflow...
        wheelShapes[i].setFillColor(sf::Color(wheel * 70, wheel * 100, wheel * 70));
        wheelShapes[i].setOutlineThickness(1.0f);
        wheelShapes[i].setOutlineColor(sf::Color::Black);

        ++i;
    }

}

void
Vehicle::createBody(const std::vector<float> &wheelSizes, const std::vector<int> &wheelVertices, float x_, float y_) {
    float centerX = std::accumulate(vertices.begin(), vertices.end(), 0.0f,
                                    [](float acc, const std::pair<float, float> &vrt) -> float {
                                        return acc += vrt.first;
                                    }) / vertices.size();

    float centerY = std::accumulate(vertices.begin(), vertices.end(), 0.0f,
                                    [](float acc, const std::pair<float, float> &vrt) -> float {
                                        return acc += vrt.second;
                                    }) / vertices.size();


    chassisBody = World::getInstance().createDynamicBody(x_ / SCALE, y_ / SCALE);

    /*
     * Iterate over every vertex and create a triangle fan centered at [centerX,centerY]
     */
    for (auto v = vertices.begin(); v != vertices.end(); ++v) {
        std::vector<std::pair<float, float>>::iterator next;
        if (v + 1 != vertices.end()) {
            next = v + 1;
        } else {
            next = vertices.begin();
        }

        b2FixtureDef triangleFixture;
        triangleFixture.density = 1.f;
        //triangleFixture.friction = 0.7;
        triangleFixture.friction = 50.0;
        triangleFixture.filter.groupIndex = -1;
        b2PolygonShape triangleShape;

        b2Vec2 triangle[3];
        triangle[0] = {centerX, centerY};
        triangle[1] = {v->first, v->second};

        triangle[2] = {next->first, next->second};

        if(triangle[1].x == triangle[2].x || triangle[1].y == triangle[2].y)
        {
            continue;
        }
        triangleShape.Set(triangle, 3);
        triangleFixture.shape = &triangleShape;
        chassisBody->CreateFixture(&triangleFixture);
    }

    int i = 0;
    for (auto const &wheel:wheelSizes) {
        b2CircleShape wheelCircle;
        wheelCircle.m_radius = wheel;
        b2FixtureDef wheelDef;
        wheelDef.shape = &wheelCircle;
        wheelDef.density = 1.0f;
        wheelDef.friction = 0.9f;
        wheelDef.filter.groupIndex = -1;
        wheelBodies[i] = World::getInstance().createDynamicBody(x_ / SCALE + vertices[wheelVertices[i]].first,
                                                                y_ / SCALE + vertices[wheelVertices[i]].second);
        wheelBodies[i]->CreateFixture(&wheelDef);

        i++;
    }

    for (size_t i = 0; i < wheelSizes.size(); i++) {
        springs[i] = World::getInstance().createWheelJoint(chassisBody, wheelBodies[i], wheelBodies[i]->GetPosition());
    }
}

const std::vector<sf::CircleShape> &Vehicle::getWheelShapes() const {
    return wheelShapes;
}

bool Vehicle::isMoving(){
    if(std::abs(chassisBody->GetLinearVelocity().x)>= 0.5f )
        timeStopped=0;
    else
        timeStopped+=1.f;

    return timeStopped < 100.0f;
}

void Vehicle::deleteBody()  {

    World::getInstance().destroyBody(chassisBody);
    for(auto w : wheelBodies)
        World::getInstance().destroyBody(w);
}

