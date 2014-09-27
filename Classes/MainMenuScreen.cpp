//
//  MainMenuScreen.cpp
//  SwingyChute
//

#include "MainMenuScreen.h"
#include "GameLayer.h"

#define FONT_04 "04B_19__.ttf"

using namespace cocos2d;

Scene* MainMenuScreen::createScene() {
    auto scene = Scene::create();
    auto mainMenu = MainMenuScreen::create();
    scene->addChild(mainMenu);
    return scene;
}

MainMenuScreen* MainMenuScreen::create() {
    auto mainMenu = new MainMenuScreen();
    if (mainMenu && mainMenu->init()) {
        mainMenu->autorelease();
        return mainMenu;
    }
    delete mainMenu;
    mainMenu = nullptr;
    return nullptr;
}

bool MainMenuScreen::init() {
    if (!Layer::init()) { return false; }
    
    this->populateScene();
    
    return true;
}

void MainMenuScreen::populateScene() {
    Size screenSize = Director::getInstance()->getWinSize();
    
    // Game title
    Label *label = Label::createWithTTF("Swingy Chute", FONT_04, 60.0f);
    label->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.8f));
    this->addChild(label);
    
    // Play button
    auto startGame = [] (Ref*) {
        auto gameScene = GameLayer::createScene();
        Director::getInstance()->replaceScene(gameScene);
    };
    label = Label::createWithTTF("Play", FONT_04, 50.0f);
    auto playButton = MenuItemLabel::create(label, startGame);
    auto menu = Menu::create(playButton, nullptr);
    menu->alignItemsHorizontally();
    menu->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.25f));
    this->addChild(menu);
}

