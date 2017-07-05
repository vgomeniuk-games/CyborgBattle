#include "globals.h"

const int Globals::WIDTH = 640;
const int Globals::HEIGHT = 352;
const int Globals::SCALE = 2;
const float Globals::PI = 3.14159f;
const bool Globals::DEBUG = false;
SDL_Renderer* Globals::renderer = nullptr;

std::string Globals::clipOffDataHeader(std::string data) {
	int pos = data.find(":", 0);
	if (pos != -1) {
		data = data.substr(pos + 1, data.length() - pos + 2);
	}
	return data;
}
