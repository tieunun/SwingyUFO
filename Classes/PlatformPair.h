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
    
    void addPlatform(cocos2d::Vec2 pos);
    
public:
    static PlatformPair* create();
    
    // overrides
    bool init() override;
    
    // etc
    void startMoving();
};

#endif /* defined(__SwingyChute__PlatformPair__) */
