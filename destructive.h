#pragma once
#include <Windows.h>
#include <string>

#include "GUI.h"
#include "VectorMath.h"
#include "Sounds.h"

#include "car.h"

class destructive_data {
public:
	std::vector<std::wstring> names = {
		L"light_L",
		L"light_R",
		L"fence",
		L"fence2",
		L"cactus"
	};
	std::vector<std::vector<IMG*>> sprites;
	
	std::vector<float> heights = {
		22.f,
		22.f,
		5.f,
		13.f,
		9.f
	};
	std::vector<vec2> collider_szs = {
		vec2(2,2),
		vec2(2,2),
		vec2(8,2),
		vec2(16,2),
		vec2(7,3),
	};
	std::vector<vec2> collider_offs = {
		vec2(0,0),
		vec2(9,0),
		vec2(0,0),
		vec2(0,0),
		vec2(0,0),
	};

	sound* sound_d = new sound();

	destructive_data(HDC hdc);
};

class destructive{
public:
	destructive(HDC hdc, vec2 position, int t, destructive_data* DD);
	void draw(HDC hdc, vec2 cam );
	bool collision_check(car* other_car);

	vec2 pos;
	bool d = false;

	int type;

	destructive_data* Dt;
private:
};