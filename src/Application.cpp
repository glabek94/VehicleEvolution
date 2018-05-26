//
// Created by dawid on 27.04.18.
//

#include <EvoAlgo/EvolutionaryAlgorithm.h>
#include "Application.h"
#include "Vehicle.h"
#include "GroundChain.h"
#include "GroundFactory.h"

void Application::run()
{

    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Vehicle Evolution");
    sf::View view;
    window.setFramerateLimit(60);

    //empty call to initialize -- not needed
    World::getInstance();

    std::vector<std::pair<float, float>> vertices;
    vertices.emplace_back(std::make_pair(0, 3));
    vertices.emplace_back(std::make_pair(-1.1, 1));
    vertices.emplace_back(std::make_pair(-2, -2));
    vertices.emplace_back(std::make_pair(2, -2));
    vertices.emplace_back(std::make_pair(1, 0));
    vertices.emplace_back(std::make_pair(2, 2));

    EvolutionaryAlgorithm algo(20, 2, 0.05);
    Vehicle car(vertices, 1.0f, 1.0f, 200, -300);

    //Vehicle car(algo.GetCurrentGeneration()[0], 200, -300);
    sf::Event event;


    //b2Vec2 array[] = {{0,0},{100/SCALE,30/SCALE},{200/SCALE,40/SCALE}, {300/SCALE,10/SCALE}};


    //b2BodyDef bodyDef;
    //bodyDef.position = b2Vec2(0,0);
    //bodyDef.type = b2_staticBody;
    //b2Body* Body = world.CreateBody(&bodyDef);

    //b2ChainShape chainShape;
    //chainShape.CreateChain(array, 4);


    //sf::RectangleShape rs;
    //rs.setOrigin(0,0);
    //rs.setPosition(30,70);
    //rs.setSize(sf::Vector2f(200, 7));
    //rs.setFillColor(sf::Color::Yellow);
    //rs.setRotation(30);

    //b2FixtureDef FixtureDef;
    //FixtureDef.density = 1.f;
    //FixtureDef.friction = 0.7f;
    //FixtureDef.shape = &chainShape;
    //Body->CreateFixture(&FixtureDef);



    //b2Vec2 a(-100/SCALE,300/SCALE);
    //b2Vec2 b(300/SCALE, 300/SCALE);
    //GroundChain ground(a,b,&world);

    std::vector<std::shared_ptr<GroundChain>> ground;

    ground.emplace_back(GroundFactory::getInstance().createGround());

    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    view.setCenter(car.getChassisShape().getPosition());
                    //view.setCenter(0,0);
                    view.setSize(window.getSize().x, window.getSize().y);
                    window.setView(view);
                    break;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int MouseX = sf::Mouse::getPosition(window).x;
            int MouseY = sf::Mouse::getPosition(window).y;
            CreateBox(view.getCenter().x + MouseX - view.getSize().x / 2,
                      view.getCenter().y + MouseY - view.getSize().y / 2);
        }

        World::getInstance().step(1 / 20.f, 8, 3);
        window.clear(sf::Color::White);

        for (auto b : boxes)
        {
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

        // check if new GroundChain needed
        if (b2Distance(GroundFactory::getInstance().getPreviousChainEnd(), car.getBody()->GetPosition()) < 20.f)
        {
            ground.emplace_back(GroundFactory::getInstance().createGround());
        }

        for (const auto& c : ground)
        {
            window.draw(c->getShapes());
        }

        car.updateShape();
        view.setCenter(car.getChassisShape().getPosition());
        //view.setCenter(0, 0);
        window.setView(view);
        window.draw(car.getChassisShape());

        for(auto const& wheel: car.getWheelShapes())
        {
            window.draw(wheel);
        }
//        window.draw(car.getLeftWheelShape());
//        window.draw(car.getRightWheelShape());

        //window.draw(rs);
        window.display();
    }
}


void Application::CreateBox(int MouseX, int MouseY)
{
    b2Body* Body = World::getInstance().createDynamicBody(MouseX / SCALE, MouseY / SCALE);
    b2PolygonShape Shape;
    Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    boxes.push_back(Body);
}