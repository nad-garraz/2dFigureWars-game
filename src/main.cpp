#include "Game.h"
#include <SFML/Graphics.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int main()
{

  Game g("../config_game");
  g.run();

  ImGui::SFML::Shutdown();
}
