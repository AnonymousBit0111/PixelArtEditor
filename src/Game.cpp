#include "Game.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include <memory>
#include <cmath>

Game::Game()
{
    ImGui::SFML::Init(window);
    window.setFramerateLimit(120);
    shapes.reserve(64 * 64);
    sf::Vector2f cellsize = {20, 20};
    for (size_t c = 0; c < 64; c++)
    {

        for (size_t r = 0; r < 64; r++)
        {
            sf::RectangleShape shape;
            shape.setPosition(c * cellsize.x, r * cellsize.y);
            shape.setSize(cellsize);
            shape.setFillColor(sf::Color::Blue);
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1);
            shapes.push_back(shape);
        }
    }
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
    delete[] buffer;
}

void Game::Run()
{
    while (window.isOpen())
    {
        sf::Event e;
        PollEvents(e);
    }
}
void Game::Update()
{
}
void Game::DrawSprites()
{
    // TODO culling

    for (auto &i : shapes)
    {
        // if (view.contains(i.getPosition()))
        // {
        window.draw(i);
        // }
    }
}
void Game::Draw()
{
    window.clear();
    ImGui::SFML::Update(window, deltaClock.restart());

    DrawSprites();
    DrawUI();

    ImGui::SFML::Render(window);
    window.display();
}
void Game::DrawUI()
{

    ImGui::Begin("Create a new project");
    ImGui::InputText("filename", buffer, sizeof(char) * 1000);
    ImGui::SameLine();
    if (ImGui::Button("Open"))
    {
        // TODO add open file logic
    }
    ImGui::End();
}

void Game::PollEvents(sf::Event &e)
{
    bool redrawneeded = false;
    while (window.pollEvent(e))
    {
        ImGui::SFML::ProcessEvent(window, e);
        switch (e.type)
        {
        case sf::Event::MouseMoved:
        // TODO: add dragging
        //  if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        //  {
        //      sf::View view = window.getView();
        //      sf::Vector2f deltaPos = sf::Vector2f(e.mouseMove.x * 0.01, e.mouseMove.y * 0.01);
        //      view.setCenter(view.getCenter() + deltaPos);
        //      window.setView(view);
        //  }
        //  break;
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::Resized:
        {
            window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, static_cast<float>(e.size.width), static_cast<float>(e.size.height))));

            break;
        }
        case sf::Event::KeyPressed:
        {
            sf::View view = window.getView();

            if (e.key.code == sf::Keyboard::A)
            {
                view.move(-1.0f, 0.0f);
                window.setView(view);
                break;
            }
            if (e.key.code == sf::Keyboard::D)
            {
                view.move(1.0f, 0.0f);
                window.setView(view);
                break;
            }
            if (e.key.code == sf::Keyboard::W)
            {
                view.move(0.0f, -1.0f);
                window.setView(view);
            }
            if (e.key.code == sf::Keyboard::S)
            {
                view.move(0.0f, 1.0f);
                window.setView(view);
            }
        }

        break;

        default:

            break;
        }
        if (!redrawneeded)
            redrawneeded = true;
    }
    if (redrawneeded)
    {

        Draw();
    }
}