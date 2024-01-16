#include "Game.h"
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
  fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >>
      m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >>
      m_enemyConfig.SI;
  fin >> category;
  fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >>
      m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >>
      m_bulletConfig.L;

  // set up default window parameters
  m_window.create(sf::VideoMode(m_windowConfig.width, m_windowConfig.height), "test-sfml",
                  m_windowConfig.fullscreen ? sf::Style::Fullscreen : sf::Style::None);
  m_window.setFramerateLimit(m_windowConfig.rate);

  // Spawn the first player
  spawnPlayer();
  // Spawn the  first enemy
  spawnEnemy();

  // seed random generator
  std::srand(time(NULL));

  ImGui::SFML::Init(m_window);
}

void Game::run()
{
  // TODO: add pause functionality in here
  // some systems should function while paused (rendering)
  // some systems shoudn't (movement / input)

  while (m_running)
  {
    m_entities.update();

    // required update call to imgui
    ImGui::SFML::Update(m_window, m_deltaClock.restart());

    sUserInput();
    if (!m_paused)
    {
      if (m_toggleMovement) sMovement();
      if (m_toggleCollision) sCollision();
      if (m_toggleEnemySpawner) sEnemySpawner();
      if (m_toggleLifespan) sLifespan();
      if (m_toggleScore) sScore();
      m_currentFrame++;
    }
    sGUI();
    sRender();
  }
};

void Game::setPaused() { m_paused = !m_paused; }

void Game::spawnPlayer()
{
  auto playerEntity = m_entities.addEntity("player");

  Vec2 middleOfWindow(m_window.getSize().x * 0.5, m_window.getSize().y * 0.5);

  playerEntity->cTransform = std::make_shared<CTransform>(middleOfWindow, Vec2(0, 0), 0.0f);

  sf::Color fillColor      = sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB);
  sf::Color outlineColor   = sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB);

  playerEntity->cShape =
      std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, fillColor, outlineColor, m_playerConfig.OT);

  // Add collisions
  playerEntity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

  // Add an input component to the player so that we can use inputs
  playerEntity->cInput = std::make_shared<CInput>();

  // Add the score to the player
  sf::Color fontColor  = sf::Color(m_fontConfig.red, m_fontConfig.green, m_fontConfig.blue);
  playerEntity->cScore = std::make_shared<CScore>(0, m_fontConfig.path, m_fontConfig.size, fontColor);

  m_player             = playerEntity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
  // TODO: make sure the enemy is spawned properly with the m_enemyConfig
  // variables the enemy must be spawned completely within the bounds of the
  // window

  auto enemy = m_entities.addEntity("enemy");

  // Random start Positiion inside screen
  Vec2  w(m_windowConfig.width, m_windowConfig.height);
  Vec2  radius(m_enemyConfig.SR, m_enemyConfig.SR);
  float deltaY        = 1 + (w.y - radius.y) - radius.y; // 1 + MAX - MIN
  float deltaX        = 1 + (w.x - radius.x) - radius.x; // 1 + MAX - MIN
  float ex            = rand() % (int)deltaX + radius.x;
  float ey            = rand() % (int)deltaY + radius.y;
  Vec2  startPosition = Vec2(ex, ey);

  // Random velocity
  float deltaS = 1 + m_enemyConfig.SMAX - m_enemyConfig.SMIN; // 1 + MAX - MIN
  float speed  = rand() % (int)deltaS + m_enemyConfig.SMIN;
  int   signX  = rand() % 2 == 0 ? 1 : -1;
  int   signY  = rand() % 2 == 0 ? 1 : -1;
  Vec2  vel    = Vec2(ex * signX, ey * signY).normalize() * speed;

  // Random vertices
  float deltaV = 1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN;
  float V      = rand() % (int)deltaV + m_enemyConfig.VMIN;

  // Random FillColors
  int       deltaColor = 1 + 255 - 0; // 1 + MAX - MIN
  sf::Color fillColor(rand() % deltaColor, rand() % deltaColor, rand() % deltaColor);

  sf::Color outlineColor(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB);

  enemy->cTransform = std::make_shared<CTransform>(startPosition, vel, 0.0f);
  enemy->cShape     = std::make_shared<CShape>(m_enemyConfig.SR, V, fillColor, outlineColor, m_enemyConfig.OT);
  enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
};

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &target)
{
  auto bullet          = m_entities.addEntity("bullet");

  Vec2 startPos        = m_player->cTransform->pos;
  Vec2 targetDirection = (target - startPos).normalize(); // bullet velocity direction

  sf::Color fillColor(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB);
  sf::Color outlineColor(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB);
  bullet->cTransform = std::make_shared<CTransform>(startPos, targetDirection * m_bulletConfig.S, 0);
  bullet->cShape     = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, fillColor, outlineColor, m_bulletConfig.OT);
  bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
  bullet->cLifespan  = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
  // Todo: spawn small enemies at the location of the input enemy e

  // when we create the smaller enemy, we have to read the values of the
  // original enemy
  //  - spawn a number of small enemies equal to the vertices of the original
  //  enemy
  //  - set each small enemy to the same color as the original, half the size
  //  - small enemies are worth double points of the original enemy

  static int      vertices, radius;
  sf::CircleShape circ(e->cShape->circle);
  vertices = circ.getPointCount();
  radius   = circ.getRadius() / 2;
  static Vec2     origin, newOrigin;
  const sf::Color color           = circ.getFillColor();
  const sf::Color outlineColor    = circ.getOutlineColor();
  const float     outlineTickness = circ.getOutlineThickness();

  origin                          = {circ.getPosition().x, circ.getPosition().y};

  for (int v = 0; v < vertices; v++)
  {
    newOrigin = Vec2(std::sin(2 * M_PI / vertices * v), std::cos(2 * M_PI / vertices * v));
    Vec2 vel  = newOrigin.normalize() * 0.8f;

    std::shared_ptr<Entity> smallEnemy;
    smallEnemy             = m_entities.addEntity("smallEnemy");
    smallEnemy->cTransform = std::make_shared<CTransform>(origin, vel, 0.0f);
    smallEnemy->cShape     = std::make_shared<CShape>(radius, vertices, color, outlineColor, outlineTickness);
    smallEnemy->cLifespan  = std::make_shared<CLifespan>(m_enemyConfig.L);
    smallEnemy->cCollision = std::make_shared<CCollision>(radius);
  }
}

