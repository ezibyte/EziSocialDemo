//
//  EventsAPIScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "EventsAPIScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* EventsAPIScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    EventsAPIScene *layer = EventsAPIScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool EventsAPIScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    this->buildScene();
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void EventsAPIScene::onEnterTransitionDidFinish()
{
    CCLog("[EventsAPIScene]: Enter Transition Finished.");
}

void EventsAPIScene::buildScene()
{
    _loggingActiveness = true;
    _storeOpen         = false;
    
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that now you can actually log the game events with Events APIs? Why not trying logging game purchases today?");

    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_EVENTS));
    this->addChild(batchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_EVENTS));
    
    buildHeader(-1, "Log Events", "Track your game events with Facebook insight");
    
    // Build the tiles.
    
    _carButton       = NULL;
    _shieldButton    = NULL;
    _lifeButton      = NULL;
    _storeButton     = NULL;
    _logActiveButton = NULL;
    
    _logActiveButton = this->buildAndPlaceTile("btn_active_enable_nrl",         "btn_active_enable_prd",        0, 0);
    _logActiveButton->setTarget(this, menu_selector(EventsAPIScene::logActiveness));
    
    _storeButton = this->buildAndPlaceTile("btn_store_open_nrl",            "btn_store_open_prd",               2, 0);
    _storeButton->setTarget(this, menu_selector(EventsAPIScene::openStore));
    
    _carButton = this->buildAndPlaceTile("btn_car_nrl",                   "btn_car_prd",                        1, 1);
    _carButton->setTarget(this, menu_selector(EventsAPIScene::buyCar));
    _carButton->setVisible(false);
    
    _shieldButton = this->buildAndPlaceTile("btn_shield_nrl",                "btn_shield_prd",                  0, 2);
    _shieldButton->setTarget(this, menu_selector(EventsAPIScene::buyShield));
    _shieldButton->setVisible(false);
    
    _lifeButton = this->buildAndPlaceTile("btn_life_nrl",                  "btn_life_prd",                  2, 2);
    _lifeButton->setTarget(this, menu_selector(EventsAPIScene::buyLife));
    _lifeButton->setVisible(false);
}

void EventsAPIScene::openStore()
{
    _storeOpen = !_storeOpen;
    
    if (_storeOpen)
    {
        _storeButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_store_close_nrl"));
        _storeButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_store_close_prd"));
    }
    else
    {
        _storeButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_store_open_nrl"));
        _storeButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_store_open_prd"));
    }
    
    _carButton->setVisible(_storeOpen);
    _shieldButton->setVisible(_storeOpen);
    _lifeButton->setVisible(_storeOpen);

}

void EventsAPIScene::buyCar()
{
    EziSocialObject::sharedObject()->logPurchase(0.99f, "USD");
}

void EventsAPIScene::buyShield()
{
    EziSocialObject::sharedObject()->logPurchase(1.99f, "USD");
}

void EventsAPIScene::buyLife()
{
    EziSocialObject::sharedObject()->logPurchase(2.99f, "USD");
}

void EventsAPIScene::logActiveness()
{
    _loggingActiveness = !_loggingActiveness;
    if (_loggingActiveness)
    {
        _logActiveButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_active_enable_nrl"));
        _logActiveButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_active_enable_prd"));
    }
    else
    {
        _logActiveButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_active_disable_nrl"));
        _logActiveButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_active_disable_prd"));
    }
    
    EziSocialObject::sharedObject()->setLogActiveness(_loggingActiveness);
}

void EventsAPIScene::showPopup()
{
}
