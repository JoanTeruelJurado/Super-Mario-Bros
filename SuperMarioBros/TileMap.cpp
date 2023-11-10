#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0) return false;
	
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;

	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile; //Tilesheet -> image.png
	std::cout << line << endl;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	getline(fin, line);
	sstream.str(line);
	int X, Y;
	sstream >> X >> Y;
	tilesheetSize = glm::vec2(X, Y);
	tileTexSize = glm::vec2(1.f / X, 1.f / Y);
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == '0' )
				map[j*mapSize.x+i] = 0;
			else if (tile >= 'a') map[j * mapSize.x + i] = tile - int('a') +10;
			else {
				map[j * mapSize.x + i] = tile - int('0');
			}
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	

	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] != 0) return true;
	}

	return false;
}

bool TileMap::collisionMoveLeft(glm::ivec2 &pos, const glm::ivec2 &size, int* posx) //AUTO CORRECTS POSITIONS
{
	int x, y0, y1;
	int BLOCKID;

	x = (pos.x + size.x - 1) / tileSize;

	if (map[(((pos.y - size.y) / tileSize) + 1) * mapSize.x + x - 1]) pos.y = (pos.y % 16 == 0) ? pos.y : pos.y + 16 - (pos.y % 16);

	y0 = ((pos.y - size.y) / tileSize) + 1;
	y1 = (pos.y + size.y - 1) / tileSize;


	for(int y=y0; y<=y1; y++)
	{
		BLOCKID = map[y * mapSize.x + x-1];
		if ((BLOCKID == 9 || BLOCKID == 10 || BLOCKID == 11 || BLOCKID == 12) && !won) {
			winPos = glm::vec2(x, y1);
			won = true;
			std::cout << "VICTORY" << endl;
		}
		if(BLOCKID != 0) {
			*posx = x*tileSize;
			return true;
		}

	}
	
	return false;
}

bool TileMap::collisionMoveRight(glm::ivec2 &pos, const glm::ivec2 &size, int* posx) //AUTO CORRECTS POSITIONS
{
	int x, y0, y1;
	int BLOCKID;
	x = (pos.x + size.x -1 ) / tileSize;

	if (map[(((pos.y - size.y) / tileSize) + 1) * mapSize.x + x]) pos.y = (pos.y % 16 == 0) ? pos.y : pos.y+16- (pos.y % 16);

	y0 = ((pos.y - size.y) / tileSize)+1;
	y1 = (pos.y + size.y -1) / tileSize;

	for(int y=y0; y <=y1; y++)
	{
		
		BLOCKID = map[y * mapSize.x + x];
		if ((BLOCKID == 9 || BLOCKID == 10 || BLOCKID == 11 || BLOCKID == 12) && !won) {
			winPos = glm::vec2(x, y1);
			won = true;
			std::cout << "VICTORY" << endl;
		}
		if(BLOCKID != 0) {
			*posx -= (*posx % 16);
			return true;
		}
		
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = (pos.y - 1) / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] != 0)
			return true;

	}

	return false;
}

enum TypePlayer
{
	Small_Mario, Star_Mario, Fire_Mario, Medium_Mario
};

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, const int Mariostate, int *posY)
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;	
	y = (pos.y -1) / tileSize;
	int blockID = 0;
	for (int x = x0; x <= x1; x++)
	{
		blockID = map[y * mapSize.x + x];

		if (blockID != 0)
		{
			if (Mariostate != Small_Mario && blockID != 2) {
				map[y * mapSize.x + x] = 0;
			}
			return true;
			/*if (blockID == 9 || blockID == 10 || blockID == 11 || blockID == 12) {
				winPos = glm::vec2(x0, y);
				won = true;
				std::cout << "VICTORY" << endl;
			}*/
		}
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y) / tileSize;

	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::WinCondition() {
	return won;
}

glm::vec2 TileMap::getWinPos() {
	return winPos;
}

