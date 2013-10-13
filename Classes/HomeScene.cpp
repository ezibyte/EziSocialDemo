//
//  HomeScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#include "HomeScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HomeScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HomeScene *layer = HomeScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool HomeScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
        
    buildScene();
    
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void HomeScene::onEnterTransitionDidFinish()
{
    CCLog("[HomeScene]: Enter Transition Finished.");
    // Set the delegates to receive responses.
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    EziSocialObject::sharedObject()->setEmailDelegate(this);
    EziSocialObject::sharedObject()->setTwitterDelegate(this);
    EziSocialObject::sharedObject()->setAutoCheckIncomingRequestsOnAppLaunch(true);
}

void HomeScene::buildScene()
{
    // Call suprt build scene...
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that you can empower your game with Facebook APIs using EziSocial plugin in just 10 mintues?");

    CCSpriteBatchNode* homeBatchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_HOME));
    this->addChild(homeBatchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_HOME));
    
    BaseScene::buildHeader(0, "EziSocial Plugin", "Integrate power of Facebook APIs in your Cocos2d-x Games");
    
    // Load Tiles Button
    CCMenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_fb_login_nrl",   "btn_fb_login_prd", 0, 0);
    menuItem->setTarget(this, menu_selector(HomeScene::facebookButtonPressed));
    
    menuItem = this->buildAndPlaceTile("btn_tweet_nrl",      "btn_tweet_prd",    1, 1);
    menuItem->setTarget(this, menu_selector(HomeScene::tweetButtonPressed));
    
    menuItem = this->buildAndPlaceTile("btn_email_nrl",      "btn_email_prd",    0, 2);
    menuItem->setTarget(this, menu_selector(HomeScene::emailButtonPressed));

}

void HomeScene::facebookButtonPressed()
{
    if (EziSocialObject::sharedObject()->isFacebookSessionActive())
    {
        this->loadFBActionScene();
    }
    else
    {
        EziSocialObject::sharedObject()->performLoginUsingFacebook(false);
    }
}

void HomeScene::tweetButtonPressed()
{
    EziSocialObject::sharedObject()->tweet("Welcome to the world of EziSocial Plugin. Try now at http://ezibyte.com", NULL);
}

void HomeScene::emailButtonPressed()
{
    EziSocialObject::sharedObject()->sendEmail("EziSocial Plugin Feedback",
                                               "<p><B>Please write your feedback here</B></p><br>",
                                               "support@ezibyte.com");
}

//-------------------------------------
    #pragma mark - eziSocial delegates
//-------------------------------------

void HomeScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
    BaseScene::fbSessionCallback(responseCode, responseMessage);
    if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL)
    {
        this->loadFBActionScene();
    }
}

