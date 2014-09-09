//
//  Player.cpp
//  SwingyChute
//

#include "Player.h"

Player::Player() {
    facingDirection = NEITHER;
}

Player* Player::create() {
    auto player = new Player();
    if (player && player->init()) {
        player->autorelease();
        return player;
    }
    delete player;
    player = nullptr;
    return nullptr;
}

void Player::update(float dt) {
    
}

void Player::switchDirections() {
    
    switch (facingDirection) {
            
        case LEFT:
            CCLOG("Switching from LEFT to RIGHT");
            facingDirection = RIGHT;
            break;
            
        case RIGHT:
            CCLOG("Switching from RIGHT to LEFT");
            facingDirection = LEFT;
            break;
            
        default:
            CCLOG("Not facing any direction, not switching");
            break;
    }
}