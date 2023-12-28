#include <SFML/Graphics.hpp>
#include <iostream>
// #include "Game.h"
#include "Vec2.h"

int main()
{
  // Game g("../config_game")
  //   g.run();

  Vec2 a(1, 1);
  Vec2 b(2, 3);
  Vec2 c(1, 1);

  a.print();
  b.print();

  if (a == c)
  {
    std::cout << true << std::endl;
  }
  else
  {
    std::cout << false << std::endl;
  }

  (a + b).print();

  (a - b).print();

  std::cout << a.length() << std::endl;
  std::cout << b.length() << std::endl;
  a.normalize();
  b.normalize();
  b.print();
}

// #include <algorithm>
// #include <cstddef>
// #include <map>
// #include <memory>
// #include <vector>
//
// typedef std::vector<std::shared_ptr<Entity>> EntityVec;
// typedef std::map<std::string, EntityVec> EntityMap;
//
// class EntityManager
// {
//   EntityVec m_entities;
//   EntityVec m_toAdd;
//   EntityVec m_entityMap;
//   size_t m_totalEntities = 0;
//
// public:
//   EntityManager();
//   void update();
//   std::shared_ptr<Entity> addEntity(const std::string &tag);
//   EntityVec &getEntities();
//   EntityVec &getEntities(const std::string &tag);
//   void destroy() {m_alive = false;};
//   const std::string& tag() {return m_tag;};
//
//
//
//
//   std::shared_ptr<Entity> EntityManager::addEntity(tag)
//   {
//     //give each entity and increaseing integer id
//     auto e = std::make_shared<Entity>(tag, m_totalEntities++);
//     m_toAdd.push_back(e);
//     return e;
//   }
//
//   //called at beginning of each fram by game engine
//   //entities added will now be available to use this frame
//   void EntityManager::update()
//   {
//     for (auto e : m_toAdd)
//     {
//       m_entities.push_back(e);
//       m_entityMap[e->tag()].push_back(e);
//     }
//     for (auto e: m_entitie){
//       // if e is dead, remove it from m_entities
//       std::remove_if()
//       // if e is dead, remove ir from m_entityMap[e->tag()]
//       // tarea
//     }
//     m_toAdd.clear();
//   }
//
//
// // };
