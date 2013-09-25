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
    std::vector<sf::Vector2u> selectedTiles;

    while(window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        sf::Vector2f offset(0,0);

        //Handle window events
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
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    selectedTiles.clear();
                    level.selectHexNeighbours(level.getHexAddress(window.mapPixelToCoords(sf::Mouse::getPosition(window))), selectedTiles);
                    selectedTiles.push_back(level.getHexAddress(window.mapPixelToCoords(sf::Mouse::getPosition(window))));
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    selectedTiles.clear();
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

        //Move the view
        levelView.move(offset);

        //Highlight field
        highlight.setPosition(level.getHexCoords(level.getHexAddress(window.mapPixelToCoords(sf::Mouse::getPosition(window)))));

        level.updateHex(sf::Vector2u(rand()%LEVEL_WIDTH,rand()%LEVEL_HEIGHT), 7);

        window.clear();
        window.setView(levelView);
        window.draw(level);
        window.draw(highlight);
        for(int i = 0; i < (int)selectedTiles.size(); ++i)
        {
            highlight.setPosition(level.getHexCoords(selectedTiles[i]));
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
