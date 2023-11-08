#include "KoopaTroopa.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


#define FALL_STEP 4
#define TIME_REVIVE 4000
#define TIME_DEATH 3000


enum KoopaTroopaAnims
{
	MOVE_LEFT, MOVE_RIGHT, REVIVE, DEATH
};


void KoopaTroopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	goLeft = TRUE;
	kill = FALSE;
	timeRevive = TIME_REVIVE;
	timeDeath = TIME_DEATH;
	spritesheet.loadFromFile("images/koopa_troopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 24), glm::vec2(0.125f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0 * 0.125f, 1.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * 0.125f, 1.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * 0.125f, 1.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3 * 0.125f, 1.f));

	sprite->setAnimationSpeed(REVIVE, 8);
	sprite->addKeyframe(REVIVE, glm::vec2(4 * 0.125f, 1.f));
	sprite->addKeyframe(REVIVE, glm::vec2(5 * 0.125f, 1.f));

	sprite->setAnimationSpeed(DEATH, 8);
	sprite->addKeyframe(DEATH, glm::vec2(5 * 0.125f, 1.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posKoopaTroopa.x), float(tileMapDispl.y + posKoopaTroopa.y)));

}

void KoopaTroopa::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (kill) {
		if (sprite->animation() != DEATH)
			sprite->changeAnimation(DEATH);
		if (goLeft) {
			posKoopaTroopa.x -= 3.f;
			if (map->collisionMoveLeft(posKoopaTroopa, glm::ivec2(16, 16))) {
				posKoopaTroopa.x += 3.f;
				goLeft = FALSE;
			}
		}
		else {
			posKoopaTroopa.x += 3.f;
			if (map->collisionMoveRight(posKoopaTroopa, glm::ivec2(16, 16))) {
				posKoopaTroopa.x -= 3.f;
				goLeft = TRUE;
			}
		}
	}
	else if (hit) {
		if (timeDeath >= 0 && sprite->animation() != DEATH)
			sprite->changeAnimation(DEATH);
		if (timeDeath < 0) {
			if (sprite->animation() != REVIVE)
				sprite->changeAnimation(REVIVE);
			if (timeRevive < 0) {
				if(goLeft)
					sprite->changeAnimation(MOVE_LEFT);
				else sprite->changeAnimation(MOVE_RIGHT);
				hit = false;
			}
			timeRevive -= deltaTime;
		}
		timeDeath -= deltaTime;
	}
	else {
		if (goLeft) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posKoopaTroopa.x -= 0.5f;
			if (map->collisionMoveLeft(posKoopaTroopa, glm::ivec2(16, 16))) {
				posKoopaTroopa.x += 0.5f;
				goLeft = FALSE;
			}
		}
		else {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posKoopaTroopa.x += 0.5f;
			if (map->collisionMoveRight(posKoopaTroopa, glm::ivec2(16, 16))) {
				posKoopaTroopa.x -= 0.5f;
				goLeft = TRUE;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posKoopaTroopa.x), float(tileMapDispl.y + posKoopaTroopa.y)));
}

void KoopaTroopa::render()
{
	sprite->render();
}

void KoopaTroopa::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void KoopaTroopa::setPosition(const glm::vec2& pos)
{
	posKoopaTroopa = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posKoopaTroopa.x), float(tileMapDispl.y + posKoopaTroopa.y)));
}

glm::vec2 KoopaTroopa::getPosition() {
	return posKoopaTroopa;
}

void KoopaTroopa::setHit() {
	hit = true;
}

bool KoopaTroopa::getHit() {
	return hit;
}

void KoopaTroopa::setKill() {
	kill = true;
}

bool KoopaTroopa::getKill() {
	return kill;
}

void KoopaTroopa::setLeft() {
	goLeft = true;
}