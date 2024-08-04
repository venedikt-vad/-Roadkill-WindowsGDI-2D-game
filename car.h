#pragma once
#include <vector>
#include <string>
#include "GUI.h"
#include "VectorMath.h"
#include "Sounds.h"

enum EnCarType{
	normal = 0,
	truck = 1,
	bus = 2,
	cistern = 3
};

enum EnCarColor {
	red = 0,
	orange = 1,
	yellow = 2,
	green = 3,
	blue = 4,
	purple = 5,
	pink = 6,
	black = 7,
	white = 8
};

#define COLLISION_BOUNCE 0.f

#define DECOR_NM_AMOUNT 6

class carDATA {
public:
	carDATA(HDC hdc);

	sound* s_expl = new sound;

	std::vector<std::vector<IMG*>> cars;

	std::vector<std::vector<IMG*>> cars_d;

	std::vector<IMG*> img_death_water;
	std::vector<IMG*> img_death_explosion;

	int targets_destroyed = 0;
};

class car{
public:
	vec2 loc;
	vec2 speed;

	vec2 size;

	float speed_max = 80.f;
	float steer_max = 15.f;
	float steer_accel = 10.f;
	float speed_accel = 12.f;

	car(EnCarType t, EnCarColor c, int d, EnCarColor decor_c, carDATA* cd);
	void move(float dt);
	void collision_check(car* other_car);
	void draw(HDC hdc, vec2 cam);
	void aiTick(vec2 cam, float dt, int bg_lines, bool is_water);
	void envTick(vec2 cam, int bg_lines, bool is_water, float bg_drag);
	void damage(int amount);

	bool is_trgt = false;

	int hp = 15;
	bool death_water = false;
	bool death_explosion = false;
	bool dd = false;
	float death_timing = 0.0f;

	int target_ln;
	int anger = 0;
	bool car_in_front = false;
	int fire_timeing = 0;

	std::vector<vec2> bulletholes;

private:
	EnCarType type;
	EnCarColor color = EnCarColor::red;
	EnCarColor color_d = EnCarColor::red;
	int decor;
	
	carDATA* car_data;

	bool is_on_road(int bg_lines);
	bool was_on_road = true;
};

