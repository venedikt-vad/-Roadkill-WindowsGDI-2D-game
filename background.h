#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "VectorMath.h"
#include "GUI.h"

#define bioms_am 4

enum EnBg {
    grass = 0,
    asphalt = 1,
    sand = 2,
    water = 3
};

struct lines {
    bool lns[4] = {false,false,false,false};
};

class background{
public:
    EnBg biome = EnBg(0);
    EnBg next_biome = EnBg(rand() % bioms_am);
    bool biome_changed = true;

    int road = 0;
    int next_road = 1;
    bool road_changed = true;

    int grid_count = 0;
    

	background(HDC hdc);
    void draw(HDC hdc, vec2 cam);
    int line(vec2 cam, vec2 pos);
    int lines_Now(vec2 cam, vec2 pos);
    bool is_water(vec2 cam, vec2 pos);

private:
    //std::vector<IMG*> bgs;
    std::vector<std::vector<IMG*>> trBgs;
    std::vector<std::vector<IMG*>> imgRoads;
    std::vector<std::vector<int>> roadsTransitions = {
        {0, 1, 2, 3, 4},
        {1, 0, 2, 3},
        {2, 0, 1 },
        {3, 0, 1 },
        {4, 0 },
    };
    std::vector<int> lns{
        0b1111,
        0b0110,
        0b0111,
        0b1110,
        0b1001
    };
};

