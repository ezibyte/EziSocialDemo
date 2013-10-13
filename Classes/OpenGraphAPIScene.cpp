//
//  OpenGraphAPIScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "OpenGraphAPIScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* OpenGraphAPIScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    OpenGraphAPIScene *layer = OpenGraphAPIScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool OpenGraphAPIScene::init()
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
void OpenGraphAPIScene::onEnterTransitionDidFinish()
{
    CCLog("[OpenGraphAPIScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void OpenGraphAPIScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that Open Graph is the coolest feature from Facebook? You can share live game action on the Facebook with just one function call at right time.");

    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_OG));
    this->addChild(batchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_OG));
    
    buildHeader(-1, "Open Graph Stories", "Increase your user base by publishing open graph stories");
    
    // Build the tiles.
    CCMenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_og_earn_achievement_nrl",   "btn_og_earn_achievement_prd",  1, 0);
    menuItem->setTarget(this, menu_selector(OpenGraphAPIScene::earnAchievement));
    
    menuItem = this->buildAndPlaceTile("btn_og_defeat_friend_nrl",      "btn_og_defeat_friend_prd",     1, 1);
    menuItem->setTarget(this, menu_selector(OpenGraphAPIScene::defeatAFriend));
    
    menuItem = this->buildAndPlaceTile("btn_og_find_item_nrl",          "btn_og_find_item_prd",         0, 2);
    menuItem->setTarget(this, menu_selector(OpenGraphAPIScene::findAGuitarItem));
    
    menuItem = this->buildAndPlaceTile("btn_og_complete_lvl_nrl",       "btn_og_complete_lvl_prd",      2, 2);
    menuItem->setTarget(this, menu_selector(OpenGraphAPIScene::completeALevel));
    
    
}

void OpenGraphAPIScene::completeALevel()
{
    CCString* levelText = CCString::createWithFormat("Level %d", rand()%10, NULL);
    EziSocialObject::sharedObject()->publishOpenGraphAction("level",
                                                            "complete",
                                                            levelText->getCString(),
                                                            "http://db.tt/bDv5KOsX",
                                                            "Hurray! I have completed a level", "This is a test body! dude!");
    EziSocialObject::sharedObject()->logEvent("Complete a level");
}

void OpenGraphAPIScene::defeatAFriend()
{
    CCArray* friendArray = getRandomFriends(1);
    
    if (friendArray != NULL)
    {
        CCString* friendString = (CCString*)friendArray->objectAtIndex(0);
        CCLOG("Randomly selected friend ID = %s", friendString->getCString());
        EziSocialObject::sharedObject()->publishOpenGraphActionWithFriend(friendString->getCString(), "defeat");
        EziSocialObject::sharedObject()->logEvent("Defeat a friend");
    }
    else
    {
        CCMessageBox("Unable to get random Friend.", "Open Graph Error");
    }
    
}

void OpenGraphAPIScene::earnAchievement()
{
    EziSocialObject::sharedObject()->publishOpenGraphAction("achievement",
                                                            "earn",
                                                            "Golden Trophy",
                                                            "http://db.tt/TUW2CQdb", "I have won a golden trophy", "I have won a golden trophy");
    EziSocialObject::sharedObject()->logEvent("Earn an achievement");
}

void OpenGraphAPIScene::findAGuitarItem()
{
    EziSocialObject::sharedObject()->publishOpenGraphAction("item", "find",
                                                            "Red Guitar", "http://db.tt/enkOAKk8",
                                                            "I have found a red guitar!", "I have found a red guitar!");
    EziSocialObject::sharedObject()->logEvent("Find a Guitar");
}

#pragma mark - Facebook Callback Methods...

void OpenGraphAPIScene::fbOpenGraphCallback(int responseCode, const char *responseMessage)
{
    CCMessageBox(responseMessage, "Open Graph Response");
    CCLOG("Message Code = %d", responseCode);
}