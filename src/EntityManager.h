#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
  EntityVec m_entities;
  EntityVec m_entitiesToAdd;
  EntityMap m_entityMap;
  size_t m_totalEntities = 0;

  bool m_alive;

public:
  EntityManager();

  void update();

  std::shared_ptr<Entity> addEntity(const std::string &tag);

  const EntityVec &getEntities();

  const EntityVec &getEntities(const std::string &tag);

  void removeDeadEntities(EntityVec &vec);
  void destroy();
};

#endif // !ENTITY_H
