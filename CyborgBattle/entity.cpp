#include <cmath>
#include "entity.h"
#include "globals.h"
#include "timeController.h"

std::list<Entity*> Entity::entities;
const int Direction::Up = 0;
const int Direction::Down = 1;
const int Direction::Left = 2;
const int Direction::Right = 3;

void Entity::update() {}

void Entity::draw() {
	if (currentFrame != nullptr && this->active) {
		this->currentFrame->draw(this->animations->getSpriteSheet(), this->x, this->y);
	}
	// Draw collision box if in debug mode
	if (this->solid && Globals::DEBUG) {
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);
	}
}

void Entity::move(float angle) {
	this->moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	/*Direction*/int newDirection = angleToDirection(angle);
	if (this->direction != newDirection) {
		direction = newDirection;
		changeAnimation(direction, false);
	}
}

void Entity::updateMovememnt() {
	updateCollisionBox();
	lastCollisionBox = collisionBox;
	
	// Update explicit movement
	if (moveSpeed > 0) {
		float moveDistance = moveSpeed * TimeController::controller.getDeltaTime() * moveLerp;
		if (moveDistance > 0) {
			float xMove = moveDistance * (cos(angle * Globals::PI / 180));
			float yMove = moveDistance * (sin(angle * Globals::PI / 180));

			this->x += xMove;
			this->y += yMove;

			if (!moving) {
				moveSpeed -= moveDistance;
			}
		}
	}

	// Update sliding if present
	if (slideAmount > 0) {
		float slideDistance = slideAmount * TimeController::controller.getDeltaTime() * moveLerp;
		if (slideDistance > 0) {
			float xMove = slideDistance * (cos(slideAngle * Globals::PI / 180));
			float yMove = slideDistance * (sin(slideAngle * Globals::PI / 180));

			this->x += xMove;
			this->y += yMove;
			slideAmount -= slideDistance;
		}
	}

	// Move collision box
	updateCollisionBox();

	// Check collision within last and current collision box positions
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void Entity::updateCollisionBox() {
	collisionBox.x = static_cast<int>(x - collisionBox.w / 2);
	collisionBox.y = static_cast<int>(y - collisionBoxYOffset);
	collisionBox.w = collisionBoxWidth;
	collisionBox.h = collisionBoxHeight;
}

void Entity::updateCollisions() {
	// Check only if it makes sence
	if (active && collideWithSolids && (moveSpeed > 0 || slideAmount > 0)) {
		// Find all possible collisions
		std::list<Entity*> collisions;
		for (auto entity : Entity::entities) {
			if (entity->active && entity->solid && entity->type != this->type && checkCollision(collisionBox, entity->collisionBox)) {
				collisions.push_back(entity);
			}
		}

		if (!collisions.empty()) {
			updateCollisionBox();

			// Find sample distance
			float distanceSize = collisionBox.w < collisionBox.h ? collisionBox.w / 4.0f : collisionBox.h / 4.0f;

			// Check collisions on next steps
			SDL_Rect sampleBox = lastCollisionBox;
			float movementAngle = Entity::getAngle(lastCollisionBox, collisionBox);
			bool detectCollision = false;
			while (!detectCollision) {
				SDL_Rect intersection;
				// Check entity we've registered collision with
				for (auto entity : collisions) {
					if (SDL_IntersectRect(&sampleBox, &entity->collisionBox, &intersection)) {
						// Disable other actions and count slideAngle
						moveSpeed = 0;
						moving = false;
						detectCollision = true;
						slideAngle = Entity::getAngle(entity, this);

						// Do a collision resolution based on smaller intersection side (width || hight)
						if (intersection.w < intersection.h) {
							// Move position left || right
							if (lastCollisionBox.x + lastCollisionBox.w / 2 < entity->collisionBox.x + entity->collisionBox.w / 2) {
								sampleBox.x -= intersection.w;
							}
							else {
								sampleBox.x += intersection.w;
							}
						}
						else {
							// Move position up || down
							if (lastCollisionBox.y + lastCollisionBox.h / 2 < entity->collisionBox.y + entity->collisionBox.h / 2) {
								sampleBox.y -= intersection.h;
							}
							else {
								sampleBox.y += intersection.h;
							}
						}
					}
				}
				if (detectCollision || (sampleBox.x == collisionBox.x && sampleBox.y == collisionBox.y)) {
					// Exit loop once first collision met
					break;
				}
				// Check next step
				if (Entity::getDistance(sampleBox, collisionBox) > distanceSize) {
					sampleBox.x += static_cast<int>(distanceSize * (cos(movementAngle * Globals::PI / 180)));
					sampleBox.y += static_cast<int>(distanceSize * (sin(movementAngle * Globals::PI / 180)));
				}
				else {
					sampleBox = collisionBox;
				}
			}
			if (detectCollision) {
				// Move entity to sampleBox position
				slideAmount = slideAmount / 2;
				x = sampleBox.x + sampleBox.w / 2.0f;
				y = static_cast<float>(sampleBox.y + collisionBoxYOffset);
			}
			updateCollisionBox();
		}
	}
}

float Entity::getDistance(SDL_Rect& r1, SDL_Rect& r2) {
	SDL_Point p1, p2;
	p1.x = r1.x + r1.w / 2;
	p1.y = r1.y + r1.h / 2;
	p2.x = r2.x + r2.w / 2;
	p2.y = r2.y + r2.h / 2;
	float distance = abs(sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)));
	return distance;
}

float Entity::getDistance(Entity* e1, Entity* e2) {
	float distance = abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));
	return distance;
}

float Entity::getAngle(float x1, float y1, float x2, float y2) {
	float deltax = x2 - x1;
	float deltay = y2 - y1;
	return atan2(deltay, deltax) * 180 / Globals::PI;
}

float Entity::getAngle(Entity* e1, Entity* e2) {
	return getAngle(e1->x, e1->y, e2->x, e2->y);
}


float Entity::getAngle(SDL_Rect& r1, SDL_Rect& r2) {
	float x1 = r1.x + (r1.w / 2);
	float y1 = r1.y + (r1.h / 2);
	float x2 = r2.x + (r2.w / 2);
	float y2 = r2.y + (r2.h / 2);
	return getAngle(x1, y1, x2, y2);
}

bool Entity::checkCollision(SDL_Rect& r1, SDL_Rect& r2) {
	if (SDL_IntersectRect(&r1, &r2, nullptr)) {
		return true;
	}
	return false;
}

/*Direction*/int Entity::angleToDirection(float angle) {
	if ((angle >= 0 && angle <= 45) || (angle >= 315 && angle <= 360)) {
		return Direction::Right;
	}
	if (angle >= 45 && angle <= 135) {
		return Direction::Down;
	}
	if (angle >= 135 && angle <= 225) {
		return Direction::Left;
	}
	return Direction::Up;
}

void Entity::add(Entity* entity) {
	entities.push_back(entity);
}

void Entity::remove(bool all, bool del) {
	for (auto entity = entities.begin(); entity != entities.end(); ) {
		if (all || !(*entity)->active) {
			if (del) {
				delete (*entity);
			}
			entity = entities.erase(entity);
			continue;
		}
		++entity;
	}
}

void Entity::updateAll() {
	for (auto entity : entities) {
		entity->update();
	}
}

void Entity::drawAll() {
	for (auto entity : entities) {
		entity->draw();
	}
}

bool Entity::operator<(Entity& other) {
	// Compare depth of entity on screen
	return this->y < other.y;
}

