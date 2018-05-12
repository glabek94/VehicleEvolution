//
// Created by dawid on 27.04.18.
//

#include "Application.h"
#include "Vehicle.h"

void Application::run() {

    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML window");
    b2Vec2 gravity(0.f, 9.8f);
    b2World world(gravity);
    window.setFramerateLimit(60);
    CreateGround(world, 0.f, 500.f);
    sf::RectangleShape ground;
    ground.setFillColor(sf::Color::Black);
    ground.setSize(sf::Vector2f(800, 16));
    ground.setPosition(0, 490);


    std::vector<std::pair<float, float>> vertices;
    vertices.push_back(std::make_pair(0,3));
    vertices.push_back(std::make_pair(-1.5,1));
    vertices.push_back(std::make_pair(-2,-2));
    vertices.push_back(std::make_pair(2,-2));
    vertices.push_back(std::make_pair(1,0));
    vertices.push_back(std::make_pair(2,2));


    Vehicle car(vertices, 1.0f, 1.0f, 30, 200, 100, &world);

    sf::Event event;

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int MouseX = sf::Mouse::getPosition(window).x;
            int MouseY = sf::Mouse::getPosition(window).y;
            CreateBox(world, MouseX, MouseY);
        }

        world.Step(1 / 60.f, 8, 3);
        window.clear(sf::Color::White);

        for(auto b : boxes){
            sf::RectangleShape rs;
            rs.setFillColor(sf::Color::Red);
            rs.setOrigin(16,16);
            rs.setSize(sf::Vector2f(32, 32));
            rs.setPosition(SCALE * b->GetPosition().x, SCALE *b->GetPosition().y);
            rs.setOutlineColor(sf::Color::Black);
            rs.setOutlineThickness(1.0f);
            rs.rotate(b->GetAngle() * 180/b2_pi);
            window.draw(rs);
        }

        window.draw(ground);
        car.updateShape();

        window.draw(car.getBodyworkShape());
        window.draw(car.getLeftWheelShape());
        window.draw(car.getRightWheelShape());

        window.display();
    }
}

void Application::CreateGround(b2World &World, float X, float Y) {
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
    BodyDef.type = b2_staticBody;
    b2Body *Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}

void Application::CreateBox(b2World& World, int MouseX, int MouseY)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(MouseX/SCALE, MouseY/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    boxes.push_back(Body);
}