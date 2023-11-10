#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "Menu.h"
#include "Camera.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	bool paused = 0;

	Scene();
	~Scene();

	void init(const int &lv);
	void update(int deltaTime);
	void render();
	void changeScene(int sceneID);
	bool isKill(glm::vec2 posPlayer, glm::vec2 posEnemy, bool kill);
	bool shellKill(glm::vec2 posShell, glm::vec2 pos);

	int getScroll();
private:
	void initShaders();
	void initNumbers();
	void assignNumber(Sprite* s, int n);
	void updateTimer();
	void updateCoins();
	void updatePoints();
	void updateLives();

private:
	TileMap *map, *backmap;
	Player *player;
	Goomba *goomba;
	KoopaTroopa *koopatroopa;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Menu  *menu;
	int level, scroll;
	Camera* camera;
	Sprite *scoreboard, *centenaPoints, *decenaPoints, *unitPoints, *decenaCoins, *unitCoins, *world, *lvl, * centenaTimer, * decenaTimer, * unitTimer, *lives;
	Texture scoreBoard, numbers;
};


#endif // _SCENE_INCLUDE

