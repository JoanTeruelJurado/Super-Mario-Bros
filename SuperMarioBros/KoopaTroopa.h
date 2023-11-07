#ifndef _KOOPATROOPA_INCLUDE
#define _KOOPATROOPA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class KoopaTroopa
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::vec2 getPosition();
	void setKill();

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posKoopaTroopa;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool goLeft, kill;
};


#endif // _KOOPATROOPA_INCLUDE
