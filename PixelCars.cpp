#include "framework.h"
#include "PixelCars.h"

#include <ctime>
#include <fstream>

#include "Input.h"
#include "GUI.h"
#include "Sounds.h"
#include "VectorMath.h"
#include "car.h"
#include "background.h"
#include "destructive.h"
#include "bullet.h"

using namespace std;

#define MAX_LOADSTRING 100
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hWnd;
HDC hdc, hCmpDC;
HBITMAP hBmp;
int Y_OFFS;
int X_OFFS;
COORD screenCoords = {64,64};
bool isActive = false;
bool isFullscreen = true;
bool changeStateFlag = false;

float dt = 0.000001f;
int screen = -10;
unsigned int seed;

HBRUSH BRUSHbg;
HPEN PENbg;


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void UpdateSize(HWND hWnd) {
    RECT Window_rect;
    GetWindowRect(hWnd, &Window_rect);
    screenCoords.X = short(Window_rect.right - Window_rect.left);
    screenCoords.Y = short(Window_rect.bottom - Window_rect.top);
    SelectObject(hdc, BRUSHbg);
    SelectObject(hdc, PENbg);
    Rectangle(hdc, 0, 0, screenCoords.X, screenCoords.Y);
}

int windowFullscreen(HINSTANCE hInstance, int nCmdShow, WNDPROC WndProc) {
    hInst = hInstance;
    hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        512 + (X_OFFS * 4), 512 + Y_OFFS + X_OFFS,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    SetWindowLong(hWnd, GWL_STYLE, 0);

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("ERROR"),
            NULL);

        return 1;
    }

    ShowWindow(hWnd, nCmdShow);

    hdc = GetWindowDC(hWnd);
    hCmpDC = CreateCompatibleDC(hdc);
    hBmp = CreateCompatibleBitmap(hdc, 64, 64);
    SelectObject(hCmpDC, hBmp);

    UpdateWindow(hWnd);
    SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);

    return 2;
}
int windowWindowed(HINSTANCE hInstance, int nCmdShow, WNDPROC WndProc) {
    hInst = hInstance;

    hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        512 + (X_OFFS * 4), 512 + Y_OFFS + X_OFFS,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("ERROR"),
            NULL);

        return 1;
    }

    ShowWindow(hWnd, nCmdShow);

    hdc = GetWindowDC(hWnd);
    hCmpDC = CreateCompatibleDC(hdc);
    hBmp = CreateCompatibleBitmap(hdc, 64, 64);
    SelectObject(hCmpDC, hBmp);

    UpdateWindow(hWnd);
    return 2;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //SM_CYFRAME        - The thickness of the sizing border around a resizable window                    
    //SM_CXPADDEDBORDER - The amount of border padding for captioned windows                      
    //SM_CYCAPTION      - The height of the caption area
    X_OFFS = (GetSystemMetrics(SM_CYFRAME));
    Y_OFFS = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER));
    
    //window_creation
    {
        LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
        LoadStringW(hInstance, IDC_PIXELCARS, szWindowClass, MAX_LOADSTRING);

        WNDCLASSEXW wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

        if (!RegisterClassEx(&wcex))
        {
            MessageBox(NULL,
                _T("Call to RegisterClassEx failed!"),
                _T("ERROR"),
                NULL);

            return 1;
        }
    }
    windowFullscreen(hInstance, nCmdShow, WndProc);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIXELCARS));
    MSG msg;

    initSounds();


    SetStretchBltMode(hdc, COLORONCOLOR);
    BRUSHbg = CreateSolidBrush(RGB(30, 30, 30));
    PENbg = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

    UpdateSize(hWnd);   //get window size

    seed = (unsigned int)time(nullptr); //rnd seed
    srand(seed);
    
    carDATA* carDT = new carDATA(hCmpDC);
    destructive_data* destrDT = new destructive_data(hCmpDC);

    sound* s_fire = new sound();
    s_fire->Open(L"res/sounds/s_fire.wav");

    sound* main_st = new sound();
    sound* menu_st = new sound();
    main_st->Open(L"res/sounds/nt14.wav");
    menu_st->Open(L"res/sounds/nt14_phase2.wav");
    main_st->SetVol(0.5f);
    menu_st->SetVol(0.5f);
    main_st->SetLooping(true);
    menu_st->SetLooping(true);
    menu_st->Play();
    //main_st->Play();

    IMG* em_water_img = new IMG(L"res/em_water.bmp", hCmpDC);

    used_keys = { key_w, key_s, key_a, key_d, 
        key_up, key_down, key_left, key_right,
        key_esc, key_alt, key_enter, key_space,
        key_semicolon,
        key_tab
    };   //used input

    IMG* scr_menu = new IMG(L"res/screens/scr_menu/scr_menu_0.bmp", hdc);
    IMG* scr_menu1 = new IMG(L"res/screens/scr_menu/scr_menu_1.bmp", hdc);
    IMG* menu_marker = new IMG(L"res/screens/selection_marker.bmp", hdc);

    IMG* scr_settings = new IMG(L"res/screens/scr_settings.bmp", hdc);
    IMG* settings_marker = new IMG(L"res/screens/selection_marker_single.bmp", hdc);
    IMG* vol_marker = new IMG(L"res/screens/vol_control.bmp", hdc);

    IMG* scr_story = new IMG(L"res/screens/scr_story.bmp", hdc);
    IMG* scr_controls = new IMG(L"res/screens/scr_controls.bmp", hdc);

    IMG* scr_splash = new IMG(L"res/vvad_pixelated.bmp", hdc);
    int spl_t = clock();

    float vol_music = 1.f;
    float vol_sfx = 1.f;

    IMG* scr_death = new IMG(L"res/screens/scr_death.bmp", hdc);

    int menu_pos = 0;
    bool key_pressed = false;

    vector<IMG*> scr_trgt_img;
    for (size_t i = 0; i < 8; i++){
        wstring p = L"res/screens/tg_screen/scr_target_";
        p.append(to_wstring(i));
        p.append(L".bmp");
        IMG* im = new IMG(p.c_str(), hdc);
        scr_trgt_img.push_back(im);
    }
    IMG* scr_score = new IMG(L"res/screens/tg_screen/scr_score.bmp", hdc);
    vector<IMG*> numbers;
    for (size_t i = 0; i < 10; i++){
        wstring p = L"res/numbers/";
        p.append(to_wstring(i));
        p.append(L".bmp");
        IMG* im = new IMG(p.c_str(), hdc);
        numbers.push_back(im);
    }

    bool scr_trgt_open = true;
    bool tab_key_flag = false;
    int scr_trgt_timing = clock();
    EnCarColor target_car_color = EnCarColor(rand() % 9);
    EnCarColor target_car_color2 = EnCarColor(rand() % 9);
    int target_car_drawing_type = (rand() % DECOR_NM_AMOUNT + 1);
    int target_timeing = clock();

    int score = 0;

    background* bg = new background(hCmpDC);

    vec2 cam = {};
    car* player = new car(EnCarType::normal, EnCarColor::red, 0, EnCarColor::white, carDT);
    player->loc = vec2(10, 0);
    player->speed = vec2(0, -43.f);
    int player_last_fire = 0;
    #define PLAYER_FIRE_DELAY 0.4f
    vector<bullet* > player_bullets;

    vector<car*> cars;
    float car_interval = 0.f;
    int t_car_swapned = 0;

    vector<bullet* > bullets;

    vector<destructive*> destr;
    vector<destructive*> destr_dead;
    float last_destr_y = 0;
    float last_destr_fence = 0;
    float last_destr_trees = 0;

    //MAIN LOOP
    while (GetMessage(&msg, nullptr, 0, 0)){
        int start_t = clock();  //current time
        if (isActive) {
            getKeys();
            //scanKeys();

            if ((keys[key_alt] != 0) && (keys[key_enter] != 0) && (!changeStateFlag)) {  //windowed/fullscreen  switch
                if (!isFullscreen) {
                    changeStateFlag = true;
                    isFullscreen = true;
                    DestroyWindow(hWnd);
                    windowFullscreen(hInstance, nCmdShow, WndProc);
                    UpdateSize(hWnd);
                }
                else {
                    changeStateFlag = true;
                    isFullscreen = false;
                    DestroyWindow(hWnd);
                    windowWindowed(hInstance, nCmdShow, WndProc);
                    UpdateSize(hWnd);
                }
            }
            if (keys[key_alt] == 0) changeStateFlag = false;
            
            
        }

        switch (screen) {
        case -10:
            scr_splash->drawImg(hCmpDC, { 0,0 });
            if (float(start_t - spl_t) / CLOCKS_PER_SEC > 5) screen = 2;
            break;


        case 2: //main menu
        {
            if (isActive) {
                
                if (keys[key_w] != 0 || keys[key_up] != 0) {
                    if (!key_pressed) {
                        menu_pos--;
                        key_pressed = true;
                    }
                }
                else if (keys[key_s] != 0 || keys[key_down] != 0) {
                    if (!key_pressed) {
                        menu_pos++;
                        key_pressed = true;
                    }
                } 
                else if ((keys[key_space] != 0) || (keys[key_enter] != 0)) {
                    if (!key_pressed) {
                        key_pressed = true;
                        if (menu_pos == 0) screen = 5;
                        if (menu_pos == 1) screen = -1;
                        if (menu_pos == 2) {
                            screen = 4; 
                            menu_pos = 0;
                        }
                        if (menu_pos == 3) return 0;
                    }
                } else {
                    key_pressed = false;
                }
                menu_pos = max(min(menu_pos, 3), 0);
            }

            if (start_t / CLOCKS_PER_SEC % 2) scr_menu->drawImg(hCmpDC, { 0,0 });
            else scr_menu1->drawImg(hCmpDC, { 0,0 });

            menu_marker->drawImgT(hCmpDC, { 7, short(26 + menu_pos*8) });
        }
        break;

        case 3: //death
        {
            scr_death->drawImg(hCmpDC, { 0,0 });

            numbers[score / 10]->drawImgT(hCmpDC, { 28, 34 });
            numbers[score % 10]->drawImgT(hCmpDC, { 34, 34 });
            
            if (isActive) {
                if ((keys[key_space] != 0) || (keys[key_enter] != 0)) {
                    key_pressed = true;
                    screen = 2;
                }
            }
        }
        break;

        case 4: //settings
        {
            scr_settings->drawImg(hCmpDC, { 0,0 });
            settings_marker->drawImgT(hCmpDC, { 4, short(11 + ((menu_pos > 1) ? (32 + 9 * (menu_pos - 2)): (17 * menu_pos))) }); 
            if (isFullscreen) SetPixel(hCmpDC, 53, 44, RGB(76, 255, 0));
            for (int i = 0; i < 26*vol_music; i++){
                vol_marker->drawImg(hCmpDC, { short(8 + 2 * i), 18 });
            }
            for (int i = 0; i < 26 * vol_sfx; i++) {
                vol_marker->drawImg(hCmpDC, { short(8 + 2 * i), 34 });
            }
         
            if (isActive) {
                if (keys[key_esc] != 0) {
                    screen = 2;
                }

                if (keys[key_w] != 0 || keys[key_up] != 0) {
                    if (!key_pressed) {
                        menu_pos--;
                        key_pressed = true;
                    }
                }
                else if (keys[key_s] != 0 || keys[key_down] != 0) {
                    if (!key_pressed) {
                        menu_pos++;
                        key_pressed = true;
                    }
                } 
                else if (keys[key_d] != 0 || keys[key_right] != 0) {
                    if (!key_pressed) {
                        if (menu_pos == 0) {
                            vol_music += 0.1f;
                            vol_music = max(0, min(1, vol_music));
                            main_st->SetVol(0.5f * vol_music);
                            menu_st->SetVol(0.5f * vol_music);
                        }
                        if (menu_pos == 1) {
                            vol_sfx += 0.1f;
                            vol_sfx = max(0, min(1, vol_sfx));
                            s_fire->SetVol(vol_sfx);
                            carDT->s_expl->SetVol(vol_sfx);
                            destrDT->sound_d->SetVol(vol_sfx);
                        }
                        key_pressed = true;
                    }
                }
                else if (keys[key_a] != 0 || keys[key_left] != 0) {
                    if (!key_pressed) {
                        if (menu_pos == 0) {
                            vol_music -= 0.1f;
                            vol_music = max(0, min(1, vol_music));
                            main_st->SetVol(0.5f * vol_music);
                            menu_st->SetVol(0.5f * vol_music);
                        }
                        if (menu_pos == 1) {
                            vol_sfx -= 0.1f;
                            vol_sfx = max(0, min(1, vol_sfx));
                            s_fire->SetVol(vol_sfx);
                            carDT->s_expl->SetVol(vol_sfx);
                            destrDT->sound_d->SetVol(vol_sfx);
                        }
                        key_pressed = true;
                    }
                }
                else if ((keys[key_space] != 0) || (keys[key_enter] != 0)) {
                    if (!key_pressed) {
                        key_pressed = true;
                        if (menu_pos == 3) screen = 2;
                        if (menu_pos == 2) {
                            if (!isFullscreen) {
                                changeStateFlag = true;
                                isFullscreen = true;
                                DestroyWindow(hWnd);
                                windowFullscreen(hInstance, nCmdShow, WndProc);
                                UpdateSize(hWnd);
                            }
                            else {
                                changeStateFlag = true;
                                isFullscreen = false;
                                DestroyWindow(hWnd);
                                windowWindowed(hInstance, nCmdShow, WndProc);
                                UpdateSize(hWnd);
                            }
                        }
                    }
                }
                else {
                    key_pressed = false;
                }
                menu_pos = max(min(menu_pos, 3), 0);
            }
        }
        break;

        case 5: //tutorial
        {
            scr_story->drawImg(hCmpDC, { 0,0 });
            if (isActive) {
                if (keys[key_esc] != 0) {
                    screen = 2;
                }

                if ((keys[key_space] != 0) || (keys[key_enter] != 0)) {
                    if (!key_pressed) {
                        key_pressed = true;
                        screen = 6;
                    }
                }
                else {
                    key_pressed = false;
                }
            }
        }
        break;
        case 6: //tutorial pt2
        {


            scr_controls->drawImg(hCmpDC, { 0,0 });
            if (isActive) {
                if (keys[key_esc] != 0) {
                    screen = 2;
                }

                if ((keys[key_space] != 0) || (keys[key_enter] != 0)) {
                    if (!key_pressed) {
                        key_pressed = true;
                        screen = 2;
                    }
                }
                else {
                    key_pressed = false;
                }
            }
        }
        break;

        case -1: //gameplay init
        {
            scr_trgt_open = true;
            tab_key_flag = false;
            scr_trgt_timing = clock();
            target_car_color = EnCarColor(rand() % 9);
            target_car_color2 = EnCarColor(rand() % 9);
            target_car_drawing_type = (rand() % DECOR_NM_AMOUNT + 1);
            target_timeing = clock();

            score = 0;
            carDT->targets_destroyed = 0;

            player->loc = vec2(10, 0);
            player->speed = vec2(0, -43.f);
            player_last_fire = 0;
            player->hp = 15;
            player->death_timing = 0.0f;
            player->death_water = false;
            player->death_explosion = false;
            player->bulletholes.clear();

            bg->biome = EnBg::grass;
            bg->grid_count = 0;
            bg->road = 0;
            bg->next_road = 0;

            for (size_t i = 0; i < player_bullets.size(); i++) {
                delete player_bullets[i];
            }
            player_bullets.clear();
            for (size_t i = 0; i < bullets.size(); i++) {
                delete bullets[i];
            }
            bullets.clear();
            for (size_t i = 0; i < cars.size(); i++) {
                delete cars[i];
            }
            cars.clear();

            car_interval = 0.f;
            t_car_swapned = 0;

            for (size_t i = 0; i < destr.size(); i++) {
                delete destr[i];
            }
            destr.clear();
            for (size_t i = 0; i < destr_dead.size(); i++) {
                delete destr_dead[i];
            }
            destr_dead.clear();

            last_destr_y = 0;
            last_destr_fence = 0;
            last_destr_trees = 0;

            menu_st->Stop();
            main_st->Play();
            screen = 0;
        }
            break;

        case 0: //gameplay
        {
            //reading input
            if (isActive) {
                if (keys[key_esc] != 0) {
                    screen = 2;
                    main_st->Stop();
                    menu_st->Play();
                }

                if (keys[key_a] != 0 || keys[key_left] != 0) player->speed.x = -15;
                else if (keys[key_d] != 0 || keys[key_right] != 0) player->speed.x = 15;
                else player->speed.x = 0;

                if (keys[key_w] != 0 || keys[key_up] != 0) player->speed.y -= 14.f * dt;
                if (keys[key_s] != 0 || keys[key_down] != 0) player->speed.y += 14.f * dt;

                if (float(start_t - player_last_fire) / CLOCKS_PER_SEC > PLAYER_FIRE_DELAY) { //player fire
                    if (keys[key_semicolon] != 0 || keys[key_space] != 0) {
                        bullet* b = new bullet(player->loc + vec2(player->size.x / 2.f, 0.f), vec2(0, -150));
                        player_bullets.push_back(b);
                        player_last_fire = start_t;
                        s_fire->Play();
                    }
                }

                if (keys[key_tab] != 0) {   //open ui
                    if (!tab_key_flag) {
                        tab_key_flag = true;
                        scr_trgt_open = !scr_trgt_open;
                        scr_trgt_timing = start_t;
                    }
                }
                else {
                    tab_key_flag = false;
                }
            }

            //score
            if (carDT->targets_destroyed > score) {
                score = carDT->targets_destroyed;
                scr_trgt_timing = start_t;
                target_car_color = EnCarColor(rand() % 9);
                target_car_color2 = EnCarColor(rand() % 9);
                target_car_drawing_type = (rand() % DECOR_NM_AMOUNT + 1);
                target_timeing = start_t;
                scr_trgt_open = true;
            }

            if (player->death_timing > 1.5) {
                screen = 3;
                main_st->Stop();
                menu_st->Play();
            }

            //car spawn
            if (float(start_t - t_car_swapned) / CLOCKS_PER_SEC > max(12 - player->speed.y / -10. - car_interval, 1.)) {
                EnCarColor new_car_color = EnCarColor(rand() % 9);
                EnCarColor new_car_color2 = EnCarColor(rand() % 9);
                int new_car_drawing_type = (rand() % DECOR_NM_AMOUNT + 1);

                bool sp_t = false;

                if (((rand() % 30 > 27) || (float(start_t - target_timeing) / CLOCKS_PER_SEC > 50)) && (bg->biome != EnBg::water && !(bg->next_biome == EnBg::water && bg->grid_count % 10 > 5))) {
                    target_timeing = start_t;
                    new_car_color = target_car_color;
                    new_car_color2 = target_car_color2;
                    new_car_drawing_type = target_car_drawing_type;
                    sp_t = true;
                }
                else {
                    if (new_car_color == target_car_color && new_car_color2 == target_car_color2 && new_car_drawing_type == target_car_drawing_type) {
                        new_car_color = EnCarColor((target_car_color + 1) % 9);
                    }
                }



                car* c = new car(EnCarType::normal, new_car_color, new_car_drawing_type, new_car_color2, carDT);
                if (sp_t) c->is_trgt = true;
                c->loc = cam + vec2(0.f, -32);
                c->speed.y = -40.f + (rand() % 20);
                car_interval = ((100.f / (rand() % 100)) * 1.5f - 0.5f);
                t_car_swapned = start_t;

                int l = rand() % 4;
                int ln = bg->line(cam, c->loc);
                bool s = true;
                while (s)
                    switch (l) {
                    case 0:
                        if (!(ln & 0b1000))l++;
                        else {
                            s = false;
                        }
                        break;
                    case 1:
                        if (!(ln & 0b0100))l++;
                        else {
                            s = false;
                        }
                        break;
                    case 2:
                        if (!(ln & 0b0010))l++;
                        else {
                            s = false;
                        }
                        break;
                    case 3:
                        if (!(ln & 0b0001))l = 0;
                        else {
                            s = false;
                        }
                        break;
                    default:
                        break;
                    }

                c->loc.x = 10.f + (l * 12);
                c->target_ln = l;

                cars.push_back(c);
            }

            //destructive spawn
            if (bg->biome != EnBg::water && !(bg->next_biome == EnBg::water && bg->grid_count % 10 > 5)) {
                //streetlights
                if (last_destr_y - cam.y > 45) {
                    destructive* d = new destructive(hCmpDC, vec2(4.f, cam.y - 10.f), 0, destrDT);
                    destr.push_back(d);
                    d = new destructive(hCmpDC, vec2(48.5f, cam.y - 10.f), 1, destrDT);
                    destr.push_back(d);
                    last_destr_y = cam.y - 10.f;
                }
                //fences
                if (last_destr_fence - cam.y > 140) {
                    int d_type = 2 + (rand() % 2);
                    float d_width = destrDT->collider_szs[d_type].x;
                    for (size_t i = 0; i < 10; i++) {
                        destructive* d = new destructive(hCmpDC, vec2(-13.f - d_width * i, cam.y - 10.f), d_type, destrDT);
                        destr.push_back(d);
                        d = new destructive(hCmpDC, vec2(63 + d_width * i, cam.y - 10.f), d_type, destrDT);
                        destr.push_back(d);
                        last_destr_fence = cam.y - 10.f;
                    }
                }
                //trees
                if (last_destr_trees - cam.y > 35) {
                    if (bg->biome == EnBg::sand) {
                        destructive* d = new destructive(hCmpDC, vec2(-rand() % 100 - 13.f, cam.y - 10.f), 4, destrDT);
                        destr.push_back(d);
                        d = new destructive(hCmpDC, vec2(63.f + (rand() % 100), cam.y - 10.f), 4, destrDT);
                        destr.push_back(d);
                        last_destr_trees = cam.y - 10.f + rand() % 30;
                    }
                }
            }

            //movement
            player->move(dt);
            player->envTick(cam, bg->lines_Now(cam, player->loc + (player->size / 2)), bg->is_water(cam, player->loc + (player->size / 2)), 0);

            for (size_t i = 0; i < cars.size(); i++) {
                cars[i]->aiTick(cam, dt, bg->line(cam, cars[i]->loc), bg->is_water(cam, cars[i]->loc + (cars[i]->size / 2)));
                cars[i]->envTick(cam, bg->lines_Now(cam, cars[i]->loc + (cars[i]->size / 2)), bg->is_water(cam, cars[i]->loc + (cars[i]->size / 2)), 0);
                cars[i]->move(dt);

                if (cars[i]->death_explosion || cars[i]->death_water) continue;

                //collision checks
                for (size_t j = 0; j < cars.size(); j++) {
                    if (j == i)continue;
                    cars[i]->collision_check(cars[j]);
                }
                for (size_t j = 0; j < destr.size(); j++) {
                    if (destr[j]->collision_check(cars[i])) {
                        destr_dead.push_back(destr[j]);
                        destr.erase(destr.begin() + j);
                        j--;
                    };
                }
                for (size_t j = 0; j < player_bullets.size(); j++) {
                    if (player_bullets[j]->collision_check(cars[i])) {
                        delete player_bullets[j];
                        player_bullets.erase(player_bullets.begin() + j);
                        j--;
                    }
                }
                cars[i]->collision_check(player);

                //fire bullets
                if ((float(start_t - cars[i]->fire_timeing) / CLOCKS_PER_SEC > 4.f) && (cars[i]->anger > 1)) {
                    vec2 b_dir = (player->loc - cars[i]->loc);
                    b_dir.Normalise();
                    bullet* b = new bullet(cars[i]->loc + (cars[i]->size / 2.f), b_dir * 1.3f);
                    bullets.push_back(b);
                    cars[i]->fire_timeing = start_t;
                    s_fire->Play();
                }
            }

            //bullets movement
            for (size_t i = 0; i < player_bullets.size(); i++) {
                player_bullets[i]->move(dt);
            }
            for (size_t i = 0; i < bullets.size(); i++) {
                bullets[i]->move(dt);
            }

            //deleting out of range cars
            for (size_t i = 0; i < cars.size(); i++) {
                if (((cars[i]->loc - cam).y > 512) ||
                    ((cars[i]->loc - cam).y < -64)) {
                    delete cars[i];
                    cars.erase(cars.begin() + i);
                    i--;
                }
            }

            //camera position
            cam = { player->loc.x - 28.f, player->loc.y - 46.f };

            //drawing
            bg->draw(hCmpDC, cam);

            for (size_t i = 0; i < destr_dead.size(); i++) {
                destr_dead[i]->draw(hCmpDC, cam);
                if (destr_dead[i]->pos.y - cam.y > 96) {
                    delete destr_dead[i];
                    destr_dead.erase(destr_dead.begin() + i);
                    i--;
                }
            }

            player->draw(hCmpDC, cam);

            for (size_t i = 0; i < cars.size(); i++) {
                cars[i]->draw(hCmpDC, cam);
            }

            for (size_t i = 0; i < player_bullets.size(); i++) {
                player_bullets[i]->draw(hCmpDC, cam);
                if ((player_bullets[i]->pos - cam).y > 64 || (player_bullets[i]->pos - cam).y < -64) {
                    delete player_bullets[i];
                    player_bullets.erase(player_bullets.begin() + i);
                    i--;
                }
            }
            for (size_t i = 0; i < bullets.size(); i++) {
                bullets[i]->draw(hCmpDC, cam);
                if (bullets[i]->collision_check(player)) {
                    delete bullets[i];
                    bullets.erase(bullets.begin() + i);
                    i--;
                }
                else {
                    if ((bullets[i]->pos - cam).y > 64) {
                        delete bullets[i];
                        bullets.erase(bullets.begin() + i);
                        i--;
                    }
                }
            }

            //draw destructive
            for (size_t i = 0; i < destr.size(); i++) {
                destr[i]->draw(hCmpDC, cam);
                if (destr[i]->collision_check(player)) {
                    destr_dead.push_back(destr[i]);
                    destr.erase(destr.begin() + i);
                    i--;
                    continue;
                }
                if (destr[i]->pos.y - cam.y > 96) {
                    delete destr[i];
                    destr.erase(destr.begin() + i);
                    i--;
                }
            }

            //draw ui
            if ((bg->next_biome == EnBg::water) && (bg->grid_count % 10 > 3)) {
                em_water_img->drawImgT(hCmpDC, { 5,5 });
            }


            if (scr_trgt_open) {
                scr_trgt_img[size_t((float)start_t / CLOCKS_PER_SEC * 10.f) % 8]->drawImg(hCmpDC, { 0, 25 });
                scr_score->drawImg(hCmpDC, { 0,0 });
                numbers[score / 10]->drawImgT(hCmpDC, { 23, 1 });
                numbers[score % 10]->drawImgT(hCmpDC, { 27, 1 });

                carDT->cars[0][target_car_color]->drawImgT(hCmpDC, { 4, 44 });
                if (target_car_drawing_type < DECOR_NM_AMOUNT) carDT->cars_d[target_car_color2][target_car_drawing_type]->drawImgT(hCmpDC, { 4, 44 });

                if (float(start_t - scr_trgt_timing) / CLOCKS_PER_SEC > 5.f) scr_trgt_open = false;

            }

            break;
        }
        case 1:
            break;
        }

        //redraw
        if(isFullscreen) StretchBlt(hdc, screenCoords.X / 2 - (screenCoords.Y) / 2, 0, screenCoords.Y, screenCoords.Y, hCmpDC, 0, 0, 64, 64, SRCCOPY);
        else StretchBlt(hdc, screenCoords.X / 2 - (screenCoords.Y - Y_OFFS - X_OFFS) / 2, Y_OFFS, screenCoords.Y - Y_OFFS - X_OFFS, screenCoords.Y - Y_OFFS - X_OFFS, hCmpDC, 0, 0, 64, 64, SRCCOPY);

        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        int end_t = clock();
        dt = (float(end_t) - float(start_t)) / CLOCKS_PER_SEC;
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message) {
    case WM_PAINT:
        break;
    case WM_SIZE:
        UpdateSize(hWnd);
        break;
    case WM_KILLFOCUS:
        isActive = false;
        break;
    case WM_SETFOCUS:
        isActive = true;
        break;
    case WM_DESTROY:
        if (!changeStateFlag) {
            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}