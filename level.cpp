#include <SFML\Graphics.hpp>
#include "level.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include "exceptions.cpp"

void HexMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();
    // apply the tileset texture
    states.texture = &m_tileset;
    // draw the vertex array
    target.draw(m_vertices, states);
}

HexMap::HexMap(const std::string& tileset):
initialized(false)
{
    // load the tileset texture
    m_tileset.loadFromFile(tileset);
    // Load the collision map
    collisionMap.loadFromFile("data\\collisionmap.png");
}

bool HexMap::load(const int* tiles, unsigned int width, unsigned int height)
{
    // Save some data
    m_width = width;
    m_height = height;

    // Helper variable for drawing
    int shift = m_hexShift.y;

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(m_width * m_height * 4);

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < m_width; ++i)
        for (unsigned int j = 0; j < m_height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[i + j * m_width];

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / m_hexSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / m_hexSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * m_width) * 4];

            // define its 4 corners
            shift = 0;
            if (i%2 != 0) shift = m_hexShift.y;
            quad[0].position = sf::Vector2f(i * m_hexShift.x, (j * m_hexShift.y * 2) + shift);
            quad[1].position = sf::Vector2f((i * m_hexShift.x) + m_hexSize.x, (j * m_hexShift.y * 2) + shift);
            quad[2].position = sf::Vector2f((i * m_hexShift.x) + m_hexSize.x, ((j * m_hexShift.y * 2) + m_hexSize.y) + shift);
            quad[3].position = sf::Vector2f(i * m_hexShift.x, ((j * m_hexShift.y * 2) + m_hexSize.y) + shift);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * m_hexSize.x, tv * m_hexSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * m_hexSize.x, tv * m_hexSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * m_hexSize.x, (tv + 1) * m_hexSize.y);
            quad[3].texCoords = sf::Vector2f(tu * m_hexSize.x, (tv + 1) * m_hexSize.y);
        }

    initialized = true;
    return true;
}

void HexMap::updateHex (sf::Vector2u hexAddress, int tileNumber)
{
    if(!initialized)
        throw new HexMapUninitializedException;

    if(hexAddress.x >= m_width || hexAddress.y >= m_height)
        throw new HexAddressOutOfBoundsException;

    // find its position in the tileset texture
    int tu = tileNumber % (m_tileset.getSize().x / m_hexSize.x);
    int tv = tileNumber / (m_tileset.getSize().x / m_hexSize.x);

    //Get a pointer to the desired Quad
    sf::Vertex* quad = &m_vertices[(hexAddress.x + hexAddress.y * m_width) * 4];

    // define its 4 texture coordinates
    quad[0].texCoords = sf::Vector2f(tu * m_hexSize.x, tv * m_hexSize.y);
    quad[1].texCoords = sf::Vector2f((tu + 1) * m_hexSize.x, tv * m_hexSize.y);
    quad[2].texCoords = sf::Vector2f((tu + 1) * m_hexSize.x, (tv + 1) * m_hexSize.y);
    quad[3].texCoords = sf::Vector2f(tu * m_hexSize.x, (tv + 1) * m_hexSize.y);
}

void HexMap::setSmooth (bool value)
{
    m_tileset.setSmooth(value);
}

