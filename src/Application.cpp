//
// Created by dawid on 27.04.18.
//

#include "Application.h"

void Application::run() {

    CreateGround(world, 400.f, 500.f);

    sf::Texture GroundTexture;
    sf::Texture BoxTexture;
    GroundTexture.loadFromFile("../../resources/ground.png");
    BoxTexture.loadFromFile("../../resources/box.png");

    sf::Event event;
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            int MouseX = sf::Mouse::getPosition(window).x;
            int MouseY = sf::Mouse::getPosition(window).y;
            CreateBox(world, MouseX, MouseY);
        }
        world.Step(1 / 60.f, 8, 3);

        window.clear(sf::Color::White);
        int BodyCount = 0;

        for (b2Body *BodyIterator = world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext()) {


            if (BodyIterator->GetType() == b2_dynamicBody) {
                sf::Sprite Sprite;
                Sprite.setTexture(BoxTexture);
                Sprite.setOrigin(16.f, 16.f);
                Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
                Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(Sprite);
                ++BodyCount;
            } else {
                sf::Sprite GroundSprite;
                GroundSprite.setTexture(GroundTexture);
                GroundSprite.setOrigin(400.f, 8.f);
                GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
                GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
                window.draw(GroundSprite);
            }

        }

        window.display();
    }
}

void Application::CreateBox(b2World &World, int MouseX, int MouseY) {
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body *Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
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