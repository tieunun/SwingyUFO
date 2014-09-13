#include "GameLayer.h"
#include "MainMenuScreen.h"
#include "PlatformPair.h"
#include "Config.h"

using namespace cocos2d;

#define LABEL_MOVE_INTERVAL 0.55f
#define START_DELAY 1.75f
#define SPAWN_DELAY 2.5f
#define GRAVITY_X 480.0f
#define GRAVITY_Y 1800.0f
#define TAG_PLATFORM 1
#define GAME_OVER_BUTTONS_PADDING 50.0f
#define Z_ORDER_HUD 2

GameLayer::GameLayer()
: mGameState(WaitingForTap)
, mStartDelayTimer(0.0f)
{}

GameLayer::~GameLayer() {
    CC_SAFE_RELEASE_NULL(mGetReadyActionIn);
    CC_SAFE_RELEASE_NULL(mGetReadyActionOut);
    CC_SAFE_RELEASE_NULL(mTapToStartActionIn);
    CC_SAFE_RELEASE_NULL(mTapToStartActionOut);
    CC_SAFE_RELEASE_NULL(mGameOverActionIn);
    CC_SAFE_RELEASE_NULL(mGameOverButtonsActionIn);
    CC_SAFE_RELEASE_NULL(mSpawnPlatformsForever);
}

Scene* GameLayer::createScene() {
    
    // Create the game scene that the director will use
    auto scene = Scene::createWithPhysics();
    auto physWorld = scene->getPhysicsWorld();
    
    // Make physical bodies visible for debugging purposes
    physWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    physWorld->setGravity(Vec2::ZERO);
    
    // Create the main game layer and add it to the scene
    auto layer = GameLayer::create();
    layer->setPhysicsWorld(physWorld);
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
    
    this->setupPhysics();
    this->populateScene();
    this->addEvents();
    this->scheduleUpdate();
    
    return true;
}

void GameLayer::setupPhysics() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto edges = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edges->setGroup(PhysicsGroup::EDGE);
    edges->setCategoryBitmask(PhysicsGroup::EDGE);
    edges->setContactTestBitmask(PhysicsGroup::PLAYER);
    auto node = Node::create();
    node->setPhysicsBody(edges);
    node->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
    this->addChild(node);
}

void GameLayer::populateScene() {
    
    mPlayer = Player::create();
    this->addChild(mPlayer);
    
    Vec2 beginPos, endPos;
    
    // "Get Ready" label
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
    
    
    // "Tap to Start" label
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
    
    
    // "Game Over" label
    mGameOverLabel = Label::createWithSystemFont("Game Over", "Arial", 50.0f);
    beginPos = Vec2(mScreenSize.width * 0.5f, mScreenSize.height + (mGameOverLabel->getContentSize().height * 0.5f));
    endPos = Vec2(beginPos.x, mScreenSize.height * 0.7f);
    mGameOverLabel->setPosition(beginPos);
    mGameOverLabel->setVisible(false);
    
    // Like the "Get Ready" label, the game over label will move down from the top of the screen
    mGameOverActionIn = EaseBounceOut::create(MoveTo::create(LABEL_MOVE_INTERVAL, endPos));
    mGameOverActionIn->retain();
    
    
    // Game over buttons (restart, home, etc)
    auto restartLabel = Label::createWithSystemFont("Restart", "Arial", 50.0f);
    auto restartMenuItem = MenuItemLabel::create(restartLabel, std::bind(&GameLayer::restart, this));
    auto homeLabel = Label::createWithSystemFont("Home", "Arial", 50.0f);
    auto homeMenuItem = MenuItemLabel::create(homeLabel, std::bind(&GameLayer::goHome, this));
    
    mGameOverButtons = Menu::create(restartMenuItem, homeMenuItem, nullptr);
    beginPos = Vec2(mScreenSize.width * 0.5f, restartLabel->getContentSize().height * -0.5f);
    endPos = Vec2(beginPos.x, mScreenSize.height * 0.25f);
    mGameOverButtons->setPosition(beginPos);
    mGameOverButtons->alignItemsHorizontallyWithPadding(GAME_OVER_BUTTONS_PADDING);
    mGameOverButtons->setLocalZOrder(Z_ORDER_HUD);
    
    // The game over buttons will move up from the bottom of the screen
    mGameOverButtonsActionIn = MoveTo::create(LABEL_MOVE_INTERVAL, endPos);
    mGameOverButtonsActionIn->retain();
    
    this->addChild(mGetReadyLabel);
    this->addChild(mTapToStartLabel);
    this->addChild(mGameOverLabel);
    this->addChild(mGameOverButtons);
    
    // Move the labels into the view of the user
    mGetReadyLabel->runAction(mGetReadyActionIn);
    mTapToStartLabel->runAction(mTapToStartActionIn);
}

