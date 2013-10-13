//
//  RequestAPIScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "RequestAPIScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "EziFacebookFriend.h"
#include "EziFBIncomingRequestManager.h"
#include "RequestListScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* RequestAPIScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    RequestAPIScene *layer = RequestAPIScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool RequestAPIScene::init()
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
void RequestAPIScene::onEnterTransitionDidFinish()
{
    CCLog("[FeedAPIScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void RequestAPIScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that you can use EziSocial to send gifts, invites and challenges to user's Facebook friends? Also you can process these requests on receiver's device");

    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_REQUEST));
    this->addChild(batchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_REQUEST));
    
    buildHeader(-1, "Request APIs", "Send gifts, invites and much more with Facebook");
    
    // Build the tiles.
    CCMenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_send_gifts_nrl",      "btn_send_gifts_prd",         0, 0);
    menuItem->setTarget(this, menu_selector(RequestAPIScene::sendGifts));
    
    menuItem = this->buildAndPlaceTile("btn_challenge_nrl",      "btn_challenge_prd",           2, 0);
    menuItem->setTarget(this, menu_selector(RequestAPIScene::challengeFriends));
    
    menuItem = this->buildAndPlaceTile("btn_invite_friends_nrl",      "btn_invite_friends_prd", 1, 1);
    menuItem->setTarget(this, menu_selector(RequestAPIScene::inviteFriends));
    
    menuItem = this->buildAndPlaceTile("btn_new_requests_nrl",      "btn_new_requests_prd",     0, 2);
    menuItem->setTarget(this, menu_selector(RequestAPIScene::refreshIncomingRequests));
    
    CCSize menuSize = menuItem->getContentSize();
    _pendingRequestLabel = CCLabelTTF::create("0", "Arial", AppDelegate::SCREEN_SCALE_FACTOR * 20);
    menuItem->addChild(_pendingRequestLabel, 5);
    _pendingRequestLabel->cocos2d::CCNode::setPosition(menuSize.width * 0.2, menuSize.height * 0.2);
    
    menuItem = this->buildAndPlaceTile("btn_view_open_nrl",      "btn_view_open_prd",           2, 2);
    menuItem->setTarget(this, menu_selector(RequestAPIScene::viewOpenRequestItems));
}


void RequestAPIScene::sendGifts()
{
    EziSocialObject::sharedObject()->isFacebookSessionActive();
    CCDictionary *giftDictionary = CCDictionary::create();
    
    giftDictionary->setObject(CCString::create("500"), "GOLD_COINS");
    giftDictionary->setObject(CCString::create("1"), "LIFE");
    
    CCArray *preselectFriends = getRandomFriends(2);
    
    if (preselectFriends != NULL)
    {
        EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT,
                                                              "I am sending you 500 gold coins & 1 extra life. Enjoy!",
                                                              preselectFriends,
                                                              giftDictionary, "");
    }
    else
    {
        CCMessageBox("Unable to send gifts as friends list is empty.", "Gift Sending Error");
    }
    
}

void RequestAPIScene::inviteFriends()
{
    EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE,
                                                          "EziSocial Plugin is damm! easy. Try now!", NULL, NULL, "");
}

void RequestAPIScene::challengeFriends()
{
    EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE,
                                                          "I have score 900 points in EziSocialDemo. Can you beat me?",
                                                          NULL,
                                                          NULL, "");

}

void RequestAPIScene::refreshIncomingRequests()
{
    EziSocialObject::sharedObject()->checkIncomingRequest();
}

void RequestAPIScene::viewOpenRequestItems()
{
    CCScene *pScene = RequestListScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFlipAngular::create(1.0f, pScene));
}

void RequestAPIScene::fbSendRequestCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests)
{
    CCMessageBox(responseMessage, "fbSendRequestCallback");
    
}

void RequestAPIScene::fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests)
{
    //CCString* message = CCString::createWithFormat("Total new incoming requests = %d", totalIncomingRequests);

    
    int pendingRequest = EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount();
    CCString* message = CCString::createWithFormat("Total new requests = %d\n Total pending requests = %d", totalIncomingRequests, pendingRequest);
    CCMessageBox(message->getCString(), "fbIncomingRequestCallback");
}