sf::Vector2u HexMap::getHexAddress(sf::Vector2i point) const
{
    //rough hex position based on collision map coordinates
    sf::Vector2u hexPos;
    //modifier applied to hexPos based on collision map to determine the exact hex address
    sf::Vector2i hexPosDelta;
    //used to get the color from collision map
    sf::Color color;

    //if point is negative, then set it to 0
    if (point.x < 0)
        point.x = 0;

    if (point.y < 0)
        point.y = 0;

    //calculate rough location based on collision map location
    hexPos.x = (point.x / collisionMap.getSize().x) * 2;
    hexPos.y = point.y / collisionMap.getSize().y;

    //check collision map point color
    color = collisionMap.getPixel(point.x % collisionMap.getSize().x,
                                  point.y % collisionMap.getSize().y);

    //determine hexPosDelta based on the collision map color
    if (color == sf::Color(255, 255, 255)) // White
    {
        hexPosDelta.x = 0;
        hexPosDelta.y = 0;
    }
    else if (color == sf::Color(0, 0, 255)) // Blue
    {
        hexPosDelta.x = -1;
        hexPosDelta.y = -1;
    }
    else if (color == sf::Color(255, 255, 0)) // Yellow
    {
        hexPosDelta.x = -1;
        hexPosDelta.y = 0;
    }
    else if (color == sf::Color(255, 0, 0)) // Red
    {
        hexPosDelta.x = 1;
        hexPosDelta.y = -1;
    }
    else if (color == sf::Color(0, 255, 0)) // Green
    {
        hexPosDelta.x =  1;
        hexPosDelta.y = 0;
    }

    //apply delta to the rough position vector and make sure the coords aren't negative
    if((int)hexPos.x + hexPosDelta.x < 0)
        hexPos.x = 0;
    else
        hexPos.x += hexPosDelta.x;

    if((int)hexPos.y + hexPosDelta.y < 0)
        hexPos.y = 0;
    else
        hexPos.y += hexPosDelta.y;

    // Make sure the coords aren't out of map
    if (hexPos.x >= m_width) hexPos.x = m_width - 1;
    if (hexPos.y >= m_height) hexPos.y = m_height - 1;

    return hexPos;
}

sf::Vector2u HexMap::getHexAddress (sf::Vector2f point) const
{
    return getHexAddress(sf::Vector2i((int)point.x, (int)point.y));
}

sf::Vector2f HexMap::getHexCoords (sf::Vector2u hexAddress) const
{
    //if the hex is in an odd column then set the proper vertical shift
    int shift = 0;
    if (hexAddress.x%2 != 0) shift = m_hexShift.y;

    //calculate hex coords based on hexAddress
    return sf::Vector2f(hexAddress.x * m_hexShift.x, (hexAddress.y * m_hexShift.y * 2) + shift);
}

bool HexMap::selectHexNeighbours (sf::Vector2u hexAddress, std::vector<sf::Vector2u>& neighbours) const
{
    //TODO: rewrite in a more generic and unspaghettified code
    if (neighbours.size() > 0)
        return false;
    else
    {
        if ((int)hexAddress.y - 1 >= 0)
            neighbours.push_back(sf::Vector2u(hexAddress.x, hexAddress.y - 1));

        if (hexAddress.y + 1 < m_height)
            neighbours.push_back(sf::Vector2u(hexAddress.x, hexAddress.y + 1));

        if (hexAddress.x % 2 == 0)
        {
            if((int)hexAddress.x - 1 >= 0)
            {
                if ((int)hexAddress.y - 1 >= 0)
                    neighbours.push_back(sf::Vector2u(hexAddress.x - 1, hexAddress.y - 1));

                neighbours.push_back(sf::Vector2u(hexAddress.x - 1, hexAddress.y));
            }

            if ((int)hexAddress.y - 1 >= 0 && hexAddress.x + 1 < m_width)
                neighbours.push_back(sf::Vector2u(hexAddress.x + 1, hexAddress.y - 1));

            if (hexAddress.x + 1 < m_width)
                neighbours.push_back(sf::Vector2u(hexAddress.x + 1, hexAddress.y));
        }
        else
        {
            if((int)hexAddress.x - 1 >= 0)
            {
                neighbours.push_back(sf::Vector2u(hexAddress.x - 1, hexAddress.y));

                if (hexAddress.y + 1 < m_height)
                    neighbours.push_back(sf::Vector2u(hexAddress.x - 1, hexAddress.y + 1));
            }

            if (hexAddress.x + 1 < m_width)
            {
                neighbours.push_back(sf::Vector2u(hexAddress.x + 1, hexAddress.y));

                if (hexAddress.y + 1 < m_height)
                    neighbours.push_back(sf::Vector2u(hexAddress.x + 1, hexAddress.y + 1));
            }
        }

        return true;
    }
}
