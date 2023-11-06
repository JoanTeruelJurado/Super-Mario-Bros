#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#include <windows.h>
#include <mmsystem.h>
// bool pl = PlaySound(L"sounds/smb_gameover.wav", NULL, SND_SYNC);  para la musica

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50
#define FALL_STEP 4
#define max_speed 4.5f/2.0f

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
	velPlayer = glm::fvec2(0.f, 0.f);
	bJumping = false;
	shader = shaderProgram;
	ChangeType(Small_Mario);
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::ChangeType(int statePlayer){
	ShaderProgram& shaderProgram = shader;
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

		spritesheet.loadFromFile("images/invencible_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.0625f, 0.25f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(11);
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(6 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(7 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(10 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(SLIDE_LEFT, 8);
		sprite->addKeyframe(SLIDE_LEFT, glm::vec2(2 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(SLIDE_RIGHT, 8);
		sprite->addKeyframe(SLIDE_RIGHT, glm::vec2(11 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(1 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(12 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(DOWN_LEFT, 8);
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0 * 0.0625f, 0.25f));

		sprite->setAnimationSpeed(DOWN_RIGHT, 8);
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(13 * 0.0625f, 0.25f));


		break;
	case Star_Mario:
		Mariostate = Star_Mario;
		mario_size = glm::ivec2(16, 32);

		spritesheet.loadFromFile("images/invencible_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.0625f, 0.25f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(11);
		sprite->setAnimationSpeed(STAND_LEFT, 32);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(6 * 0.0625f, 0.25f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(6 * 0.0625f, 2*0.25f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(6 * 0.0625f, 3*0.25f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(6 * 0.0625f, 4*0.25f));

		sprite->setAnimationSpeed(STAND_RIGHT, 32);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(7 * 0.0625f, 0.25f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(7 * 0.0625f, 2 * 0.25f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(7 * 0.0625f, 3 * 0.25f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(7 * 0.0625f, 4 * 0.25f));

		sprite->setAnimationSpeed(MOVE_LEFT, 32);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 0.0625f,2* 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 0.0625f,3* 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 0.0625f,4* 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * 0.0625f, 2 * 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * 0.0625f, 3 * 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * 0.0625f, 4 * 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * 0.0625f, 2 * 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * 0.0625f, 3 * 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * 0.0625f, 4 * 0.25f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 32);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(10 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(10 * 0.0625f,2* 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(10 * 0.0625f,3* 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(10 * 0.0625f,4* 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * 0.0625f, 2 * 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * 0.0625f, 3 * 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * 0.0625f, 4 * 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * 0.0625f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * 0.0625f, 2 * 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * 0.0625f, 3 * 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * 0.0625f, 4 * 0.25f));

		sprite->setAnimationSpeed(SLIDE_LEFT, 32);
		sprite->addKeyframe(SLIDE_LEFT, glm::vec2(2 * 0.0625f, 0.25f));
		sprite->addKeyframe(SLIDE_LEFT, glm::vec2(2 * 0.0625f,2* 0.25f));
		sprite->addKeyframe(SLIDE_LEFT, glm::vec2(2 * 0.0625f,3* 0.25f));
		sprite->addKeyframe(SLIDE_LEFT, glm::vec2(2 * 0.0625f,4* 0.25f));

		sprite->setAnimationSpeed(SLIDE_RIGHT, 32);
		sprite->addKeyframe(SLIDE_RIGHT, glm::vec2(11 * 0.0625f, 0.25f));
		sprite->addKeyframe(SLIDE_RIGHT, glm::vec2(11 * 0.0625f,2* 0.25f));
		sprite->addKeyframe(SLIDE_RIGHT, glm::vec2(11 * 0.0625f,3* 0.25f));
		sprite->addKeyframe(SLIDE_RIGHT, glm::vec2(11 * 0.0625f,4* 0.25f));

		sprite->setAnimationSpeed(JUMP_LEFT, 32);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(1 * 0.0625f, 0.25f));
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(1 * 0.0625f,2* 0.25f));
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(1 * 0.0625f,3* 0.25f));
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(1 * 0.0625f,4* 0.25f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 32);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(12 * 0.0625f, 0.25f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(12 * 0.0625f,2* 0.25f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(12 * 0.0625f,3* 0.25f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(12 * 0.0625f,4* 0.25f));

		sprite->setAnimationSpeed(DOWN_LEFT, 32);
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0 * 0.0625f, 0.25f));
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0 * 0.0625f,2* 0.25f));
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0 * 0.0625f,3* 0.25f));
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0 * 0.0625f,4* 0.25f));

		sprite->setAnimationSpeed(DOWN_RIGHT, 32);
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(13 * 0.0625f, 0.25f));
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(13 * 0.0625f,2* 0.25f));
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(13 * 0.0625f,3* 0.25f));
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(13 * 0.0625f,4* 0.25f));

		break;
	}
}

