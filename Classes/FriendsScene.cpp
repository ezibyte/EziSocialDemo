//
//  FriendsScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "FriendsScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "FriendListScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* FriendsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    FriendsScene *layer = FriendsScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool FriendsScene::init()
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
void FriendsScene::onEnterTransitionDidFinish()
{
    CCLog("[FriendsScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void FriendsScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that with EziSocial you fetch the friends details including id name, score and photos?");

    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_FRIENDS));
    this->addChild(batchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_FRIENDS));
    
    buildHeader(-1, "Friends", "Get the list of facebook friends");
    
    // Build the tiles.
    CCMenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_all_friends_nrl",       "btn_all_friends_prd",      2, 0);
    menuItem->setTarget(this, menu_selector(FriendsScene::showAllFriends));
    
    menuItem = this->buildAndPlaceTile("btn_who_nrl",               "btn_who_prd",              1, 1);
    menuItem->setTarget(this, menu_selector(FriendsScene::showOnlyPlayingFriends));
    
    menuItem = this->buildAndPlaceTile("btn_invite_missing_nrl",    "btn_invite_missing_prd",   0, 2);
    menuItem->setTarget(this, menu_selector(FriendsScene::showNonPlayingFriends));
    
    menuItem = this->buildAndPlaceTile("btn_special_friends_nrl",   "btn_special_friends_prd",  2, 2);
    menuItem->setTarget(this, menu_selector(FriendsScene::showOnlyFriendsPlayingWithMyKindOfDevice));

}

void FriendsScene::showAllFriends()
{
    CCScene* pScene = FriendListScene::scene(ALL_FRIENDS);
    ReplaceToRight(pScene);
}

void FriendsScene::showNonPlayingFriends()
{
    CCScene* pScene = FriendListScene::scene(NON_PLAYING_ONLY);
    ReplaceToRight(pScene);
}

void FriendsScene::showOnlyPlayingFriends()
{
    CCScene* pScene = FriendListScene::scene(INSTALLED_ONLY);
    ReplaceToRight(pScene);
}

void FriendsScene::showOnlyFriendsPlayingWithMyKindOfDevice()
{
    CCScene* pScene = FriendListScene::scene(MY_DEVICE_ONLY);
    ReplaceToRight(pScene);
}
