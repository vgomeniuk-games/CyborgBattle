#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "SDL.h"
#include "entity.h"

class LivingEntity : public Entity {
public:
	virtual void updateHitBox();
	virtual void updateDamage() = 0;
	virtual void die() = 0;
	virtual void updateInvincibleTimer();

	void draw() override;

protected:
	int hp;
	int hpMax;
	SDL_Rect hitBox;
	int damage{ 0 };
	float invincibleTimer{ 0 };
};
#endif // !LIVINGENTITY_H
