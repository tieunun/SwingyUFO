//
//  MainMenuScreen.h
//  SwingyChute
//

#ifndef __SwingyChute__MainMenuScreen__
#define __SwingyChute__MainMenuScreen__

#include "cocos2d.h"

class MainMenuScreen : public cocos2d::LayerColor {
private:
    void populateScene();
    
public:
    static MainMenuScreen* create();
    static cocos2d::Scene* createScene();
    
    // overrides
    bool init() override;
    
    // etc
    void onPressPlay(cocos2d::Ref *ref);
};

#endif /* defined(__SwingyChute__MainMenuScreen__) */
