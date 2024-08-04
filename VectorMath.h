#pragma once
#include <Windows.h>
#include <math.h>
#include <cstdint>

class vec2 {
public:
	float x = 0.f;
	float y = 0.f;

	vec2();
	vec2(float x, float y);

	COORD GetPos();
	float GetLength();
	void Normalise();

	vec2 operator+(const vec2& b);
	vec2 operator-(const vec2& b);
	vec2 operator*(float b);
	vec2 operator/(float b);
};

bool rectOverlap(vec2 a1, vec2 a2, vec2 b1, vec2 b2);