bool TileMap::collisionTo(const glm::ivec2& Cpos, const glm::ivec2& Npos, const glm::ivec2& size, const int Mariostate) {
	if (Cpos == Npos) return false; //no se mueve
	
	int Diffx = (Cpos.x <= Npos.x) ? 1 : -1;
	int Diffy = (Cpos.y <= Npos.y) ? 1 : -1;

	int Xor, Yor, Xds, Yds;
	Xor = Cpos.x / tileSize;
	Yor = Cpos.y / tileSize;

	Xds = (Npos.x) / tileSize;
	Yds = (Npos.y) / tileSize;
	std::cout << "DIRECTION " << Diffx << " " << Diffy << endl;
	std::cout << "POS fl: " << Cpos.x << " " << Cpos.y << " -//- " << Npos.x << " " << Npos.y << endl;
	std::cout << "POS ID: " << Xor << " " << Yor << " // " << Xds << " " << Yds << endl;

	vector<glm::vec2> Physical_blocks (0);

	glm::ivec2 MaxNextPos = glm::vec2(0, 0);
	for (int X = Xor; X != Xds; X+=Diffx) {
		for (int Y = Yor; Y != Yds; Y += Diffy) {
			cout <<  "coords: " << X << " " << Y << endl;

			if (map[Y * mapSize.x + X] != 0) {
				Physical_blocks.push_back(glm::vec2(X,Y));
				std::cout << "COLLISION on: " << X << " " << Y << endl;
			}
			
		}
	}
	//if (Physical_blocks.empty()) return false;
	//else return Intersect(Physical_blocks);
	return false;
}

bool TileMap::Intersect(const glm::ivec2& myPos, const glm::ivec2& myDst, const vector<glm::vec2>& BlocksToCheck, const glm::ivec2& size) {
	/*
	glm::ivec2 a,b,c,d; //origen
	glm::ivec2 A,B,C,D; //destino

	Z3	Z---Z Z4	C->	X---X <-D
block->	|XXX|			|   |		destino
	Z1	Z---Z Z2	A->	X---X <-B

	c->	X---X <-d
		|   |		origen
	a->	X---X <-b

	Los bloques Z no dependen del tamaño del mario

	Para saber si otros cuadrados intersecan las lineas a->A, b->B... debemos:
		1) generar a,b,c,d i A,B,C,D
		2) para cada bloque que debemos comprovar -> generar las diagonales (Z1-Z4) i (Z2-Z3)
		3) Comprovar si hay colisiones
	*/

	glm::ivec2 a, b, c, d; //origen
	glm::ivec2 A, B, C, D; //destino
	a = myPos;
	A = myDst;

	b = glm::vec2(myPos.x + size.x, myPos.y);
	B = glm::vec2(myDst.x + size.x, myDst.y);

	c = glm::vec2(myPos.x, myPos.y- size.y);
	C = glm::vec2(myDst.x, myDst.y- size.y);

	d = glm::vec2(myPos.x + size.x, myPos.y - size.y);
	D = glm::vec2(myDst.x + size.x, myDst.y - size.y);

	bool answer = false;

	glm::ivec2 Z1, Z2, Z3, Z4, ID;
	for (int i = 0; i < BlocksToCheck.size(); i++) {
		ID = BlocksToCheck[i];
		Z1 = glm::vec2(ID.x* tileSize, ID.y*tileSize);
		Z2 = glm::vec2(ID.x * tileSize + tileSize, ID.y * tileSize);
		Z3 = glm::vec2(ID.x * tileSize, ID.y * tileSize - tileSize);
		Z4 = glm::vec2(ID.x * tileSize + tileSize, ID.y * tileSize - tileSize);
		
		answer |= (CCW(a, Z2, Z3) != CCW(A, Z2, Z3) && CCW(a, A, Z2) != CCW(a, A, Z3));
		answer |= (CCW(a, Z1, Z4) != CCW(A, Z1, Z4) && CCW(a, A, Z1) != CCW(a, A, Z4));
	}

	return answer;
}

bool TileMap::CCW(const glm::ivec2& A, const glm::ivec2& B, const glm::ivec2& C) {
	return ((C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x));
}
