#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "livingEntity.h"
#include "animationSet.h"


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

protected:
	/*AnimationState*/int state;

};

#endif // !HERO_H
