//
// Created by dawid on 27.04.18.
//

#include <EvolutionaryAlgorithm.h>
#include "Application.h"
#include "Vehicle.h"
#include "GroundChain.h"
#include "GroundFactory.h"

void Application::run() {

    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Vehicle Evolution");
    sf::View view;
    window.setFramerateLimit(200);

    //empty call to initialize -- not needed
    World::getInstance();

    EvolutionaryAlgorithm algo(30, 2, 0.1f);

    std::vector<Vehicle> cars(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());

    std::vector<float> fitness(cars.size());

    std::vector<std::shared_ptr<GroundChain>> ground;


    std::vector<sf::RectangleShape> recordMarks;

    sf::Event event;
    ground.emplace_back(GroundFactory::getInstance().createGround());

    sf::Text ranking;
    ranking.setFont(font);
    sf::FloatRect rankingRect = ranking.getLocalBounds();


    sf::Text info;
    info.setFont(font);


    int curGeneration = 1;
    float record = 0.0;

    while (window.isOpen()) {
        //find furthest car
        auto furthestCar = std::min_element(cars.begin(), cars.end(),
                                            [](const Vehicle &a, const Vehicle &b) {
                                                if (a.getBody()->IsAwake() && b.getBody()->IsAwake()) {
                                                    return a.getBody()->GetPosition().x > b.getBody()->GetPosition().x;
                                                } else if (a.getBody()->IsAwake()) {
                                                    return true;
                                                } else {
                                                    return false;
                                                }
                                            });

        while (window.pollEvent(event)) {
            switch (event.type) {
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

//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
//            int MouseX = sf::Mouse::getPosition(window).x;
//            int MouseY = sf::Mouse::getPosition(window).y;
//            CreateBox(view.getCenter().x + MouseX - view.getSize().x / 2,
//                      view.getCenter().y + MouseY - view.getSize().y / 2);
//        }

//        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
//            //int MouseX = sf::Mouse::getPosition(window).x;
//           //int MouseY = sf::Mouse::getPosition(window).y;
//            //CreateBox(view.getCenter().x + MouseX - view.getSize().x / 2,
//            //view.getCenter().y + MouseY - view.getSize().y / 2);
//            std::vector<float> fitness;
//            std::transform(cars.begin(), cars.end(), std::back_inserter(fitness), [&](Vehicle &c) {
//                auto toReturn = c.getBody()->GetPosition().x;
//                c.deleteBody();
//                return toReturn;
//            });
//
//            cars.clear();
//            algo.EvaluateCurrentGenarationAndEvolve(fitness);
//            cars = std::vector<Vehicle>(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
//        }

        World::getInstance().step(1 / 20.f, 8, 3);
        window.clear(sf::Color::White);

        for (auto b : boxes) {
            sf::RectangleShape rs;
            rs.setFillColor(sf::Color::Red);
            rs.setOrigin(16, 16);
            rs.setSize(sf::Vector2f(32, 32));
            rs.setPosition(SCALE * b->GetPosition().x, SCALE * b->GetPosition().y);
            rs.setOutlineColor(sf::Color::Black);
            rs.setOutlineThickness(1.0f);
            rs.rotate(b->GetAngle() * 180 / b2_pi);
            window.draw(rs);
        }


        view.setCenter(furthestCar->getChassisShape().getPosition());
        //view.setCenter(0, 0);
        window.setView(view);

        // check if new GroundChain needed
        if (b2Distance(GroundFactory::getInstance().getPreviousChainEnd(), furthestCar->getBody()->GetPosition()) <
            20.f) {
            ground.emplace_back(GroundFactory::getInstance().createGround());
        }

        for (const auto &c : ground) {
            window.draw(c->getShapes());
        }


        //check any vehicle is moving
        bool someoneIsMoving = false;
        for (auto &c : cars) {
            if (c.isMoving()) {
                someoneIsMoving = true;

            } else if (c.getBody()->IsAwake())
                c.getBody()->SetAwake(false);
        }

        //new generation if no vehicle is moving
        if (!someoneIsMoving) {

            sf::RectangleShape recordMark(sf::Vector2f(7, 2*view.getSize().y));
            recordMark.setPosition(furthestCar->getChassisShape().getPosition() + sf::Vector2f(0,-view.getSize().y));
            sf::Color fc = furthestCar->getChassisShape().getFillColor();
            fc.a = 100;
            recordMark.setFillColor(fc);
            recordMarks.emplace_back(recordMark);

            for (int i = 0; i < cars.size(); ++i) {
                fitness[i] = computeFitness(cars[i]);
                std::cerr << i << ' ' << fitness[i] << ';' << std::endl;
                cars[i].deleteBody();
            }
            std::cerr << std::endl;

            auto best = *std::max_element(cars.begin(), cars.end(),
                                          [](const Vehicle &c1, const Vehicle &c2) {
                                              return c1.getBody()->GetPosition().x < c2.getBody()->GetPosition().x;
                                          });
            if (best.getBody()->GetPosition().x > record) {
                record = best.getBody()->GetPosition().x;
            }

            algo.EvaluateCurrentGenarationAndEvolve(fitness);
            cars.clear();
            std::fill(fitness.begin(), fitness.end(), 0.0);

            cars = std::vector<Vehicle>(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
            ++curGeneration;

        }

        for(auto &l : recordMarks)
            window.draw(l);

        for (auto &c : cars) {
            c.updateShape();
        }

        std::string txt = "";

        for (auto const &c : cars) {
            window.draw(c.getChassisShape());

            for (auto const &wheel: c.getWheelShapes())
                window.draw(wheel);

            txt += std::to_string(c.getBody()->GetPosition().x);
            txt += '\n';
        }


        ranking.setString(txt);

//        ranking.setOrigin(rankingRect.left + rankingRect.width / 2.0f,
//                          rankingRect.top + rankingRect.height / 2.0f);
        //ranking.setPosition(furthestCar->getChassisShape().getPosition());
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
        window.display();
    }
}


void Application::CreateBox(int MouseX, int MouseY) {
    b2Body *Body = World::getInstance().createDynamicBody(MouseX / SCALE, MouseY / SCALE);
    b2PolygonShape Shape;
    Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    boxes.push_back(Body);
}

float Application::computeFitness(const Vehicle &car) {
    auto dist = car.getBody()->GetPosition().x;
    if (dist < 20.0) { return 0.0f; }
    return dist;
}