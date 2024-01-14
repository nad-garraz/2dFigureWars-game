#ifndef GAME_H
#define GAME_H

#include "Components.h"
#include "Entity.h"
#include "EntityManager.h"

#include "imgui-SFML/imgui-SFML.h"
#include "imgui-SFML/imgui/imgui.h"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

struct WindowConfig
{
  int width, height, rate, fullscreen;
};

struct FontConfig
{
  std::string path;
  int         size, red, green, blue;
};

struct PlayerConfig
{ // SR: Shape Radius, CR: Collision Radius, FR: FillRed,
  // FG: FillGreen, FB: FillBlue, OR: OutlineRed, OG:
  // OutlineGreen, OB: OutlineBlue, OT: OutlineThickness, V:
  // ShapeVertices, S: Speed(magnitude)
  int   SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
  float S;
};
struct EnemyConfig
{ // SMAX, SMIN: Max and Min Speed,
  // VMIN, VMAX: Min and Max vertices,
  // SP: Spawn interval
  // L: Small enemy Lifespan
  int   SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
  float SMIN, SMAX;
};
struct BulletConfig
{
  int   SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
  float S;
};

class Game
{
  sf::RenderWindow m_window;   // the window we will draw to
  EntityManager    m_entities; // Vector of entities to maintain
  sf::Font         m_font;     // the font we will use to draw
  sf::Text         m_text;     // the score text to be drawn to the screen
  WindowConfig     m_windowConfig;
  FontConfig       m_fontConfig;
  PlayerConfig     m_playerConfig;
  EnemyConfig      m_enemyConfig;
  BulletConfig     m_bulletConfig;
  int              m_score              = 0;
  int              m_currentFrame       = 0;
  int              m_lastEnemySpawnTime = 0;
  bool             m_paused             = false; // whether we update game logic
  bool             m_running            = true;  // whether the game is running
  sf::Clock        m_deltaClock;                 // imgui

  std::shared_ptr<Entity> m_player;

  void init(const std::string &config); // initialize the GameState with a config file path
  void setPaused();                     // pause the game

  void sMovement();     // System: Entity position / movement update
  void sUserInput();    // System: User input
  void sLifespan();     // System: Lifespan
  void sRender();       // System: Render / Drawing
  void sEnemySpawner(); // System: Spawns Enemies
  void sCollision();    // System: Collisions
  void sScore();        // System Score
  void sGUI();

  void spawnPlayer();
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity> entity);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mousePos);
  void spawnSpecialWeapon(const Vec2 &target);

public:
  Game(const std::string &config); // constructor, takes in game config
                                   //
  PlayerConfig getPlayerConfig() { return m_playerConfig; };

  void run();
  void config(const std::string &configFilePath);
};

#endif // !GAME_H
