#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#include <windows.h>
#include <mmsystem.h>

#define SCREEN_X 224
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 11

#define ortho_size 300.f

Scene::Scene()
{
	scroll = 0;
	map = NULL;
	background = NULL;
	player = NULL;
	goomba = NULL;
	koopatroopa = NULL;
	camera = new Camera();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (goomba != NULL)
		delete goomba;
	if (koopatroopa != NULL)
		delete koopatroopa;
	if (menu != NULL)
		delete menu;
	if (background != NULL) 
	delete background;
}


void Scene::init(const int &lv)
{
	level = lv;
	if (level == 0) {
		initShaders();
		menu = new Menu();
		menu->init(texProgram);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
		PlaySound(L"sounds/01.-Ground-Theme.wav", NULL, SND_ASYNC|SND_LOOP);
	}
	else {
		initShaders();
		scroll = 0;
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(0, 0), texProgram);
		//background = TileMap::get
		player = new Player();
		player->init(glm::ivec2(0, 0), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);
		goomba = new Goomba();
		goomba->init(glm::ivec2(0, 0), texProgram);
		goomba->setPosition(glm::vec2(10 * map->getTileSize(), 12 * map->getTileSize()));
		goomba->setTileMap(map);
		koopatroopa = new KoopaTroopa();
		koopatroopa->init(glm::ivec2(0, 0), texProgram);
		koopatroopa->setPosition(glm::vec2(15 * map->getTileSize(), 12 * map->getTileSize() - 8));
		koopatroopa->setTileMap(map);
		//projection = glm::ortho(0.f, float(SCREEN_WIDTH ), float(SCREEN_HEIGHT), 0.f);
		projection = glm::ortho(0.f, 300.f, 225.f, 0.f); // 300 225
		currentTime = 0.0f;
	}
}

void Scene::update(int deltaTime)
{
	if (level != 0) {
		int marioposx = player->getPos().x;
		if (marioposx >= scroll + ortho_size /2) {
			scroll += abs(marioposx - (scroll + ortho_size / 2));
			camera->CameraUpdate(scroll);
			player->setMinPos(scroll);
		}
		if (player->getandset()) { //el jugador ha muerto
			scroll = 0;
			player->init(glm::ivec2(0, 0), texProgram);
			player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
			camera->CameraUpdate(0);
			player->setMinPos(0);
		}
	}

	if (paused) return;
	currentTime += deltaTime;
	if (level != 0) {
		if (isKill(player->getPos(), goomba->getPosition(), goomba->getKill()))
			goomba->setKill();
		if (goomba->getTimeDeath() < 0)
			goomba->setPosition(glm::vec2(0, 20 * map->getTileSize()));
		if (koopatroopa->getKill()) {
			if (isKill(player->getPos(), koopatroopa->getPosition(), koopatroopa->getHit()))
				koopatroopa->setPosition(glm::vec2(0, 20 * map->getTileSize()));
			else
				if (shellKill(koopatroopa->getPosition(), goomba->getPosition()))
					goomba->setKill();
				/*if (shellKill(koopatroopa->getPosition(), player->getPos())) {
					player->setDeath();
				}*/ //-> Descomentar cuando funcione animaci�n matar player
		}
		else if (isKill(player->getPos(), koopatroopa->getPosition(), koopatroopa->getHit())) {
			if (!koopatroopa->getHit()) {
				koopatroopa->setHit();
			}
			koopatroopa->setKill();
		}
		player->update(deltaTime);
		goomba->update(deltaTime);
		koopatroopa->update(deltaTime);		
	}
	else {
		level = menu->update(deltaTime);
		if (level != 0) {
			init(level);
		}
	}
}

void Scene::render()
{
	glm::mat4 model, view;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	model = glm::mat4(1.0f);
	view = camera->view();
	texProgram.setUniformMatrix4f("model", model);
	texProgram.setUniformMatrix4f("view", view);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (level != 0) {
		map->render();
		player->render();
		goomba->render();
		koopatroopa->render();
	}
	else {
		menu->render();
	}
	
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::changeScene(int sceneID) {
	glClearColor(92.0f / 255.0f, 148.0f / 255.0f, 252.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	level = sceneID;
	scroll = 0;
	initShaders();
	if (sceneID == 1) map = TileMap::createTileMap("levels/level01.txt", glm::vec2(0, 0), texProgram);
	else if (sceneID == 2) map = TileMap::createTileMap("levels/level02.txt", glm::vec2(0, 0), texProgram);
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	projection = glm::ortho(0.f, 300.f, 225.f, 0.f); //300 225

	map->render();
	player->render();
	currentTime = 0.0f;
	return;
}

bool Scene::isKill(glm::vec2 posPlayer, glm::vec2 posEnemy, bool kill) {
	float leftP = posPlayer.x;
	float rightP = posPlayer.x + 16;
	float bottomP = posPlayer.y;

	float leftE = posEnemy.x;
	float rightE = posEnemy.x + 16;
	float topE = posEnemy.y - 16;

	if ((bottomP - 2 >= topE && bottomP <= topE + 2) && (leftP < rightE && rightP > leftE)) return true;
	else if (!kill && (bottomP > topE) && (leftP < rightE && rightP > leftE)) player->setDeath();
	return false;
}

bool Scene::shellKill(glm::vec2 posShell, glm::vec2 pos) {
	float leftS = posShell.x;
	float rightS = posShell.x + 16;
	float bottomS = posShell.y;

	float left = pos.x;
	float right = pos.x + 16;
	float top = pos.y - 16;

	if ((bottomS > top) && (leftS < right && rightS > left))  return true;
	return false;
}

int Scene::getScroll() {
	return scroll;
}
