#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>

void Game::init()
{
	bPlay = true;
	glClearColor(92.0f / 255.0f, 148.0f / 255.0f, 252.0f / 255.0f, 1.0f);
	scene.init(0);
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

using namespace std;
void Game::keyPressed(int key)
{
	//std::cout << key << endl;
	if (key == 49) scene.changeScene(1); // '1' key
	if (key == 50) scene.changeScene(2); // '2' key
	if (key == 112 || key==80) { scene.paused = !(scene.paused);} // P key
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}
//#include <windows.h>
//#include <mmsystem.h>
// bool pl = PlaySound(L"sounds/smb_gameover.wav", NULL, SND_SYNC);  para la musica
void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	//std::cout << key << endl;
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





