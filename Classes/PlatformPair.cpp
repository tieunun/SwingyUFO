//
//  PlatformPair.cpp
//  SwingyChute
//

#include "PlatformPair.h"
#include "Config.h"

using namespace cocos2d;

#define GAP_DISTANCE 285.0f
#define WIDTH_16_9 450.0f
#define PLATFORM_HEIGHT 32
#define PLATFORM_WIDTH (600.0f - GAP_DISTANCE)
#define PLATFORM_TRAVEL_TIME 5.0f

PlatformPair* PlatformPair::create() {
    auto pair = new PlatformPair();
    if (pair && pair->init()) {
        pair->autorelease();
        return pair;
    }
    delete pair;
    pair = nullptr;
    return nullptr;
}

bool PlatformPair::init() {
    
    if (!Node::init()) { return false; }
    
    mScreenSize = Director::getInstance()->getWinSize();
    
    float minStartX, maxStartX;
    Vec2 startPos;
    
    // The most narrow devices (those with an aspect ratio of 16:9)
    // will have an adjusted width of 450. On these devices, the gaps
    // will sometimes reach the outermost edges of the screen, while on
    // other devices gaps will get close but never fully touch the edges.
    float halfDiff = (mScreenSize.width - WIDTH_16_9) / 2.0f;
    float halfWidth = PLATFORM_WIDTH * 0.5f;
    minStartX = halfDiff - halfWidth;
    maxStartX = mScreenSize.width - halfDiff - GAP_DISTANCE - halfWidth;
    
    //    CCLOG("minStartX = %f", minStartX);
    //    CCLOG("maxStartX = %f", maxStartX);
    
    // Random number between minStartX and maxStartX
    startPos.x = (rand() % int(maxStartX - minStartX + 1)) + minStartX;
    startPos.y = -0.5f * PLATFORM_HEIGHT;
    
    addPlatform(startPos);
    // We must add the platform width to account for anchor points
    startPos.x += GAP_DISTANCE + PLATFORM_WIDTH;
    addPlatform(startPos);
    startPos.x -= (GAP_DISTANCE + PLATFORM_WIDTH) * 0.5f;
    addPointZone(startPos);
    
    return true;
}

void PlatformPair::addPlatform(Vec2 pos) {
    auto plat = Sprite::create();
    Rect rect = Rect(0, 0, PLATFORM_WIDTH, PLATFORM_HEIGHT);
    auto body = PhysicsBody::createBox(rect.size);
    body->setDynamic(false);    // Platforms are not affected by gravity
    body->setCollisionBitmask(0);
    body->setCategoryBitmask(PhysicsGroup::PLATFORM);
    body->setContactTestBitmask(PhysicsGroup::PLAYER);
    plat->setPhysicsBody(body);
    plat->setTextureRect(rect);
    plat->setColor(Color3B(0, 255, 0));
    plat->setPosition(pos);
    this->addChild(plat);
}

void PlatformPair::addPointZone(Vec2 pos) {
    auto zone = Node::create();
    auto body = PhysicsBody::createBox(Size(GAP_DISTANCE * 0.9f, PLATFORM_HEIGHT * 0.5f));
    body->setDynamic(false);
    body->setCollisionBitmask(0);
    body->setCategoryBitmask(PhysicsGroup::POINT_ZONE);
    body->setContactTestBitmask(PhysicsGroup::PLAYER);
    zone->setPhysicsBody(body);
    zone->setPosition(pos);
    this->addChild(zone);
}

void PlatformPair::startMoving() {
    auto move = MoveBy::create(PLATFORM_TRAVEL_TIME, Vec2(0.0f, mScreenSize.height + PLATFORM_HEIGHT));
    
    // After moving past the top of the screen, this node will remove itself from the game layer
    auto removeSelf = CallFunc::create([=] {
        this->removeFromParent();
    });
    
    auto moveThenRemoveSelf = Sequence::create(move, removeSelf, nullptr);
    
    this->runAction(moveThenRemoveSelf);
}