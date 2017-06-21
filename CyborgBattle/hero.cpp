#include <vector>
#include <string>

#include "hero.h"
#include "globals.h"
#include "timeController.h"


const int HeroState::Idle = 0;
const int HeroState::Move = 1;
const int HeroState::Slash = 2;
const int HeroState::Dash = 3;
const int HeroState::Dead = 4;

std::vector<std::vector<std::string>> HeroAnimations = {
	/* Direction:	Up,			Down			Left			Right */
	/* Idle */	{ "idleUp",		"idleDown",		"idleLeft",		"idleRight" },
	/* Move */	{ "up",			"down",			"left",			"right" },
	/* Slash */	{ "slashUp",	"slashDown",	"slashLeft",	"slashRight" },
	/* Dash */	{ "dashUp" ,	"dashDown",		"dashLeft",		"dashRight" },
	/* Die */	{ "die",		"die",			"die",			"die"}
};


Hero::Hero(AnimationSet* animSet, int x, int y) {
	this->animations = animSet;
	this->type = "hero";
	this->x = x;
	this->y = y;

	// Setup default Hero values
	this->moveSpeed = 0;
	this->moveSpeedMax = 80;
	this->hpMax = 20;
	this->damage = 0;
	this->collisionBoxWidth = 20;
	this->collisionBoxHeight = 24;
	this->collisionBoxYOffset = -20;
	this->direction = Direction::Down;
	revive();
	updateCollisionBox();
}

void Hero::update() {
	// Check if dead
	if (hp < 1 && state != HeroState::Dead) {
		changeAnimation(HeroState::Dead, true);
		die();
		return;  // TODO
	}
	// Update hero
	updateCollisionBox();
	updateMovememnt();
	updateCollisions();
	updateHitBox();
	updateDamage();
	updateAnimation();
	updateInvincibleTimer();
}

void Hero::slash() {
	if (hp > 0 && (state == HeroState::Move || state == HeroState::Idle)) {
		moving = false;
		frameTimer = 0;
		changeAnimation(HeroState::Slash, true);
		// TODO add sound
	}
}

void Hero::dash() {
	if (hp > 0 && (state == HeroState::Move || state == HeroState::Idle)) {
		moving = false;
		frameTimer = 0;

		// push the player in the travel direction
		slideAngle = angle;
		slideAmount = 300;
		invincibleTimer = 0.1;

		changeAnimation(HeroState::Dash, true);
		// TODO add sound
	}
}

void Hero::die() {
	moving = false;
	changeAnimation(HeroState::Dead, true);
}

void Hero::revive() {
	hp = hpMax;
	changeAnimation(HeroState::Idle, true);
	moving = false;
	x = Globals::WIDTH / 2;
	y = Globals::HEIGHT / 2;
	slideAmount = 0;
}

void Hero::changeAnimation(/*AnimationStates*/int newState, bool reset) {
	state = newState;
	currentAnimation = animations->getAnimation(HeroAnimations[newState][direction]);
	currentFrame = currentAnimation->getFrame( reset ? 0 : currentFrame->getFrameNumber() );
}

void Hero::updateAnimation() {
	if (currentFrame == nullptr || currentAnimation == nullptr) {
		return;
	}
	// Set correct state if stop moving
	if (state == HeroState::Move && !moving) {
		changeAnimation(HeroState::Idle, true);
	}
	// Change frame if current one's duration exeed
	frameTimer += TimeController::controller.getDeltaTime();
	if (frameTimer >= currentFrame->getDuration()) {
		// Animation end
		if (currentFrame->getFrameNumber() == currentAnimation->getEndFrameNumber()) {
			// Unless hero is slashing || dashing - restart animation from start
			switch (state)
			{
			case HeroState::Slash:
			case HeroState::Dash:
				changeAnimation(HeroState::Move, true);
				break;
			default:
				currentFrame = currentAnimation->getFrame(0);
			}
		}
		// Move to next frame
		else {
			currentFrame = currentAnimation->getNextFrame(currentFrame);
		}
		// Restart frame duration timer
		frameTimer = 0;
	}
}

void Hero::updateDamage() {
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity : Entity::entities) {
			if (entity->isActive() && entity->getType() == "enemy") {
				LivingEntity* enemy = (LivingEntity*)entity;
				if (enemy->getDamage() > 0 && Entity::checkCollision(collisionBox, enemy->getHitBox())) {
					hp -= enemy->getDamage();
				}
				// If still alive
				if (hp > 0) {
					invincibleTimer = 0.3;
					// TODO play sound
				}
				slideAngle = Entity::getAngle(entity, this);
				slideAmount = 200;
			}
		}
	}
}