void Game::spawnSpecialWeapon(const Vec2 &target)
{
  // The specialWeapon is gonna be a black hole, that pulls the enemys
  // towards it.

  std::shared_ptr<Entity> sW;
  sW             = m_entities.addEntity("specialWeapon");

  Vec2 origin    = target;

  sW->cTransform = std::make_shared<CTransform>(origin, Vec2(0, 0), 0.0f);
  sW->cShape     = std::make_shared<CShape>(10.0f, 50, sf::Color(0, 0, 0), sf::Color(0, 0, 0), 10.0f);
  sW->cLifespan  = std::make_shared<CLifespan>(600);
  sW->cCollision = std::make_shared<CCollision>(30.0f);
}

void Game::sCollision()
{
  // Helper. Check for collision between playerAttacks and enemies
  // If enemy dies, spawns smallEnemies. Collision between bullets and enemies
  // will destroy bullets, but not the specialWeapon.
  auto checkAttackCollision = [&](std::shared_ptr<Entity> &attackType, const std::string &enemyType, int scoreMultiplier)
  {
    int  bulletR = attackType->cCollision->radius;
    Vec2 bulletP(attackType->cShape->circle.getPosition().x, attackType->cShape->circle.getPosition().y);

    for (auto enemy : m_entities.getEntities(enemyType))
    {
      int  enemyR = enemy->cCollision->radius;
      Vec2 enemyP(enemy->cShape->circle.getPosition().x, enemy->cShape->circle.getPosition().y);

      // Entities are spawn in the left top corner, this avoids a glitch
      if (enemyP == Vec2(0, 0)) continue;

      float distBE            = enemyP.dist(bulletP);
      float collisionDistance = (bulletR + enemyR);

      if (distBE <= collisionDistance)
      {
        if (attackType->getTag() == "bullet") attackType->destroy();
        enemy->destroy();
        m_player->cScore->score += scoreMultiplier * enemy->cShape->circle.getPointCount();
        if (enemyType == "enemy") spawnSmallEnemies(enemy);
      }
    }
  };

  // Helper. Establish player limits in the screen. Check for collision with
  // enemies.
  auto checkPlayerCollision = [&](std::shared_ptr<Entity> player, const std::string &enemyType)
  {
    float playerR = player->cCollision->radius;
    Vec2  playerP(player->cShape->circle.getPosition().x, player->cShape->circle.getPosition().y);

    int playerTop    = playerP.y - playerR;
    int playerBottom = playerP.y + playerR;
    int playerLeft   = playerP.x - playerR;
    int playerRight  = playerP.x + playerR;

    Vec2 vel(player->cTransform->velocity);

    if (playerBottom >= m_windowConfig.height && vel.y > 0) player->cTransform->pos.y = m_windowConfig.height - playerR;
    if (playerTop <= 0 && vel.y < 0) player->cTransform->pos.y = 0 + playerR;
    if (playerRight >= m_windowConfig.width && vel.x > 0) player->cTransform->pos.x = m_windowConfig.width - playerR;
    if (playerLeft <= 0 && vel.x < 0) player->cTransform->pos.x = 0 + playerR;

    for (auto enemy : m_entities.getEntities(enemyType))
    {
      int  enemyR = enemy->cCollision->radius;
      Vec2 enemyP(enemy->cShape->circle.getPosition().x, enemy->cShape->circle.getPosition().y);

      if (enemyP == Vec2(0, 0)) continue;

      float distPE            = enemyP.dist(playerP);
      float collisionDistance = (playerR + enemyR);

      if (distPE <= collisionDistance)
      {
        enemy->destroy();
        player->destroy();
        spawnPlayer();
        if (enemy->getTag() == "enemy") spawnSmallEnemies(enemy);
      }
    }
  };

  // bullets and enemies collision
  for (auto bullet : m_entities.getEntities("bullet"))
  {
    checkAttackCollision(bullet, "enemy", 100);
  }

  // bullets and small enemies collision
  for (auto bullet : m_entities.getEntities("bullet"))
  {
    checkAttackCollision(bullet, "smallEnemy", 200);
  }

  // enemies and walls collision
  for (auto &enemy : m_entities.getEntities("enemy"))
  {
    int  enemyR = enemy->cCollision->radius;
    Vec2 enemyP(enemy->cShape->circle.getPosition().x, enemy->cShape->circle.getPosition().y);

    if (enemyP == Vec2(0, 0)) continue;

    float enemyTop    = enemyP.y - enemyR;
    float enemyBottom = enemyP.y + enemyR;
    float enemyLeft   = enemyP.x - enemyR;
    float enemyRight  = enemyP.x + enemyR;

    Vec2 vel(enemy->cTransform->velocity);
    if ((enemyBottom >= m_window.getSize().y && vel.y > 0) || (enemyTop <= 0 && vel.y < 0))
    {
      enemy->cTransform->velocity.y *= -1.0f;
    }
    if ((enemyRight >= m_window.getSize().x && vel.x > 0) || (enemyLeft <= 0 && vel.x < 0))
    {
      enemy->cTransform->velocity.x *= -1.0f;
    }
  }

  // player and enemies collision
  for (auto player : m_entities.getEntities("player"))
  {
    checkPlayerCollision(player, "enemy");
    checkPlayerCollision(player, "smallEnemy");
  }

  // specialWeapon and small enemies collision
  for (auto specialWeapon : m_entities.getEntities("specialWeapon"))
  {
    checkAttackCollision(specialWeapon, "smallEnemy", 5);
    checkAttackCollision(specialWeapon, "enemy", 10);
  }
}

