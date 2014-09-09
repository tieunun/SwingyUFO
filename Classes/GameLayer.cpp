#include "GameLayer.h"

USING_NS_CC;

Scene* GameLayer::createScene() {
    
    // Create the game scene that the director will use
    auto scene = Scene::create();
    
    // Create the main game layer and add it to the scene
    auto layer = GameLayer::create();
    scene->addChild(layer);

    return scene;
}

bool GameLayer::init() {
    
    if (!Layer::init()) { return false; }
    
    return true;
}