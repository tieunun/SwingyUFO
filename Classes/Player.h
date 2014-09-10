//
//  Player.h
//  SwingyChute
//

#ifndef __SwingyChute__Player__
#define __SwingyChute__Player__

#include "cocos2d.h"

class Player : public cocos2d::Sprite {
public:
    typedef enum {
        LEFT,
        RIGHT,
        NEITHER
    } Direction;
    
private:
    cocos2d::Size mScreenSize;
    
    cocos2d::Vec2 mVelocity;
    const cocos2d::Vec2 mAcceleration;
    
    Direction mFacingDirection;
  
public:
    Player();
    
    static Player* create();
    
    // overrides
    bool init() override;
    void update(float dt) override;

    // etc
    void reset();
    void switchDirections();
    void setDirection(Direction dir) { mFacingDirection = dir; }
};

#endif /* defined(__SwingyChute__Player__) */
