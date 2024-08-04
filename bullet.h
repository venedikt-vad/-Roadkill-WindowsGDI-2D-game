#pragma once
#include <Windows.h>

#include "car.h"

class bullet{
public:
	bullet(vec2 position, vec2 velocity);

	void move(float dt);
	bool collision_check(car* other_car);
	void draw(HDC hdc, vec2 cam);

	vec2 pos;
	vec2 vel;
};

