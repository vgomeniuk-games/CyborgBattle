#ifndef WALL_H
#define WALL_H

#include "entity.h"
#include "animationSet.h"

class Wall : public Entity {
public:
	Wall(AnimationSet* animations, int x, int y);
	void update();
	void changeAnimation(/*Direction*/int newState, bool reset);
	inline void updateCollisions() { /* No nned to update collisions on walls */ }

};

#endif // !WALL_H
