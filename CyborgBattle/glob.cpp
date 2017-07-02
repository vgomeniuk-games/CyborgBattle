#include <cstdlib>
#include <vector>
#include <string>
#include "glob.h"
#include "timeController.h"

const int GlobState::Idle = 0;
const int GlobState::Move = 1;
const int GlobState::Attack = 2;
const int GlobState::Telegraph = 3;
const int GlobState::Dead = 4;

const int GlobAI::Normal = 0;
const int GlobAI::Chase = 1;

int Glob::killed = 0;

std::vector<std::vector<std::string>> GlobAnimations = {
	/* Direction:		Up,				Down				Left				Right */
	/* Idle */		{ "idleUp",			"idleDown",			"idleLeft",			"idleRight" },
	/* Move */		{ "up",				"down",				"left",				"right" },
	/* Attack */	{ "attackUp",		"attackDown",		"attackLeft",		"attackRight" },
	/* Dash */		{ "telegraphUp" ,	"telegraphDown",	"telegraphLeft",	"telegraphRight" },
	/* Die */		{ "die",			"die",				"die",				"die" }
};

Glob::Glob(AnimationSet* animations, int x, int y, int invincible) {
	this->animations = animations;
	this->type = "enemy";
	this->x = x;
	this->y = y;
	this->moveSpeed = 0;
	this->moveSpeedMax = 20;
	this->hp = 10 + rand() % 20;
	this->damage = 0;
	this->collisionBox.w = this->collisionBoxWidth = 18;
	this->collisionBox.h = this->collisionBoxHeight = 20;
	this->collisionBoxYOffset = 14;
	this->direction = Direction::Down;
	this->invincibleTimer = invincible;
	this->active = true;

	changeAnimation(GlobState::Idle, true);
	updateCollisionBox();
}

void Glob::update() {
	// Check if dead
	if (hp < 1 && state != GlobState::Dead) {
		changeAnimation(GlobState::Dead, true);
		moving = false;
		die();
	}
	think();
	updateCollisionBox();
	updateMovememnt();
	updateCollisions();
	updateHitBox();
	updateDamage();
	updateAnimation();
	updateInvincibleTimer();
}

void Glob::think() {
	if (state == GlobState::Idle || state == GlobState::Move) {
		thinkTimer -= TimeController::controller.getDeltaTime();
		if (thinkTimer <= 0) {
			int action = rand() % 10;
			if (action < 3) {
				moving = false;
				aiState = GlobAI::Normal;
				changeAnimation(GlobState::Idle, true);
			}
			else {
				find();  // Find nearest target
				if (target != nullptr && target->getHP() > 0) {
					float dist = Entity::getDistance(this, target);
					if (dist < 100) {  // Attack if within range
						telegraph();
						aiState = GlobAI::Normal;
					}
					// Move to target otherwise
					else {
						aiState = GlobAI::Chase;
						moving = true;
						changeAnimation(GlobState::Move, state != GlobState::Move);
					}
				}
				else {
					moving = false;
					aiState = GlobAI::Normal;
					changeAnimation(GlobState::Idle, true);
				}
			}
			thinkTimer = rand() % 5;
		}
	}
	// If chasing
	if (aiState == GlobAI::Chase && hp > 0 && active) {
		this->angle = Entity::getAngle(this, target);
		this->move(angle);
	}
}

void Glob::telegraph() {
	// Prepare the attack
	moving = false;
	frameTimer = 0;
	changeAnimation(GlobState::Telegraph, true);
}

void Glob::attack() {
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(GlobState::Attack, true);
}

void Glob::die() {
	moving = false;
	state = GlobState::Dead;
	changeAnimation(state, true);
	// TODO add some sound
	++Glob::killed;
}

void Glob::find() {
	target = nullptr;
	for (auto entity : Entity::entities) {
		if (entity->getType() == "hero" && entity->isActive()) {
			// If found target is the only one or is closer then the old target
			if (target == nullptr || Entity::getDistance(this, entity) < Entity::getDistance(this, target)) {
				target = dynamic_cast<LivingEntity*>(entity);
			}
		}
	}
}

void Glob::changeAnimation(int newState, bool reset) {
	state = newState;
	currentAnimation = animations->getAnimation(GlobAnimations[newState][direction]);
	currentFrame = currentAnimation->getFrame(reset ? 0 : currentFrame->getFrameNumber());
}

void Glob::updateAnimation() {
	if (currentFrame == nullptr || currentAnimation == nullptr) { return; }
	// Set correct state if stop moving
	if (state == GlobState::Move && !moving) { changeAnimation(GlobState::Idle, true); }
	// Set correct state if moving
	if (!state == GlobState::Move && moving) { changeAnimation(GlobState::Move, true);	}

	// Change frame if current one's duration exeed
	frameTimer += TimeController::controller.getDeltaTime();
	if (frameTimer >= currentFrame->getDuration()) {
		// Animation end
		if (currentFrame->getFrameNumber() == currentAnimation->getEndFrameNumber()) {
			// Unless glob is telegraphing || attacking - restart animation from start
			switch (state)
			{
			case GlobState::Telegraph:
				attack();
				break;
			case GlobState::Attack:
				changeAnimation(GlobState::Move, true);
				break;
			case GlobState::Dead:
				frameTimer = 0;
				if (hp > 0) { changeAnimation(GlobState::Move, true); }
				else { active = false; }
				break;
			default:
				currentFrame = currentAnimation->getFrame(0);
			}
		}
		// Move to next frame
		else { currentFrame = currentAnimation->getNextFrame(currentFrame); }
		// Restart frame duration timer
		frameTimer = 0;
	}

}

void Glob::updateDamage() {
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity : Entity::entities) {
			if (entity->isActive() && entity->getType() == "hero") {
				auto enemy = dynamic_cast<LivingEntity*>(entity);
				// If enemy hits
				if (enemy->getDamage() > 0 && Entity::checkCollision(collisionBox, enemy->getHitBox())) {
					hp -= enemy->getDamage();
					if (hp > 0) {
						// TODO play sound
						invincibleTimer = 0.1;
					}
					// Slide backwards from the enemy
					slideAngle = Entity::getAngle(enemy, this);
					slideAmount = 300;
				}
			}
		}
	}
}

void Glob::reset() {
	Glob::killed = 0;
}

float Glob::score() {
	return Glob::killed;
}
