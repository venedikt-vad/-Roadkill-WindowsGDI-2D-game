#include "car.h"

using namespace std;

carDATA::carDATA(HDC hdc) {
	s_expl->Open(L"res/sounds/s_explosion.wav");

	for (size_t i = 0; i < 1; i++) {
		vector<IMG*> v;
		wstring car_type = L"res/cars/";
		switch (EnCarType(i)) {
		case normal:
			car_type.append(L"nm/");
			break;
		case truck:
			car_type.append(L"tr/");
			break;
		case bus:
			car_type.append(L"bs/");
			break;
		case cistern:
			car_type.append(L"cs/");
			break;
		default:
			break;
		}

		for (size_t j = 0; j < 9; j++) {
			wstring carColor = L"car_";
			switch (EnCarColor(j)) {
			case red:
				carColor.append(L"red");
				break;
			case orange:
				carColor.append(L"orange");
				break;
			case yellow:
				carColor.append(L"yellow");
				break;
			case green:
				carColor.append(L"green");
				break;
			case blue:
				carColor.append(L"blue");
				break;
			case purple:
				carColor.append(L"purple");
				break;
			case pink:
				carColor.append(L"pink");
				break;
			case black:
				carColor.append(L"black");
				break;
			case white:
				carColor.append(L"white");
				break;
			default:
				break;
			}
			carColor.append(L".bmp");
			wstring IMGpath = car_type + carColor;
			IMG* c = new IMG(IMGpath.c_str(), hdc);
			v.push_back(c);
		}
		cars.push_back(v);
	}

	
	for (size_t j = 0; j < 9; j++) {
		wstring carColor = L"res/cars/nm/decor/";
		switch (EnCarColor(j)) {
		case red:
			carColor.append(L"red");
			break;
		case orange:
			carColor.append(L"orange");
			break;
		case yellow:
			carColor.append(L"yellow");
			break;
		case green:
			carColor.append(L"green");
			break;
		case blue:
			carColor.append(L"blue");
			break;
		case purple:
			carColor.append(L"purple");
			break;
		case pink:
			carColor.append(L"pink");
			break;
		case black:
			carColor.append(L"black");
			break;
		case white:
			carColor.append(L"white");
			break;
		default:
			break;
		}
		carColor.append(L"/");

		std::vector<IMG*> v;

		for (size_t i = 0; i < DECOR_NM_AMOUNT; i++) {
			wstring p = carColor;
			p.append(to_wstring(i));
			p.append(L".bmp");
			IMG* im = new IMG(p.c_str(), hdc);
			v.push_back(im);
		}
		cars_d.push_back(v);
	}


	for (size_t i = 0; i < 10; i++){
		wstring p = L"res/death/dw/dw_0";
		p.append(to_wstring(i));
		p.append(L".bmp");
		IMG* im = new IMG(p.c_str(), hdc);
		img_death_water.push_back(im);
	}
	for (size_t i = 0; i < 9; i++) {
		wstring p = L"res/death/de/de_0";
		p.append(to_wstring(i));
		p.append(L".bmp");
		IMG* im = new IMG(p.c_str(), hdc);
		img_death_explosion.push_back(im);
	}
}

car::car(EnCarType t, EnCarColor c, int d, EnCarColor decor_c, carDATA* cd){
	type = t;
	color = c;
	color_d = decor_c;
	decor = d;
	car_data = cd;
	size = vec2(9, 16);
	target_ln = 0;
}

void car::move(float dt){
	speed.y = min(speed.y, -1.f);
	speed.y = max(-speed_max, speed.y);
	speed.x = min(steer_max, speed.x);
	speed.x = max(-steer_max, speed.x);

	if ((death_water || death_explosion)) { 
		death_timing += dt;
		return;
	}
	loc = loc + (speed * dt);
}

void car::collision_check(car* other_car){
	if (rectOverlap(loc, loc + size, other_car->loc, other_car->loc + other_car->size)) {
		if (abs(other_car->loc.y - loc.y) / other_car->size.y > abs(other_car->loc.x - loc.x) / other_car->size.x) {
			if (abs(other_car->loc.x - loc.x) < size.x - 1) {
				if (loc.y < other_car->loc.y) {
					other_car->loc.y = loc.y + size.y;
				}
				else {
					loc.y = other_car->loc.y + other_car->size.y;
					car_in_front = true;
				}
				float n_s = (speed.y + other_car->speed.y) / 2.f;
				speed.y = n_s - COLLISION_BOUNCE;
				other_car->speed.y = n_s + COLLISION_BOUNCE;
			};
		}
		else {
			if (abs(other_car->loc.y - loc.y) < size.y - 1) {
				if (loc.x < other_car->loc.x) {
					if(abs(speed.x) > abs(other_car->speed.x)) other_car->loc.x = loc.x + size.x;
					else loc.x = other_car->loc.x - size.x;
				}
				else {
					if (abs(speed.x) > abs(other_car->speed.x)) other_car->loc.x = loc.x - size.x;
					else loc.x = other_car->loc.x + other_car->size.x;

					//loc.x = other_car->loc.x + other_car->size.x;
				}
				float n_s = (speed.x + other_car->speed.x) / 2.f;
				speed.x = n_s - COLLISION_BOUNCE;
				other_car->speed.x = n_s + COLLISION_BOUNCE;
			};
		}
	};
}

