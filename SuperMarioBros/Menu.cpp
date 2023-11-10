#include "Menu.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>

#define TIME_FOR_CHANGE 0.16

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::init(ShaderProgram& shaderProgram) {
    state = 0;
    menu = 0;
    changedRecently = false;
    timerChangemenuion = 0.f;
    menu_1.loadFromFile("images/menu_1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    menu1 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &menu_1, &shaderProgram);
    menu_2.loadFromFile("images/menu_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    menu2 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &menu_2, &shaderProgram);
    menu_3.loadFromFile("images/menu_3.png", TEXTURE_PIXEL_FORMAT_RGBA);
    menu3 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &menu_3, &shaderProgram);
    levels_1.loadFromFile("images/levels_menu_1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    levels1 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &levels_1, &shaderProgram);
    levels_2.loadFromFile("images/levels_menu_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    levels2 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &levels_2, &shaderProgram);
    levels_3.loadFromFile("images/levels_menu_3.png", TEXTURE_PIXEL_FORMAT_RGBA);
    levels3 = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &levels_3, &shaderProgram);
    instructions_0.loadFromFile("images/instruccions.png", TEXTURE_PIXEL_FORMAT_RGBA);
    instructions = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &instructions_0, &shaderProgram);
    loadScreen_0.loadFromFile("images/LoadScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
    loadScreen = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &loadScreen_0, &shaderProgram);

    menu1->setPosition(glm::vec2(float(0), float(0)));
    menu2->setPosition(glm::vec2(float(0), float(0)));
    menu3->setPosition(glm::vec2(float(0), float(0)));
    levels1->setPosition(glm::vec2(float(0), float(0)));
    levels2->setPosition(glm::vec2(float(0), float(0)));
    levels3->setPosition(glm::vec2(float(0), float(0)));
    instructions->setPosition(glm::vec2(float(0), float(0)));
    loadScreen->setPosition(glm::vec2(float(0), float(0)));
}

int Menu::update(int deltaTime) {
    if (changedRecently) {
        timerChangemenuion += deltaTime / 1000.0;
        if (timerChangemenuion >= TIME_FOR_CHANGE) changedRecently = false;
    }
    if (!changedRecently && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
        ++menu;
        if (menu == 3) menu = 0;
        timerChangemenuion = 0;
        changedRecently = true;
    }
    else if (!changedRecently && Game::instance().getSpecialKey(GLUT_KEY_UP)) {
        --menu;
        if (menu == -1) menu = 2;
        timerChangemenuion = 0;
        changedRecently = true;
    }
    else if (!changedRecently && (Game::instance().getKey(13) || Game::instance().getKey('a'))) {
        if (state == 0) {
            if (menu == 0) state = 1;
            else if (menu == 1) state = 2;
            else if (menu == 2) state = 33;
        }
        else if (state == 1) {
            if (menu == 0) return 1;
            else if (menu == 1) return 2;
            else if (menu == 2) state = 0;
        }
        else if (state == 2) {
            state = 0;
        }
        menu = 0;
        timerChangemenuion = 0;
        changedRecently = true;
    }
    else if (!changedRecently && Game::instance().getKey('b')) {
        if (state == 0) state = 33;
        else state = 0;
        menu = 0;
        timerChangemenuion = 0;
        changedRecently = true;
    }
    return 0;
}
//if (state == exit) { credits , Sleep(2000), exit(),}
void Menu::render() {
    if (state == 0) {
        if (menu == 0) menu1->render();
        else if (menu == 1) menu2->render();
        else if (menu == 2) menu3->render();
    }
    else if (state == 1) {
        if (menu == 0) levels1->render();
        else if (menu == 1) levels2->render();
        else if (menu == 2) levels3->render();
    }
    else if (state == 2) {
        instructions->render();
    }
}