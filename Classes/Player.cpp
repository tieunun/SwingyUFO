//
//  Player.cpp
//  SwingyChute
//

#include "Player.h"
#include "Config.h"

using namespace cocos2d;

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
    
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("player0.png");
    
    if (!Sprite::initWithSpriteFrame(spriteFrame)) { return false; }

    float scaleFactor = Director::getInstance()->getContentScaleFactor();
    float scale = 3.0f * scaleFactor;
    this->setScale(scale, scale);
    this->getTexture()->setAliasTexParameters();
    
    auto body = PhysicsBody::createBox(this->getBoundingBox().size);
    body->setGroup(PhysicsGroup::PLAYER);
    body->setCollisionBitmask(PhysicsGroup::EDGE);
    body->setCategoryBitmask(PhysicsGroup::PLAYER);
    body->setContactTestBitmask(PhysicsGroup::PLATFORM | PhysicsGroup::POINT_ZONE | PhysicsGroup::EDGE);
    body->setRotationEnable(false);
    
    this->setPhysicsBody(body);

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
            this->setFlippedX(false);
            break;
            
        case RIGHT:
            CCLOG("Switching from RIGHT to LEFT");
            this->getPhysicsBody()->setVelocity(Vec2::ZERO);
            this->getPhysicsBody()->applyImpulse(Vec2(-PLAYER_IMPULSE, 0));
            mFacingDirection = LEFT;
            this->setFlippedX(true);
            break;
            
        default:
            CCLOG("Not facing any direction, not switching");
            break;
    }
}