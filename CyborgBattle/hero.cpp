#include "hero.h"
#include "globals.h"

const std::string HERO_ANIM_UP = "up";
const std::string HERO_ANIM_DOWN = "down";
const std::string HERO_ANIM_LEFT = "left";
const std::string HERO_ANIM_RIGHT = "right";
const std::string HERO_ANIM_IDLE_UP = "idleUp";
const std::string HERO_ANIM_IDLE_DOWN = "idleDown";
const std::string HERO_ANIM_IDLE_LEFT = "idleLeft";
const std::string HERO_ANIM_IDLE_RIGHT = "idleRight";
const std::string HERO_ANIM_SLASH_UP = "slashUp";
const std::string HERO_ANIM_SLASH_DOWN = "slashDown";
const std::string HERO_ANIM_SLASH_LEFT = "slashLeft";
const std::string HERO_ANIM_SLASH_RIGHT = "slashRight";
const std::string HERO_ANIM_DASH_UP = "dashUp";
const std::string HERO_ANIM_DASH_DOWN = "dashDown";
const std::string HERO_ANIM_DASH_LEFT = "dashLeft";
const std::string HERO_ANIM_DASH_RIGHT = "dashRight";
const std::string HERO_ANIM_DASH_DIE = "die";

const int AnimationState::Idle = 0;
const int AnimationState::Move = 1;
const int AnimationState::Slash = 2;
const int AnimationState::Dash = 3;
const int AnimationState::Dead = 4;


Hero::Hero(AnimationSet* animSet) {
	this->animations = animSet;
	this->type = "hero";

	// Setup default Hero values
	this->x = Globals::WIDTH / 2;
	this->y = Globals::HEIGHT / 2;
	this->moveSpeed = 0;
	this->moveSpeedMax = 80;
	this->hp = 20;
	this->hpMax = 20;
	this->damage = 0;
	this->collisionBoxWidth = 20;
	this->collisionBoxHeight = 24;
	this->collisionBoxYOffset = -20;
	this->direction = Direction::Down;
	changeAnimation(AnimationState::Idle, true);
}

void Hero::update() {

}

void Hero::slash() {

}

void Hero::dash() {

}

void Hero::die() {

}

void Hero::revive() {

}

void Hero::changeAnimation(/*AnimationStates*/int newState, bool reset) {

}

void Hero::updateAnimation() {

}

void Hero::updateDamages() {

}
