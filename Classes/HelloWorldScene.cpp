//
//  HelloWorldScene.cpp
//  EziSocial
//
//  Created by Paras Mendiratta on 11/04/13.
//  Copyright @EziByte 2013 (http://www.ezibyte.com)
//
//  Version 1.2 (Dt: 30-May-2013)
//
/***
 
 This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source distribution.
 
 */


#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "EziSocialObject.h"
#include "EziSocialDefinition.h"

#include "FriendList.h"

#include <cstring>

#include "EziFBIncomingRequestManager.h"
#include "RequestList.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    SCREEN_WIDTH  = CCDirector::sharedDirector()->getVisibleSize().width;
    SCREEN_HEIGHT = CCDirector::sharedDirector()->getVisibleSize().height;
    
    SCALE_FACTOR  = SCREEN_HEIGHT / 768.0f;
    SCALE_FACTOR = MAX(0.5, SCALE_FACTOR);
    GAP = 10.0f * SCALE_FACTOR;
    profileID = "";
    
    
    mBallSprite = CCSprite::create("ball.png");
    mBallSprite->setScale(SCALE_FACTOR);
    
    this->addChild(mBallSprite, 100);
    mBallSprite->setPosition(ccp(SCREEN_WIDTH, SCREEN_HEIGHT - mBallSprite->getContentSize().height * SCALE_FACTOR * 0.5));
    
    CCMoveTo *moveLeft = CCMoveTo::create(4.0f * SCALE_FACTOR, ccp(0, mBallSprite->getPositionY()));
    CCMoveTo *moveRight = CCMoveTo::create(4.0f * SCALE_FACTOR, ccp(SCREEN_WIDTH, mBallSprite->getPositionY()));
    
    CCSequence* ballBoune = CCSequence::create(moveLeft, moveRight, NULL);
    
    CCRotateBy* rotation = CCRotateBy::create(4.0f * SCALE_FACTOR, 360);
    
    CCRepeatForever *ballMovementAction = CCRepeatForever::create(CCEaseInOut::create(ballBoune, 2.0f));
    CCRepeatForever *ballRotationAction = CCRepeatForever::create(CCEaseElasticInOut::create(rotation, 2.0f));
    
    
    
    mBallSprite->runAction(ballMovementAction);
    mBallSprite->runAction(ballRotationAction);
    
    
    this->prepareScene();
    
    // Set Delegates
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    EziSocialObject::sharedObject()->setEmailDelegate(this);
    EziSocialObject::sharedObject()->setTwitterDelegate(this);
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setAnchorPoint(ccp(1, 0));
    
    
    
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - GAP, GAP));
    
    pCloseItem->setScale(SCALE_FACTOR);
    
    
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    
    
    
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void HelloWorld::onEnterTransitionDidFinish()
{
    CCLog("Enter Transition Finished.");
    this->loginViaFacebook();
    
    
}

// ---------------------------------------------------------
#pragma mark - Page Preparation Methods
// ---------------------------------------------------------

void HelloWorld::placeLabelInRow(CCLabelTTF* topLabel, CCLabelTTF* labelToPlace)
{
    //labelToPlace->setAnchorPoint(ccp(0.0, 1.0));
    labelToPlace->setPosition(ccp(topLabel->getPositionX(), topLabel->getPositionY() -
                                  topLabel->getContentSize().height - GAP));
}

void HelloWorld::prepareHighScorePage()
{
    _highScoreLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
    
    int maxPlayersToDisplay = 10;
    
    _highScorePlayerNames = CCArray::createWithCapacity(maxPlayersToDisplay);
    _highScores = CCArray::createWithCapacity(maxPlayersToDisplay);
    
    _highScores->retain();
    _highScorePlayerNames->retain();
    
    float fontSize = 40.0f * SCALE_FACTOR;
    const char* fontName = "Arial";
    
    for (int i=0; i<maxPlayersToDisplay; i++)
    {
        CCLabelTTF* playerName  = CCLabelTTF::create("N.A.", fontName, fontSize);
        CCLabelTTF* playerScore = CCLabelTTF::create("0", fontName, fontSize);
        
        playerName->setAnchorPoint(ccp(1, 1));
        playerScore->setAnchorPoint(ccp(0, 1));
        
        playerName->setPosition(ccp(SCREEN_WIDTH/2 - GAP,
                                    SCREEN_HEIGHT - ((GAP * (i+1)) + (i * playerName->getContentSize().height))));
        
        playerScore->setPosition(ccp(SCREEN_WIDTH/2 + GAP, playerName->getPositionY()));
        
        _highScorePlayerNames->addObject(playerName);
        _highScores->addObject(playerScore);
        
        _highScoreLayer->addChild(playerName);
        _highScoreLayer->addChild(playerScore);
    }
    
    _highScoreLayer->setPosition(ccp(0, -SCREEN_HEIGHT));
    this->addChild(_highScoreLayer);
}

