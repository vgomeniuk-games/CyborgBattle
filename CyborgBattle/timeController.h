#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#include <cstdint>
#include <SDL.h>

/* Possible game states */
enum class State { Play, Pause };

class TimeController {
public:
	TimeController();

	/* Update lastUpdate counter and deltaTime */
	void updateTime();

	/* State constrolling functions */
	void pause();
	void resume();
	void reset();

	float getDeltaTime();

public:
	static TimeController controller;

private:
	float deltaTime /*seconds*/;
	State state;
	Uint32 lastUpdate;  // Last counter of ticks

};

#endif // !TIMECONTROLLER_H
