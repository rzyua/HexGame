#include <SFML\Graphics.hpp>
#include <iostream>
#include <forward_list>
#include <iterator>
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
    //pixels per second
    const double SCROLL_SPEED = 1000;
    const unsigned int LEVEL_WIDTH = 24;
    const unsigned int LEVEL_HEIGHT = 32;

    //placeholder level generation
    int tiles[LEVEL_WIDTH*LEVEL_HEIGHT];\
    for(int i = 0; i < LEVEL_WIDTH*LEVEL_HEIGHT; ++i)
    {
        tiles[i] = rand()%16;
    }

    HexMap level("textures\\hexmap.png");
    level.load(tiles, LEVEL_WIDTH, LEVEL_HEIGHT);

    //Create the window and view
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hex test", sf::Style::Titlebar + sf::Style::Close);
    //sf::RenderWindow window(sf::VideoMode(1920, 1080), "Hex test", sf::Style::None);0    window.setPosition(sf::Vector2i(o,0));
    window.setVerticalSyncEnabled(true);
    sf::View levelView = window.getDefaultView();

    //AnimatedSprite test
    sf::Texture anim;
    anim.loadFromFile("textures\\animtest.png");
    AnimatedSprite animSprite;
    animSprite.setTexture(anim, sf::Vector2u(64, 64), 16);
    animSprite.setScale(2,2);

    //Highlight
    sf::Texture highlight_texture;
    highlight_texture.loadFromFile("textures\\highlight.png");
    sf::Sprite highlight(highlight_texture);
    std::forward_list<sf::Vector2u> selectedTiles;

    while(window.isOpen())
    {
        //Restart clock and get time elapsed from previous frame
        const sf::Time elapsed = clock.restart();

        //Get mouse position
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        //View offset
        sf::Vector2f offset(0,0);

        //Handle window events
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

            case sf::Event::MouseButtonReleased:
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:
                    selectedTiles.clear();
                    level.selectHexNeighbours(level.getHexAddress(window.mapPixelToCoords(sf::Mouse::getPosition(window))), selectedTiles);
                    break;

                case sf::Mouse::Right:
                    selectedTiles.clear();
                    break;

                default:
                    std::cout<<"Mouse button: "<<event.mouseButton.button<<std::endl;
                }
                break;

            default:
                break;
            }
        }

        //Handle keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            offset.x += SCROLL_SPEED*elapsed.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {

            offset.x -= SCROLL_SPEED*elapsed.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {

            offset.y += SCROLL_SPEED*elapsed.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {

            offset.y -= SCROLL_SPEED*elapsed.asSeconds();
        }

        //Speed up scrolling when shift is pressed
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            offset.x *= 2;
            offset.y *= 2;
        }

        //Move the view
        levelView.move(offset);

        //Highlight field
        highlight.setPosition(level.getHexCoords(level.getHexAddress(window.mapPixelToCoords(sf::Mouse::getPosition(window)))));

        window.clear();
        window.setView(levelView);
        window.draw(level);
        window.draw(highlight);
        for(std::forward_list<sf::Vector2u>::iterator it = selectedTiles.begin(); it != selectedTiles.end(); ++it)
        {
            highlight.setPosition(level.getHexCoords(*it));
            window.draw(highlight);
        }
        //Animation test
        {
            animSprite.update(elapsed);
            animSprite.setPosition(animSprite.getPosition().x + (60.0 * elapsed.asSeconds()), animSprite.getPosition().y + (21.0 * elapsed.asSeconds()));
            window.draw(animSprite);
        }
        window.display();
    }
}
