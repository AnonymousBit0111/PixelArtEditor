#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#define NO_FREETYPE
#include <pngwriter.h>
#include <chrono>

namespace Utility
{

    void GetMemoryUsage(double &vm_usage, double &resident_set);
    bool SaveToPNG(std::string &path, std::vector<sf::RectangleShape> &shapes, int Xsize, int Ysize);
    std::vector<sf::RectangleShape> OpenPNG(std::string &path, int *Xsize, int *Ysize);
    namespace Profiler
    {
        struct Timer
        {
            std::chrono::time_point<std::chrono::system_clock> starttime;
            std::chrono::time_point<std::chrono::system_clock> end;
            std::chrono::duration<float> duration;
            Timer();
            ~Timer();
            void start();
            float stop();
        };

        static std::vector<float> frametimes;
        void BeginFrame();

        void EndFrame();
    }
};