#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

//#include "irrKlang.h"

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
	backmap = NULL;
	player = NULL;
	goomba = NULL;
	koopatroopa = NULL;
	camera = new Camera();
}

Scene::~Scene()
{
	if (backmap != NULL)
		delete backmap;
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

}


void Scene::init(const int &lv)
{

	level = lv;
	scroll = 0;
	camera->CameraUpdate(0);
	initShaders();
	initNumbers();
	if (level == 0) {
		player = new Player();
		menu = new Menu();
		menu->init(texProgram);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
		PlaySound(L"sounds/01.-Ground-Theme.wav", NULL, SND_ASYNC|SND_LOOP);
	}
	else if (level == 1) {
		glClearColor(92.0f / 255.0f, 148.0f / 255.0f, 252.0f / 255.0f, 1.0f);
		
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(0, 0), texProgram);
		backmap = TileMap::createTileMap("levels/level01_background.txt", glm::vec2(0, 0), texProgram);
		scoreBoard.loadFromFile("images/scoreboard.png", TEXTURE_PIXEL_FORMAT_RGBA);
		scoreboard = Sprite::createSprite(glm::ivec2(300, 20), glm::vec2(1.0, 1.0), &scoreBoard, &texProgram);
		scoreboard->setPosition(glm::vec2(0, 0));
		
		player->init(glm::ivec2(0, 0), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);
		player->setlvl(1);
		goomba = new Goomba();
		goomba->init(glm::ivec2(0, 0), texProgram);
		goomba->setPosition(glm::vec2(10 * map->getTileSize(), 12 * map->getTileSize()));
		goomba->setTileMap(map);
		koopatroopa = new KoopaTroopa();
		koopatroopa->init(glm::ivec2(0, 0), texProgram);
		koopatroopa->setPosition(glm::vec2(15 * map->getTileSize(), 12 * map->getTileSize() - 8));
		koopatroopa->setTileMap(map);
		assignNumber(world, 1);
		assignNumber(lvl, 1);

		projection = glm::ortho(0.f, 300.f, 225.f, 0.f); // 300 225
		currentTime = 0.0f;
	}

	else if (level == 2) {
		glClearColor(.0f, .0f, .0f, 1.0f);
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(0, 0), texProgram);
		scoreBoard.loadFromFile("images/scoreboard.png", TEXTURE_PIXEL_FORMAT_RGBA);
		scoreboard = Sprite::createSprite(glm::ivec2(300, 20), glm::vec2(1.0, 1.0), &scoreBoard, &texProgram);
		scoreboard->setPosition(glm::vec2(0, 0));
		player = new Player();
		player->init(glm::ivec2(0, 0), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);
		assignNumber(world, 1);
		assignNumber(lvl, 2);

		player->setlvl(2);
		projection = glm::ortho(0.f, 300.f, 225.f, 0.f); // 300 225
		currentTime = 0.0f;
	}
	else if (level == 3) {
		menu->setState(55);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	}
	else if (level == 4) {
		menu->setState(44);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH)-40, float(SCREEN_HEIGHT), 0.f);
	}
}

