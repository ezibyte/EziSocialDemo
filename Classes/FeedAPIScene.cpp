//
//  FeedAPIScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "FeedAPIScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* FeedAPIScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    FeedAPIScene *layer = FeedAPIScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool FeedAPIScene::init()
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
void FeedAPIScene::onEnterTransitionDidFinish()
{
    CCLog("[FeedAPIScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void FeedAPIScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that can actually publish on user wall without notifying him? Just try not so often to avoid getting your app blocked by user.");

    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_FEED_API));
    this->addChild(batchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_FEED_API));
    buildHeader(-1, "Feed APIs", "Fetch the details of user from Facebook");
    
    CCMenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_post_message_nrl",         "btn_post_message_prd",      0, 0);
    menuItem->setTarget(this, menu_selector(FeedAPIScene::postMessageOnUserWall));
    
    menuItem = this->buildAndPlaceTile("btn_auto_post_message_nrl",    "btn_auto_post_message_prd", 1, 1);
    menuItem->setTarget(this, menu_selector(FeedAPIScene::autoPostMessageOnUserWall));
    
    menuItem = this->buildAndPlaceTile("btn_post_photo_nrl",           "btn_post_photo_prd",        0, 2);
    menuItem->setTarget(this, menu_selector(FeedAPIScene::postPhotoInUserAlbum));

    
}

void FeedAPIScene::postMessageOnUserWall()
{
    EziSocialObject::sharedObject()->postMessageOnWall("EziSocial Plugin by EziByte",
                                                       "Have you tried it?",
                                                       "Try EziSocial Plugin developed by EziByte team to harness the power of Facebook APIs in your games",
                                                       "Try EziSocial Plugin developed by EziByte team to harness the power of Facebook APIs in your games",
                                                       "https://1.gravatar.com/avatar/5c85a0424f7948506baed85755bc0f8d",
                                                       "http://ezibyte.com");
}

void FeedAPIScene::autoPostMessageOnUserWall()
{
    EziSocialObject::sharedObject()->autoPostMessageOnWall("EziSocial Plugin by EziByte",
                                                           "Have you tried it?",
                                                           "Using Facebook in games got easier :)",
                                                           "Try EziSocial Plugin developed by EziByte team to harness the power of Facebook APIs in your games",
                                                           "https://1.gravatar.com/avatar/5c85a0424f7948506baed85755bc0f8d",
                                                           "http://ezibyte.com");
}

void FeedAPIScene::postPhotoInUserAlbum()
{
    CCRenderTexture *render = CCRenderTexture::create(AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT);
    render->setPosition(ccp(AppDelegate::SCREEN_WIDTH / 2, AppDelegate::SCREEN_HEIGHT / 2));
    render->begin();
    CCDirector::sharedDirector()->getRunningScene()->visit();
    render->end();
    render->saveToFile("ScreenShot.jpg", kCCImageFormatJPEG);
    
    std::string filePathName = CCFileUtils::sharedFileUtils()->getWritablePath().append("ScreenShot.jpg");
    
    
    EziSocialObject::sharedObject()->postPhoto(filePathName.c_str(), "This is photo test message");
    //EziSocialObject::sharedObject()->postPhoto("ball.png", "This is photo test message");
}

void FeedAPIScene::fbMessageCallback(int responseCode, const char* responseMessage)
{
    CCMessageBox(responseMessage, "Message Response");
}

void FeedAPIScene::fbPostPhotoCallback(int responseCode, const char* responseMessage)
{
    CCMessageBox(responseMessage, "Photo post response");
}
