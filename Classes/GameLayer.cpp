#include "GameLayer.h"

using namespace cocos2d;

Scene* GameLayer::createScene() {
    
    // Create the game scene that the director will use
    auto scene = Scene::create();
    
    // Create the main game layer and add it to the scene
    auto layer = GameLayer::create();
    scene->addChild(layer);

    return scene;
}

GameLayer* GameLayer::create() {
    auto layer = new GameLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    delete layer;
    layer = nullptr;
    return nullptr;
}

bool GameLayer::init() {
    
    if (!Layer::init()) { return false; }
    
    this->populateScene();
    this->addEvents();
    this->scheduleUpdate();
    
    return true;
}

void GameLayer::populateScene() {
    
    player = Player::create();
    this->addChild(player);
}

void GameLayer::addEvents() {
    
    // Register to listen for touch events
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
}

void GameLayer::update(float dt) {
    player->update(dt);
}

bool GameLayer::onTouchBegan(Touch *touch, Event *event) {
    player->switchDirections();
    return true;
}
