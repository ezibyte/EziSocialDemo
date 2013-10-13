//
//  ShareDialogScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "ShareDialogScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* ShareDialogScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ShareDialogScene *layer = ShareDialogScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool ShareDialogScene::init()
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
void ShareDialogScene::onEnterTransitionDidFinish()
{
    CCLog("[ShareDialogScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void ShareDialogScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that Share Dialogs are views from Facebook App? Also, EziSocial automatically fallback to web dialog if Facebook app is not installed.");

    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_SHARE_DIALOG));
    CCSpriteBatchNode* batchNode2 = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_MORE));
    this->addChild(batchNode);
    this->addChild(batchNode2);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_SHARE_DIALOG));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_MORE));
    
    
    buildHeader(-1, "Share Dialogs & More", "Use native share dialogs");
    
    CCMenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_share_dg_status_nrl",  "btn_share_dg_status_prd",  0, 0);
    menuItem->setTarget(this, menu_selector(ShareDialogScene::publishMessage));
    
    menuItem = this->buildAndPlaceTile("btn_share_dg_link_nrl",    "btn_share_dg_link_prd",    1, 1);
    menuItem->setTarget(this, menu_selector(ShareDialogScene::publishLink));
    
    menuItem = this->buildAndPlaceTile("btn_check_fb_app_nrl",     "btn_check_fb_app_prd",     0, 1);
    menuItem->setTarget(this, menu_selector(ShareDialogScene::checkIfFacebookAppInstalled));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    menuItem = this->buildAndPlaceTile("btn_share_dg_og_nrl",      "btn_share_dg_og_prd",      2, 0);
    menuItem->setTarget(this, menu_selector(ShareDialogScene::publishOpenGraphStory));
#endif
    
    menuItem = this->buildAndPlaceTile("btn_open_fb_page_nrl",      "btn_open_fb_page_prd",      0, 2);
    menuItem->setTarget(this, menu_selector(ShareDialogScene::openFacebookPage));
    
    menuItem = this->buildAndPlaceTile("btn_page_like_nrl",      "btn_page_like_prd",      2, 2);
    menuItem->setTarget(this, menu_selector(ShareDialogScene::checkIfUserLikeMyPage));
    
}

void ShareDialogScene::publishMessage()
{
    EziSocialObject::sharedObject()->useShareDialogForStatusForUpdate();
}

void ShareDialogScene::publishLink()
{
    EziSocialObject::sharedObject()->useShareDialogForLink("http://ezibyte.com");
}

void ShareDialogScene::publishOpenGraphStory()
{
    EziSocialObject::sharedObject()->useShareDialogForOpenGraph("item", "find", "Red Guitar",
                                                                "http://db.tt/enkOAKk8", "Red Guitar is lucky for me.");
}

void ShareDialogScene::checkIfFacebookAppInstalled()
{
    if (EziSocialObject::sharedObject()->canUseShareDialog())
    {
        CCMessageBox("Yes! Facebook app is installed.", "Can use share dialog?");
    }
    else
    {
        CCMessageBox("No! Facebook app is not installed.", "Can use share dialog?");
    }
}

void ShareDialogScene::checkIfUserLikeMyPage()
{
    EziSocialObject::sharedObject()->hasUserLikedMyFBPage(FACEBOOK_PAGE_ID);
}

void ShareDialogScene::openFacebookPage()
{
    EziSocialObject::sharedObject()->openFacebookPage(FACEBOOK_PAGE_ID, true);
}

void ShareDialogScene::fbShareDialogCallback(int responseCode, const char* responseMessage)
{
    CCMessageBox(responseMessage, "Share Dialog Response");
}

void ShareDialogScene::fbPageLikeCallback(int responseCode, const char *responseMessage)
{
    CCMessageBox(responseMessage, "PageLike Callback");
}
