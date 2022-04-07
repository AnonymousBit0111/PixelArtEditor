#include "Game.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include <memory>
#include <cmath>
#include <fstream>
#include "Utility.hpp"

// test will move this later

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow &window, float zoom)
{
    const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
    sf::View view{window.getView()};
    view.zoom(zoom);
    window.setView(view);
    const sf::Vector2f afterCoord{window.mapPixelToCoords(pixel)};
    const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
    view.move(offsetCoords);
    window.setView(view);
}

Game::Game()
{
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
            shape.setFillColor(sf::Color::White);
            shape.setOutlineColor(sf::Color::White);
            shapes.push_back(shape);
        }
    }
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
    delete[] buffer;
    delete[] currentfile;
}

void Game::Run()
{
    while (window.isOpen())
    {
        sf::Event e;
        PollEvents(e);
        Draw();
    }
}
void Game::Update()
{
}
void Game::DrawSprites()
{
    // TODO culling

    for (int i = 0; i < shapes.size(); i++)
    {
        window.draw(shapes[i]);
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
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New"))
        {
            // Do something
        }
        if (ImGui::Button("Save"))
        {
            std::string path = "/home/ilya/Desktop/PixelArtEditor/PixelArtEditor/test.png";
            Utility::SaveToPNG(path, shapes, Xsize, Ysize);
        }
        if (ImGui::BeginMenu("Open PNG"))
        {

            if (ImGui::Button("open"))
            {
                selected = nullptr;
                std::ofstream log("./log.txt");

                for (auto &i : shapes)
                {
                    log << (uint)i.getFillColor().r << "," << (uint)i.getFillColor().g << "," << (uint)i.getFillColor().b << "\n";
                }
                log << "END:\n\n\n\n";

                shapes = Utility::OpenPNG(currentfile, Xsize, Ysize);
                for (auto &i : shapes)
                {
                    log << (uint)i.getFillColor().r << "," << (uint)i.getFillColor().g << "," << (uint)i.getFillColor().b << "\n";
                }
            }
            ImGui::InputText("pathtofile", currentfile, sizeof(char) * 1000);
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Open custom sprite"))
        {
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings"))
    {

        // ImGui::BeginMenu("size");

        if (ImGui::BeginMenu("size"))
        {
            ImGui::InputInt("x", &Xsize);

            ImGui::InputInt("y", &Ysize);
            if (ImGui::Button("Resize"))
            {
                if (shapes.size() < (Xsize * Ysize))
                {
                    std::cout << "resized\n";

                    shapes.resize(Xsize * Ysize);

                    for (int i = 0; i < Xsize; i++)
                    {
                        for (int r = 0; r < Ysize; r++)
                        {
                            //  if ((i * r) < shapes.size())
                            //    continue;
                            if (i < OldYsize && r < OldXsize)
                                continue;

                            sf::RectangleShape shape;
                            shape.setPosition(i * 20, r * 20);
                            shape.setSize({20, 20});
                            shape.setFillColor(sf::Color::White);
                            shape.setOutlineColor(sf::Color::White);
                            shapes.push_back(shape);
                        }
                    }
                    OldXsize = Xsize;
                    OldYsize = Ysize;
                }
            }
            ImGui::SameLine();

            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    ImGui::Begin("Colour picker");
    ImGui::ColorPicker3("Colour", currentcolor, ImGuiColorEditFlags_PickerHueWheel);
    ImGui::End();

    ImGui::Begin("Pallet Options");

    ImGui::SliderFloat("scrollspeed", &scrollSpeed, 1, 2);
    ImGui::End();

    // TODO show some debug info
    ImGui::Begin("debug info");

    ImGui::End();
}
void Game::SelectShapeAt(sf::Vector2f pos)
{
    bool selectionmade = false;
    for (auto &i : shapes)
    {
        if (i.getGlobalBounds().contains(pos.x, pos.y))
        {
            if (selected != nullptr)
            {
                selected->setFillColor(prevcolor);
            }
            prevcolor = i.getFillColor();
            selected = &i;
            selected->setFillColor(sf::Color(currentcolor[0] * 255, currentcolor[1] * 255, currentcolor[2] * 255));
            selectionmade = true;
        }
    }
    if (!selectionmade)
    {
        if (selected != nullptr)
        {
            selected->setFillColor(prevcolor);
            selected = nullptr;
        }
    }
}

void Game::PollEvents(sf::Event &e)
{
    if (window.pollEvent(e))
    {
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            ImGui::SFML::ProcessEvent(window, e);

            // only input if imgui isnt using the mouse

            if (!io.WantCaptureMouse)
                switch (e.type)
                {
                case sf::Event::MouseButtonPressed:
                    if (selected != nullptr)
                    {
                        selected->setFillColor(sf::Color(currentcolor[0] * 255, currentcolor[1] * 255, currentcolor[2] * 255));
                        prevcolor = sf::Color(currentcolor[0] * 255, currentcolor[1] * 255, currentcolor[2] * 255);
                    }
                    break;

                case sf::Event::MouseMoved:
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (selected != nullptr)
                        {
                            selected->setFillColor(sf::Color(currentcolor[0] * 255, currentcolor[1] * 255, currentcolor[2] * 255));
                            prevcolor = sf::Color(currentcolor[0] * 255, currentcolor[1] * 255, currentcolor[2] * 255);
                        }
                    }
                    // TODO: add dragging because this doesnt work
                    //  if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    //  {
                    //      sf::View view = window.getView();
                    //      sf::Vector2f deltaPos = sf::Vector2f(e.mouseMove.x * 0.01, e.mouseMove.y * 0.01);
                    //      view.setCenter(view.getCenter() + deltaPos);
                    //      window.setView(view);
                    //  }
                    {
                        auto pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        SelectShapeAt(pos);
                    }
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                {

                    window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, static_cast<float>(e.size.width), static_cast<float>(e.size.height))));

                    break;
                }
                case sf::Event::MouseWheelMoved:
                {
                    // sf::View view = window.getView();
                    // sf::Vector2 size = view.getSize();
                    // int delta = 1;
                    // if (e.mouseWheel.delta < 0)
                    //     delta = -1;
                    // view.setSize(size.x + e.mouseWheel.x * delta, size.y + e.mouseWheel.y * delta);
                    // window.setView(view);

                    if (e.mouseWheel.delta > 0)
                        zoomViewAt({e.mouseWheel.x, e.mouseWheel.y}, window, (1.f / scrollSpeed));
                    else if (e.mouseWheel.delta < 0)
                        zoomViewAt({e.mouseWheel.x, e.mouseWheel.y}, window, scrollSpeed);

                    break;
                }
                case sf::Event::KeyPressed:
                {
                    sf::View view = window.getView();

                    if (e.key.code == sf::Keyboard::A)
                    {
                        view.move(-1.0f * scrollSpeed, 0.0f);
                        window.setView(view);
                        break;
                    }
                    if (e.key.code == sf::Keyboard::D)
                    {
                        view.move(1.0f * scrollSpeed, 0.0f);
                        window.setView(view);
                        break;
                    }
                    if (e.key.code == sf::Keyboard::W)
                    {
                        view.move(0.0f, -1.0f * scrollSpeed);
                        window.setView(view);
                    }
                    if (e.key.code == sf::Keyboard::S)
                    {
                        view.move(0.0f, 1.0f * scrollSpeed);
                        window.setView(view);
                    }
                }

                break;

                default:

                    break;
                }
        }
    }
}