void car::draw(HDC hdc, vec2 cam){
	//if (!dd)
	if (death_water && death_timing > 1)return;
	if (death_explosion && death_timing > 1)return;

	if (!death_explosion || death_timing < 0.3) {
		car_data->cars[type][color]->drawImgT(hdc, { (short)((short)floor(loc.x - cam.x)), (short)((short)floor(loc.y - cam.y)) });
		if (decor < DECOR_NM_AMOUNT) car_data->cars_d[color_d][decor]->drawImgT(hdc, { (short)((short)floor(loc.x - cam.x)), (short)((short)floor(loc.y - cam.y)) });
		for (size_t i = 0; i < bulletholes.size(); i++){
			SetPixel(hdc, (short)floor(bulletholes[i].x + loc.x - cam.x), (short)floor(bulletholes[i].y + loc.y - cam.y), RGB(30, 30, 30));
		}
	}

	if (death_water) {
		int ln_now = int(floor((loc.x + (size.x / 2) - 8.f) / 12.f));
		loc.x = 10.f + (ln_now * 12);
		car_data->img_death_water[size_t(min(floor(death_timing * 10.f), 9))]->drawImgT(hdc, { (short)((short)floor(loc.x - cam.x)), (short)((short)floor(loc.y - cam.y)) });
	}
	else if (death_explosion) {
		car_data->img_death_explosion[size_t(min(floor(death_timing * 15.f), 8))]->drawImgT(hdc, { (short)((short)floor(loc.x - cam.x - 5)), (short)((short)floor(loc.y - cam.y - 2)) });
	}
}

void car::aiTick(vec2 cam, float dt, int bg_lines, bool is_water){
	

	int ln = bg_lines;
	int ln_now = int(floor((loc.x+(size.x/2) - 8.f) / 12.f));
		switch (ln_now) {
		case 0:
			if (!(ln & 0b1000)) {
				target_ln = ln_now + 1;		
			}
			break;
		case 1:
			if (!(ln & 0b0100)) {
				if (!(ln & 0b1000)) target_ln = ln_now +1;
				else target_ln = ln_now -1;
			}
			break;
		case 2:
			if (!(ln & 0b0010)) {
				if (!(ln & 0b0001)) target_ln = ln_now - 1;
				else target_ln = ln_now + 1;
			}
			break;
		case 3:
			if (!(ln & 0b0001)) {
				target_ln = ln_now - 1;
			}
			break;
		default:
			break;
		}

	

	if (anger>0){
		speed.y -= speed_accel * dt;
		if (anger == 5 && !is_water) {
			anger++;
			target_ln = rand()%20 -10;
		}

		if(anger<5){
			if (car_in_front) {
				speed.y += speed_accel * 1.3f * dt;
				switch (ln_now) {
				case 0:
					if ((ln & 0b0100)) {
						target_ln = ln_now + 1;
						car_in_front = false;
					}
					break;
				case 1:
					if ((ln & 0b0010)) {
						target_ln = ln_now + 1;
						car_in_front = false;
					}
					else if ((ln & 0b1000)) {
						target_ln = ln_now - 1;
						car_in_front = false;
					}
					break;
				case 2:
					if ((ln & 0b0001)) {
						target_ln = ln_now + 1;
						car_in_front = false;
					}
					else if ((ln & 0b0100)) {
						target_ln = ln_now - 1;
						car_in_front = false;
					}
					break;
				case 3:
					if ((ln & 0b0010)) {
						target_ln = ln_now - 1;
						car_in_front = false;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	//loc.x = 10.f + (target_ln * 12);

	if (abs(loc.x - (10.f + (target_ln * 12))) > 1.) {
		if ((loc.x - (10.f + (target_ln * 12))) < 0.) {
			speed.x = +steer_accel;
		}
		else {
			speed.x -= steer_accel;
		}

	}
	else {
		speed.x = 0;
	}
}

void car::envTick(vec2 cam, int bg_lines, bool is_water, float bg_drag){
	if ((death_water || death_explosion)) return;
	if (!is_on_road(bg_lines)) {
		if (is_water) {
			death_water = true;
			if (is_trgt) car_data->targets_destroyed++;
		}

		if (was_on_road) {
			was_on_road = false;
			//speed.y += 5.f;
			speed_max -= 5.f;
		}
	} else {
		if (!was_on_road) {
			was_on_road = true;
			speed_max += 5.f;
		}
	}
	dd = !is_on_road(bg_lines);
}

void car::damage(int amount){
	if ((death_water || death_explosion)) return;
	hp -= amount;
	anger += 1;
	bulletholes.push_back(vec2(float(rand()%(int)size.x), float(rand() % (int)size.y)));
	if (hp <= 0) { 
		death_explosion = true; 
		car_data->s_expl->Play();
		if (is_trgt) car_data->targets_destroyed++;
	}
}

bool car::is_on_road(int bg_lines){
	int ln = bg_lines;
	int ln_now = int(floor((loc.x + (size.x / 2.f) - 8.f) / 12.f));
	switch (ln_now) {
	case 0:
		if (!(ln & 0b1000)) {
			return false;
		}
		break;
	case 1:
		if (!(ln & 0b0100)) {
			return false;
		}
		break;
	case 2:
		if (!(ln & 0b0010)) {
			return false;
		}
		break;
	case 3:
		if (!(ln & 0b0001)) {
			return false;
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}
