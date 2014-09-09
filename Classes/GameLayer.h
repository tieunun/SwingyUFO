#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"

class GameLayer : public cocos2d::Layer {
public:
    // manually implements the static create() method
    CREATE_FUNC(GameLayer);
    static cocos2d::Scene* createScene();
    
    // overrides
    bool init() override;
};

#endif
