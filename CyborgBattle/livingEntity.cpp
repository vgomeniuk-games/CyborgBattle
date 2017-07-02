#include "livingEntity.h"
#include "groupBox.h"
#include "groupNumber.h"
#include "groupBuilder.h"
#include "timeController.h"
#include "globals.h"

// TODO
#include <iostream>

void LivingEntity::updateHitBox() {
	damage = 0;
	GroupBox* hitBoxes = static_cast<GroupBox*>(GroupBuilder::findGroupByName("hitBox", currentFrame->getFrameData()));
	if (hitBoxes != nullptr && hitBoxes->numberOfDataInGroup() > 0) {
		// Update hitbox
		SDL_Rect hb = hitBoxes->getFirst();
		hitBox.x = x - currentFrame->getOffset().x + hb.x;
		hitBox.y = y - currentFrame->getOffset().y + hb.y;
		hitBox.w = hb.w;
		hitBox.h = hb.h;

		// Update damage
		GroupNumber* damages = static_cast<GroupNumber*>(GroupBuilder::findGroupByName("damage", currentFrame->getFrameData()));
		if (damages != nullptr && damages->numberOfDataInGroup() > 0) {
			damage = damages->getFirst();
		}
	}
}

void LivingEntity::updateInvincibleTimer() {
	if (invincibleTimer > 0) {
		invincibleTimer -= TimeController::controller.getDeltaTime();
	}
}

void LivingEntity::draw() {
	if (currentFrame != nullptr && this->active) {
		if (invincibleTimer > 0 && animations->getWhiteSpriteSheet() != nullptr) {
			this->currentFrame->draw(this->animations->getWhiteSpriteSheet(), this->x, this->y);
		}
		else {
			this->currentFrame->draw(this->animations->getSpriteSheet(), this->x, this->y);
		}
		
	}
	if (solid && Globals::DEBUG) {
		// Also draw few more things while debugging
		SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &hitBox);
	}
}