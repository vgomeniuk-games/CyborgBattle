#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "livingEntity.h"
#include "animationSet.h"

struct HeroState {
	static const int Idle;
	static const int Move;
	static const int Slash;
	static const int Dash;
	static const int Dead;
};

class Hero : public LivingEntity {
public:
	Hero(AnimationSet* animSet);
	void update();
	void slash();
	void dash();
	void die();
	void revive();
	void changeAnimation(/*HeroState*/int newState, bool reset);
	void updateAnimation();
	void updateDamages();
	inline int getState() { return state; }
	inline void move(bool move) { moving = move; }

protected:
	/*AnimationState*/int state;

};

#endif // !HERO_H
