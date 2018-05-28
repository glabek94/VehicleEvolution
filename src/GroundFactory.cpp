//
// Created by ccc on 5/13/18.
//

#include "GroundFactory.h"
#include <iostream>

GroundFactory::GroundFactory() : previousChainEnd{0, 0} {

}

/*
 * At the first use world can't be null.
 */
GroundFactory &GroundFactory::getInstance() {
    static GroundFactory instance;
    return instance;
}

std::shared_ptr<GroundChain> GroundFactory::createGround() {
    std::vector<sf::Vector2f> verts;
    verts.emplace_back(sf::Vector2f(previousChainEnd.x * SCALE, previousChainEnd.y * SCALE));
    for (int i = 1; i < numberOfEdges; ++i) {
        float curMax = maxDegree - difficultyCoeff * maxDegree;
        float deg = RandomNumberGenerator::Instance().GetFloatFromUniformDist(-curMax,
                                                                              curMax);

        int curLength = RandomNumberGenerator::Instance().GetIntFromUniformDist(difficultyCoeff * edgeLength,
                                                                                edgeLength);
        if (curLength < 10)
            curLength = 10;

        verts.emplace_back(
                sf::Vector2f(verts[i - 1].x + curLength * std::cos(deg), verts[i - 1].y + curLength * std::sin(deg)));
    }

    auto result = std::make_shared<GroundChain>(verts);

    if (previousChain != nullptr) {
        previousChain->getChainShape()->SetNextVertex(previousChainEnd);
        result->getChainShape()->SetPrevVertex(previousChainEnd);
    }

    previousChain = result;
    previousChainEnd = b2Vec2(verts.back().x / SCALE, verts.back().y / SCALE);

    difficultyCoeff *= difficultyCoeff;
    return result;
}

b2Vec2 GroundFactory::getPreviousChainEnd() const {
    return previousChainEnd;
}