#include "Game.h"

#include <Components.h>
// #include <fstream>
// #include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

Game::Game(const std::string &config)
{
  init(config);
}

void Game::init(const std::string &path)
{
  // Todo: read in config file here use the premade PlayerCOnfi, Enmyconfig,
  // Bulletconfig variables. Set up default window paramenters

  // std::ifstream fin(path);
  // std::string temp;
  // std::string category;
  //
  // while (fin >> temp)
  // {
  //   if (temp == "Player")
  //   {
  //     fin >> category >> m_playerConfig.SR >> m_playerConfig.CR >>
  //     m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >>
  //         m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >>
  //         m_playerConfig.OT >> m_playerConfig.V >> m_playerConfig.S;
  //   }

  // fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.OR >>
  // m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >>
  //     m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >>
  //     m_enemyConfig.SI >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
  //
  // fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.FR >>
  // m_bulletConfig.FG >> m_bulletConfig.FB >>
  //     m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >>
  //     m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.S >>
  //     m_bulletConfig.L;
  // };

  // set up default window parameters
  //

  m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
  m_window.setFramerateLimit(60);

  spawnPlayer();
}

void Game::run()
{
  // TODO: add pause functionality in here
  // some systems shoud function while paused (rendering)
  // some systems shoudn't (movement / input)
  while (m_running)
  {
    m_entities.update();

    if (!m_paused)
    {
      sEnemySpawner();
      sMovement();
      sCollision();
      sUserInput();
    }

    sRender();

    // increment the current frame
    //  may need to be moved when pause implemented
    m_currentFrame++;
  }
};

void Game::setPaused(bool paused)
{
  // TODO
}

// respawn the player in the middle of the screen
//

void Game::spawnPlayer()
{
  // TODO: FInish adding all properties of the player with the correct values
  // from the config

  // We create every entity by calling EntityManager.addEntity(tag)
  //  This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing

  auto entity = m_entities.addEntity("player");

  // Give this entity a Transform so it spawns at (200,200) with velocity (1,1)
  // and angle 0
  entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);

  // The entity's shape will have radius 32,  sides, dark grey fill, and red
  // outline of thickness 4
  entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

  // Add an input component to the player so that we can use inputs
  entity->cInput = std::make_shared<CInput>();

  // SInce we want this Entity to be our player, set our Game's player variable
  // to be this Entity
  //  This goes slightly against the EntityManager paradigm, but we use the
  //  player so much it's worth it
  m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
  // TODO: make sure the enemy is spawned properly with the m_enemyConfig
  // variables the enemy must be spawned completely within the bounds of the
  // window

  auto entity = m_entities.addEntity("enemy");
  // Giv this entity a Transform so it spawns at (200,200) with velocity (1,1)
  // and angle 0
  float ex           = rand() % m_window.getSize().x;
  float ey           = rand() % m_window.getSize().y;

  entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(1.0f, 1.0f), 0.0f);

  entity->cShape     = std::make_shared<CShape>(32.0f, 3, sf::Color(10, 10, 255), sf::Color(255, 255, 255), 4.0f);
};

void Game::sCollision()
{
  // TODO: implement all proper collisions between entities
  // be sure to use the coliision radius, NOT the shaperadius
  for (auto b : m_entities.getEntities("bullet"))
  {
    for (auto e: m_entities.getEntities("enemy"))
    {

    }
  }
}

void Game::sEnemySpawner()
{
  // Todo: code which implements enemy spawing should go here
  //
  //(use m_currentFrame - m_lastEnemySpawnTieme) to determine
  // how long it has been since the lat enemy spawned
  spawnEnemy();
}

void Game::sRender()
{
  // TODO: change the code below to draw ALL of the entities
  // sample drawing of the player Entity that we have created
  m_window.clear();

  // set the position of the shape vased on the entity's transform->pos
  m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

  // set the rotation of the shape based on the entity's transform -> angle
  m_player->cTransform->angle += 1.0f;
  m_player->cShape->circle.setRotation(m_player->cTransform->angle);

  // draw the enitty's sf::CicleShape
  m_window.draw(m_player->cShape->circle);

  m_window.display();
}

void Game::sUserInput()
{

  // TODO: handle user input here
  // note that you should only be seting the player's input component variables
  // here you should not implement the player's movement logic here the movement
  // system will read the variables you set in this function
  //
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    // this event triggers when the window is closed
    if (event.type == sf::Event::Closed)
    {
      m_running = false;
    }

    // this event is triggered when a key is pressed
    if (event.type == sf::Event::KeyPressed)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::W:
        std::cout << "W Key Pressed\n";
        m_player->cInput->up = true;
        break;
      default:
        break;
      }
    }

    // this event is triggered when a key is released
    if (event.type == sf::Event::KeyReleased)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::W:
        std::cout << "W Key Released\n";
        m_player->cInput->up = false;
        break;
      default:
        break;
      }
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
        // call spawnBullet here
        spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
      }

      if (event.mouseButton.button == sf::Mouse::Right)
      {
        std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
        // call spawnSpecialWeapon here
      }
    }
  }
}

void Game::sMovement()
{
  // TODO: implement all entity movement in this function
  // you should read the m_player->cInput component to determine if the player
  // is moving
  m_player->cTransform->velocity = {0, 0};
  // implement player movement
  if (m_player->cInput->up)
  {
    m_player->cTransform->velocity.y = -5;
  }

  // Sample movement speed update
  m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
  m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &target)
{
  // TODO: inplement the spawning of a bullet which travels toward target
  //- bullet speed is given as a scalar speed
  //- you must set the velocity by using formula in notes

  auto bullet        = m_entities.addEntity("bullet");
  bullet->cTransform = std::make_shared<CTransform>(target, Vec2(0, 0), 0);
  bullet->cShape     = std::make_shared<CShape>(10, 8, sf::Color(255, 255, 255), sf::Color(255, 0, 0), 2);
}
