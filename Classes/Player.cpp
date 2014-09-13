//
//  Player.cpp
//  SwingyChute
//

#include "Player.h"
#include "Config.h"

using namespace cocos2d;

#define PLAYER_HEIGHT 64
#define PLAYER_WIDTH 64
#define PLAYER_MAX_VELOCITY 480.0f
#define PLAYER_IMPULSE 480.0f

Player::Player() : mFacingDirection(NEITHER) {
    mScreenSize = Director::getInstance()->getWinSize();
}

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
    
    Rect rect = Rect(0.0f, 0.0f, PLAYER_WIDTH, PLAYER_HEIGHT);
    
    auto body = PhysicsBody::createBox(rect.size);
    body->setGroup(PhysicsGroup::PLAYER);
    body->setCollisionBitmask(PhysicsGroup::EDGE);
    body->setCategoryBitmask(PhysicsGroup::PLAYER);
    body->setContactTestBitmask(PhysicsGroup::PLATFORM | PhysicsGroup::POINT_ZONE | PhysicsGroup::EDGE);
    body->setRotationEnable(false);
    
    this->setPhysicsBody(body);
    this->setTextureRect(rect);
    this->setColor(Color3B::BLUE);
    this->reset();
    
    return true;
}

void Player::reset() {
    this->setPosition(Vec2(mScreenSize.width * 0.5f, mScreenSize.height * 0.8f));
    mFacingDirection = NEITHER;
    this->getPhysicsBody()->resetForces();
}

void Player::switchDirections() {
    
    switch (mFacingDirection) {
            
        case LEFT:
            CCLOG("Switching from LEFT to RIGHT");
            this->getPhysicsBody()->setVelocity(Vec2::ZERO);
            this->getPhysicsBody()->applyImpulse(Vec2(PLAYER_IMPULSE, 0));
            mFacingDirection = RIGHT;
            break;
            
        case RIGHT:
            CCLOG("Switching from RIGHT to LEFT");
            this->getPhysicsBody()->setVelocity(Vec2::ZERO);
            this->getPhysicsBody()->applyImpulse(Vec2(-PLAYER_IMPULSE, 0));
            mFacingDirection = LEFT;
            break;
            
        default:
            CCLOG("Not facing any direction, not switching");
            break;
    }
}