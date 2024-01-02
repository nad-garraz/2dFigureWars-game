#include "EntityManager.h"

EntityManager::EntityManager(){};

void EntityManager::update()
{
  // TODO: add entities from m_entitiesToAdd to the proper location(s):
  //  - add them to the vector of all entities
  //  - add them to the vector inside the map, with the tag as a key
  for (auto e : m_entitiesToAdd)
  {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }

  // This lines prevents from adding entities added in previous iteration.
  m_entitiesToAdd.clear();

  //  Remove dead entities from the vector of all entities
  removeDeadEntities(m_entities);

  // remove dead entities from each vector in the entity map
  for (auto &[tag, entityVec] : m_entityMap)
  {
    removeDeadEntities(entityVec);
  }
}

const EntityVec &EntityManager::getEntities() { return m_entities; }

void EntityManager::removeDeadEntities(EntityVec &vec)
{
  auto newEnd = remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> e) { return !e->isActive(); });
  vec.erase(newEnd, vec.end());
};

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
  auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

  m_entitiesToAdd.push_back(entity);

  return entity;
}

const EntityVec &EntityManager::getEntities(const std::string &tag) { return m_entityMap[tag]; }
