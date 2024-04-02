#ifndef BREAKOUT_COMPONENTS_H
#define BREAKOUT_COMPONENTS_H


#include <memory>
#include <SFML/Graphics.hpp>
#include "Utilities.h"
#include "Animation.h"
#include <bitset>


struct Component
{
    bool		has{ false };
    Component() = default;
};


struct CAnimation : public Component {
    Animation   animation;

    CAnimation() = default;
    CAnimation(const Animation& a) : animation(a) {}

};

struct CSprite : public Component {
    sf::Sprite sprite;

    CSprite() = default;

    CSprite(const sf::Texture &t)
            : sprite(t) {
        centerOrigin(sprite);
    }

    CSprite(const sf::Texture &t, sf::IntRect r)
            : sprite(t, r) {
        centerOrigin(sprite);
    }
};


struct CTransform : public Component
{

    sf::Transformable  tfm;
    sf::Vector2f	pos			{ 0.f, 0.f };
    sf::Vector2f	prevPos		{ 0.f, 0.f };
    sf::Vector2f	vel			{ 0.f, 0.f };
    sf::Vector2f	scale		{ 0.f, 0.f };

    float           angVel{ 0 };
    float	        angle{ 0.f };

    CTransform() = default;
    CTransform(const sf::Vector2f& p) : pos(p)  {}
    CTransform(const sf::Vector2f& p, const sf::Vector2f& v, const sf::Vector2f& sc)
            : pos(p), prevPos(p),  vel(v), scale(sc){}
};



struct CBoundingBox : public Component
{
    sf::Vector2f size{0.f, 0.f};
    sf::Vector2f pos{ 0.f, 0.f };
    sf::Vector2f halfSize{ 0.f, 0.f };

    CBoundingBox() = default;
    CBoundingBox(const sf::Vector2f& s, const sf::Vector2f&p) : size(s), pos(p), halfSize(0.5f * s)
    {}
};

struct CHitBox : public Component
{
    sf::Vector2f size{ 0.f, 0.f };
    sf::Vector2f pos{ 0.f, 0.f };
    sf::Vector2f halfSize{ 0.f, 0.f };

    CHitBox() = default;
    CHitBox(const sf::Vector2f& s, const sf::Vector2f& p) : size(s), pos(p), halfSize(0.5f * s)
    {}
};

struct CHurtBox : public Component
{
    sf::Vector2f size{ 0.f, 0.f };
    sf::Vector2f pos{ 0.f, 0.f };
    sf::Vector2f halfSize{ 0.f, 0.f };

    CHurtBox() = default;
    CHurtBox(const sf::Vector2f& s, const sf::Vector2f& p) : size(s), pos(p), halfSize(0.5f * s)
    {}
};

struct CState : public Component {
    std::string state{"none"};

    CState() = default;
    CState(const std::string& s) : state(s){}

};

struct CSide : public Component {
    std::string side{ "none" };

    CSide() = default;
    CSide(const std::string& s) : side(s) {}

};


struct CInput : public Component
{
    bool UP{ false };
    bool LEFT{ false };
    bool RIGHT{ false };
    bool DOWN{ false };

    bool UP2{ false };
    bool LEFT2{ false };
    bool RIGHT2{ false };
    bool DOWN2{ false };

    CInput() = default;
};


#endif //BREAKOUT_COMPONENTS_H
