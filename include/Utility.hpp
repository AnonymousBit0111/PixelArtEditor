#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#define NO_FREETYPE
#include <pngwriter.h>

namespace Utility
{

    void GetMemoryUsage(double &vm_usage, double &resident_set);
    bool SaveToPNG(std::string &path, std::vector<sf::RectangleShape> &shapes, int Xsize, int Ysize);
    std::vector<sf::RectangleShape> OpenPNG(char *path, int Xsize, int Ysize);
};