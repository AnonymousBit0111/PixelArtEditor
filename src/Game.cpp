#include "Game.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>

Game::Game()
{
    ImGui::SFML::Init(window);
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
}

void Game::Run()
{
    while (window.isOpen())
    {
        PollEvents();
        Update();
        Draw();
    }
}
void Game::Update()
{
}
void Game::Draw()
{
    window.clear();
    ImGui::SFML::Update(window, deltaClock.restart());



    ImGui::SFML::Render(window);
    window.display();
}
    void Game::DrawUI(){
        ImGui::Begin("PixelArtEditor");
        ImGui::Text("Hello, world!");
        ImGui::End();
    }

void Game::PollEvents()
{
    sf::Event e;
    while (window.pollEvent(e))
    {
        ImGui::SFML::ProcessEvent(window, e);
        if (e.type == sf::Event::Closed)
            window.close();
    }
}