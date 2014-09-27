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
    
    cocos2d::Sprite* addPlatform(float swingRotation, bool isLeft = true, cocos2d::Vec2 startPos = cocos2d::Vec2::ZERO);
    cocos2d::Sprite* createPlatform();
    cocos2d::Sprite* createSwing(float rotation);
    cocos2d::Sprite* createObstacleSprite(const std::string &fileName, float scaleX, float scaleY);
    void addPointZone(cocos2d::Vec2 pos);
    
public:
    static PlatformPair* create();
    
    // overrides
    bool init() override;
    
    // etc
    void startMoving();
};

#endif /* defined(__SwingyChute__PlatformPair__) */
