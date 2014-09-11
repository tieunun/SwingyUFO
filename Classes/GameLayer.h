#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "Player.h"
#include "cocos2d.h"

class GameLayer : public cocos2d::Layer {
private:
    
    typedef enum {
        WaitingForTap,
        Starting,
        Running,
        GameOver
    } GameState;
    
    cocos2d::Size mScreenSize;
    
    GameState mGameState;
    float mStartDelayTimer;
    
    Player *mPlayer;
    
    cocos2d::Label *mGetReadyLabel;
    cocos2d::Label *mTapToStartLabel;
    
    cocos2d::Action *mGetReadyActionIn;
    cocos2d::Action *mGetReadyActionOut;
    cocos2d::Action *mTapToStartActionIn;
    cocos2d::Action *mTapToStartActionOut;
    cocos2d::Action *mSpawnPlatformsForever;
    
    void spawnPlatformPair();
    
public:
    GameLayer();
    ~GameLayer();
    
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
