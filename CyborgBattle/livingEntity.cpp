#include "livingEntity.h"
#include "groupBox.h"
#include "groupNumber.h"
#include "groupBuilder.h"
#include "timeController.h"
#include "globals.h"

void LivingEntity::updateHitBox() {
	auto frameData = currentFrame->getFrameData();
	GroupBox* hitBoxes = static_cast<GroupBox*>(GroupBuilder::findGroupByName("hitBox", frameData));
	if (hitBoxes != nullptr && hitBoxes->numberOfDataInGroup() > 0) {
		// Update hitbox
		SDL_Rect hb = hitBoxes->getFirst();
		hitBox.x = x - currentFrame->getOffset().x + hb.x;
		hitBox.y = y - currentFrame->getOffset().y + hb.y;
		hitBox.w = hb.w;
		hitBox.h = hb.h;

		// Update damage
		GroupNumber* damages = static_cast<GroupNumber*>(GroupBuilder::findGroupByName("damage", frameData));
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
	Entity::draw();
	if (solid && Globals::DEBUG) {
		// Also draw few more things while debugging
		SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &hitBox);
	}
}