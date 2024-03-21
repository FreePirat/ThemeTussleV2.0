#include "Physics.h"
#include <cmath>

sf::Vector2f Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    sf::Vector2f overlap(0.f, 0.f);
    if (!a->hasComponent<CBoundingBox>() or !b->hasComponent<CBoundingBox>())
        return overlap;

    auto atx = a->getComponent<CTransform>();
    auto abb = a->getComponent<CBoundingBox>();
    auto btx = b->getComponent<CTransform>();
    auto bbb = b->getComponent<CBoundingBox>();


    if (abb.has && bbb.has)
    {
        float dx = std::abs(atx.pos.x - btx.pos.x);
        float dy = std::abs(atx.pos.y - btx.pos.y);
        overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, abb.halfSize.y + bbb.halfSize.y - dy);
    }
    return overlap;
}

sf::Vector2f Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    sf::Vector2f overlap(0.f, 0.f);
    if (!a->hasComponent<CBoundingBox>() or !b->hasComponent<CBoundingBox>())
        return overlap;

    auto atx = a->getComponent<CTransform>();
    auto abb = a->getComponent<CBoundingBox>();
    auto btx = b->getComponent<CTransform>();
    auto bbb = b->getComponent<CBoundingBox>();

    if (abb.has && bbb.has)
    {
        float dx = std::abs(atx.prevPos.x - btx.prevPos.x);
        float dy = std::abs(atx.prevPos.y - btx.prevPos.y);
        overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, abb.halfSize.y + bbb.halfSize.y - dy);
    }
    return overlap;
}



sf::Vector2f Physics::getHitOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    sf::Vector2f hitOverlap(0.f, 0.f);
    if (!a->hasComponent<CHitBox>() or !b->hasComponent<CHurtBox>())
        return hitOverlap;

    auto atx = a->getComponent<CHitBox>().pos;
    auto ahb = a->getComponent<CHitBox>();
    auto btx = b->getComponent<CHurtBox>().pos;
    auto bhr = b->getComponent<CHurtBox>();


    if (ahb.has && bhr.has)
    {
        float dx = std::abs(atx.x - btx.x);
        float dy = std::abs(atx.y - btx.y);
        hitOverlap = sf::Vector2f(ahb.halfSize.x + bhr.halfSize.x - dx, ahb.halfSize.y + bhr.halfSize.y - dy);
    }
    return hitOverlap;
}