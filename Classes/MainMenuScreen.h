//
//  MainMenuScreen.h
//  SwingyChute
//
//  Created by Casey Caldwell on 9/12/14.
//
//

#ifndef __SwingyChute__MainMenuScreen__
#define __SwingyChute__MainMenuScreen__

#include "cocos2d.h"

class MainMenuScreen : public cocos2d::Layer {
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