void GameLayer::addEvents() {
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    
    // Register to listen for single touch events
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = std::bind(&GameLayer::onTouchBegan,
                                                 this,
                                                 std::placeholders::_1,
                                                 std::placeholders::_2);
    eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    // Register to listen for contact between physics bodies
    auto contactEventListener = EventListenerPhysicsContact::create();
    contactEventListener->onContactBegin = std::bind(&GameLayer::onContactBegin,
                                                     this,
                                                     std::placeholders::_1);
    eventDispatcher->addEventListenerWithSceneGraphPriority(contactEventListener, this);
}

void GameLayer::update(float dt) {
    
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
                
                if (rand() % 2) {
                    mPlayer->setDirection(Player::Direction::LEFT);
                    mPhysWorld->setGravity(Vec2(-GRAVITY_X, 0.0f));
                } else {
                    mPlayer->setDirection(Player::Direction::RIGHT);
                    mPhysWorld->setGravity(Vec2(GRAVITY_X, 0.0f));
                }
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

void GameLayer::restart() {
    auto newGameScene = GameLayer::createScene();
    Director::getInstance()->replaceScene(newGameScene);
}

void GameLayer::goHome() {
    auto homeScene = MainMenuScreen::createScene();
    Director::getInstance()->replaceScene(homeScene);
}

bool GameLayer::onTouchBegan(Touch *touch, Event *event) {
    
    switch (mGameState) {
        case WaitingForTap:
            if (mGetReadyActionIn->isDone() && mTapToStartActionIn->isDone()) {
                // Move the labels out of the way
                mGetReadyLabel->runAction(mGetReadyActionOut);
                mTapToStartLabel->runAction(mTapToStartActionOut);
                mGameState = Starting;
            }
            break;
            
        case Starting:
            break;
            
        case Running:
            switch (mPlayer->getDirection()) {
                case Player::Direction::LEFT:
                    mPhysWorld->setGravity(Vec2(GRAVITY_X, 0.0f));
                    break;
                
                case Player::Direction::RIGHT:
                    mPhysWorld->setGravity(Vec2(-GRAVITY_X, 0.0f));
                    break;
                    
                default:
                    break;
            }
            mPlayer->switchDirections();
            break;
            
        case GameOver:
            break;
    }
    
    return true;
}

bool GameLayer::onContactBegin(cocos2d::PhysicsContact &contact) {
    CCLOG("Contact detected");
    if (mGameState != GameOver) {
        mGameState = GameOver;
        mPhysWorld->setGravity(Vec2(0.0f, -GRAVITY_Y));   // Gravity becomes vertical when the player dies
        
        // Stop spawning platforms
        this->stopAllActions();
        
        // Stop all currently spawned platforms
        for (auto &child : this->getChildren()) {
            if (child->getTag() == TAG_PLATFORM) {
                child->stopAllActions();
            }
        }
        
        mGameOverLabel->setVisible(true);
        mGameOverLabel->runAction(mGameOverActionIn);
        mGameOverButtons->setVisible(true);
        mGameOverButtons->runAction(mGameOverButtonsActionIn);
        
        return true;
    }
    return false;   // When the game is over, the player will not collide with the edges anymore
}

void GameLayer::spawnPlatformPair() {
    auto platforms = PlatformPair::create();
    platforms->setTag(TAG_PLATFORM);
    this->addChild(platforms);
    platforms->startMoving();
}