void Scene::update(int deltaTime)
{

	if (level == 0) {
		level = menu->update(deltaTime);
		if (level != 0) {
			init(level);
		}
	}
	else {
		
		

		if (player->getlives() <= 0) {
			
			init(4);
			player->setlives(3);
			return;
		}
		int marioposx = player->getPos().x;
		if (marioposx >= scroll + ortho_size /2) {
			scroll += abs(marioposx - (scroll + ortho_size / 2));
			camera->CameraUpdate(scroll);
			player->setMinPos(scroll);
			scoreboard->setPosition(glm::vec2(scroll, 0));
			centenaPoints->setPosition(glm::vec2(scroll + 22, 10));
			decenaPoints->setPosition(glm::vec2(scroll + 30, 10));
			unitPoints->setPosition(glm::vec2(scroll + 38, 10));
			decenaCoins->setPosition(glm::vec2(scroll + 84, 10));
			unitCoins->setPosition(glm::vec2(scroll + 92, 10));
			world->setPosition(glm::vec2(scroll + 138, 10));
			lvl->setPosition(glm::vec2(scroll + 154, 10));
			centenaTimer->setPosition(glm::vec2(scroll + 198, 10));
			decenaTimer->setPosition(glm::vec2(scroll + 206, 10));
			unitTimer->setPosition(glm::vec2(scroll + 214, 10));
			lives->setPosition(glm::vec2(scroll + 265, 10));
		}
		if (player->getMariostate() == 4) { //el jugador ha muerto
			init(level);
			player->init(glm::ivec2(0, 0), texProgram);
			//player->setlives(lives - 1);
			player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
			camera->CameraUpdate(0);
			player->setMinPos(0);
			scoreboard->setPosition(glm::vec2(scroll, 0));
		}
		if (player->getMariostate() == 6) { //el jugador ha ganado
			init(level+1);
			player->init(glm::ivec2(0, 0), texProgram);
			//player->setlives(lives);
			player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
			camera->CameraUpdate(0);
			player->setMinPos(0);
		}
		updateTimer();
		updateCoins();
		updatePoints();
		updateLives();
	}

	if (paused) return;
	currentTime += deltaTime;
	if (level == 1) {
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

	if (level == 2) {
		player->update(deltaTime);
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

	if (level == 0) {
		menu->render();
	}
	else if (level == 1) {
		backmap->render();
		map->render();
		player->render();
		goomba->render();
		koopatroopa->render();
		scoreboard->render();
		centenaPoints->render();
		decenaPoints->render();
		unitPoints->render();
		decenaCoins->render();
		unitCoins->render();
		world->render();
		lvl->render();
		centenaTimer->render();
		decenaTimer->render();
		unitTimer->render();
		lives->render();
	}
	else if (level == 2){
		map->render();
		player->render();
		scoreboard->render();
		centenaTimer->render();
		decenaTimer->render();
		unitTimer->render();
		centenaPoints->render();
		decenaPoints->render();
		unitPoints->render();
		decenaCoins->render();
		unitCoins->render();
		world->render();
		lvl->render();
		centenaTimer->render();
		decenaTimer->render();
		unitTimer->render();
		lives->render();
	}
	else if (level == 3) {
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
	init(sceneID);
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
	else if (!kill && (bottomP > topE) && (leftP < rightE && rightP > leftE)) {
		if (player->getMariostate() == 1) return true;
		else if (player->getMariostate() != 0) player->ChangeType(0);
		else player->setDeath();
	}
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

enum Numbers
{
	CERO, UNO, DOS, TRES, CUATRO, CINCO, SEIS, SIETE, OCHO, NUEVE
};

void Scene::initNumbers() {
	numbers.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	centenaPoints = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	centenaPoints->setNumberAnimations(10);
	centenaPoints->setAnimationSpeed(CERO, 8);
	centenaPoints->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	centenaPoints->setAnimationSpeed(UNO, 8);
	centenaPoints->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	centenaPoints->setAnimationSpeed(DOS, 8);
	centenaPoints->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	centenaPoints->setAnimationSpeed(TRES, 8);
	centenaPoints->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	centenaPoints->setAnimationSpeed(CUATRO, 8);
	centenaPoints->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
	centenaPoints->setAnimationSpeed(CINCO, 8);
	centenaPoints->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
	centenaPoints->setAnimationSpeed(SEIS, 8);
	centenaPoints->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
	centenaPoints->setAnimationSpeed(SIETE, 8);
	centenaPoints->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
	centenaPoints->setAnimationSpeed(OCHO, 8);
	centenaPoints->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
	centenaPoints->setAnimationSpeed(NUEVE, 8);
	centenaPoints->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
	centenaPoints->setPosition(glm::vec2(22, 10));

	decenaPoints = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	decenaPoints->setNumberAnimations(10);
	decenaPoints->setAnimationSpeed(CERO, 8);
	decenaPoints->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	decenaPoints->setAnimationSpeed(UNO, 8);
	decenaPoints->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	decenaPoints->setAnimationSpeed(DOS, 8);
	decenaPoints->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	decenaPoints->setAnimationSpeed(TRES, 8);
	decenaPoints->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	decenaPoints->setAnimationSpeed(CUATRO, 8);
	decenaPoints->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
	decenaPoints->setAnimationSpeed(CINCO, 8);
	decenaPoints->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
	decenaPoints->setAnimationSpeed(SEIS, 8);
	decenaPoints->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
	decenaPoints->setAnimationSpeed(SIETE, 8);
	decenaPoints->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
	decenaPoints->setAnimationSpeed(OCHO, 8);
	decenaPoints->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
	decenaPoints->setAnimationSpeed(NUEVE, 8);
	decenaPoints->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
	decenaPoints->setPosition(glm::vec2(30, 10));

	unitPoints = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	unitPoints->setNumberAnimations(10);
	unitPoints->setAnimationSpeed(CERO, 8);
	unitPoints->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	unitPoints->setAnimationSpeed(UNO, 8);
	unitPoints->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	unitPoints->setAnimationSpeed(DOS, 8);
	unitPoints->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	unitPoints->setAnimationSpeed(TRES, 8);
	unitPoints->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	unitPoints->setAnimationSpeed(CUATRO, 8);
	unitPoints->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
	unitPoints->setAnimationSpeed(CINCO, 8);
	unitPoints->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
	unitPoints->setAnimationSpeed(SEIS, 8);
	unitPoints->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
	unitPoints->setAnimationSpeed(SIETE, 8);
	unitPoints->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
	unitPoints->setAnimationSpeed(OCHO, 8);
	unitPoints->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
	unitPoints->setAnimationSpeed(NUEVE, 8);
	unitPoints->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
	unitPoints->setPosition(glm::vec2(38, 10));

	decenaCoins = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	decenaCoins->setNumberAnimations(10);
	decenaCoins->setAnimationSpeed(CERO, 8);
	decenaCoins->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	decenaCoins->setAnimationSpeed(UNO, 8);
	decenaCoins->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	decenaCoins->setAnimationSpeed(DOS, 8);
	decenaCoins->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	decenaCoins->setAnimationSpeed(TRES, 8);
	decenaCoins->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	decenaCoins->setAnimationSpeed(CUATRO, 8);
	decenaCoins->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
	decenaCoins->setAnimationSpeed(CINCO, 8);
	decenaCoins->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
	decenaCoins->setAnimationSpeed(SEIS, 8);
	decenaCoins->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
	decenaCoins->setAnimationSpeed(SIETE, 8);
	decenaCoins->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
	decenaCoins->setAnimationSpeed(OCHO, 8);
	decenaCoins->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
	decenaCoins->setAnimationSpeed(NUEVE, 8);
	decenaCoins->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
	decenaCoins->setPosition(glm::vec2(84, 10));

	unitCoins = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	unitCoins->setNumberAnimations(10);
	unitCoins->setAnimationSpeed(CERO, 8);
	unitCoins->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	unitCoins->setAnimationSpeed(UNO, 8);
	unitCoins->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	unitCoins->setAnimationSpeed(DOS, 8);
	unitCoins->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	unitCoins->setAnimationSpeed(TRES, 8);
	unitCoins->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	unitCoins->setAnimationSpeed(CUATRO, 8);
	unitCoins->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
	unitCoins->setAnimationSpeed(CINCO, 8);
	unitCoins->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
	unitCoins->setAnimationSpeed(SEIS, 8);
	unitCoins->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
	unitCoins->setAnimationSpeed(SIETE, 8);
	unitCoins->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
	unitCoins->setAnimationSpeed(OCHO, 8);
	unitCoins->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
	unitCoins->setAnimationSpeed(NUEVE, 8);
	unitCoins->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
	unitCoins->setPosition(glm::vec2(92, 10));

	world = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	world->setNumberAnimations(1);
	world->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	world->setAnimationSpeed(UNO, 8);
	world->setPosition(glm::vec2(138, 10));

	lvl = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	lvl->setNumberAnimations(1);
	lvl->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	lvl->setAnimationSpeed(UNO, 8);
	lvl->addKeyframe(DOS, glm::vec2(0.1f, 1.0f));
	lvl->setAnimationSpeed(DOS, 8);
	lvl->setPosition(glm::vec2(154, 10));

	centenaTimer = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	centenaTimer->setNumberAnimations(10);
	centenaTimer->setAnimationSpeed(CERO, 8);
	centenaTimer->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	centenaTimer->setAnimationSpeed(UNO, 8);
	centenaTimer->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	centenaTimer->setAnimationSpeed(DOS, 8);
	centenaTimer->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	centenaTimer->setAnimationSpeed(TRES, 8);
	centenaTimer->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	centenaTimer->setAnimationSpeed(CUATRO, 8);
	centenaTimer->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
	centenaTimer->setAnimationSpeed(CINCO, 8);
	centenaTimer->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
	centenaTimer->setAnimationSpeed(SEIS, 8);
	centenaTimer->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
	centenaTimer->setAnimationSpeed(SIETE, 8);
	centenaTimer->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
	centenaTimer->setAnimationSpeed(OCHO, 8);
	centenaTimer->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
	centenaTimer->setAnimationSpeed(NUEVE, 8);
	centenaTimer->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
	centenaTimer->setPosition(glm::vec2(198, 10));

	decenaTimer = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	decenaTimer->setNumberAnimations(10);
	decenaTimer->setAnimationSpeed(CERO, 8);
	decenaTimer->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	decenaTimer->setAnimationSpeed(UNO, 8);
	decenaTimer->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	decenaTimer->setAnimationSpeed(DOS, 8);
	decenaTimer->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	decenaTimer->setAnimationSpeed(TRES, 8);
	decenaTimer->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	decenaTimer->setAnimationSpeed(CUATRO, 8);
	decenaTimer->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
	decenaTimer->setAnimationSpeed(CINCO, 8);
	decenaTimer->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
	decenaTimer->setAnimationSpeed(SEIS, 8);
	decenaTimer->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
	decenaTimer->setAnimationSpeed(SIETE, 8);
	decenaTimer->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
	decenaTimer->setAnimationSpeed(OCHO, 8);
	decenaTimer->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
	decenaTimer->setAnimationSpeed(NUEVE, 8);
	decenaTimer->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
	decenaTimer->setPosition(glm::vec2(206, 10));

	unitTimer = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	unitTimer->setNumberAnimations(10);
	unitTimer->setAnimationSpeed(CERO, 8);
	unitTimer->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	unitTimer->setAnimationSpeed(UNO, 8);
	unitTimer->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	unitTimer->setAnimationSpeed(DOS, 8);
	unitTimer->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	unitTimer->setAnimationSpeed(TRES, 8);
	unitTimer->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	unitTimer->setAnimationSpeed(CUATRO, 8);
	unitTimer->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
	unitTimer->setAnimationSpeed(CINCO, 8);
	unitTimer->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
	unitTimer->setAnimationSpeed(SEIS, 8);
	unitTimer->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
	unitTimer->setAnimationSpeed(SIETE, 8);
	unitTimer->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
	unitTimer->setAnimationSpeed(OCHO, 8);
	unitTimer->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
	unitTimer->setAnimationSpeed(NUEVE, 8);
	unitTimer->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
	unitTimer->setPosition(glm::vec2(214, 10));

	lives = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1.0), &numbers, &texProgram);
	lives->setNumberAnimations(10);
	lives->setAnimationSpeed(CERO, 8);
	lives->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
	lives->setAnimationSpeed(UNO, 8);
	lives->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
	lives->setAnimationSpeed(DOS, 8);
	lives->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
	lives->setAnimationSpeed(TRES, 8);
	lives->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
	lives->setPosition(glm::vec2(265, 10));
}

void Scene::assignNumber(Sprite* s, int n) {
	if (n < 0 || n > 9) s->changeAnimation(0);
	else s->changeAnimation(n);
}

void Scene::updateTimer() {
	int time = player->gettime() / 100;
	assignNumber(unitTimer, time % 10);
	time = time / 10;
	assignNumber(decenaTimer, time % 10);
	time = time / 10;
	assignNumber(centenaTimer, time % 10);
}

void Scene::updateCoins() {
	int coins = player->getcoins();
	assignNumber(unitCoins, coins % 10);
	coins = coins / 10;
	assignNumber(decenaCoins, coins % 10);
}

void Scene::updatePoints() {
	int points = player->getscore();
	assignNumber(unitPoints, points % 10);
	points = points / 10;
	assignNumber(decenaPoints, points % 10);
	points = points / 10;
	assignNumber(centenaPoints, points % 10);
}

void Scene::updateLives() {
	int l = player->getlives();
	assignNumber(lives, l % 10);
}