void Game::sEnemySpawner()
{
  // if there are more than 5 entity enemies, then slow down spawn speed
  int enemiesAlive = m_entities.getEntities("enemy").size();
  if (m_currentFrame % 60 == 0 && enemiesAlive <= 5 && m_enemyConfig.SI > 60) m_enemyConfig.SI -= 5;
  else if (m_currentFrame % 60 == 0 && enemiesAlive > 5 && m_enemyConfig.SI < 600)
    m_enemyConfig.SI += 5;
  if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SI)
  {
    spawnEnemy();
    m_lastEnemySpawnTime = m_currentFrame;
  }
}

void Game::sRender()
{
  // TODO: change the code below to draw ALL of the entities
  // sample drawing of the player Entity that we have created

  // Change screen background color when specialWeapon is alive.
  auto sW = m_entities.getEntities("specialWeapon");
  sW.empty() ? m_window.clear(sf::Color(0, 0, 0, 255)) : m_window.clear(sf::Color(200, 200, 200, 255));

  for (auto e : m_entities.getEntities())
  {
    if (e->cScore)
    {
      m_window.draw(e->cScore->Score);
    }
    if (e->cShape && e->cTransform)
    {
      // set the position of the shape based on the entity's transform->pos
      e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

      // set the rotation of the shape based on the entity's transform ->
      // angle
      e->cTransform->angle += 1.0f;
      e->cShape->circle.setRotation(e->cTransform->angle);

      // draw the entity's sf::CicleShape
      m_window.draw(e->cShape->circle);
    }
  }

  // draw the ui last
  ImGui::SFML::Render(m_window);

  m_window.display();
}

