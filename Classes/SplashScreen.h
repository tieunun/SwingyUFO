//
//  SplashScreen.h
//  SwingyChute
//

#ifndef __SwingyChute__SplashScreen__
#define __SwingyChute__SplashScreen__

#include "cocos2d.h"

class SplashScreen : public cocos2d::Layer {
private:
    float mTimer;
    
    void populateScene();
    void loadSprites();
    
public:
    SplashScreen();
    static SplashScreen* create();
    static cocos2d::Scene* createScene();
    
    // overrides
    bool init() override;
    void update(float dt) override;
};

#endif /* defined(__SwingyChute__SplashScreen__) */
