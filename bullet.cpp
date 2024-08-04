#include "bullet.h"

bullet::bullet(vec2 position, vec2 velocity){
	pos = position;
	vel = velocity;
}

void bullet::move(float dt){
	pos = pos + (vel * dt);
}

bool bullet::collision_check(car* other_car){
	if ((pos.x > other_car->loc.x) && (pos.x < other_car->loc.x + other_car->size.x) && (pos.y > other_car->loc.y) && (pos.y < other_car->loc.y + other_car->size.y)) {
		other_car->damage(1);
		return true;
	}
	return false;
}

void bullet::draw(HDC hdc, vec2 cam){
	SetPixel(hdc, (int)floor(pos.x - cam.x), (int)floor(pos.y - cam.y), RGB(255,50,0));
}
