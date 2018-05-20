//
// Created by dawid on 27.04.18.
//

#include "Application.h"
#include "Vehicle.h"
#include "GroundChain.h"
#include "GroundFactory.h"

void Application::run() {

    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Vehicle Evolution");
    sf::View view;
    window.setFramerateLimit(60);



    b2Vec2 gravity(0.f, 9.8f);
    b2World world(gravity);

    //CreateGround(world, 0.f, 500.f);


    std::vector<std::pair<float, float>> vertices;
    vertices.push_back(std::make_pair(0,3));
    vertices.push_back(std::make_pair(-1.1,1));
    vertices.push_back(std::make_pair(-2,-2));
    vertices.push_back(std::make_pair(2,-2));
    vertices.push_back(std::make_pair(1,0));
    vertices.push_back(std::make_pair(2,2));


    Vehicle car(vertices, 1.0f, 1.0f, 200, -300, &world);

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

    GroundChain ground = GroundFactory::getInstance(&world).createGround();

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            switch (event.type) {
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
            CreateBox(world, view.getCenter().x+MouseX-view.getSize().x/2, view.getCenter().y+MouseY-view.getSize().y/2);
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

        window.draw(ground.getShapes());
        car.updateShape();
        view.setCenter(car.getChassisShape().getPosition());
        //view.setCenter(0, 0);
        window.setView(view);
        window.draw(car.getChassisShape());
        window.draw(car.getLeftWheelShape());
        window.draw(car.getRightWheelShape());

        //window.draw(rs);
        window.display();
    }
}

void Application::CreateGround(b2World &World, float X, float Y) {
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
    BodyDef.type = b2_staticBody;
    b2Body *Body = World.CreateBody(&BodyDef);

    //b2PolygonShape Shape;
    //Shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE);
    b2EdgeShape Shape;
    Shape.Set(b2Vec2((X-400)/SCALE,(Y-400)/SCALE),b2Vec2((400+X)/SCALE,(Y-400)/SCALE));
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