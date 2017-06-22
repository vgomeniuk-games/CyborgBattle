#ifndef GLOB_H
#define GLOB_H

#include "livingEntity.h"
#include "animationSet.h"

struct GlobState {
	static const int Idle;
	static const int Move;
	static const int Attack;
	static const int Telegraph;
	static const int Dead;
};

struct GlobAI {
	static const int Normal;
	static const int Chase;
};

class Glob : public LivingEntity {
public:
	Glob(AnimationSet* animations, int x, int y);
	void update();
	void think();
	void telegraph();
	void attack();
	void die();
	void find();
	void changeAnimation(int newState, bool reset);
	void updateAnimation();
	void updateDamage();

private:
	static int killed;  // Keep track of N of killed Globs
	int state = GlobState::Idle;
	int aiState = GlobAI::Normal;
	float thinkTimer = 0;  // AI: Time to think on next action
	LivingEntity* target = nullptr;
};

#endif // !GLOB_H
