#include "wall.h"
#include "timeController.h"

Wall::Wall(AnimationSet* animations) {
	this->animations = animations;
	this->solid = true;
	this->collisionBoxHeight = 32;
	this->collisionBoxWidth = 32;
	this->collisionBoxYOffset = -16;
	updateCollisionBox();
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
