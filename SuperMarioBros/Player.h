#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void ChangeType(int statePlayer);
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setDeath();
	glm::ivec2 getPos();
	glm::ivec2 gettileMapDispl();
	void setcoins(int a);
	void setlives(int a);
	void setscore(int a);


	int getcoins();
	int getlives();
	int getscore();
	void setlvl(int a);
	void setMinPos(int scroll);
	float gettime();
	int getMariostate();

private:
	bool bJumping;
	glm::ivec2 tileMapDispl = glm::ivec2(0,0), posPlayer;
	glm::fvec2 velPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::ivec2 mario_size = glm::ivec2(16, 16);
	bool Looking_left = 0;
	int Mariostate = 0;
	ShaderProgram shader;

	void death_anim();
	void pre_death();
	void flag_pole_anim();
	void SolveCollisions();

	int MinPos;
	
	int coins  = 0;
	int lives = 3;
	int score = 0;

	int lvl = 1;

	float time = 30000.f;
};


#endif // _PLAYER_INCLUDE


