//
// Created by dawid on 27.04.18.
//

#include <EvoAlgo/EvolutionaryAlgorithm.h>
#include "Application.h"
#include "Vehicle.h"
#include "GroundChain.h"
#include "GroundFactory.h"

void Application::run() {

    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Vehicle Evolution");
    sf::View view;
    window.setFramerateLimit(60);

    //empty call to initialize -- not needed
    World::getInstance();

    EvolutionaryAlgorithm algo(20, 2, 0.1f);

    std::vector<Vehicle> cars(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());

    std::vector<float> fitness(cars.size());

    std::vector<std::shared_ptr<GroundChain>> ground;

    sf::Event event;
    ground.emplace_back(GroundFactory::getInstance().createGround());

    sf::Text ranking;
    ranking.setFont(font);

    sf::FloatRect textRect = ranking.getLocalBounds();
    while (window.isOpen()) {
        //find furthest car
        auto furthestCar = std::min_element(cars.begin(), cars.end(),
                                            [](const Vehicle &a, const Vehicle &b) {
                                                return a.getBody()->IsAwake() &&
                                                       (a.getBody()->GetPosition().x > b.getBody()->GetPosition().x);
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
            for (int i = 0; i < cars.size(); ++i) {
                fitness[i] = cars[i].getBody()->GetPosition().x;
                std::cerr << i << ' ' << fitness[i] << ';' << std::endl;
                cars[i].deleteBody();
            }
            std::cerr << std::endl;
            cars.clear();
            algo.EvaluateCurrentGenarationAndEvolve(fitness);
            fitness.clear();
            cars.clear();
            cars = std::vector<Vehicle>(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
            fitness.reserve(cars.size());
        }

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

        ranking.setOrigin(textRect.left + textRect.width / 2.0f,
                          textRect.top + textRect.height / 2.0f);
        ranking.setPosition(furthestCar->getChassisShape().getPosition());
        ranking.setCharacterSize(24);
        ranking.setFillColor(sf::Color::Black);

        window.draw(ranking);

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