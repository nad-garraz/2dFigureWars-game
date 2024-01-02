#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec>     EntityMap;

class EntityManager
{
  EntityVec m_entities;
  EntityVec m_entitiesToAdd;
  EntityMap m_entityMap;
  size_t    m_totalEntities = 0;

public:
  EntityManager();

  void update();
  void removeDeadEntities(EntityVec &vec);

  std::shared_ptr<Entity> addEntity(const std::string &tag);

  const EntityVec &getEntities();
  const EntityVec &getEntities(const std::string &tag);

  const std::map<std::string, EntityVec> &getEntityMap();
};

#endif // !ENTITY_H
