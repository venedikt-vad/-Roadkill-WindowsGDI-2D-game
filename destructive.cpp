#include "destructive.h"

destructive::destructive(HDC hdc, vec2 position, int t, destructive_data* DD) {
	pos = position;
	type = t;
	Dt = DD;
}

void destructive::draw(HDC hdc, vec2 cam){
	if (!d) {
		Dt->sprites[type][0]->drawImgT(hdc, (pos - cam).GetPos());
	}else {
		Dt->sprites[type][1]->drawImgT(hdc, (pos - cam - vec2(0, Dt->heights[type])).GetPos());
	}
}

bool destructive::collision_check(car* other_car){
	if (d) return true;
	if (rectOverlap(pos + Dt->collider_offs[type], pos + Dt->collider_offs[type] + Dt->collider_szs[type], other_car->loc, other_car->loc + other_car->size)) {
		d = true;
		Dt->sound_d->Play();
		return true;
	}
	return false;
}

destructive_data::destructive_data(HDC hdc){
	for (size_t i = 0; i < names.size(); i++){
		std::vector<IMG*> v;
		
		std::wstring path = L"res/destructive/";
		path.append(names[i]);
		path.append(L".bmp");
		IMG* image = new IMG(path.c_str(), hdc);
		v.push_back(image);

		path = L"res/destructive/";
		path.append(names[i]);
		path.append(L"_d.bmp");
		image = new IMG(path.c_str(), hdc);
		v.push_back(image);

		sprites.push_back(v);
	}

	sound_d->Open(L"res/sounds/s_destruction.wav");
}
