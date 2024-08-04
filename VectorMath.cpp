#include "VectorMath.h"

float Q_rsqrt(float number){
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		uint32_t i;
	} conv = { number }; // member 'f' set to value of 'number'.
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= threehalfs - x2 * conv.f * conv.f;
	return conv.f;
}

//class vec2
vec2::vec2(){
}

vec2::vec2(float X, float Y){
	x = X;
	y = Y;
}

COORD vec2::GetPos(){
	return {short(x),short(y)};
}

float vec2::GetLength(){
	return float(sqrt(pow(x,2) + pow(y,2)));
}

void vec2::Normalise(){
	float n = Q_rsqrt(float(pow(x, 2) + pow(y, 2)));
	x = x * n;
	y = y * n;
}

vec2 vec2::operator+(const vec2& b){
	return vec2(this->x + b.x, this->y + b.y);
}

vec2 vec2::operator-(const vec2& b){
	return vec2(this->x - b.x, this->y - b.y);
}

vec2 vec2::operator*(float b){
	return vec2(this->x * b, this->y * b);
}

vec2 vec2::operator/(float b){
	return vec2(this->x / b, this->y / b);
}

//endclass vec2

bool rectOverlap(vec2 a1, vec2 a2, vec2 b1, vec2 b2){
    if (a1.x > b2.x || b1.x > a2.x)
        return false;
 
    if (a2.y < b1.y || b2.y < a1.y)
        return false;
 
    return true;
}