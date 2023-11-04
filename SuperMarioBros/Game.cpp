#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
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
#include <windows.h>
#include <mmsystem.h>
using namespace std;
void Game::keyPressed(int key)
{
	if (key == 49) scene.changeScene(1); // '1' key
	if (key == 50) scene.changeScene(2); // '2' key
	if (key == 112) { scene.paused = !(scene.paused); bool pl = PlaySound(L"sounds/smb_gameover.wav", NULL, SND_ASYNC); } // P key
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
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





