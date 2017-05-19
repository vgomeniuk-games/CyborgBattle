#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#include <cstdint>
#include <SDL.h>

/* Possible game states */
enum State { Play, Pause };

class TimeController {
public:
	TimeController();

	/* Update lastUpdate counter and deltaTime */
	void updateTime();

	/* State constrolling functions */
	void pause();
	void resume();
	void reset();

private:
	static TimeController controller;
	State state;
	Uint32 lastUpdate;  // Last counter of ticks
	float deltaTime /*seconds*/;

};

#endif // !TIMECONTROLLER_H
