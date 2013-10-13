//
//  ProfileScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "ProfileScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* ProfileScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ProfileScene *layer = ProfileScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool ProfileScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void ProfileScene::onEnterTransitionDidFinish()
{
    CCLog("[ProfileScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    if (EziSocialObject::sharedObject()->getCurrentFacebookUser() != NULL)
    {
        this->buildScene();
    }
    else
    {
        EziSocialObject::sharedObject()->setFacebookDelegate(this);
        EziSocialObject::sharedObject()->fetchFBUserDetails(true);
    }
}

void ProfileScene::buildScene()
{

    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that you can give personalize experience to your users? Just Fetch the details from Facebook using EziSocial. :)");
    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_PROFILE));
    this->addChild(batchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_PROFILE));
    buildHeader(-1, "Profile Page", "Fetch the details of user from Facebook");
    
    _userSprite = CCSprite::createWithSpriteFrameName("fb_user_icon");
    _userSprite->setAnchorPoint(ccp(1.0f, 1.0f));
    _userSprite->setScale(2.0f);
    
    this->addChild(_userSprite);
    PlaceNode(_userSprite, 0.98, 0.756);
    
    EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
    
    CCLabelTTF* userName = CCLabelTTF::create(currentUser->getFullName(), "Arial", SCALED_VALUE(80.0f));
    userName->setAnchorPoint(ccp(1.0f, 1.0f));
    PlaceNode(userName, 0.98, 0.5);
    userName->setPositionY(_userSprite->getPositionY() - (_userSprite->getContentSize().height * _userSprite->getScaleY()));
    
    CCLabelTTF* userEmailID = CCLabelTTF::create(currentUser->getEmailID(), "Arial", SCALED_VALUE(40.0f));
    userEmailID->setColor(ccc3(255, 255, 0));
    userEmailID->setAnchorPoint(ccp(1.0f, 1.0f));
    PlaceNode(userEmailID, 0.98, 0.5);
    userEmailID->setPositionY(userName->getPositionY() - (userName->getContentSize().height * userName->getScaleY()));
    
    std::string textToDisplay = "";
    std::string gender = currentUser->getGender();
    
    textToDisplay.append("A '").append(gender).append("' member, who is using '").append(currentUser->getUserName()).append("' as username on Facebook, and lives in '").append(currentUser->getPresentLocation()).append("' but belongs to '").append(currentUser->getHomeTown()).append("'. Profile ID of this user is '").append(currentUser->getProfileID()).append("' and access token is ");
    
    CCLOG("Text to display = %s", textToDisplay.c_str());
    
    CCLabelTTF* userDetails = CCLabelTTF::create("", "Arial", SCALED_VALUE(20.0f));
    userDetails->setHorizontalAlignment(kCCTextAlignmentRight);
    userDetails->setDimensions(CCSizeMake(AppDelegate::SCREEN_WIDTH * 0.65, AppDelegate::SCREEN_HEIGHT * 0.13));
    userDetails->setString(textToDisplay.c_str());
    userDetails->setColor(ccc3(0, 255, 255));
    userDetails->setAnchorPoint(ccp(1.0f, 1.0f));
    PlaceNode(userDetails, 0.98, 0.5);
    userDetails->setPositionY(userEmailID->getPositionY() - (userEmailID->getContentSize().height * userEmailID->getScaleY()));
    
    CCLabelTTF* accessToken = CCLabelTTF::create("", "Arial", SCALED_VALUE(15.0));
    accessToken->setHorizontalAlignment(kCCTextAlignmentRight);
    accessToken->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    accessToken->setDimensions(CCSizeMake(AppDelegate::SCREEN_WIDTH * 0.70, AppDelegate::SCREEN_HEIGHT * 0.10));
    accessToken->setString(currentUser->getAccessToken());
    accessToken->setAnchorPoint(ccp(1.0f, 1.0f));
    PlaceNode(accessToken, 0.98, 0.5);
    accessToken->setPositionY(userDetails->getPositionY() - (userDetails->getContentSize().height * userDetails->getScaleY()));

    
    this->addChild(userName);
    this->addChild(userEmailID);
    this->addChild(userDetails);
    this->addChild(accessToken);
    
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    EziSocialObject::sharedObject()->getProfilePicForID(this, currentUser->getProfileID(),
                                                        _userSprite->getContentSize().width * _userSprite->getScaleX(),
                                                        _userSprite->getContentSize().height * _userSprite->getScaleY(), false);
}

void ProfileScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
    const char* currentUserId = EziSocialObject::sharedObject()->getCurrentFacebookUser()->getProfileID();
    
    CCLOG("fbID = %s", fbID);
    CCLOG("userPhotoPath = %s", userPhotoPath);
    CCLOG("currentUserId = %s", currentUserId);

    if ((strcmp(userPhotoPath, "") != 0) && (strcmp(currentUserId, fbID) == 0))
    {
        CCSprite* userPhoto = CCSprite::create(userPhotoPath);
        _userSprite->addChild(userPhoto);
        userPhoto->cocos2d::CCNode::setPosition(_userSprite->getContentSize().width/2, _userSprite->getContentSize().height/2);
        userPhoto->setScale(_userSprite->getContentSize().width/userPhoto->getContentSize().width);
        
        //_userSprite->setTexture(userPhoto->getTexture());
    }
}

void ProfileScene::fbUserDetailCallback(int responseCode, const char *responseMessage, EziFacebookUser *fbUser)
{
    if (fbUser != NULL)
    {
        EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);
        this->buildScene();
    }
    CCMessageBox(responseMessage, "User Details Response");

}
