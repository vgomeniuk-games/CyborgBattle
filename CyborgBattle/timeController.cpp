#include "timeController.h"

TimeController::TimeController() : deltaTime{ 0 }, lastUpdate{ 0 }, state{State::Play} {

}

void TimeController::updateTime() {
	if (state == State::Pause) {
		deltaTime = 0;
	}
	else {
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		deltaTime = static_cast<float>(timeDiff) / 1000.0f;  // milliseconds => seconds
	}
	lastUpdate = SDL_GetTicks();
}

void TimeController::pause() {
	state = State::Pause;
}
void TimeController::resume() {
	state = State::Play;
}
void TimeController::reset() {
	deltaTime = 0;
	lastUpdate = SDL_GetTicks();
}
