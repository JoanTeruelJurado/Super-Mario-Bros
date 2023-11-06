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
#define INIT_PLAYER_Y_TILES 2

Scene::Scene()
{
	scroll = 0;
	map = NULL;
	player = NULL;
	camera = new Camera();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (menu != NULL)
		delete menu;
	
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
		player = new Player();
		player->init(glm::ivec2(0, 0), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH ), float(SCREEN_HEIGHT), 0.f);
		currentTime = 0.0f;
	}
}

void Scene::update(int deltaTime)
{

	if (level != 0) scroll = max(player->getPos().x, scroll);
	if (Game::instance().getSpecialKey(0x066)) camera->CameraUpdate(scroll);

	if (paused) return;
	currentTime += deltaTime;
	if (level != 0) {
		player->update(deltaTime);
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

	if (level != 0) {
		//scroll = player->getPos().x;
		// = glm::translate(modelview, glm::vec3(-scroll, 0, 0));
		//modelview = glm::translate(modelview, glm::vec3(-(float(player->gettileMapDispl().x + player->getPos().x)), 0, 0));

		texProgram.setUniformMatrix4f("model", model);
		texProgram.setUniformMatrix4f("view", view);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		map->render();
		player->render();
	}
	else {
		texProgram.setUniformMatrix4f("model", model);
		texProgram.setUniformMatrix4f("view", view);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
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
	if (sceneID == 1) map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	else if (sceneID == 2) map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//ayer = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = glm::ortho(224.f, 524.f, 250.f, 25.f); //300 225

	map->render();
	player->render();
	currentTime = 0.0f;
	return;
}




