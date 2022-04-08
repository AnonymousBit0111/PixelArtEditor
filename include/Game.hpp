#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "imgui.h"
#include "imgui-SFML.h"

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
    sf::RectangleShape *selected = nullptr;
    bool showcolour;

    bool init = ImGui::SFML::Init(window);
    // previous colour of the currently selected rectangle , do not modify anymore
    sf::Color prevcolor;
    float currentcolor[3] = {1.0f, 1.0f, 1.0f};
    float scrollSpeed = 1.5;

    std::string currentfile;
    std::string file_to_open;
    ImGuiIO &io = ImGui::GetIO();
    std::vector<sf::RectangleShape*> drawlist;

    int Xsize = 64;
    int Ysize = 64;
    int OldXsize = 64;
    int OldYsize = 64;

public:
    Game();
    ~Game();
    void Run();
    void SelectShapeAt(sf::Vector2f pos);
    void Update();
    void DrawSprites();
    void Draw();
    void PollEvents(sf::Event &e);
    void DrawUI();
    void CheckDrawList();
};