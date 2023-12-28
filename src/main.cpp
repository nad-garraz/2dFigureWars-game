#include "Game.h"
#include <SFML/Graphics.hpp>
#include <string>

int main()
{
  Game g("../config_game");
  g.run();

  return 0;
}