void Game::sUserInput()
{

  sf::Event event;
  while (m_window.pollEvent(event))
  {
    ImGui::SFML::ProcessEvent(m_window, event);
    // this event is triggered when a key is pressed
    if (event.type == sf::Event::KeyPressed)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::P: // Pause the game
        setPaused();
        break;
      case sf::Keyboard::Q: // Close window and stop game
        m_window.close();
        m_running = false;
        break;
      case sf::Keyboard::W:
        m_player->cInput->up = true;
        break;
      case sf::Keyboard::S:
        m_player->cInput->down = true;
        break;
      case sf::Keyboard::A:
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
        m_player->cInput->up = false;
        break;
      case sf::Keyboard::S:
        m_player->cInput->down = false;
        break;
      case sf::Keyboard::A:
        m_player->cInput->left = false;
        break;
      case sf::Keyboard::D:
        m_player->cInput->right = false;
        break;
      default:
        break;
      }
    }

    // Separates the ImGui window from the input of the game
    if (!ImGui::GetIO().WantCaptureMouse)
    {
      if (event.type == sf::Event::MouseButtonPressed)
      {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
          // Condition needed, or else bullet is show when game paused and left
          // mouseButton clicked in the ImGui window.
          if (!m_paused) spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
        }

        if (event.mouseButton.button == sf::Mouse::Right)
        {
          // If the vector as a specialWeapon, I can not spawn a new one. One at
          // a time.
          if (m_entities.getEntities("specialWeapon").empty()) spawnSpecialWeapon(Vec2(event.mouseButton.x, event.mouseButton.y));
        }
      }
    }
  }
}

void Game::sMovement()
{
  std::shared_ptr<CInput> &input = m_player->cInput;

  // When refreshing set speed to zero
  m_player->cTransform->velocity = {0, 0};

  // implement player movement
  if (input->up) m_player->cTransform->velocity.y = -1;
  if (input->down) m_player->cTransform->velocity.y = 1;
  if (input->left) m_player->cTransform->velocity.x = -1;
  if (input->right) m_player->cTransform->velocity.x = 1;

  // Update velocity
  m_player->cTransform->velocity = m_player->cTransform->velocity.normalize() * m_playerConfig.S;

  // specialWeapon
  auto specialWeaponMovement = [&](std::string enemyType, int gravityPull)
  {
    auto SW = m_entities.getEntities("specialWeapon");
    if (!SW.empty())
    {
      auto sW = SW[0];
      for (auto &e : m_entities.getEntities(enemyType))
      {
        Vec2  accDir             = (sW->cTransform->pos - e->cTransform->pos).normalize();
        float d                  = (sW->cTransform->pos - e->cTransform->pos).length();
        Vec2  a                  = accDir * gravityPull / (d * d);
        e->cTransform->velocity += a;
      }
    }
  };
  specialWeaponMovement("enemy", 10000);
  specialWeaponMovement("smallEnemy", 15000);

  // Sample movement speed update
  for (auto &e : m_entities.getEntities())
  {
    e->cTransform->pos += e->cTransform->velocity;
  }
}

