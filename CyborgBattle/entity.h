#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <string>
#include <SDL.h>
#include "animationSet.h"
#include "animation.h"
#include "frame.h"

enum class Direction { Up, Down, Left, Right, None };

// Abstract
class Entity {
public:
	virtual void update();
	virtual void draw();
	virtual void move(float angle);
	virtual void updateMovememnt();
	virtual void updateCollisionBox();
	virtual void changeAnimation(Direction newState, bool reset) = 0;
	virtual void updateCollisions();

	// Helper functions (static)
	static float getDistance(SDL_Rect& r1, SDL_Rect& r2);
	static float getDistance(Entity* e1, Entity* e2);
	static float getAngle(Entity* e1, Entity* e2);
	static float getAngle(float x1, float y1, float x2, float y2);
	static float getAngle(SDL_Rect& r1, SDL_Rect& r2);
	static bool checkCollision(SDL_Rect& r1, SDL_Rect& r2);
	static Direction angleToDirection(float angle);
	static void remove(bool all = false, bool del = false);  // Clean [inactive] enities [, and delete them]

	// Overloaded
	bool operator<(Entity& other);
	
private:
	Direction direction;
	float x;
	float y;
	bool solid = true;
	bool collideWithSolids = true;
	bool active = true;
	bool moving = true;
	float angle;  // Angle to move entity (360 degree angle)
	float moveSpeed;
	float moveSpeedMax;
	float moveLerp;
	float slideAngle;  // Direction when pushed
	float slideAmount;
	std::string type = "Entity";

	SDL_Rect collisionBox;
	SDL_Rect lastCollisionBox;  // Last collision box position
	int collisionBoxWidth;
	int collisionBoxHeight;
	float collisionBoxYOffset;  // Offset by Y plane as X is always centered

	AnimationSet* animations;
	Animation* currentAnimation;
	Frame* currentFrame;
	float frameTimer;

	// Collection of all entities
	static std::list<Entity*> entities;
};

#endif // !ENTITY_H
