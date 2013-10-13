//
//  FBActionScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#include "FBActionScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

#include "ProfileScene.h"
#include "FeedAPIScene.h"
#include "ScoreAPIScene.h"
#include "EventsAPIScene.h"
#include "OpenGraphAPIScene.h"
#include "RequestAPIScene.h"
#include "FriendsScene.h"
#include "ShareDialogScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* FBActionScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    FBActionScene *layer = FBActionScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool FBActionScene::init()
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
void FBActionScene::onEnterTransitionDidFinish()
{
    CCLog("[FBActionScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
    // Check if the Facebook User is available.
    EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
    
    if (currentUser)
    {
        this->showCurrentUserData();
    }
}

void FBActionScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that EziSocial Plugin supports more than 30 functionalities to empower your games with Facebook APIs? Try some of them.");
    
    _firstName = NULL;
    _lastName  = NULL;
    
    CCSpriteBatchNode* fbActionBatchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_FBACTION));
    this->addChild(fbActionBatchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_FBACTION));
    
    CCMenuItemImage* currentMenuItem = NULL;
    _profileMenuItem = NULL;
    
    BaseScene::buildHeader(1, "EziSocial Plugin", "Integrate power of Facebook APIs in your Cocos2d-x Games");

    _profileMenuItem = this->buildAndPlaceTile("btn_profile_nrl", "btn_profile_prd", 0, 0);
    _profileMenuItem->setTarget(this, menu_selector(FBActionScene::loadProfileScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_feed_api_nrl", "btn_feed_api_prd", 2, 0);
    currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadFeedAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_score_api_nrl", "btn_score_api_prd", 3, 0);
    currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadScoreAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_event_api_nrl", "btn_event_api_prd", 0, 1);
    currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadeventAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_open_graph_api_nrl", "btn_open_graph_api_prd", 1, 1);
    currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadOpenGraphAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_request_api_nrl", "btn_request_api_prd", 3, 1);
    currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadRequestAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_friends_api_nrl", "btn_friends_api_prd", 0, 2);
    currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadFriendsAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_share_dialog_api_nrl", "btn_share_dialog_api_prd", 2, 2);
    currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadShareDialogScene));
    
    
}

void FBActionScene::loadProfileScene()
{
    CCScene *pScene = ProfileScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadFeedAPIScene()
{
    CCScene *pScene = FeedAPIScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadScoreAPIScene()
{
    CCScene *pScene = ScoreAPIScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadeventAPIScene()
{
    CCScene *pScene = EventsAPIScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadOpenGraphAPIScene()
{
    CCScene *pScene = OpenGraphAPIScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadRequestAPIScene()
{
    CCScene *pScene = RequestAPIScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadFriendsAPIScene()
{
    CCScene *pScene = FriendsScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadShareDialogScene()
{
    CCScene *pScene = ShareDialogScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::showCurrentUserData()
{
    EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
    
    if (currentUser)
    {
        CCLOG("Username = %s", currentUser->getUserName());
        _firstName = CCLabelTTF::create(currentUser->getFirstName(), "Arial", SCALED_VALUE(20.0f));
        _lastName  = CCLabelTTF::create(currentUser->getLastName(), "Arial", SCALED_VALUE(20.0f));
        
        if (_profileMenuItem != NULL)
        {
            _profileMenuItem->addChild(_firstName);
            _profileMenuItem->addChild(_lastName);
            
            _firstName->setAnchorPoint(ccp(0.0f, 0.0));
            _lastName->setAnchorPoint(ccp(0.0f, 0.0));
            
            CCSize menuItemSize = _profileMenuItem->getContentSize();
            
            _firstName->cocos2d::CCNode::setPosition(menuItemSize.width * 0.5, menuItemSize.height * 0.22f);
            _lastName->cocos2d::CCNode::setPosition(menuItemSize.width * 0.5, menuItemSize.height * 0.08f);
            
            EziSocialObject::sharedObject()->setFacebookDelegate(this);
            EziSocialObject::sharedObject()->getProfilePicForID(this, currentUser->getProfileID(), menuItemSize.width * 0.4,
                                                                menuItemSize.width * 0.4, false);
        }
    }
    else
    {
        CCLOG("Cannot find current user.");
    }
}

void FBActionScene::showCurrentUserPhoto()
{
    const char*  picPath = EziSocialObject::sharedObject()->getCurrentFacebookUser()->getPhotoPath();
    
    if (strcmp("", picPath) != 0)
    {
        CCSize menuSize = _profileMenuItem->getContentSize();
        
        CCLayerColor* whiteBGLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), menuSize.width * 0.44, menuSize.width * 0.44);
        _profileMenuItem->addChild(whiteBGLayer);
        
        float wPy = (menuSize.height - whiteBGLayer->getContentSize().height)/2;
        float wPx = wPy;
        
        whiteBGLayer->cocos2d::CCNode::setPosition(wPx, wPy);
        
        _profileSprite = CCSprite::create(picPath);
        _profileSprite->setAnchorPoint(ccp(0.0, 0.0));
        
        float pPy = (menuSize.height - _profileSprite->getContentSize().height)/2;
        float pPx = pPy;
        
        _profileSprite->cocos2d::CCNode::setPosition(pPx, pPy);
        
        _profileMenuItem->addChild(_profileSprite);
        
    }
    else
    {
        // Display rotating loading image.
        CCLOG("Image not available yet.");
    }
}


//-------------------------------------
    #pragma mark - eziSocial delegates
//-------------------------------------

void FBActionScene::fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_USER_DETAIL_SUCCESS:
            showCurrentUserData();
            break;
            
        default:
            CCMessageBox(responseMessage, "Error - User details");
            break;
    }
}

void FBActionScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbUserID)
{
    if (strcmp(fbUserID, EziSocialObject::sharedObject()->getCurrentFacebookUser()->getProfileID()) == 0)
    {        
        EziSocialObject::sharedObject()->getCurrentFacebookUser()->saveUserPhotoPath(userPhotoPath);
        showCurrentUserPhoto();
    }
    
}