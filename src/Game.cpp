#include "Game.h"

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

  ImGui::SFML::Init(m_window);

  // Spawn the first player
  spawnPlayer();
  // Spawn the  first enemy
  spawnEnemy();

  // seed random generator
  std::srand(time(NULL));
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
      sMovement();
      sCollision();
      sEnemySpawner();
      sLifespan();
      sScore();
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

  playerEntity->cShape     = std::make_shared<CShape>(
      m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
      sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

  // Add collisions
  playerEntity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

  // Add an input component to the player so that we can use inputs
  playerEntity->cInput = std::make_shared<CInput>();

  // Add the score to the player
  playerEntity->cScore = std::make_shared<CScore>(0, m_fontConfig.path, m_fontConfig.size,
                                                  sf::Color(m_fontConfig.red, m_fontConfig.green, m_fontConfig.blue));
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
  float deltaY = 1 + (w.y - radius.y) - radius.y;
  float deltaX = 1 + (w.x - radius.x) - radius.x;
  float ex     = rand() % (int)deltaX + radius.x;
  float ey     = rand() % (int)deltaY + radius.y;

  // Random velocity
  float deltaS = 1 + m_enemyConfig.SMAX - m_enemyConfig.SMIN;
  float speed  = rand() % (int)deltaS + m_enemyConfig.SMIN;
  int   signX  = rand() % 2 == 0 ? 1 : -1;
  int   signY  = rand() % 2 == 0 ? 1 : -1;
  Vec2  vel    = Vec2(ex * signX, ey * signY).normalize() * speed;

  // Random vertices
  float deltaV = 1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN;
  float V      = rand() % (int)deltaV + m_enemyConfig.VMIN;

  // Random FillColors
  int   deltaColor   = 1 + 255 - 0; // 1 + MAX - MIN
  float r            = rand() % deltaColor + 0;
  float g            = rand() % deltaColor + 0;
  float b            = rand() % deltaColor + 0;

  Vec2 startPosition = Vec2(ex, ey);

  enemy->cTransform  = std::make_shared<CTransform>(startPosition, vel, 0.0f);
  enemy->cShape      = std::make_shared<CShape>(m_enemyConfig.SR, V, sf::Color(r, g, b),
                                           sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
  enemy->cCollision  = std::make_shared<CCollision>(m_enemyConfig.CR);
};

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &target)
{
  auto bullet          = m_entities.addEntity("bullet");

  Vec2 startPos        = m_player->cTransform->pos;
  Vec2 targetDirection = (target - startPos).normalize();

  bullet->cTransform   = std::make_shared<CTransform>(startPos, targetDirection * m_bulletConfig.S, 0);
  bullet->cShape       = std::make_shared<CShape>(
      m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
      sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
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
    smallEnemy->cCollision  = std::make_shared<CCollision>(radius);
  }
}

