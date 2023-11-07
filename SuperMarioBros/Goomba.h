#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Goomba
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::vec2 getPosition();
	void setKill();
	float getTimeDeath();

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posGoomba;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool goLeft, kill;
	float timeDeath;
};


#endif // _GOOMBA_INCLUDE
