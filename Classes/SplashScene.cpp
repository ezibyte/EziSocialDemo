//
//  SplashScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#include "SplashScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* SplashScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SplashScene *layer = SplashScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_BG));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_SPLASH));
    
    CCLayerColor* bgLayer = CCLayerColor::create(ccc4(255, 255, 255, 255), AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT);
    this->addChild(bgLayer);
    
    CCSprite* ezibyteLogo = CCSprite::createWithSpriteFrameName("ezibyte_logo");
    this->addChild(ezibyteLogo);
    PlaceNode(ezibyteLogo, 0.5, 0.5);

    
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void SplashScene::onEnterTransitionDidFinish()
{
    CCLog("Enter Transition Finished.");
    
    CCDelayTime* waitTime  = CCDelayTime::create(kSPALSH_WAIT_TIME);
    CCCallFunc*  callFunct = CCCallFunc::create(this, callfunc_selector(SplashScene::loadHomeScene));
    CCSequence* actionSeq  = CCSequence::create(waitTime, callFunct, NULL);
    
    this->runAction(actionSeq);
    
}

void SplashScene::loadHomeScene()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(PLIST_FILE(kRESOURCE_SPLASH));
    CCScene *pScene = HomeScene::scene();
    ReplaceToRight(pScene);
}
