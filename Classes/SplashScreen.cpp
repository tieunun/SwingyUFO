//
//  SplashScreen.cpp
//  SwingyChute
//

#include "SplashScreen.h"
#include "MainMenuScreen.h"

#define FONT_04 "04B_19__.ttf"

using namespace cocos2d;

#define WAIT_TIME 2.0f

SplashScreen::SplashScreen()
: mTimer(0.0f)
{}

Scene* SplashScreen::createScene() {
    auto scene = Scene::create();
    auto splash = SplashScreen::create();
    scene->addChild(splash);
    return scene;
}

SplashScreen* SplashScreen::create() {
    auto splash = new SplashScreen();
    if (splash && splash->init()) {
        splash->autorelease();
        return splash;
    }
    delete splash;
    splash = nullptr;
    return nullptr;
}

bool SplashScreen::init() {
    if (!Layer::init()) { return false; }
    
    this->loadSprites();
    this->populateScene();
    this->scheduleUpdate();
    return true;
}

void SplashScreen::loadSprites() {
    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("sprite_sheet.plist");
}

void SplashScreen::update(float dt) {
    
    mTimer += dt;
    
    if (mTimer >= WAIT_TIME) {
        this->unscheduleUpdate();
        Director::getInstance()->replaceScene(MainMenuScreen::createScene());
    }
}

void SplashScreen::populateScene() {
    Size screenSize = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("Splash", FONT_04, 60.0f);
    label->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.5f));
    this->addChild(label);
}