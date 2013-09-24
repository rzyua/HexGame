#include <SFML\Graphics.hpp>
#include <iostream>
#include <cmath>
#include "entities.hpp"
#include "exceptions.cpp"

void AnimatedSprite::setTexture(sf::Texture &texture, sf::Vector2u frameSize, sf::Time frameLength, bool resetRect)
{
    //call sf::Sprite setTexture method
    sf::Sprite::setTexture(texture, resetRect);

    s_frameSize = frameSize;
    s_frameLength = frameLength;

    //calculate number of frames
    s_frames = texture.getSize().x / frameSize.x;

    //calculate number of states
    s_states = texture.getSize().y / frameSize.y;

    //set initial frame
    s_currentFrame = 0;
    s_currentState = 0;
    setFrame(s_currentFrame);
}

void AnimatedSprite::update(sf::Time elapsed)
{
    //update current frame number
    s_currentFrame += elapsed.asSeconds() / s_frameLength.asSeconds();

    //make sure frame number is not out of bounds
    if(s_currentFrame >= (double)s_frames)
        s_currentFrame -= (double)s_frames;

    std::cout<<"Frame: "<<s_currentFrame+1<<"/"<<s_frames<<std::endl;
    std::cout<<"State: "<<s_currentState+1<<"/"<<s_states<<std::endl;

    //set the texture rect
    updateRect();
}

void AnimatedSprite::setFrame(unsigned int frame)
{
    if(frame >= s_frames || frame < 0)
        throw FrameOutOfRangeException();

    s_currentFrame = (double)frame;

    updateRect();
}

void AnimatedSprite::updateRect()
{
    setTextureRect(sf::IntRect(floor(s_currentFrame) * s_frameSize.x, s_currentState*s_frameSize.y, s_frameSize.x, s_frameSize.y));
}

void AnimatedSprite::setState(unsigned int state)
{
    if (state >= s_states)
        throw StateOutOfRangeException();

    s_currentState = state;
}