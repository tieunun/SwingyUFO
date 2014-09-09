#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "Player.h"
#include "cocos2d.h"

class GameLayer : public cocos2d::Layer {
private:
    Player *player;
    
public:
    static GameLayer* create();
    static cocos2d::Scene* createScene();
    
    // overrides
    bool init() override;
    void update(float dt) override;
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    
    // etc
    void addEvents();
    void populateScene();
};

#endif
