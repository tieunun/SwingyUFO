#include "GameLayer.h"

using namespace cocos2d;

GameLayer::GameLayer() {}

GameLayer::~GameLayer() {
    CC_SAFE_RELEASE_NULL(mGetReadyActionIn);
    CC_SAFE_RELEASE_NULL(mGetReadyActionOut);
    CC_SAFE_RELEASE_NULL(mTapToStartActionIn);
    CC_SAFE_RELEASE_NULL(mTapToStartActionOut);
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
    beginPos = Vec2(mScreenSize.width * 0.5f, mScreenSize.height + mGetReadyLabel->boundingBox().getMaxY());
    endPos = Vec2(beginPos.x, mScreenSize.height * 0.7f);
    mGetReadyLabel->setPosition(beginPos);
    
    // The "Get Ready" label will move downwards from beginPos to endPos
    mGetReadyActionIn = EaseElasticOut::create(MoveTo::create(1.3f, endPos), 0.5f);
    mGetReadyActionIn->retain();
    
    endPos = Vec2(endPos.x, beginPos.y);

    auto resetGetReadyPos = [=] {
        this->mGetReadyLabel->setPosition(beginPos);
    };
    
    // After the user taps the screen, we will run a sequence that will move the label up and
    // then reset the label's position
    mGetReadyActionOut = Sequence::create(EaseElasticIn::create(MoveTo::create(1.3f, endPos), 0.5f),
                                          CallFunc::create(resetGetReadyPos),
                                          nullptr);
    mGetReadyActionOut->retain();
    
    
    // Prepare the "Tap to Start" label and its actions
    mTapToStartLabel = Label::createWithSystemFont("Tap to start", "Arial", 50.0f);
    beginPos = Vec2(-1.0f * mTapToStartLabel->getBoundingBox().getMaxX(), mScreenSize.height * 0.5f);
    endPos = Vec2(mScreenSize.width * 0.5f, beginPos.y);
    mTapToStartLabel->setPosition(beginPos);
    
    // The "Tap to Start" label will move left to right from beginPos to endPos
    mTapToStartActionIn = EaseElasticOut::create(MoveTo::create(1.3f, endPos), 0.5f);
    mTapToStartActionIn->retain();
    
    endPos = Vec2(mScreenSize.width - beginPos.x, endPos.y);
    
    auto resetTapPos = [=] {
        this->mTapToStartLabel->setPosition(beginPos);
    };
    
    // After the user taps the screen, we will run a sequence that will move the label more to
    // the right and then reset the label's position
    mGetReadyActionOut = Sequence::create(EaseElasticIn::create(MoveTo::create(1.3f, endPos), 0.5f),
                                          CallFunc::create(resetTapPos),
                                          nullptr);
    mGetReadyActionOut->retain();
    
    this->addChild(mGetReadyLabel);
    this->addChild(mTapToStartLabel);
    
    mGetReadyLabel->runAction(mGetReadyActionIn);
    mTapToStartLabel->runAction(mTapToStartActionIn);
}

void GameLayer::addEvents() {
    // Register to listen for touch events
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
}

bool GameLayer::onTouchBegan(Touch *touch, Event *event) {
    mPlayer->switchDirections();
    return true;
}