void Game::sLifespan()
{
  // TODO: implement all lifespan functionality
  //
  //  if entity has > 0 remaining lifespan, subtract 1
  //  if it has lifespan and is alive scale its alpha channel properly
  //  if it has lifespan and its time is up destroy the entity
  for (auto &e : m_entities.getEntities("bullet"))
  {
    if (e->cLifespan) // if entity has no lifespan component, skip it
    {
      e->cLifespan->remaining--;
      float alpha = (float)e->cLifespan->remaining / e->cLifespan->total * 255;
      e->cShape->circle.setFillColor(sf::Color(255, 255, 255, alpha));
      e->cShape->circle.setOutlineColor(sf::Color(255, 255, 255, alpha));

      if (e->cLifespan->remaining <= 0) e->destroy();
    }
  }
  for (auto &e : m_entities.getEntities("smallEnemy"))
  {
    if (e->cLifespan) // if entity has no lifespan component, skip it
    {
      sf::Color fillColor = e->cShape->circle.getFillColor(), outLineColor = e->cShape->circle.getOutlineColor();

      e->cLifespan->remaining--;

      int alpha = (float)e->cLifespan->remaining / e->cLifespan->total * 255;
      e->cShape->circle.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, alpha));
      e->cShape->circle.setOutlineColor(sf::Color(outLineColor.r, outLineColor.g, outLineColor.b, alpha));

      if (e->cLifespan->remaining <= 0) e->destroy();
    }
  }

  for (auto sW : m_entities.getEntities("specialWeapon"))
  {
    sW->cLifespan->remaining--;
    if (sW->cLifespan->remaining == 0) sW->destroy();
  }
}

void Game::sScore()
{
  // TODO: put the score in the TOP-LEFT corner
  static std::string newScore;
  newScore = std::to_string(m_player->cScore->score);
  m_player->cScore->Score.setString("Score: " + newScore);
}

void Game::sGUI()
{
  static float f1  = m_player->cShape->circle.getRadius();
  static float eSI = m_enemyConfig.SI;

  ImGui::Begin("Window title");
  ImGui::Text("Window Text!");
  ImGui::Checkbox("Pause Game", &m_paused);
  ImGui::SameLine();
  if (ImGui::Button("Pause")) m_paused = !m_paused;

  ImGui::Checkbox("Toggle EnemySpawner", &m_toggleEnemySpawner);
  ImGui::Checkbox("Toggle Score", &m_toggleScore);
  ImGui::Checkbox("Toggle Collision", &m_toggleCollision);
  ImGui::Checkbox("Toggle Movement", &m_toggleMovement);
  ImGui::Checkbox("Toggle UserInput", &m_toggleUserInput);
  ImGui::Checkbox("Toggle Lifespan", &m_toggleLifespan);

  m_enemyConfig.SI = eSI;
  ImGui::DragFloat("Enemy Spawn Interval", &eSI);

  ImGui::BeginChild("Scrolling");
  ImGui::BeginTable("table1", 4);
  ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
  ImGui::TableSetupColumn("ID");
  ImGui::TableSetupColumn("Vertices");
  ImGui::TableSetupColumn("TAG");
  ImGui::TableSetupColumn("Speed");
  ImGui::TableHeadersRow();
  for (auto e : m_entities.getEntities())
  {
    sf::Color fillColor = e->cShape->circle.getFillColor();
    float r = (float)fillColor.r / 255, g = (float)fillColor.g / 255, b = (float)fillColor.b / 255, a = (float)fillColor.a / 255;
    ImGui::TableNextColumn();
    ImGui::TextColored(ImVec4(r, g, b, a), std::to_string(e->getId()).c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", std::to_string(e->cShape->circle.getPointCount()).c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", e->getTag().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%f", e->cTransform->velocity.length());
  }

  ImGui::EndTable();
  ImGui::EndChild();
  ImGui::End();
}
