//
// Created by dawid on 27.04.18.
//

#ifndef VEHICLEEVOLUTION_APPLICATION_H
#define VEHICLEEVOLUTION_APPLICATION_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Constants.h"
#include "World.h"
#include "Vehicle.h"
#include "GroundChain.h"
#include "EvolutionaryAlgorithm.h"

class Application {
private:
    sf::Font font;

    sf::RenderWindow window;

    sf::View view;

    EvolutionaryAlgorithm algo;

    std::vector<Vehicle> cars;

    std::vector<float> fitness;

    std::vector<std::shared_ptr<GroundChain>> ground;

    std::vector<sf::RectangleShape> recordMarks;

    sf::Text ranking;

    sf::FloatRect rankingRect;

    sf::Text info;
public:
    void run();
    Application();

private:
    float computeFitness(const Vehicle &car);

    void newGeneration(int &curGeneration, float &record);

    void drawRankingAndInfo(int curGeneration, float record, const std::string &txt);

    std::string drawCars();
};


#endif //VEHICLEEVOLUTION_APPLICATION_H
