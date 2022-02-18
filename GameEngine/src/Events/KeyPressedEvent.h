#pragma once
#include <SDL_keycode.h>
#include "../EventBus/Event.h"

class KeyPressedEvent :public Event {
public:
	SDL_Keycode key;
	KeyPressedEvent(SDL_Keycode key) {
		this->key = key;
	}
};