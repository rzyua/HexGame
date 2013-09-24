#include <SFML\Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include "level.hpp"

int main()
{
    srand(time(NULL));

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
    //window.setVerticalSyncEnabled(true);

    while(window.isOpen())
    {
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
        window.display();
    }
}
