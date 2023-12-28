#include "EntityManager.h"

EntityManager::EntityManager(){};

void EntityManager::update()
{
  // TODO: add entities from m_entitiesToAdd the proper location(s)
  //  - add them to the vector of all entities
  //  - add them to the vector inside the map, with the tag as a key
  for (auto e : m_entitiesToAdd)
  {
    m_entities.push_back(e);
  }

  m_entitiesToAdd.clear();
  //  remove dead entities from the vector of all entities
  removeDeadEntities(m_entities);

  // remove dead entities from each vector in the entity map
  // c++17 way or iterating through [key,value] pairs in a map

  for (auto &[tag, entityVec] : m_entityMap)
  {
    removeDeadEntities(entityVec);
  }
}

const EntityVec &EntityManager::getEntities()
{
  return m_entities;
}

void EntityManager::destroy()
{
  m_alive = false;
};

const std::string &tag()
{
  return m_tag;
};

void EntityManager::removeDeadEntities(EntityVec &vec)
{
  // Todo: remoe all dead entities from the input
  // vector this is called by the update() function
  for (auto e : vec)
  {
    if (!e->isActive())
    {
      // remove from vec using std::remove_if
    }
  }
};

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
  auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

  m_entitiesToAdd.push_back(entity);
  return entity;
}

const EntityVec &EntityManager::getEntities(const std::string &tag)
{
  // Todo: this is incorrect, return the correct vector from the map
  // return m_entities;
  return m_entityMap[tag];
}
