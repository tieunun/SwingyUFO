//
//  MainMenuScreen.cpp
//  SwingyChute
//

#include "MainMenuScreen.h"
#include "GameLayer.h"
#include "Config.h"

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
    if (!LayerColor::initWithColor(BG_COLOR)) { return false; }
    this->populateScene();
    
    return true;
}

void MainMenuScreen::populateScene() {
    Size screenSize = Director::getInstance()->getWinSize();
    
    // Game title
    Label *label = Label::createWithTTF("Swingy UFO", FONT_04, 60.0f);
    label->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.8f));
    this->addChild(label, 2);
    
    // Play button
    auto startGame = [] (Ref*) {
        auto gameScene = GameLayer::createScene();
        Director::getInstance()->replaceScene(gameScene);
    };
    auto frameCache = SpriteFrameCache::getInstance();
    auto playSprite = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("play_button.png"));
    auto playPressedSprite = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName(("play_button_pressed.png")));
    playSprite->getTexture()->setAliasTexParameters();
    float scaleFactor = Director::getInstance()->getContentScaleFactor();
    float scale = 3.0f * scaleFactor;
    auto playButton = MenuItemSprite::create(playSprite, playPressedSprite, startGame);
    playButton->setScale(scale);
    auto menu = Menu::create(playButton, nullptr);
    menu->alignItemsHorizontally();
    menu->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.25f));
    this->addChild(menu, 2);
    
    auto particle = ParticleSystemQuad::create("stars.plist");
    this->addChild(particle);
}

