#ifndef HERO_H
#define HERO_H

#include <string>
#include "entity.h"
#include "livingEntity.h"
#include "animationSet.h"


struct AnimationState { static const int Idle; static const int Move; static const int Slash; static const int Dash; static const int Dead; };

class Hero : public LivingEntity {
public:
	Hero(AnimationSet* animSet);
	void update();
	void slash();
	void dash();
	void die();
	void revive();
	void changeAnimation(/*AnimationStates*/int newState, bool reset);
	void updateAnimation();
	void updateDamages();

private:
	static const std::string HERO_ANIM_UP;
	static const std::string HERO_ANIM_DOWN;
	static const std::string HERO_ANIM_LEFT;
	static const std::string HERO_ANIM_RIGHT;
	static const std::string HERO_ANIM_IDLE_UP;
	static const std::string HERO_ANIM_IDLE_DOWN;
	static const std::string HERO_ANIM_IDLE_LEFT;
	static const std::string HERO_ANIM_IDLE_RIGHT;
	static const std::string HERO_ANIM_SLASH_UP;
	static const std::string HERO_ANIM_SLASH_DOWN;
	static const std::string HERO_ANIM_SLASH_LEFT;
	static const std::string HERO_ANIM_SLASH_RIGHT;
	static const std::string HERO_ANIM_DASH_UP;
	static const std::string HERO_ANIM_DASH_DOWN;
	static const std::string HERO_ANIM_DASH_LEFT;
	static const std::string HERO_ANIM_DASH_RIGHT;
	static const std::string HERO_ANIM_DASH_DIE;
};

#endif // !HERO_H
