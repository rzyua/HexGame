class HexMap : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Image collisionMap;
    bool initialized;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    const sf::Vector2i m_hexShift = sf::Vector2i(93, 34);
    const sf::Vector2u m_hexSize = sf::Vector2u(128, 70);
    unsigned int m_width;
    unsigned int m_height;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    HexMap(const std::string& tileset);
    bool load(const int* tiles, unsigned int width, unsigned int height);
    void updateHex (sf::Vector2u hexAddress, int tileNumber);
    void setSmooth (bool value);
    sf::Vector2u getHexAddress(sf::Vector2i point) const;
    sf::Vector2u getHexAddress (sf::Vector2f point) const;
    sf::Vector2f getHexCoords (sf::Vector2u hexAddress) const;
    bool selectHexNeighbours (sf::Vector2u hexAddress, std::vector<sf::Vector2u>& neighbours, bool includeCenter = true) const;
};

