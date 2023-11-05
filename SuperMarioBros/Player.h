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
	glm::ivec2 getPos();
	

	enum TypePlayer {
		Small_Mario, Star_Mario, Fire_Mario, Medium_Mario
	};
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer, velPlayer, accPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::ivec2 mario_size = glm::ivec2(16, 16);
	bool Looking_left = 0;
	int Mariostate = Small_Mario;
	ShaderProgram shader;
};


#endif // _PLAYER_INCLUDE


