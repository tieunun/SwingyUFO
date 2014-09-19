//
//  PlatformPair.h
//  SwingyChute
//

#ifndef __SwingyChute__PlatformPair__
#define __SwingyChute__PlatformPair__

#include "cocos2d.h"

class PlatformPair : public cocos2d::Node {
private:
    cocos2d::Size mScreenSize;
    
    void addPlatform(cocos2d::Vec2 pos, bool isLeft);
    cocos2d::Sprite* createPlatform();
    cocos2d::Sprite* createSwing();
    cocos2d::Sprite* createObstacleSprite(float width, float height, cocos2d::Color3B color = cocos2d::Color3B(0, 255, 0));
    void addPointZone(cocos2d::Vec2 pos);
    
public:
    static PlatformPair* create();
    
    // overrides
    bool init() override;
    
    // etc
    void startMoving();
};

#endif /* defined(__SwingyChute__PlatformPair__) */
