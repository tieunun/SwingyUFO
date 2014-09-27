//
//  PlatformPair.cpp
//  SwingyChute
//

#include "PlatformPair.h"
#include "Config.h"

using namespace cocos2d;

#define GAP_DISTANCE 300.0f
#define WIDTH_16_9 450.0f
#define PLATFORM_TRAVEL_TIME 5.0f
#define START_PADDING 25.0f

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

    float swingRotation = rand() % 360;
    auto plat = addPlatform(swingRotation);
    // We must add the platform width to account for anchor points
    Vec2 offset = Vec2(GAP_DISTANCE + plat->getBoundingBox().size.width, 0.0f);
    auto plat2 = addPlatform(swingRotation, false, plat->getPosition() + offset);
    addPointZone(Vec2((plat->getPosition().x + plat2->getPosition().x) * 0.5f, plat->getPosition().y));
    
    return true;
}

Sprite* PlatformPair::addPlatform(float swingRotation, bool isLeft, Vec2 pos) {
    auto plat = createPlatform();
    auto swing = createSwing(swingRotation);
    this->addChild(plat);
    this->addChild(swing);
    
    if (isLeft) {
        float firstMinStartX, firstMaxStartX;
        float halfWidth = plat->getBoundingBox().size.width * 0.5f;
        firstMinStartX = -1.0f * halfWidth + START_PADDING;
        firstMaxStartX = mScreenSize.width - GAP_DISTANCE - halfWidth - START_PADDING;
        
        // Random number between minStartX and maxStartX
        pos.x = (rand() % int(firstMaxStartX - firstMinStartX + 1)) + firstMinStartX;
        pos.y -= 0.5f * plat->getBoundingBox().size.height + swing->getBoundingBox().size.height;
    } else {
        plat->setFlippedX(true);
    }
    
    plat->setPosition(pos);
    
    if (isLeft)
        pos.x += plat->getBoundingBox().size.width * 0.45f;
    else
        pos.x -= plat->getBoundingBox().size.width * 0.45f;
    
    swing->setPosition(pos);
    swing->runAction(RepeatForever::create(RotateBy::create(2.0f, 360.0f)));
    
    return plat;
}

Sprite* PlatformPair::createObstacleSprite(const std::string &fileName, float scaleX, float scaleY) {
    auto obstacle = Sprite::createWithSpriteFrameName(fileName);
    obstacle->setScale(scaleX, scaleY);
    obstacle->getTexture()->setAliasTexParameters();
    auto body = PhysicsBody::createBox(obstacle->getBoundingBox().size);
    body->setDynamic(false);      // Platforms are not affected by gravity
    body->setCollisionBitmask(0);
    body->setCategoryBitmask(PhysicsGroup::PLATFORM);
    body->setContactTestBitmask(PhysicsGroup::PLAYER);
    obstacle->setPhysicsBody(body);
    return obstacle;
}

Sprite* PlatformPair::createPlatform() {
    float scaleFactor = Director::getInstance()->getContentScaleFactor();
    float scale = 3.0f * scaleFactor;
    return createObstacleSprite("platform.png", scale, scale);
}

Sprite* PlatformPair::createSwing(float rotation) {
    auto swingBody = Sprite::createWithSpriteFrameName("chain.png");
    float scale = 3.0f * Director::getInstance()->getContentScaleFactor();
    swingBody->setScale(scale, scale);
    swingBody->getTexture()->setAliasTexParameters();
    swingBody->setAnchorPoint(Vec2(0.5f, 0.0f));
    auto swingHead = createObstacleSprite("asteroid.png", 1.0f, 1.0f);
    swingHead->setPosition(swingBody->getPositionX() + swingBody->getContentSize().width * 0.5f, swingBody->getContentSize().height + swingHead->getContentSize().height * 0.6f);
    swingBody->addChild(swingHead);
    swingBody->setRotation(rotation);
    
    return swingBody;
}

void PlatformPair::addPointZone(Vec2 pos) {
    auto zone = Node::create();
    auto body = PhysicsBody::createBox(Size(GAP_DISTANCE * 0.9f, 16.0f));
    body->setDynamic(false);
    body->setCollisionBitmask(0);
    body->setCategoryBitmask(PhysicsGroup::POINT_ZONE);
    body->setContactTestBitmask(PhysicsGroup::PLAYER);
    zone->setPhysicsBody(body);
    zone->setPosition(pos);
    this->addChild(zone);
}

void PlatformPair::startMoving() {
    auto move = MoveBy::create(PLATFORM_TRAVEL_TIME, Vec2(0.0f, mScreenSize.height * 1.5f));
    
    // After moving past the top of the screen, this node will remove itself from the game layer
    auto removeSelf = CallFunc::create([=] {
        this->removeFromParent();
    });
    
    auto moveThenRemoveSelf = Sequence::create(move, removeSelf, nullptr);
    
    this->runAction(moveThenRemoveSelf);
}