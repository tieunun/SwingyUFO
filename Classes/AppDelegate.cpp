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
    
    Size screenSize = glview->getFrameSize();
    Size designSize = Size(480, 800);
    Size adjustedSize = director->getWinSize();

    CCLOG("actual screen width: %f, actual screen height: %f", screenSize.width, screenSize.height);
    CCLOG("design screen width: %f, design screen height: %f", designSize.width, designSize.height);
    CCLOG("adjusted screen width: %f, adjusted screen height: %f", adjustedSize.width, adjustedSize.height);
    
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);
    
    // Determine what scale factor will be used for positions, sizes, etc.
    float scale = 1.0f;
    
    if (screenSize.height > 1024) {
        scale = 2.0f;
        
    } else if (screenSize.height > 480) {
        scale = 1.0f;
        
    } else {
        scale = 0.5f;
    }
    
    // Set the scale factor and the search paths
    director->setContentScaleFactor(scale);

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