void Player::update(int deltaTime)
{
	if (Game::instance().getKey(103) || Game::instance().getKey(71)) { if (Mariostate == Small_Mario) posPlayer.y -=16; ChangeType(Star_Mario); } // KEY pressed G
	else if (Game::instance().getKey(109) || Game::instance().getKey(77)) { if (Mariostate == Small_Mario) posPlayer.y -= 16; ChangeType(Medium_Mario); } // KEY pressed M
	
	if (((sprite->animation() != DOWN_LEFT) || (sprite->animation() != DOWN_RIGHT)) && (Mariostate != Small_Mario)) { mario_size = glm::ivec2(16, 32); }

	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		Looking_left = true;
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

		velPlayer.x = max(velPlayer.x-acceleration, -max_speed);
		if(map->collisionMoveLeft(posPlayer, mario_size))
		{

			velPlayer.x = 0;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		Looking_left = false;
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

		velPlayer.x = min(velPlayer.x + acceleration, max_speed);
		if(map->collisionMoveRight(posPlayer, mario_size))
		{

			velPlayer.x = 0;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if((Game::instance().getSpecialKey(GLUT_KEY_DOWN)) && Mariostate != Small_Mario) {
		posPlayer.y -= 4;
		mario_size = glm::ivec2(16, 16);

		if (Looking_left) sprite->changeAnimation(DOWN_LEFT);
		else sprite->changeAnimation(DOWN_RIGHT);
	}
	else
	{
		if (velPlayer.x > 0) velPlayer.x -= acceleration;
		if (velPlayer.x < 0) velPlayer.x += acceleration;

		if(Looking_left) sprite->changeAnimation(STAND_LEFT);
		else sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
			
			if (Looking_left) sprite->changeAnimation(JUMP_LEFT);
			else sprite->changeAnimation(JUMP_RIGHT);


			if (jumpAngle == 180)
			{
				bJumping = false;
				sprite->changeAnimation(STAND_RIGHT);
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) { 
					bJumping = !map->collisionMoveDown(posPlayer, mario_size, &posPlayer.y);
				}
				else if (map->collisionMoveUp(posPlayer, mario_size)) { jumpAngle = 90 + abs(90 - jumpAngle); } // to touch	
			}
		
	}
	else
	{

		posPlayer.y += FALL_STEP;

		if (map->collisionMoveDown(posPlayer, mario_size, &posPlayer.y))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				if (Mariostate == Small_Mario) PlaySound(L"sounds/smb_jump-small.wav", NULL, SND_ASYNC);
				else PlaySound(L"sounds/smb_jump-super.wav", NULL, SND_ASYNC);
				
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
		
	}
	posPlayer.x += velPlayer.x;
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

glm::ivec2 Player::getPos() {
	return posPlayer;
}

glm::ivec2 Player::gettileMapDispl() {
	return tileMapDispl;
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


int Player::getcoins() {
	return coins;
}
int Player::getlives() {
	return lives;
}
int Player::getscore() {
	return score;
}

