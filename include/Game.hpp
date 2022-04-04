#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class Game
{
private:
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800, 600, 8), "PixelArtEditor");
    sf::Clock deltaClock;

public:
    Game();
    ~Game();
    void Run();
    void Update();
    void Draw();
    void PollEvents();
    void DrawUI();
};