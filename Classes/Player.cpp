//
//  Player.cpp
//  SwingyChute
//

#include "Player.h"

using namespace cocos2d;

Player::Player() : mFacingDirection(NEITHER) {}

Player* Player::create() {
    auto player = new Player();
    if (player && player->init()) {
        player->autorelease();
        return player;
    }
    delete player;
    player = nullptr;
    return nullptr;
}

bool Player::init() {
    
    if (!Sprite::init()) { return false; }
    
    mScreenSize = Director::getInstance()->getWinSize();
    
    this->setTextureRect(Rect(0, 0, 64, 64));
    this->setColor(Color3B::BLUE);
    this->setPosition(Vec2(mScreenSize.width * 0.5f, mScreenSize.height * 0.8f));
    
    return true;
}

void Player::update(float dt) {
}

void Player::switchDirections() {
    
    switch (mFacingDirection) {
            
        case LEFT:
            CCLOG("Switching from LEFT to RIGHT");
            mFacingDirection = RIGHT;
            break;
            
        case RIGHT:
            CCLOG("Switching from RIGHT to LEFT");
            mFacingDirection = LEFT;
            break;
            
        default:
            CCLOG("Not facing any direction, not switching");
            break;
    }
}