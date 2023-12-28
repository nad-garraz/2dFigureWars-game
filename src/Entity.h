#ifndef ENTITY_H
#define ENTITY_H

#include "Components.h"
#include <memory>
#include <string>

class Entity
{

  friend class EntityMandager;

  bool m_active = true;
  size_t m_id = 0;
  std::string m_tag = "default";

  //contructor and destructo
  Entity (const size_t id, const std::string& tag);


public:
  std::<CTransform> cTransform;
  std::<CShape> cShape;
  std::<CInput> cInput;
  std::<CScore> cScore;
  std::<CLifespan> cLifespan;

  //private member access functions
  //
  bool isActive() const;
  const std::string & tag() const;
  const size_t id() const;
  void destroy();
}

#endif
