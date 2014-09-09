//
//  Player.h
//  SwingyChute
//

#ifndef __SwingyChute__Player__
#define __SwingyChute__Player__

#include "cocos2d.h"

class Player : public cocos2d::Sprite {
    
    typedef enum {
        LEFT,
        RIGHT,
        NEITHER
    } Direction;
    
    Direction facingDirection;
  
public:
    Player();
    
    static Player* create();
    
    // overrides
    void update(float dt) override;
    
    // etc
    void switchDirections();
};

#endif /* defined(__SwingyChute__Player__) */
