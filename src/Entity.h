#ifndef ENTITY_H
#define ENTITY_H

#include "Components.h"

#include <memory>
#include <string>

class Entity
{
  friend class EntityManager;

  bool m_active = true;
  const size_t m_id = 0;
  std::string m_tag = "default";
  

  //contructor and destructo
  Entity (const size_t id, const std::string& tag);


public:
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CShape> cShape;
  std::shared_ptr<CInput> cInput;
  std::shared_ptr<CScore> cScore;
  std::shared_ptr<CLifespan> cLifespan;

  //private member access functions

  bool isActive() const;
  const std::string & tag();
  const size_t &id() const;

  void destroy();
};

#endif
