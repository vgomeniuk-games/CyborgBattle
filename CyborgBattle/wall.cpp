#include "wall.h"
#include "timeController.h"

Wall::Wall(AnimationSet* animations, int x, int y) {
	this->animations = animations;
	this->x = x;
	this->y = y;
	this->solid = true;
	this->active = true;  // TODO
	this->collisionBoxHeight = 32;
	this->collisionBoxWidth = 32;
	this->collisionBoxYOffset = 16;
	updateCollisionBox();
	changeAnimation(0, false);
}

void Wall::update() {
	updateCollisionBox();

	// Do nothing if no animation
	if (currentFrame == nullptr || currentAnimation == nullptr) { return; }

	// Change frame at tick
	frameTimer += TimeController::controller.getDeltaTime();
	if (frameTimer >= currentFrame->getDuration()) {
		currentFrame = currentAnimation->getNextFrame(currentFrame);
		frameTimer = 0;
	}
}

void Wall::changeAnimation(/*Direction*/int newState, bool reset) {
	currentAnimation = animations->getAnimation("wall");
	currentFrame = currentAnimation->getFrame(0);
}
