#include "AppDelegate.h"
#include "GameLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    // Initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Game");
        director->setOpenGLView(glview);
    }

    // Turn on display FPS
    director->setDisplayStats(true);

    // Set the FPS
    director->setAnimationInterval(1.0 / 60);

    // Create the game scene
    auto scene = GameLayer::createScene();

    // Run the scene with the Director
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. It is also invoked when there is an incoming phone call.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// This function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
