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
    Direction mFacingDirection;
    
    void setUpAnimation();
    void setUpPhysics();
  
public:
    Player();
    
    static Player* create();
    
    // overrides
    bool init() override;

    // etc
    void reset();
    void die();
    void switchDirections();
    void setDirection(Direction dir);
    Direction getDirection() const { return mFacingDirection; }
};

#endif /* defined(__SwingyChute__Player__) */