void Game::sCollision()
{
  /*
   * bullets and enemies
   */
  for (auto bullet : m_entities.getEntities("bullet"))
  {
    int  bulletR = bullet->cCollision->radius;
    Vec2 bulletP(bullet->cShape->circle.getPosition().x, bullet->cShape->circle.getPosition().y);

    for (auto enemy : m_entities.getEntities("enemy"))
    {
      int  enemyR = enemy->cCollision->radius;
      Vec2 enemyP(enemy->cShape->circle.getPosition().x, enemy->cShape->circle.getPosition().y);
      // Hacky line to prevent those spawny collisions at (0,0)
      // that I don't understand
      if (enemyP == Vec2(0, 0)) continue;

      float distBE            = enemyP.dist(bulletP);
      float collisionDistance = (bulletR + enemyR);

      if (distBE <= collisionDistance)
      {
        bullet->destroy();
        enemy->destroy();
        m_player->cScore->score += 100 * enemy->cShape->circle.getPointCount();
        spawnSmallEnemies(enemy);
      }
    }
  }

  for (auto bullet : m_entities.getEntities("bullet"))
  {
    int  bulletR = bullet->cCollision->radius;
    Vec2 bulletP(bullet->cShape->circle.getPosition().x, bullet->cShape->circle.getPosition().y);

    for (auto sEnemy : m_entities.getEntities("smallEnemy"))
    {
      int  enemyR = sEnemy->cCollision->radius;
      Vec2 enemyP(sEnemy->cShape->circle.getPosition().x, sEnemy->cShape->circle.getPosition().y);
      // Hacky line to prevent those spawny collisions at (0,0)
      // that I don't understand
      if (enemyP == Vec2(0, 0)) continue;

      float distBE            = enemyP.dist(bulletP);
      float collisionDistance = (bulletR + enemyR);

      if (distBE <= collisionDistance)
      {
        bullet->destroy();
        sEnemy->destroy();
        m_player->cScore->score += 400 * sEnemy->cShape->circle.getPointCount();
      }
    }
  }
  /*
   * Enemies and Walls
   */
  for (auto &enemy : m_entities.getEntities("enemy"))
  { // Collisions in the screen's border
    int enemyR = enemy->cCollision->radius;

    Vec2 enemyP(enemy->cShape->circle.getPosition().x, enemy->cShape->circle.getPosition().y);
    // Hacky line to prevent those spawny collisions at (0,0)
    // that I don't understand
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

  /*
   * Player and Walls
   * and Player and enemies
   */
  for (auto player : m_entities.getEntities("player"))
  {
    float playerR = player->cCollision->radius;

    Vec2 playerP(player->cShape->circle.getPosition().x, player->cShape->circle.getPosition().y);

    int playerTop    = playerP.y - playerR;
    int playerBottom = playerP.y + playerR;
    int playerLeft   = playerP.x - playerR;
    int playerRight  = playerP.x + playerR;

    // Prevent Shape from going beyond screen limits
    Vec2 vel(player->cTransform->velocity);

    if (playerBottom >= m_windowConfig.height && vel.y > 0) player->cTransform->pos.y = m_windowConfig.height - playerR;

    if (playerTop <= 0 && vel.y < 0) player->cTransform->pos.y = 0 + playerR;

    if (playerRight >= m_windowConfig.width && vel.x > 0) player->cTransform->pos.x = m_windowConfig.width - playerR;
    if (playerLeft <= 0 && vel.x < 0) player->cTransform->pos.x = 0 + playerR;

    /*
    // Collision Player enemies
    */
    for (auto enemy : m_entities.getEntities("enemy"))
    {
      int  enemyR = enemy->cCollision->radius;
      Vec2 enemyP(enemy->cShape->circle.getPosition().x, enemy->cShape->circle.getPosition().y);
      // Hacky line to prevent those spawny collisions at (0,0)
      // that I don't understand
      if (enemyP == Vec2(0, 0)) continue;

      float distPE            = enemyP.dist(playerP);
      float collisionDistance = (playerR + enemyR);

      if (distPE <= collisionDistance)
      {
        enemy->destroy();
        player->destroy();
        spawnPlayer();
        spawnSmallEnemies(enemy);
      }
    }
  }
};

void Game::sEnemySpawner()
{
  // if (m_currentFrame - m_lastEnemySpawnTime > 600)
  // if there are more than 5 entity enemies, then slow down spawn speed
  if (m_currentFrame % 60 == 0 && m_entities.getEntities("enemy").size() <= 5 && m_enemyConfig.SI > 60) m_enemyConfig.SI -= 5;
  else if (m_currentFrame % 60 == 0 && m_entities.getEntities("enemy").size() > 5 && m_enemyConfig.SI < 600)
    m_enemyConfig.SI += 5;
  // std::cout << m_enemyConfig.SI << std::endl;
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
  m_window.clear();

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

    if (!ImGui::GetIO().WantCaptureMouse) // Separates the ImGui window from the
                                          // input of the game
    {
      if (event.type == sf::Event::MouseButtonPressed)
      {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
          if (!m_paused)
          {
            spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
          }
        }

        if (event.mouseButton.button == sf::Mouse::Right)
        {
          // call spawnSpecialWeapon here
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
}

void Game::sScore()
{
  // TODO: put the score in the TOP-LEFT corner
  static std::string newScore;
  newScore = std::to_string(m_player->cScore->score);
  m_player->cScore->Score.setString("Score: " + newScore);
}

void spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
  // TODO: implement your own special weapon
}

void Game::sGUI()
{
  static float f1 = m_player->cShape->circle.getRadius();
  // static float f1 = m_player->cShape->circle.getRadius();
  ImGui::Begin("Window title");
  ImGui::Text("Window Text!");
  ImGui::DragFloat("drag float", &f1);
  m_player->cShape->circle.setRadius(f1);
  ImGui::Checkbox("Checkbox", &m_paused);
  if (ImGui::Button("Pause"))
  {
    m_paused = !m_paused;
  }
  ImGui::SameLine();
  ImGui::End();
}
