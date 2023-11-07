#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Goomba.h"
#include "Menu.h"
#include "Camera.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	bool paused = 0;
	//int level = 1;

	Scene();
	~Scene();

	void init(const int &lv);
	void update(int deltaTime);
	void render();
	void changeScene(int sceneID);

private:
	void initShaders();
	bool isKill(glm::vec2 posPlayer, glm::vec2 posEnemy);

private:
	TileMap *map;
	Player *player;
	Goomba *goomba;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Menu  *menu;
	int level, scroll;
	Camera* camera;
};


#endif // _SCENE_INCLUDE

