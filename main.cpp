#include <SFML\Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include "level.hpp"
#include "entities.hpp"
#include "exceptions.hpp"

int main()
{
    srand(time(NULL));
    sf::Clock clock;

    const int tiles[] = { 0, 1,  2,  3,  4,  5,  6,  7,
                          8, 9, 10, 11, 12, 13, 14, 15,
                          0, 1,  2,  3,  4,  5,  6,  7,
                          8, 9, 10, 11, 12, 13, 14, 15,
                          0, 1,  2,  3,  4,  5,  6,  7,
                          8, 9, 10, 11, 12, 13, 14, 15,
                          0, 1,  2,  3,  4,  5,  6,  7,
                          8, 9, 10, 11, 12, 13, 14, 15,
                        };

    HexMap level("textures\\hexmap.png");
    level.load(tiles, 8, 8);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hex test", sf::Style::Titlebar + sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    //AnimatedSprite test
    sf::Texture anim;
    anim.loadFromFile("textures\\animtest.png");
    AnimatedSprite animSprite;
    animSprite.setTexture(anim, sf::Vector2u(64,64), 20);
    animSprite.setScale(4,4);

    while(window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                break;

            default:
                break;
            }
        }

        level.updateHex(sf::Vector2u(rand()%8,rand()%8), rand()%16);

        window.clear();
        window.draw(level);
        animSprite.update(elapsed);
        window.draw(animSprite);
        window.display();
    }
}
