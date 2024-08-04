#include "background.h"

using namespace std;

wstring EnBgToStr(EnBg i)
{
    std::wstring s;
    switch (i) {
    case grass:
        s = L"grass";
        break;
    case asphalt:
        s = L"asphalt";
        break;
    case sand:
        s = L"sand";
        break;
    case water:
        s = L"water";
        break;
    default:
        break;
    }
    return s;
};

background::background(HDC hdc){
    //biomes IMGs load
    for (size_t i = 0; i < bioms_am; i++) {
        wstring s = L"res/bgs/";
        s.append(EnBgToStr(EnBg(i)));
        wstring s2 = s;
        s2.append(L".bmp");
        //bgs.push_back(new IMG(s2.c_str(), hdc));
        vector<IMG*> v;
        for (size_t j = 0; j < bioms_am; j++) {
            wstring s3 = s;
            s3.append(L"_");
            s3.append(EnBgToStr(EnBg(j)));
            s3.append(L".bmp");
            if (i == j) s3 = s2;
            v.push_back(new IMG(s3.c_str(), hdc));
        }
        trBgs.push_back(v);
    }
    for (size_t i = 0; i < roadsTransitions.size(); i++) {
        wstring s = L"res/roads/road_";
        s.append(to_wstring(i));
        s.append(L"_");
        vector<IMG*> v;
        for (size_t j = 0; j < roadsTransitions[i].size(); j++) {
            wstring s2 = s;
            s2.append(to_wstring(roadsTransitions[i][j]));
            s2.append(L".bmp");
            v.push_back(new IMG(s2.c_str(), hdc));
        }
        imgRoads.push_back(v);
    }
}

void background::draw(HDC hdc, vec2 cam){
    grid_count = int(floor(-cam.y / 64.f));

    //draw background
    for (int offs = -1; offs < 2; offs++) {
        if (grid_count % 10 == 5) {
            biome_changed = false;
            trBgs[biome][biome]->drawImg(hdc, { short(0 - int(cam.x) % 64 + 64 * offs)  ,short(0 - int(cam.y) % 64) });
            trBgs[biome][next_biome]->drawImg(hdc, { short(0 - int(cam.x) % 64 + 64 * offs)  ,short(0 - int(cam.y) % 64 - 64) });
        }
        else if (grid_count % 10 == 6) {
            trBgs[biome][next_biome]->drawImg(hdc, { short(0 - int(cam.x) % 64 + 64 * offs)  ,short(0 - int(cam.y) % 64) });
            trBgs[next_biome][next_biome]->drawImg(hdc, { short(0 - int(cam.x) % 64 + 64 * offs)  ,short(0 - int(cam.y) % 64 - 64) });
        }
        else {
            if (grid_count % 10 == 7 && !biome_changed) {
                biome = next_biome;
                next_biome = EnBg(rand() % bioms_am);
                biome_changed = true;
            }
            trBgs[biome][biome]->drawImg(hdc, { short(0 - int(cam.x) % 64 + 64 * offs)  ,short(0 - int(cam.y) % 64) });
            trBgs[biome][biome]->drawImg(hdc, { short(0 - int(cam.x) % 64 + 64 * offs)  ,short(0 - int(cam.y) % 64 - 64) });
        }
    }

    //draw road
    if (grid_count % 6 == 3) {
        road_changed = false;
        imgRoads[road][0]->drawImgT(hdc, { short(0 - int(cam.x))  ,short(0 - int(cam.y) % 64) });
        imgRoads[road][next_road]->drawImgT(hdc, { short(0 - int(cam.x))  ,short(0 - int(cam.y) % 64 - 64) });
    }
    else if (grid_count % 6 == 4) {
        imgRoads[road][next_road]->drawImgT(hdc, { short(0 - int(cam.x))  ,short(0 - int(cam.y) % 64) });
        imgRoads[roadsTransitions[road][next_road]][0]->drawImgT(hdc, { short(0 - int(cam.x))  ,short(0 - int(cam.y) % 64 - 64) });
    }
    else {
        if (grid_count % 6 == 5 && !road_changed) {
            road = roadsTransitions[road][next_road];
            next_road = rand() % roadsTransitions[road].size();
            road_changed = true;
        }
        imgRoads[road][0]->drawImgT(hdc, { short(0 - int(cam.x))  ,short(0 - int(cam.y) % 64) });
        imgRoads[road][0]->drawImgT(hdc, { short(0 - int(cam.x))  ,short(0 - int(cam.y) % 64 - 64) });
    }
}

int background::line(vec2 cam, vec2 pos){
    if (grid_count % 6 == 3) {
        if ((pos.y - cam.y) > int(-cam.y) % 64 - 32) {
            return lns[road];
        } else {
            return lns[roadsTransitions[road][next_road]];
        }
    }
    else if (grid_count % 6 == 4) {
        if ((pos.y - cam.y) > int(-cam.y) % 64 + 32) {
            return lns[road];
        }
        else {
            return lns[roadsTransitions[road][next_road]];
        }
    }
    else {
        return lns[road];
    }
}

int background::lines_Now(vec2 cam, vec2 pos){
    if (grid_count % 6 == 3) {
        if ((pos.y - cam.y) < int(-cam.y) % 64) {
            return lns[road] | lns[roadsTransitions[road][next_road]];
        } else {
            return lns[road];
        }
    }
    else if (grid_count % 6 == 4) {
        if ((pos.y - cam.y) > int(-cam.y) % 64 ) {
            return lns[road] | lns[roadsTransitions[road][next_road]];
        } else {
            return lns[roadsTransitions[road][next_road]];
        }
        
    } else {
        return lns[road];
    }
}

bool background::is_water(vec2 cam, vec2 pos){
    if (grid_count % 10 == 5) {
        if ((pos.y - cam.y) > int(-cam.y) % 64 - 32) {
            return biome == EnBg::water;
        }
        else {
            return next_biome == EnBg::water;
        }
    }
    else if (grid_count % 10 == 6) {
        if ((pos.y - cam.y) > int(-cam.y) % 64 +32) {
            return biome == EnBg::water;
        }
        else {
            return next_biome == EnBg::water;
        }
    }
    else {
        return biome == EnBg::water;
    }
}
