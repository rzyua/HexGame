class AnimatedSprite : public sf::Sprite
{
private:
    unsigned int s_frames;
    unsigned int s_states;
    double s_currentFrame;
    unsigned int s_currentState;
    sf::Vector2u s_frameSize;
    sf::Time s_frameLength;

    void updateRect();

public:
    void setTexture(sf::Texture &texture, sf::Vector2u frameSize, sf::Time frameLength, bool resetRect=false);
    void update(sf::Time elapsed);
    void setFrame(unsigned int frame);
    void setState(unsigned int state);
};
