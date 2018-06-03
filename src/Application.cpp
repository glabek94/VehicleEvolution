//
// Created by dawid on 27.04.18.
//

#include <EvolutionaryAlgorithm.h>
#include "Application.h"
#include "Vehicle.h"
#include "GroundChain.h"
#include "GroundFactory.h"


Application::Application() :
        window(sf::VideoMode(800, 600, 32), "Vehicle Evolution"),
        algo(30, 2, 0.1f),
        cars(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end()) {
    window.setFramerateLimit(200);
    window.setKeyRepeatEnabled(false);
    World::getInstance();
    fitness.resize(cars.size());
    ground.emplace_back(GroundFactory::getInstance().createGround());
    font.loadFromFile("arial.ttf");
    ranking.setFont(font);
    rankingRect = ranking.getLocalBounds();
    info.setFont(font);
}

void Application::run() {
    sf::Event event;
    int curGeneration = 1;
    float record = 0.0;
    while (window.isOpen()) {
        bool resetGeneration = false;

        //find furthest moving car
        auto furthestCar = std::min_element(cars.begin(), cars.end(),
                                            [](const Vehicle& a, const Vehicle& b) {
                                                if (a.getBody()->IsAwake() && b.getBody()->IsAwake()) {
                                                    return a.getBody()->GetPosition().x > b.getBody()->GetPosition().x;
                                                }
                                                return a.getBody()->IsAwake();
                                            });

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Enter)
                        resetGeneration = true;
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    view.setCenter(furthestCar->getChassisShape().getPosition());
                    //view.setCenter(0,0);
                    view.setSize(window.getSize().x, window.getSize().y);
                    window.setView(view);
                    break;
                default:
                    break;
            }
        }


        World::getInstance().step(1 / 20.f, 8, 3);
        window.clear(sf::Color::White);
        view.setCenter(furthestCar->getChassisShape().getPosition());
        window.setView(view);

        // check if new GroundChain needed
        if (b2Distance(GroundFactory::getInstance().getPreviousChainEnd(), furthestCar->getBody()->GetPosition()) <
            20.f) {
            ground.emplace_back(GroundFactory::getInstance().createGround());
        }

        for (const auto& c : ground) {
            window.draw(c->getShapes());
        }


        //check any vehicle is moving
        bool someoneIsMoving = false;
        for (auto& c : cars) {
            if (c.isMoving()) {
                someoneIsMoving = true;

            } else if (c.getBody()->IsAwake())
                c.getBody()->SetAwake(false);
        }

        //new generation if no vehicle is moving
        if (!someoneIsMoving || resetGeneration) {
            newGeneration(curGeneration, record);
        }

        std::string txt = drawCars();
        drawRankingAndInfo(curGeneration, record, txt);
        window.display();
    }
}

std::string Application::drawCars() {
    for (auto& l : recordMarks)
        window.draw(l);

    for (auto& c : cars) {
        c.updateShape();
    }

    std::string txt = "";

    for (auto const& c : cars) {
        window.draw(c.getChassisShape());

        for (auto const& wheel: c.getWheelShapes())
            window.draw(wheel);

        txt += std::to_string(c.getBody()->GetPosition().x);
        txt += '\n';
    }
    return txt;
}

void Application::drawRankingAndInfo(int curGeneration, float record, const std::string& txt) {

    ranking.setString(txt);
    ranking.setPosition(view.getCenter().x - view.getSize().x / 2,
                        view.getCenter().y - view.getSize().y / 2);
    ranking.setCharacterSize(12);
    ranking.setFillColor(sf::Color::Black);

    std::string infoText = "Generacja #" + std::to_string(curGeneration);
    infoText += "\nRekord = " + std::to_string(record);

    info.setString(infoText);

    sf::FloatRect infoRect = info.getLocalBounds();

    info.setOrigin(infoRect.left + infoRect.width / 0.9f,
                   infoRect.top - infoRect.height / 2.0f);
    info.setPosition(view.getCenter().x + view.getSize().x / 2,
                     view.getCenter().y - view.getSize().y / 2);
    info.setCharacterSize(12);
    info.setFillColor(sf::Color::Blue);


    window.draw(ranking);
    window.draw(info);
}

void Application::newGeneration(int& curGeneration, float& record) {
    auto globalFurthestCar = max_element(cars.begin(), cars.end(), [](Vehicle& a, Vehicle& b) -> bool {
        return a.getBody()->GetPosition().x < b.getBody()->GetPosition().x;
    });
    sf::RectangleShape recordMark(sf::Vector2f(7, 2 * view.getSize().y));
    recordMark.setPosition(globalFurthestCar->getChassisShape().getPosition() + sf::Vector2f(0, -view.getSize().y));
    sf::Color fc = globalFurthestCar->getChassisShape().getFillColor();
    fc.a = 80;
    recordMark.setFillColor(fc);
    recordMarks.emplace_back(recordMark);

    std::cerr << std::endl;

    auto best = *max_element(cars.begin(), cars.end(),
                             [](const Vehicle& c1, const Vehicle& c2) {
                                 return c1.getBody()->GetPosition().x < c2.getBody()->GetPosition().x;
                             });
    if (best.getBody()->GetPosition().x > record) {
        record = best.getBody()->GetPosition().x;
    }

    for (size_t i = 0; i < cars.size(); ++i) {
        fitness[i] = computeFitness(cars[i]);
        cars[i].deleteBody();
    }
    algo.EvaluateCurrentGenarationAndEvolve(fitness);
    cars.clear();
    fill(fitness.begin(), fitness.end(), 0.0);

    cars = std::vector<Vehicle>(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
    ++curGeneration;
}


float Application::computeFitness(const Vehicle& car) {
    auto dist = car.getBody()->GetPosition().x;
    if (dist < 20.0) { return 0.0f; }
    return dist;
}


