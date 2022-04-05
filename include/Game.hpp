#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

enum class State
{
    FirstTime,
    LastProject,
    InProject
};
class Game
{
private:
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(640, 640, 8), "PixelArtEditor");
    sf::Clock deltaClock;
    std::vector<sf::RectangleShape> shapes;
    

    char *buffer = new char[1000];

public:
    Game();
    ~Game();
    void Run();
    void Update();
    void DrawSprites();
    void Draw();
    void PollEvents(sf::Event &e);
    void DrawUI();
};