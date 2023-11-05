#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, SLIDE_LEFT, SLIDE_RIGHT, JUMP_LEFT, JUMP_RIGHT, DEATH, DOWN_LEFT, DOWN_RIGHT
};

enum TypePlayer
{
	Small_Mario, Star_Mario, Fire_Mario, Medium_Mario
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	
	ChangeType(Small_Mario, shaderProgram);

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::ChangeType(int statePlayer, ShaderProgram& shaderProgram){
	switch(statePlayer) {
	case Small_Mario:
		Mariostate = Small_Mario;
		mario_size = glm::ivec2(16, 16);
		spritesheet.loadFromFile("images/small_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 1.0f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(11);

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(5 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(6 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * 0.0625f, 1.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 0.0625f, 1.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(2 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7 * 0.0625f, 1.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * 0.0625f, 1.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(SLIDE_LEFT, 8);
		sprite->addKeyframe(SLIDE_LEFT, glm::vec2(1 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(SLIDE_RIGHT, 8);
		sprite->addKeyframe(SLIDE_RIGHT, glm::vec2(10 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(11 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(DEATH, 8);
		sprite->addKeyframe(DEATH, glm::vec2(12 * 0.0625f, 1.f));

		break;

	case Medium_Mario:
		Mariostate = Medium_Mario;
		mario_size = glm::ivec2(16, 32);
		spritesheet.loadFromFile("images/super_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.0625f, 1.0f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(11);
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(6 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(7 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 0.0625f, 1.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * 0.0625f, 1.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(10 * 0.0625f, 1.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * 0.0625f, 1.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(SLIDE_LEFT, 8);
		sprite->addKeyframe(SLIDE_LEFT, glm::vec2(2 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(SLIDE_RIGHT, 8);
		sprite->addKeyframe(SLIDE_RIGHT, glm::vec2(11 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(1 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(12 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(DOWN_LEFT, 8);
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0 * 0.0625f, 1.f));

		sprite->setAnimationSpeed(DOWN_RIGHT, 8);
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(13 * 0.0625f, 1.f));


		break;
	case Fire_Mario:
		Mariostate = Fire_Mario;
		mario_size = glm::ivec2(16, 32);
		break;
	case Star_Mario:
		Mariostate = Star_Mario;
		mario_size = glm::ivec2(16, 32);
		break;
	}
}

void Player::update(int deltaTime)
{
	//if ((sprite->animation() != DOWN_LEFT) || (sprite->animation() != DOWN_RIGHT) || (Mariostate != Small_Mario)) mario_size = glm::ivec2(16, 32);

	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		Looking_left = 1;
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, mario_size))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		Looking_left = 0;
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, mario_size))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if((Game::instance().getSpecialKey(GLUT_KEY_DOWN))) {
		//mario_size = glm::ivec2(16, 16);

		if (Looking_left) sprite->changeAnimation(DOWN_LEFT);
		else sprite->changeAnimation(DOWN_RIGHT);
	}
	else
	{
		if(Looking_left) sprite->changeAnimation(STAND_LEFT);
		else sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		if (map->collisionMoveUp(posPlayer, mario_size)) { jumpAngle = 160.f; } // to touch
		if (Looking_left) sprite->changeAnimation(JUMP_LEFT);
		else sprite->changeAnimation(JUMP_RIGHT);
		
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			sprite->changeAnimation(STAND_RIGHT);
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, mario_size, &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, mario_size, &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPos() {
	return posPlayer;
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




