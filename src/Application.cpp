//
// Created by dawid on 27.04.18.
//

#include <EvoAlgo/EvolutionaryAlgorithm.h>
#include "Application.h"
#include "Vehicle.h"
#include "GroundChain.h"
#include "GroundFactory.h"

void Application::run() {

    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Vehicle Evolution");
    sf::View view;
    window.setFramerateLimit(60);

    //empty call to initialize -- not needed
    World::getInstance();

    EvolutionaryAlgorithm algo(10, 4, 0.05);

    std::vector<Vehicle> cars(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
    std::vector<float> fitness(cars.size());
    std::vector<std::shared_ptr<GroundChain>> ground;

    sf::Event event;
    ground.emplace_back(GroundFactory::getInstance().createGround());

    while (window.isOpen()) {
        //find furthest car
        auto furthestCar = std::min_element(cars.begin(), cars.end(),
                                            [](const Vehicle &a, const Vehicle &b) {
                                                return a.getBody()->GetPosition().x > b.getBody()->GetPosition().x;
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)){
            int MouseX = sf::Mouse::getPosition(window).x;
            int MouseY = sf::Mouse::getPosition(window).y;
            CreateBox(view.getCenter().x + MouseX - view.getSize().x / 2,
            view.getCenter().y + MouseY - view.getSize().y / 2);
        }

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


        //delete cars that are not moving and note their fitness
        int i=0;
        cars.erase(std::remove_if(cars.begin(), cars.end(),
                                  [&fitness, &i](Vehicle& c) -> bool {
                                           if(!c.isMoving())
                                            {
                                                fitness[i++] = c.getBody()->GetPosition().x;
                                                c.deleteBody();
                                                return true;
                                            }
                                            i++;
                                            return false;
                                  }), cars.end());


        //std::cout<<"BODY COUNT: "<<World::getInstance().getBodyCount()<<std::endl;
        if (cars.empty()){
            algo.EvaluateCurrentGenarationAndEvolve(fitness);
            cars.clear();
            cars = std::vector<Vehicle>(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
            fitness.clear();

        }
        /*
        for (std::vector<Vehicle>::iterator it = cars.begin(); it!=cars.end(); ++it) {
                if(!it->isMoving())
                    cars.erase(it);
        }
        */

        for(auto &c : cars) {
            c.updateShape();
        }

        for (auto const &c : cars) {
            window.draw(c.getChassisShape());

            for (auto const &wheel: c.getWheelShapes())
                window.draw(wheel);
        }

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