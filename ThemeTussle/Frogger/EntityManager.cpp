#include "EntityManager.h"
#include "Entity.h"

EntityManager::EntityManager() : m_totalEntities(0)  {}


std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
    // create a new Entity object
    auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

    // store it in entities vector
    m_EntitiesToAdd.push_back(e);
    // return shared pointer to it
    return e;
}


EntityVec &EntityManager::getEntities(const std::string &tag) {
    return m_entityMap[tag];
}


void EntityManager::update() {
    // Remove dead entities
    removeDeadEntities(m_entities);
    for (auto& [_, entityVec] : m_entityMap)
        removeDeadEntities(entityVec);


    // add new entities
    for (auto e : m_EntitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->getTag()].push_back(e);
    }
    m_EntitiesToAdd.clear();
}


EntityVec &EntityManager::getEntities() {
    return m_entities;
}


void EntityManager::removeDeadEntities(EntityVec &v) {
    v.erase(std::remove_if(v.begin(), v.end(), [](auto e) {return!(e->isActive()); }), v.end());
}
