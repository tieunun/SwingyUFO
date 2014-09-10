//
//  Player.cpp
//  SwingyChute
//

#include "Player.h"

using namespace cocos2d;

#define ACCELERATION_X 480.0f

Player::Player() : mVelocity(Vec2::ZERO), mAcceleration(ACCELERATION_X, 0.0f), mFacingDirection(NEITHER) {}

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
    this->reset();
    
    return true;
}

void Player::update(float dt) {
    
    switch (mFacingDirection) {
            
        case LEFT:
            mVelocity.x += -mAcceleration.x * dt;
            break;
            
        case RIGHT:
            mVelocity.x += mAcceleration.x * dt;
            break;
            
        default:
            break;
    }
    
    // Velocity capping
    if (mVelocity.x > 480.0f) { mVelocity.x = 480.0f; }
    if (mVelocity.x < -480.0f) { mVelocity.x = -480.0f; }
    
    float posX = this->getPositionX() + (mVelocity.x * dt);
    
    // Boundary checks
    float halfWidth = this->getContentSize().width * 0.5f;
    if (posX > mScreenSize.width - halfWidth) { posX = mScreenSize.width - halfWidth; }
    if (posX < halfWidth) { posX = halfWidth; }
    
    this->setPositionX(posX);
}

void Player::reset() {
    this->setPosition(Vec2(mScreenSize.width * 0.5f, mScreenSize.height * 0.8f));
    mVelocity = Vec2::ZERO;
    mFacingDirection = NEITHER;
}

void Player::switchDirections() {
    
    switch (mFacingDirection) {
            
        case LEFT:
            CCLOG("Switching from LEFT to RIGHT");
            mFacingDirection = RIGHT;
            mVelocity = Vec2::ZERO;
            break;
            
        case RIGHT:
            CCLOG("Switching from RIGHT to LEFT");
            mFacingDirection = LEFT;
            mVelocity = Vec2::ZERO;
            break;
            
        default:
            CCLOG("Not facing any direction, not switching");
            break;
    }
}