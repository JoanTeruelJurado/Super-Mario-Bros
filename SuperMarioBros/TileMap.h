#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program, bool Back);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program, bool Back);

	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveLeft( glm::ivec2& pos, const glm::ivec2& size, int* posy);
	bool collisionMoveRight( const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveRight( glm::ivec2& pos, const glm::ivec2& size,int* posy);
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, const int Mariostate, int* posy);
	bool collisionTo(const glm::ivec2& Cpos, const glm::ivec2& Npos, const glm::ivec2& size, const int Mariostate);

	bool WinCondition();
	glm::vec2 getWinPos();
	
private:
	bool loadLevel(const string& levelFile, bool Back);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	bool won = false;
	glm::ivec2 winPos;

	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map, * backmap;

	bool Intersect(const glm::ivec2& myPos, const glm::ivec2& myDst, const vector<glm::vec2>& BlocksToCheck, const glm::ivec2& size);
	bool CCW(const glm::ivec2& A, const glm::ivec2& B, const glm::ivec2& C);

};
#endif // _TILE_MAP_INCLUDE