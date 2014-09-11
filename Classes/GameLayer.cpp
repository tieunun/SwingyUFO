#include "GameLayer.h"
#include "PlatformPair.h"

using namespace cocos2d;

#define LABEL_MOVE_INTERVAL 0.55f
#define START_DELAY 1.75f
#define SPAWN_DELAY 2.5f

GameLayer::GameLayer() :mGameState(WaitingForTap), mStartDelayTimer(0.0f) {}

GameLayer::~GameLayer() {
    CC_SAFE_RELEASE_NULL(mGetReadyActionIn);
    CC_SAFE_RELEASE_NULL(mGetReadyActionOut);
    CC_SAFE_RELEASE_NULL(mTapToStartActionIn);
    CC_SAFE_RELEASE_NULL(mTapToStartActionOut);
    CC_SAFE_RELEASE_NULL(mSpawnPlatformsForever);
}

Scene* GameLayer::createScene() {
    
    // Create the game scene that the director will use
    auto scene = Scene::create();
    
    // Create the main game layer and add it to the scene
    auto layer = GameLayer::create();
    scene->addChild(layer);

    return scene;
}

GameLayer* GameLayer::create() {
    auto layer = new GameLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    delete layer;
    layer = nullptr;
    return nullptr;
}

bool GameLayer::init() {
    
    if (!Layer::init()) { return false; }
    
    mScreenSize = Director::getInstance()->getWinSize();
    
    this->populateScene();
    this->addEvents();
    this->scheduleUpdate();
    
    return true;
}

void GameLayer::populateScene() {
    
    mPlayer = Player::create();
    this->addChild(mPlayer);
    
    Vec2 beginPos, endPos;
    
    // Prepare the "Get Ready" label and its actions
    mGetReadyLabel = Label::createWithSystemFont("Get Ready", "Arial", 50.0f);
    beginPos = Vec2(mScreenSize.width * 0.5f, mScreenSize.height + (mGetReadyLabel->getContentSize().height * 0.5f));
    endPos = Vec2(beginPos.x, mScreenSize.height * 0.7f);
    mGetReadyLabel->setPosition(beginPos);
    
    // The "Get Ready" label will move downwards from beginPos to endPos
    mGetReadyActionIn = MoveTo::create(LABEL_MOVE_INTERVAL, endPos);
    mGetReadyActionIn->retain();
    
    endPos = Vec2(endPos.x, beginPos.y);

    auto resetGetReadyPos = [=] {
        this->mGetReadyLabel->setPosition(beginPos);
    };
    
    // After the user taps the screen for the first time, we will run a sequence that will
    // move the label upwards and then reset the label's position
    mGetReadyActionOut = Sequence::create(MoveTo::create(LABEL_MOVE_INTERVAL, endPos),
                                          CallFunc::create(resetGetReadyPos),
                                          nullptr);
    mGetReadyActionOut->retain();
    
    
    // Prepare the "Tap to Start" label and its actions
    mTapToStartLabel = Label::createWithSystemFont("Tap to start", "Arial", 50.0f);
    beginPos = Vec2(mTapToStartLabel->getContentSize().width * -0.5f, mScreenSize.height * 0.5f);
    endPos = Vec2(mScreenSize.width * 0.5f, beginPos.y);
    mTapToStartLabel->setPosition(beginPos);
    
    // The "Tap to Start" label will move left to right from beginPos to endPos
    mTapToStartActionIn = MoveTo::create(LABEL_MOVE_INTERVAL, endPos);
    mTapToStartActionIn->retain();
    
    endPos = Vec2(mScreenSize.width - beginPos.x, endPos.y);
    
    auto resetTapPos = [=] {
        this->mTapToStartLabel->setPosition(beginPos);
    };
    
    // After the user taps the screen for the first time, we will run a sequence that will
    // move the label more to the right and then reset the label's position
    mTapToStartActionOut = Sequence::create(MoveTo::create(LABEL_MOVE_INTERVAL, endPos),
                                            CallFunc::create(resetTapPos),
                                            nullptr);
    mTapToStartActionOut->retain();
    
    
    // We will spawn a platform pair every few seconds and repeat this forever once the game starts
    auto spawn = CallFunc::create(std::bind(&GameLayer::spawnPlatformPair, this));
    auto delay = DelayTime::create(SPAWN_DELAY);
    auto spawnThenWait = Sequence::create(spawn, delay, nullptr);
    mSpawnPlatformsForever = RepeatForever::create(spawnThenWait);
    mSpawnPlatformsForever->retain();
    
    this->addChild(mGetReadyLabel);
    this->addChild(mTapToStartLabel);
    
    // Move the labels into the view of the user
    mGetReadyLabel->runAction(mGetReadyActionIn);
    mTapToStartLabel->runAction(mTapToStartActionIn);
}

void GameLayer::addEvents() {
    // Register to listen for single touch events
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = std::bind(&GameLayer::onTouchBegan,
                                                 this,
                                                 std::placeholders::_1,
                                                 std::placeholders::_2);
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
}

void GameLayer::update(float dt) {
    
    mPlayer->update(dt);
    
    switch (mGameState) {
            
        case WaitingForTap:
            break;
            
        case Starting:
            // Adds a short delay before the obstacles come into view
            if (mStartDelayTimer < START_DELAY) {
                mStartDelayTimer += dt;
            } else {
                mStartDelayTimer = 0.0f;
                mGameState = Running;
                mPlayer->setDirection((rand() % 2 == 0) ? Player::Direction::LEFT : Player::Direction::RIGHT);
                // Start spawning platforms
                this->runAction(mSpawnPlatformsForever);
            }
            break;
            
        case Running:
            break;
            
        case GameOver:
            break;
    }
}

bool GameLayer::onTouchBegan(Touch *touch, Event *event) {
    
    switch (mGameState) {
        case WaitingForTap:
            if (mGetReadyActionIn->isDone() && mTapToStartActionIn->isDone()) {
                CCLOG("Both actions done");
                // Move the labels out of the way
                mGetReadyLabel->runAction(mGetReadyActionOut);
                mTapToStartLabel->runAction(mTapToStartActionOut);
                mGameState = Starting;
            } else {
                CCLOG("Actions are not done");
            }
            break;
            
        case Starting:
            break;
            
        case Running:
            mPlayer->switchDirections();
            break;
            
        case GameOver:
            break;
    }
    
    return true;
}

void GameLayer::spawnPlatformPair() {
    auto platforms = PlatformPair::create();
    this->addChild(platforms);
    platforms->startMoving();
}
