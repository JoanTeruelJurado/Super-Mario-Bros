#pragma once
#define _MENU_INCLUDE
#define _MENU_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Menu {
public:
    Menu();
    ~Menu();

    Sprite* menu1, * menu2, * menu3, * levels1, * levels2, * levels3, * instructions;
    TileMap* map;
    Texture menu_1, menu_2, menu_3, levels_1, levels_2, levels_3, instructions_0;
    int menu, state;
    bool changedRecently;
    float timerChangemenuion;

    void init(ShaderProgram& shaderProgram);
    void render();
    int update(int deltaTime);
};

