#include "Game.h"

#include "Components.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>

Game::Game(const std::string &config) { init(config); }

void Game::init(const std::string &path)
{
  // TODO: read in config file here use the premade PlayerCOnfi, Enmyconfig,
  // Bulletconfig variables. Set up default window paramenters

  std::ifstream fin(path);
  std::string   category;
  fin >> category;
  fin >> m_windowConfig.width >> m_windowConfig.height >> m_windowConfig.rate >> m_windowConfig.fullscreen;
  fin >> category;
  fin >> m_fontConfig.path >> m_fontConfig.size >> m_fontConfig.red >> m_fontConfig.green >> m_fontConfig.blue;
  fin >> category;
  fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >>
      m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
  fin >> category;
  fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >>
      m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
  fin >> category;
  fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >>
      m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >>
      m_bulletConfig.L;

  // set up default window parameters
  m_window.create(sf::VideoMode(m_windowConfig.width, m_windowConfig.height), "test-sfml",
                  m_windowConfig.fullscreen ? sf::Style::Fullscreen : sf::Style::None);
  m_window.setFramerateLimit(m_windowConfig.rate);

  spawnPlayer();
  spawnEnemy();
}

void Game::run()
{
  // TODO: add pause functionality in here
  // some systems should function while paused (rendering)
  // some systems shoudn't (movement / input)

  while (m_running)
  {
    m_entities.update();

    if (!m_paused)
    {
      sEnemySpawner();
      sMovement();
      sCollision();
      m_currentFrame++;
    }
    sUserInput();
    sRender();
    // Increment the current frame. May need to be moved when pause implemented
  }
};

void Game::setPaused() { m_paused = !m_paused; }

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
  Vec2 lowerRight(m_window.getSize().x, m_window.getSize().y);
  entity->cTransform = std::make_shared<CTransform>(lowerRight * 0.5f, Vec2(0, 0), 0.0f);

  entity->cShape     = std::make_shared<CShape>(
      m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
      sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
  // Add an input component to the player so that we can use inputs
  entity->cInput = std::make_shared<CInput>();

  m_player       = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
  // TODO: make sure the enemy is spawned properly with the m_enemyConfig
  // variables the enemy must be spawned completely within the bounds of the
  // window

  auto  entity = m_entities.addEntity("enemy");
  // Give this entity a Transform so it spawns at (200,200) with velocity (1,1)
  // and angle 0
  float ex           = rand() % m_window.getSize().x;
  float ey           = rand() % m_window.getSize().y;

  entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(1.0f, 1.0f), 90.0f);
  entity->cShape =
      std::make_shared<CShape>(m_enemyConfig.SR, m_enemyConfig.VMIN, sf::Color(100, 100, 100),
                               sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
  // entity->cShape = std::make_shared<CShape>(40.0f, 5, sf::Color(10, 10, 255), sf::Color(255, 255, 255), 4.0f);
  };

void Game::sCollision()
{
  // TODO: implement all proper collisions between entities
  // be sure to use the coliision radius, NOT the shaperadius
  for (auto b : m_entities.getEntities("bullet"))
  {
    for (auto e : m_entities.getEntities("enemy"))
    {
    }
  }
}

void Game::sEnemySpawner()
{
  if (m_currentFrame - m_lastEnemySpawnTime > 300) // 5 seconds
  {
    spawnEnemy();
    m_lastEnemySpawnTime = m_currentFrame;
  }
}

void Game::sRender()
{
  // TODO: change the code below to draw ALL of the entities
  // sample drawing of the player Entity that we have created
  m_window.clear();

  for (auto e : m_entities.getEntities())
  {
    if (e->cShape && e->cTransform)
    {
      // set the position of the shape based on the entity's transform->pos
      e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
      // set the rotation of the shape based on the entity's transform -> angle
      e->cTransform->angle += 1.0f;
      e->cShape->circle.setRotation(e->cTransform->angle);
      // draw the entity's sf::CicleShape
      m_window.draw(e->cShape->circle);
    }
  }
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
    // if (event.type == sf::Event::Closed)
    // {
    // }

    // this event is triggered when a key is pressed
    if (event.type == sf::Event::KeyPressed)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::P:
        setPaused();
        break;
      case sf::Keyboard::Q: // Close window and stop game
        m_window.close();
        m_running = false;
        break;
      case sf::Keyboard::W:
        std::cout << "W Key Pressed\n";
        m_player->cInput->up = true;
        break;
      case sf::Keyboard::S:
        std::cout << "S Key Pressed\n";
        m_player->cInput->down = true;
        break;
      case sf::Keyboard::A:
        std::cout << "A Key Pressed\n";
        m_player->cInput->left = true;
        break;
      case sf::Keyboard::D:
        m_player->cInput->right = true;
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
      case sf::Keyboard::S:
        std::cout << "S Key Released\n";
        m_player->cInput->down = false;
        break;
      case sf::Keyboard::A:
        std::cout << "A Key Released\n";
        m_player->cInput->left = false;
        break;
      case sf::Keyboard::D:
        std::cout << "D Key Released\n";
        m_player->cInput->right = false;
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
  std::shared_ptr<CInput> &input    = m_player->cInput;
  Vec2                    &velocity = m_player->cTransform->velocity;

  // When refreshing set speed to zero
  velocity = {0, 0};

  // implement player movement
  if (input->up) velocity.y = -5;
  if (input->down) velocity.y = 5;
  if (input->left) velocity.x = -5;
  if (input->right) velocity.x = 5;

  // Sample movement speed update
  for (auto &e : m_entities.getEntities())
  {
    e->cTransform->pos += e->cTransform->velocity;
  }
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &target)
{
  auto bullet          = m_entities.addEntity("bullet");

  Vec2 startPos        = m_player->cTransform->pos;
  Vec2 targetDirection = (target - startPos).normalize();

  bullet->cTransform   = std::make_shared<CTransform>(startPos, targetDirection * m_bulletConfig.S, 1);
  bullet->cShape       = std::make_shared<CShape>(
      m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
      sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
}
