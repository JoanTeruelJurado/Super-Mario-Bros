#include "Goomba.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


#define FALL_STEP 4
#define TIME_DEATH 2


enum GoombaAnims
{
	MOVE, DEATH
};


void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	goLeft = TRUE;
	kill = FALSE;
	timeDeath = TIME_DEATH;
	spritesheet.loadFromFile("images/little_goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE, 8);
	sprite->addKeyframe(MOVE, glm::vec2(0 * 0.25f, 1.f));
	sprite->addKeyframe(MOVE, glm::vec2(1 * 0.25f, 1.f));

	sprite->setAnimationSpeed(DEATH, 8);
	sprite->addKeyframe(DEATH, glm::vec2(2 * 0.25f, 1.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));

}

void Goomba::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (kill) {
		if (sprite->animation() != DEATH)
			sprite->changeAnimation(DEATH);
		timeDeath -= deltaTime;
	}
	else {
		if (goLeft) {
			posGoomba.x -= 0.5f;
			if (map->collisionMoveLeft(posGoomba, glm::ivec2(16, 16))) {
				posGoomba.x += 0.5f;
				goLeft = FALSE;
			}
		}
		else {
			posGoomba.x += 0.5f;
			if (map->collisionMoveRight(posGoomba, glm::ivec2(16, 16))) {
				posGoomba.x -= 0.5f;
				goLeft = TRUE;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
}

void Goomba::render()
{
	sprite->render();
}

void Goomba::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Goomba::setPosition(const glm::vec2& pos)
{
	posGoomba = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
}

glm::vec2 Goomba::getPosition() {
	return posGoomba;
}

void Goomba::setKill() {
	kill = true;
}

float Goomba::getTimeDeath() {
	return timeDeath;
}