void HelloWorld::prepareFacebookActionPage()
{
    
    _facebookActionLayer = CCLayerColor::create(ccc4(20, 100, 100, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    CCMenuItemImage *fbPostMessageButton = CCMenuItemImage::create("post_message_on_wall.png",
                                                                   "post_message_on_wall_pressed.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::postMessageOnFacebookWall));
    
    CCMenuItemImage *fbAutoPostMessageButton = CCMenuItemImage::create("auto_post_message.png",
                                                                       "auto_post_message_pressed.png",
                                                                       this,
                                                                       menu_selector(HelloWorld::autoPostMessageOnFacebookWall));
    
    CCMenuItemImage *fbFriendsListButton = CCMenuItemImage::create("friends_and_high_score.png",
                                                                   "friends_and_high_score_pressed.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::getListOfFriendsUsingThisApp));
    
    
    CCMenuItemImage *fbPageLikeButton = CCMenuItemImage::create("check_page_like.png",
                                                                "check_page_like_pressed.png",
                                                                this,
                                                                menu_selector(HelloWorld::checkIfUserLikeMyFacebookPage));
    
    CCMenuItemImage *fbUserDetailsButton = CCMenuItemImage::create("user_details.png",
                                                                   "user_details_pressed.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::fetchFBUserDetails));
    
    CCMenuItemImage *fbPostScoreButton = CCMenuItemImage::create("post_score.png",
                                                                 "post_score_pressed.png",
                                                                 this,
                                                                 menu_selector(HelloWorld::postScore));
    
    
    CCMenuItemImage *fbHighScoreButton = CCMenuItemImage::create("user_profile_picture.png",
                                                                 "user_profile_picture_pressed.png",
                                                                 this,
                                                                 menu_selector(HelloWorld::postPhoto));
    
    
    CCMenuItemImage *fbPageButton = CCMenuItemImage::create("open_facebook_page.png",
                                                            "open_facebook_page_pressed.png",
                                                            this,
                                                            menu_selector(HelloWorld::openFacebookPage));
    
    
    // Third Menu
    CCMenuItemImage *fbSendGift = CCMenuItemImage::create("send_gift.png",
                                                          "send_gift_pressed.png",
                                                          this,
                                                          menu_selector(HelloWorld::sendGiftsToFriends));
    
    CCMenuItemImage *fbSendInvite = CCMenuItemImage::create("challenge_friends.png",
                                                            "challenge_friends_pressed.png",
                                                            this,
                                                            menu_selector(HelloWorld::challengeFriends));
    
    CCMenuItemImage *fbSendChallenge = CCMenuItemImage::create("send_invite.png",
                                                               "send_invite_pressed.png",
                                                               this,
                                                               menu_selector(HelloWorld::inviteFriends));
    
    CCMenuItemImage *checkSession = CCMenuItemImage::create("session_state.png",
                                                            "session_state_pressed.png",
                                                            this,
                                                            menu_selector(HelloWorld::checkSessionStatus));
    
    
    CCMenuItemImage *incomingButton = CCMenuItemImage::create("incoming_normal.png",
                                                              "incoming_pressed.png",
                                                              this,
                                                              menu_selector(HelloWorld::showRequestList));
    
    
    CCMenu* fbIncomingMenu   = CCMenu::create(incomingButton, NULL);
    
    CCMenu* fbMainActionMenu = CCMenu::create(fbUserDetailsButton,
                                              fbHighScoreButton,
                                              fbPageButton,
                                              fbPageLikeButton,
                                              NULL);
    
    
    CCMenu* fbMainActionMenu2 = CCMenu::create(fbPostMessageButton,
                                               fbAutoPostMessageButton,
                                               fbPostScoreButton,
                                               fbFriendsListButton,
                                               NULL);
    
    CCMenu* fbMainActionMenu3 = CCMenu::create(fbSendGift,
                                               fbSendInvite,
                                               fbSendChallenge,
                                               checkSession,
                                               NULL);
    
    
    pendingRequestCount = CCLabelTTF::create("0", "Arial", 30);
    
    //CCString* incomingCount = CCString::createWithFormat("%d", EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount());
    
    CCString* incomingCount = NULL;
    if (EziFBIncomingRequestManager::sharedManager() != NULL)
    {
        int totalPendingRequest = EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount();
        
        incomingCount = CCString::createWithFormat("%d", totalPendingRequest);
    }
    else
    {
        incomingCount = CCString::createWithFormat("%d", 0);
    }
    
    pendingRequestCount->setString(incomingCount->getCString());
    
    if (pendingRequestCount)
    {
        pendingRequestCount->setPosition(ccp(incomingButton->getContentSize().width - 47,
                                             incomingButton->getContentSize().height/2 + 5));
        incomingButton->addChild(pendingRequestCount);
    }
    
    
    incomingButton->setScale(SCALE_FACTOR);
    
    fbPostMessageButton->setScale(SCALE_FACTOR);
    fbAutoPostMessageButton->setScale(SCALE_FACTOR);
    fbFriendsListButton->setScale(SCALE_FACTOR);
    fbPageLikeButton->setScale(SCALE_FACTOR);
    fbUserDetailsButton->setScale(SCALE_FACTOR);
    fbHighScoreButton->setScale(SCALE_FACTOR);
    fbPostScoreButton->setScale(SCALE_FACTOR);
    fbPageButton->setScale(SCALE_FACTOR);
    
    fbSendChallenge->setScale(SCALE_FACTOR);
    fbSendGift->setScale(SCALE_FACTOR);
    fbSendInvite->setScale(SCALE_FACTOR);
    checkSession->setScale(SCALE_FACTOR);
    
    fbMainActionMenu->alignItemsVerticallyWithPadding(GAP);
    fbMainActionMenu2->alignItemsVerticallyWithPadding(GAP);
    fbMainActionMenu3->alignItemsVerticallyWithPadding(GAP);
    
    float buttonWidth = fbPageButton->getContentSize().width * SCALE_FACTOR;
    float buttonHeight = fbPageButton->getContentSize().height * SCALE_FACTOR;
    float buttonGap   = 5 * SCALE_FACTOR;
    
    fbMainActionMenu->setPosition(ccp(buttonWidth/2 + buttonGap, SCREEN_HEIGHT/2));
    fbMainActionMenu2->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    fbMainActionMenu3->setPosition(ccp(SCREEN_WIDTH - (buttonWidth/2 + buttonGap), SCREEN_HEIGHT/2));
    
    fbIncomingMenu->setPosition(ccp(SCREEN_WIDTH - (buttonWidth/2 + buttonGap), SCREEN_HEIGHT - (buttonHeight/2 + buttonGap)));
    
    _facebookActionLayer->addChild(fbMainActionMenu);
    _facebookActionLayer->addChild(fbMainActionMenu2);
    _facebookActionLayer->addChild(fbMainActionMenu3);
    _facebookActionLayer->addChild(fbIncomingMenu);
    
    _facebookActionLayer->setPositionY(-SCREEN_HEIGHT);
    
    this->addChild(_facebookActionLayer);
    
}


void HelloWorld::checkSessionStatus()
{
    bool activeSession = EziSocialObject::sharedObject()->isFacebookSessionActive();
    
    if (activeSession)
    {
        CCMessageBox("Facebook Session is active", "Session Status");
    }
    else
    {
        CCMessageBox("Facebook session is NOT active.", "Session Status");
    }
}

void HelloWorld::prepareUserDetailPage()
{
    
    _userDetailsLayer    = CCLayerColor::create(ccc4(0, 0, 0, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
    
    float fontSize = 40.0f * SCALE_FACTOR;
    const char* fontName = "Arial";
    
    cocos2d::CCLabelTTF* labelName            = CCLabelTTF::create("Name: ", fontName, fontSize);
    cocos2d::CCLabelTTF* labelUsername        = CCLabelTTF::create("UserName: ", fontName, fontSize);
    cocos2d::CCLabelTTF* labelGender          = CCLabelTTF::create("Gender: ", fontName, fontSize);
    cocos2d::CCLabelTTF* labelHometown        = CCLabelTTF::create("Hometown: ", fontName, fontSize);
    cocos2d::CCLabelTTF* labelPresentLocation = CCLabelTTF::create("Present Location: ", fontName, fontSize);
    cocos2d::CCLabelTTF* labelProfileID       = CCLabelTTF::create("Profile ID: ", fontName, fontSize);
    cocos2d::CCLabelTTF* labelFirstName       = CCLabelTTF::create("First Name: ", fontName, fontSize);
    cocos2d::CCLabelTTF* labelLastName        = CCLabelTTF::create("Last Name: ", fontName, fontSize);
    cocos2d::CCLabelTTF* labelAccessToken     = CCLabelTTF::create("Access Token: ", fontName, fontSize);
    
    
    mName            = CCLabelTTF::create("N.A.", fontName, fontSize);
    mUsername        = CCLabelTTF::create("N.A.", fontName, fontSize);
    mGender          = CCLabelTTF::create("N.A.", fontName, fontSize);
    mHometown        = CCLabelTTF::create("N.A.", fontName, fontSize);
    mPresentLocation = CCLabelTTF::create("N.A.", fontName, fontSize);
    mProfileID       = CCLabelTTF::create("N.A.", fontName, fontSize);
    mFirstName       = CCLabelTTF::create("N.A.", fontName, fontSize);
    mLastName        = CCLabelTTF::create("N.A.", fontName, fontSize);
    mAccessToken     = CCLabelTTF::create("N.A.", fontName, fontSize);
    
    mAccessToken = CCLabelTTF::create("N.A.", fontName, fontSize/2, CCSizeMake(fontSize * 15, fontSize * 20), kCCTextAlignmentLeft);
    
    
    
    labelName->setPosition(ccp(SCREEN_WIDTH * 0.38  - GAP, SCREEN_HEIGHT - GAP));
    labelName->setAnchorPoint(ccp(1.0, 1.0f));
    labelUsername->setAnchorPoint(ccp(1.0, 1.0f));
    labelGender->setAnchorPoint(ccp(1.0, 1.0f));
    labelHometown->setAnchorPoint(ccp(1.0, 1.0f));
    labelPresentLocation->setAnchorPoint(ccp(1.0, 1.0f));
    labelProfileID->setAnchorPoint(ccp(1.0, 1.0f));
    labelFirstName->setAnchorPoint(ccp(1.0, 1.0f));
    labelLastName->setAnchorPoint(ccp(1.0, 1.0f));
    labelAccessToken->setAnchorPoint(ccp(1.0, 1.0f));
    
    this->placeLabelInRow(labelName, labelUsername);
    this->placeLabelInRow(labelUsername, labelGender);
    this->placeLabelInRow(labelGender, labelHometown);
    this->placeLabelInRow(labelHometown, labelPresentLocation);
    this->placeLabelInRow(labelPresentLocation, labelProfileID);
    this->placeLabelInRow(labelProfileID, labelFirstName);
    this->placeLabelInRow(labelFirstName, labelLastName);
    this->placeLabelInRow(labelLastName, labelAccessToken);
    
    mName->setAnchorPoint(ccp(0.0, 1.0f));
    mName->setPosition(ccp(labelName->getPositionX()+GAP, labelName->getPositionY()));
    
    mUsername->setAnchorPoint(mName->getAnchorPoint());
    mGender->setAnchorPoint(mName->getAnchorPoint());
    mHometown->setAnchorPoint(mName->getAnchorPoint());
    mPresentLocation->setAnchorPoint(mName->getAnchorPoint());
    mProfileID->setAnchorPoint(mName->getAnchorPoint());
    mFirstName->setAnchorPoint(mName->getAnchorPoint());
    mLastName->setAnchorPoint(mName->getAnchorPoint());
    mAccessToken->setAnchorPoint(mName->getAnchorPoint());
    
    this->placeLabelInRow(mName, mUsername);
    this->placeLabelInRow(mUsername, mGender);
    this->placeLabelInRow(mGender, mHometown);
    this->placeLabelInRow(mHometown, mPresentLocation);
    this->placeLabelInRow(mPresentLocation, mProfileID);
    this->placeLabelInRow(mProfileID, mFirstName);
    this->placeLabelInRow(mFirstName, mLastName);
    this->placeLabelInRow(mLastName, mAccessToken);
    
    _userDetailsLayer->addChild(labelName);
    _userDetailsLayer->addChild(labelUsername);
    _userDetailsLayer->addChild(labelGender);
    _userDetailsLayer->addChild(labelHometown);
    _userDetailsLayer->addChild(labelPresentLocation);
    _userDetailsLayer->addChild(labelProfileID);
    _userDetailsLayer->addChild(labelFirstName);
    _userDetailsLayer->addChild(labelLastName);
    _userDetailsLayer->addChild(labelAccessToken);
    
    _userDetailsLayer->addChild(mName);
    _userDetailsLayer->addChild(mUsername);
    _userDetailsLayer->addChild(mGender);
    _userDetailsLayer->addChild(mHometown);
    _userDetailsLayer->addChild(mPresentLocation);
    _userDetailsLayer->addChild(mProfileID);
    _userDetailsLayer->addChild(mFirstName);
    _userDetailsLayer->addChild(mLastName);
    _userDetailsLayer->addChild(mAccessToken);
    
    _userDetailsLayer->setPositionY(-SCREEN_HEIGHT);
    this->addChild(_userDetailsLayer);
    
}

void HelloWorld::prepareLoginPage()
{
    
    _loginLayer          = CCLayerColor::create(ccc4(0, 0, 0, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    
    CCMenuItemImage *fbLoginButton = CCMenuItemImage::create("FB_Login_Button.png", "FB_Login_Button_Pressed.png", this, menu_selector(HelloWorld::loginViaFacebook));
    
    fbLoginButton->setScale(SCALE_FACTOR);
    
    CCMenuItemImage *twLoginButton = CCMenuItemImage::create("TW_Tweet_Message_Button.png", "TW_Tweet_Message_Button_Pressed.png", this, menu_selector(HelloWorld::loginViaTwitter));
    
    CCMenuItemImage *internetStatusButton = CCMenuItemImage::create("Internet Status_Button.png", "Internet Status_Button_Pressed.png", this, menu_selector(HelloWorld::checkNetworkStatus));
    
    CCMenuItemImage *sendEmailButton = CCMenuItemImage::create("Send_Email_Button.png", "Send_Email_Button_Pressed.png", this, menu_selector(HelloWorld::sendEmail));
    
    
    twLoginButton->setScale(SCALE_FACTOR);
    internetStatusButton->setScale(SCALE_FACTOR);
    sendEmailButton->setScale(SCALE_FACTOR);
    
    CCMenu *loginMenu = CCMenu::create(fbLoginButton,
                                       twLoginButton,
                                       internetStatusButton,
                                       sendEmailButton, NULL);
    
    loginMenu->alignItemsVerticallyWithPadding(20 * SCALE_FACTOR);
    
    
    _loginLayer->addChild(loginMenu);
    
    this->addChild(_loginLayer);
    
    _currentLayer = _loginLayer;
}

void HelloWorld::prepareScene()
{
    this->prepareLoginPage();
    this->prepareUserDetailPage();
    this->prepareFacebookActionPage();
    this->prepareHighScorePage();
    
    CCMenuItemImage *fbLogoutButton = CCMenuItemImage::create("FB_Logout_Button.png",
                                                              "FB_Logout_Button_Pressed.png",
                                                              this,
                                                              menu_selector(HelloWorld::logoutFromFacebook));
    
    fbLogoutButton->setScale(SCALE_FACTOR);
    fbLogoutButton->setAnchorPoint(ccp(0.5, 0));
    
    CCMenuItemImage *fbActionButton = CCMenuItemImage::create("FB_Action_Menu_Button.png", "FB_Action_Menu_Button_Pressed.png", this, menu_selector(HelloWorld::showFacebookActionPage));
    
    fbActionButton->setScale(SCALE_FACTOR);
    fbActionButton->setAnchorPoint(ccp(0.5, 0));
    
    
    mFBActionMenu = CCMenu::create(fbActionButton, fbLogoutButton, NULL);
    mFBActionMenu->alignItemsHorizontallyWithPadding(GAP);
    
    mFBActionMenu->setPosition(ccp(SCREEN_WIDTH/2, GAP));
    //mFBActionMenu->setPosition(ccp(GAP + (fbActionButton->getContentSize().width/2 * SCALE_FACTOR), 0));
    this->addChild(mFBActionMenu, 100);
    mFBActionMenu->setVisible(false);
    
    
}

// ---------------------------------------------------------
#pragma mark - Page Display Methods
// ---------------------------------------------------------


void HelloWorld::showLoginPage()
{
    mFBActionMenu->setVisible(false);
    this->showLayer(_loginLayer);
}

void HelloWorld::showUserDetailPage()
{
    this->showLayer(_userDetailsLayer);
    mFBActionMenu->setVisible(true);
}

void HelloWorld::showFacebookActionPage()
{
    
    this->showLayer(_facebookActionLayer);
    mFBActionMenu->setVisible(true);
}

void HelloWorld::showHighScorePage(cocos2d::CCArray* highScoreList)
{
    int totalScores = highScoreList->count();
    
    for (int i=0; i<totalScores; i++)
    {
        CCLabelTTF* playerName  = (CCLabelTTF*)_highScorePlayerNames->objectAtIndex(i);
        CCLabelTTF* playerScore = (CCLabelTTF*)_highScores->objectAtIndex(i);
        
        CCDictionary* scoreDict = (CCDictionary*)highScoreList->objectAtIndex(i);
        
        playerName->setString(((CCString*)scoreDict->objectForKey("name"))->getCString());
        playerScore->setString(((CCString*)scoreDict->objectForKey("score"))->getCString());
    }
    mFBActionMenu->setVisible(true);
    this->showLayer(_highScoreLayer);
}

void HelloWorld::showLayer(cocos2d::CCLayerColor* layerToShow)
{
    if (_currentLayer && _currentLayer!= layerToShow)
    {
        _previousLayer = _currentLayer;
    }
    
    _currentLayer = layerToShow;
    
    if (_currentLayer)
    {
        _currentLayer->runAction(CCMoveTo::create(1.0f, ccp(0, 0)));
    }
    if (_previousLayer)
    {
        _previousLayer->runAction(CCMoveTo::create(1.0f, ccp(0, -SCREEN_HEIGHT)));
    }
}

void HelloWorld::showRequestList()
{
    CCScene *pScene = CCScene::create();
	RequestList *pLayer = RequestList::create();
	pScene->addChild(pLayer);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

// ---------------------------------------------------------
#pragma mark - Facebook Action Methods
// ---------------------------------------------------------

void HelloWorld::loginViaFacebook()
{
    if (EziSocialObject::sharedObject()->isFacebookSessionActive() == false)
    {
        bool needsPublishPermission = true;
        EziSocialObject::sharedObject()->performLoginUsingFacebook(needsPublishPermission);
    }
    else
    {
        // User is already logined via Facebook. Just show him the main screen.
        this->showFacebookActionPage();
    }
}

void HelloWorld::logoutFromFacebook()
{
    EziSocialObject::sharedObject()->perfromLogoutFromFacebook();
}

void HelloWorld::postPhoto()
{
    CCRenderTexture *render = CCRenderTexture::create(SCREEN_WIDTH, SCREEN_HEIGHT);
    render->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
    render->begin();
    CCDirector::sharedDirector()->getRunningScene()->visit();
    render->end();
    render->saveToFile("ScreenShot.jpg", kCCImageFormatJPEG);
    
    std::string filePathName = CCFileUtils::sharedFileUtils()->getWritablePath().append("ScreenShot.jpg");
    
    
    EziSocialObject::sharedObject()->postPhoto(filePathName.c_str(), "This is photo test message");
    //EziSocialObject::sharedObject()->postPhoto("ball.png", "This is photo test message");
}

void HelloWorld::getUserPhoto()
{
    
    if (strcmp("", profileID) == 0)
    {
        CCMessageBox("Fetch User Details first", "Profile ID Error");
    }
    else
    {
        EziSocialObject::sharedObject()->getProfilePicForID(profileID, 200 * SCALE_FACTOR/2, 200 * SCALE_FACTOR/2, false);
        
    }
    
    //EziSocialObject::sharedObject()->getUserPhoto(false);
    //EziSocialObject::sharedObject()->getUserPhoto(EziSocialWrapperNS::FBUSER::PIC_LARGE);
    //EziSocialObject::sharedObject()->getUserCustomPhoto(200 * SCALE_FACTOR/2, 200 * SCALE_FACTOR/2);
    
    //ImageDownloader *downloader = new ImageDownloader();
    //downloader->startDownload(this, "1507014347");
    
}

void HelloWorld::postMessageOnFacebookWall()
{
    EziSocialObject::sharedObject()->postMessageOnWall("My Action Game",
                                                       "With Any Caption",
                                                       "This is a test message",
                                                       "I just scored 393,384 points on My Action Game for iOS",
                                                       "http://db.tt/ZhlVyMtp",
                                                       "https://itunes.apple.com/app/storm-the-train/id553852137?mt=8");
    
    
}

void HelloWorld::autoPostMessageOnFacebookWall()
{
    EziSocialObject::sharedObject()->autoPostMessageOnWall("My Action Game - Auto Post Test",
                                                           "With Any Caption",
                                                           "This is test message",
                                                           "I just scored 393,384 points on My Action Game for iOS",
                                                           "",
                                                           "https://itunes.apple.com/app/storm-the-train/id553852137?mt=8");
}

void HelloWorld::checkIfUserLikeMyFacebookPage()
{
    EziSocialObject::sharedObject()->hasUserLikedMyFBPage(FACEBOOK_PAGE_ID);
}

void HelloWorld::getListOfFriendsUsingThisApp()
{
    CCScene *pScene = CCScene::create();
	FriendList *pLayer = FriendList::create();
	pScene->addChild(pLayer);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void HelloWorld::postScore()
{
    EziSocialObject::sharedObject()->postScore(rand()%1000000);
    //EziSocialObject::sharedObject()->deleteScore();
    //EziSocialObject::sharedObject()->checkIncomingRequest();
}

void HelloWorld::getHighScores()
{
    EziSocialObject::sharedObject()->getHighScores();
}

void HelloWorld::fetchFBUserDetails()
{
    EziSocialObject::sharedObject()->fetchFBUserDetails(false);
}

void HelloWorld::openFacebookPage()
{
    EziSocialObject::sharedObject()->openFacebookPage(FACEBOOK_PAGE_ID, true);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

// ---------------------------------------------------------
#pragma mark - Facebook Request Methods
// ---------------------------------------------------------


void HelloWorld::sendGiftsToFriends()
{
    CCDictionary *giftDictionary = CCDictionary::create();
    
    giftDictionary->setObject(CCString::create("500"), "GOLD_COINS");
    giftDictionary->setObject(CCString::create("1"), "LIFE");
    
    CCArray *preselectFriends = CCArray::create();
    
    preselectFriends->addObject(CCString::create("100000706355105"));
    preselectFriends->addObject(CCString::create("100002554472355"));
    
    EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT,
                                                          "I am sending you 500 gold coins & 1 extra life. Enjoy!",
                                                          preselectFriends,
                                                          giftDictionary);
}

void HelloWorld::challengeFriends()
{
    EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE,
                                                          "I have score 900 points in EziSocialDemo. Can you beat me?",
                                                          NULL,
                                                          NULL);
}

void HelloWorld::inviteFriends()
{
    EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE,
                                                          "EziSocial Plugin is damm! easy. Try now!", NULL, NULL);
}


// ---------------------------------------------------------
#pragma mark - Twitter Action Methods
// ---------------------------------------------------------


void HelloWorld::loginViaTwitter()
{
    
    EziSocialObject::sharedObject()->tweet("Twitter Test Message: UTF-8 Support test œ å á â", "ball.png");
    //CCMessageBox("Menu Action: Login Via Twitter pressed", "Login - Twitter");
}

// ---------------------------------------------------------
#pragma mark - Email Action Methods
// ---------------------------------------------------------

void HelloWorld::sendEmail()
{
    EziSocialObject::sharedObject()->sendEmail("UTF-8 Support test àáâäæãåā + Email Subject Line", "<HTML><BODY><B>π Title of the mail</B><BR>This is a test email</BODY></HTML>", "testUserOne@gmail.com;testUserTwo@gmail.com");
}

// ---------------------------------------------------------
#pragma mark - Network Status
// ---------------------------------------------------------

void HelloWorld::checkNetworkStatus()
{
    bool result = EziSocialObject::sharedObject()->checkNetworkStatusForHost("www.facebook.com");
    
    if (result)
    {
        CCMessageBox("Network is available", "Network Status");
    }
    else
    {
        CCMessageBox("Network is not available", "Network Status");
    }
}

// ---------------------------------------------------------
#pragma mark - Facebook Delegate Methods
// ---------------------------------------------------------


void HelloWorld::fbSessionCallback(int responseCode, const char* responseMessage)
{
    
    
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL:
        {
            
            int pendingRequestCountValue = 0;
            if (EziFBIncomingRequestManager::sharedManager() != NULL)
            {
                pendingRequestCountValue = EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount();
            }
            CCString* incomingCount = CCString::createWithFormat("%d", pendingRequestCountValue);
            pendingRequestCount->setString(incomingCount->getCString());
            
            this->showFacebookActionPage();
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_FAILED:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_PERMISSION_DENIED:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_APP_NOT_ALLOWERD_TO_USE_FB:
        {
            CCMessageBox(responseMessage, "Facebook Session - Response");
            this->showLoginPage();
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGOUT_SUCCESSFUL:
        {
            this->showLoginPage();
            break;
        }
        default:
            break;
    }
    
}

void HelloWorld::fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser)
{
    CCMessageBox(responseMessage, "User Details - Response");
    
    if (fbUser)
    {
        mCurrentFacebookUser = fbUser;
        
        mUsername->setString(fbUser->getUserName());
        
        mGender->setString(fbUser->getGender());
        mHometown->setString(fbUser->getHomeTown());
        mPresentLocation->setString(fbUser->getPresentLocation());
        mProfileID->setString(fbUser->getProfileID());
        mFirstName->setString(fbUser->getFirstName());
        mLastName->setString(fbUser->getLastName());
        mAccessToken->setString(fbUser->getAccessToken());
        mName->setString(fbUser->getFullName());
        
        this->showUserDetailPage();
        
        profileID = fbUser->getProfileID();
        this->getUserPhoto();
        
    }
}

void HelloWorld::fbMessageCallback(int responseCode, const char* responseMessage)
{
    const char* title = "";
    
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_NORMAL_MESSAGE_PUBLISHED:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_NORMAL_MESSAGE_CANCELLED:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_NORMAL_MESSAGE_ERROR:
        {
            title = "Normal Post Response";
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_AUTO_MESSAGE_PUBLISHED:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_AUTO_MESSAGE_ERROR:
        {
            title = "Auto Post Response";
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_SCORE_POSTED:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_SCORE_POSTING_ERROR:
        {
            title = "Post Score API Response";
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_SCORE_DELETED:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_SCORE_DELETE_ERROR:
        {
            title = "Delete Score API Response";
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::ERROR_PUBLISH_PERMISSION_ERROR:
        {
            title = "Publish Permission - Denied";
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::ERROR_INTERNET_NOT_AVAILABLE:
        {
            title = "Internet not available";
            break;
        }
            
        default:
            break;
    }
    
    CCMessageBox(responseMessage, title);
}

void HelloWorld::fbSendRequestCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_GIFT_SENDING_ERROR:
            CCMessageBox(responseMessage, "Gift Request Error");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::FB_GIFT_SENDING_CANCELLED:
            CCMessageBox(responseMessage, "Gift Request Cancelled");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::FB_GIFT_SENT:
            CCMessageBox(responseMessage, "Gift Request Success");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::FB_CHALLENGE_SENDING_ERROR:
            CCMessageBox(responseMessage, "Challenge Request Error");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::FB_CHALLEGE_CANCELLED:
            CCMessageBox(responseMessage, "Challenge Request Cancelled");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::FB_CHALLEGE_SENT:
            CCMessageBox(responseMessage, "Challenge Request Success");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::FB_INVITE_SENDING_ERROR:
            CCMessageBox(responseMessage, "Invite Request Error");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::FB_INVITE_CANCELLED:
            CCMessageBox(responseMessage, "Invite Request Cancelled");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::FB_INVITE_SENT:
            CCMessageBox(responseMessage, "Invite Request Success");
            break;
            
            
            
        default:
            break;
    }
}

void HelloWorld::fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests)
{
    if (totalIncomingRequests > 0)
    {
        CCMessageBox(CCString::createWithFormat("Total Pending Requests = %d", totalIncomingRequests, NULL)->getCString(),
                     "Got New Incoming Request");
        CCString* incomingCount = CCString::createWithFormat("%d", totalIncomingRequests);
        pendingRequestCount->setString(incomingCount->getCString());
    }
    else if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::ERROR_INTERNET_NOT_AVAILABLE ||
             responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_INCOMING_REQUEST_ERROR)
    {
        CCMessageBox(responseMessage, "Incoming Request Error");
    }
    else if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_NO_NEW_INCOMING_REQUEST)
    {
        CCLOG(responseMessage);
    }
}

void HelloWorld::fbPageLikeCallback(int responseCode, const char* responseMessage)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_PAGELIKE_ERROR:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_PAGELIKE_NEGATIVE:
        case EziSocialWrapperNS::RESPONSE_CODE::FB_PAGELIKE_POSITIVE:
        case EziSocialWrapperNS::RESPONSE_CODE::ERROR_INTERNET_NOT_AVAILABLE:
        {
            CCMessageBox(responseMessage, "PageLike Response");
            break;
        }
        default:
            break;
    }
}

void HelloWorld::fbPostPhotoCallback(int responseCode, const char* responseMessage)
{
    CCMessageBox(responseMessage, CCString::createWithFormat("Response Code = %d", responseCode, NULL)->getCString());
}


// ---------------------------------------------------------
#pragma mark - EMail Delegate Methods
// ---------------------------------------------------------
void HelloWorld::mailCallback(int responseCode)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_SUCCESS:
            CCMessageBox("Email sent successfully", "Email Status");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_STORED:
            CCMessageBox("User stored the email", "Email Status");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_CANCLLED:
            CCMessageBox("User cancelled email.", "Email Status");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_ERROR:
            CCMessageBox("Got error while sending email.", "Email Status");
            break;
            
            
        default:
            break;
    }
}

void HelloWorld::fbUserPhotoCallback(const char *userPhotoPath)
{
    if (mCurrentFacebookUser)
    {
        CCSprite* userPhoto = EziSocialObject::sharedObject()->generateCCSprite(userPhotoPath);
        mCurrentFacebookUser->saveUserPhotoPath(userPhotoPath);
        
        this->addChild(userPhoto);
        userPhoto->setAnchorPoint(ccp(0, 1));
        userPhoto->setPosition(ccp(10*SCALE_FACTOR, SCREEN_HEIGHT - 10*SCALE_FACTOR));
    }
    
}

void HelloWorld::twitterCallback(int responseCode)
{
    if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::TWIT_SEND)
    {
        CCMessageBox("Tweet Send", "Twitter");
    }
    else if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::TWIT_CANCEL)
    {
        CCMessageBox("Tweet Cancelled", "Twitter");
    }
}