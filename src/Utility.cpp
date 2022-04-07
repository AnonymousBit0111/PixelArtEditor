#include "Utility.hpp"
#include <sys/resource.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <imgui.h>

// //////////////////////////////////////////////////////////////////////

Utility::Profiler::Timer::Timer()
{
}
void Utility::Profiler::Timer::start()
{
    starttime = std::chrono::high_resolution_clock::now();
}
float Utility::Profiler::Timer::stop()
{
    end = std::chrono::high_resolution_clock::now();
    duration = end - starttime;
    return duration.count() * 1000;
}
Utility::Profiler::Timer::~Timer()
{
}

void Utility::GetMemoryUsage(double &vm_usage, double &resident_set)
{
    vm_usage = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

bool Utility::SaveToPNG(std::string &path, std::vector<sf::RectangleShape> &shapes, int Xsize, int Ysize)
{
    pngwriter writer(Xsize, Ysize, 1, path.c_str());
    writer.clear();

    for (auto &i : shapes)
    {
        int x = i.getPosition().x / 20;
        int y = i.getPosition().y / 20;
        int r, g, b;
        r = i.getFillColor().r * 255;
        g = i.getFillColor().g * 255;
        b = i.getFillColor().b * 255;

        writer.plot(x, y, r, g, b);
    }
    writer.write_png();
    writer.close();
    return true;
}
// write this function
std::vector<sf::RectangleShape> Utility::OpenPNG(std::string &path, int *Xsize, int *Ysize)
{
    pngwriter reader(1, 1, 1, path.c_str());
    reader.readfromfile(path.c_str());
    *Xsize = reader.getwidth();
    *Ysize = reader.getheight();

    std::vector<sf::RectangleShape> newshapes;
    newshapes.reserve(reader.getwidth() * reader.getheight());
    for (int i = 0; i < reader.getwidth(); i++)
    {
        for (int r = 0; r < reader.getheight(); r++)
        {
            int rr, gg, bb;
            rr = reader.read(i, r, 1);
            gg = reader.read(i, r, 2);
            bb = reader.read(i, r, 3);

            sf::RectangleShape shape;
            shape.setPosition(i * 20, r * 20);
            shape.setSize({20, 20});
            shape.setFillColor(sf::Color(rr / 255, gg / 255, bb / 255));
            newshapes.push_back(shape);
        }
    }
    reader.close();
    return newshapes;
}
static Utility::Profiler::Timer timer;

void Utility::Profiler::BeginFrame()
{
    timer.start();
}
float avg(std::vector<float> &vec)
{
    float total = 0;
    for (auto &i : vec)
    {
        total += i;
    }
    return total / vec.size();
}
void Utility::Profiler::EndFrame()
{
    float time = timer.stop();
    if (frametimes.size() >= 100)
    {
        frametimes.erase(frametimes.begin());
    }
    frametimes.push_back(time);
    ImGui::Begin("debug");
    float *frametimesarr = &frametimes[0];
    ImGui::ShowDemoWindow();
    float average = avg(frametimes);
    ImGui::PlotHistogram("frametimes", frametimesarr, frametimes.size(), 0, NULL, 0.0f, average+(average/2), ImVec2(250.0f,100.0f));
    ImGui::Text("avg : %f ms",average );
    ImGui::End();
}