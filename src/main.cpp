#include "imgui.h"
#include "imgui-SFML.h"
#include "Game.hpp"

#include "SFML/Graphics.hpp"
#include "Utility.hpp"

int main(int argv, char **argc)
{
    Utility::Profiler::Timer t;
    t.start();
    sf::sleep(sf::milliseconds(16));
   float time = t.stop();
   std::cout<< time<<"\n";
    Game game;
    game.Run();
}
