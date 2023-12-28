#include "EntityManager.h"

  EntityManager::EntityManager(){};

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
    // Todo: remoe all dead entities from the inpu
    // vector this is called by the update() function
  }

  std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
  {
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

    m_entitiesToAdd.push_back(entity);
    return entity;
  }

  const EntityVec &EntityManager::getEntities()
  {
    return m_entities;
  }

  const EntityVec &EntityManager::getEntities(const std::string &tag)
  {
    // Todo: this is incorrect, return the correct vector from the map
    return m_entities;
  }
