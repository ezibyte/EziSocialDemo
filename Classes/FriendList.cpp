//
//  FriendList.cpp
//  EziSocialDemo
//
//  Created by Paras Mendiratta on 04/05/13.
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

#include "FriendList.h"
#include "HelloWorldScene.h"
#include "EziSocialObject.h"
#include "EziFacebookFriend.h"

USING_NS_CC;
USING_NS_CC_EXT;

static float SCREEN_WIDTH  = 480;
static float SCREEN_HEIGHT = 320;
static float CELL_HEIGHT = 320;

static int FRIEND_ID_LABEL_TAG          = 100;
static int FRIEND_NAME_LABEL_TAG        = FRIEND_ID_LABEL_TAG           + 1;
static int FRIEND_INSTALLED_LABEL_TAG   = FRIEND_NAME_LABEL_TAG         + 1;
static int FRIEND_SCORE_LABEL_TAG       = FRIEND_INSTALLED_LABEL_TAG    + 1;
static int FRIEND_PHOTO_TAG             = FRIEND_SCORE_LABEL_TAG        + 1;
static const char* FB_DEFAULT_PHOTO     = "fb_user_icon.jpg";

static float MENU_FONT_SCALE     = 1.0f;
static float PHOTO_SCALE = 1.0f;
static bool ALL_DOWNLOAD_COMPLETE = true;

bool FriendList::init()
{
    if ( !CCLayer::init())
    {
        return false;
    }
    
    mEnableHighScoreDisplay = false;
    mEnableInstalledDisplay = false;
    mReadyForNextDownload   = false;
    mPhotoLoadIndex         = 0;
    
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    SCREEN_WIDTH  = winSize.width;
    SCREEN_HEIGHT = winSize.height;
    
    CCLayerColor *backgroundLayer = CCLayerColor::create(ccc4(20, 100, 100, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
    
    this->addChild(backgroundLayer);
    
    
    MENU_FONT_SCALE = SCREEN_HEIGHT/320;
    
    
    // Back Menu
    
    CCMenuItemFont *itemBack = CCMenuItemFont::create("Back", this, menu_selector(FriendList::showHomePage));
	itemBack->setPosition(ccp(SCREEN_WIDTH/2, (itemBack->getContentSize().height/2 + 2) * MENU_FONT_SCALE));
	
    CCMenuItemFont *itemHighScore = CCMenuItemFont::create("High Scores", this, menu_selector(FriendList::showHighScoreList));
    itemHighScore->setAnchorPoint(ccp(1, 0.5));
    itemHighScore->setPosition(ccp((SCREEN_WIDTH-10), itemBack->getPositionY()));
    
    CCMenuItemFont *allFriends = CCMenuItemFont::create("All Friends", this, menu_selector(FriendList::showAllFriendsList));
    allFriends->setAnchorPoint(ccp(0, 0.5));
    allFriends->setPosition(ccp((10), itemBack->getPositionY()));
    
    CCMenuItemFont *installedOnly = CCMenuItemFont::create("Installed Only", this, menu_selector(FriendList::showInstalledList));
    installedOnly->setAnchorPoint(ccp(0, 1));
    installedOnly->setPosition(ccp((10), SCREEN_HEIGHT - 2));
    
    CCMenuItemFont *notPlaying = CCMenuItemFont::create("Friends Not Playing", this, menu_selector(FriendList::showNotInstalledList));
    notPlaying->setAnchorPoint(ccp(1, 1));
    notPlaying->setPosition(ccp((SCREEN_WIDTH-10), SCREEN_HEIGHT - 2));
    
    
    
    
    itemBack->setScale(MENU_FONT_SCALE);
    itemHighScore->setScale(MENU_FONT_SCALE);
    allFriends->setScale(MENU_FONT_SCALE);
    installedOnly->setScale(MENU_FONT_SCALE);
    notPlaying->setScale(MENU_FONT_SCALE);
    
    CCMenu *menuBack = CCMenu::create(itemBack, itemHighScore, allFriends, installedOnly, notPlaying, NULL);
	menuBack->setPosition(CCPointZero);
	addChild(menuBack);
    
    float gap = itemBack->getContentSize().height * 2 * MENU_FONT_SCALE + (10 * MENU_FONT_SCALE);
    
    mFriendList = NULL;
    
    PHOTO_SCALE = SCREEN_HEIGHT/1536;
    
    if (PHOTO_SCALE <= 0.5 && PHOTO_SCALE > 0.25)
    {
        FB_DEFAULT_PHOTO = "fb_user_icon_half.jpg";
    }
    else if (PHOTO_SCALE < 0.25)
    {
        FB_DEFAULT_PHOTO = "fb_user_icon_quater.jpg";
    }
    
    
    CCSprite *sprite = CCSprite::create(FB_DEFAULT_PHOTO);
    CELL_HEIGHT = (sprite->getContentSize().height) + (40 * PHOTO_SCALE);
    
    
    mTableView = CCTableView::create(this, CCSizeMake(SCREEN_WIDTH, SCREEN_HEIGHT - gap));
    mTableView->setDirection(kCCScrollViewDirectionVertical);
    mTableView->setPosition(ccp(0, gap/2));
    
    mTableView->setDelegate(this);
    this->addChild(mTableView);
    
    mTableView->reloadData();
    
    ALL_DOWNLOAD_COMPLETE = true;
    
    mLoadingImage = CCSprite::create("ball.png");
    mLoadingImage->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    mLoadingImage->retain();
    
    this->addChild(mLoadingImage);
    this->hideLoadingAction();
    
    return true;
}


// ---------------------------------------------
#pragma mark - TableView Delegate Methods
// ---------------------------------------------

void FriendList::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize FriendList::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(SCREEN_WIDTH, CELL_HEIGHT + 2);
}

CCTableViewCell* FriendList::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    EziFacebookFriend* friendDetails = (EziFacebookFriend*)mFriendList->objectAtIndex(idx);
    std::string friendID            = friendDetails->getFBID();
    std::string friendName          = friendDetails->getName();
    std::string installed           = "";
    std::string score               = "";
    CCSprite* profilePic            = NULL;
    
    // Set the score.
    long mylong = friendDetails->getScore();
    std::stringstream mystream;
    mystream << mylong;
    score = mystream.str();
    
    // Set the install status
    if (friendDetails->isInstalled())
    {
        installed = "Installed";
    }
    else
    {
        installed = "Not Installed";
    }
    
    
    CCTableViewCell *cell = table->dequeueCell();
    
    
    if (!cell) // Creation of Cell.
    {
        cell = new CCTableViewCell();
        cell->autorelease();
        
        CCLayerColor* colorLayer = CCLayerColor::create(ccc4(20, 0, 40, 255), SCREEN_WIDTH, CELL_HEIGHT);
        cell->addChild(colorLayer);
        
        CCSprite *sprite = NULL;
        
        if (strcmp(friendDetails->getPhotoPath(), "") != 0)
        {
            const char*  picPath = friendDetails->getPhotoPath();
            sprite = CCSprite::create(picPath);
        }
        else
        {
            sprite = CCSprite::create(FB_DEFAULT_PHOTO);
        }
        
        if (sprite == NULL)
        {
            CCLOG("Sprite is NULL");
        }
        
        sprite->setAnchorPoint(ccp(0, 0.5));
        sprite->setPosition(ccp(20, CELL_HEIGHT/2));
        cell->addChild(sprite);
        //sprite->setScale(0.9);
        sprite->setTag(FRIEND_PHOTO_TAG);
        
        // Friend Facebook ID
        CCLabelTTF *friendIDLabel = CCLabelTTF::create(friendID.c_str(), "Helvetica", 20.0 * MENU_FONT_SCALE);
        
        float gapY =  (CELL_HEIGHT - (friendIDLabel->getContentSize().height * 2)) / 3;
        
        if (sprite)
        {
            friendIDLabel->setPosition(ccp(sprite->getPositionX() + sprite->getContentSize().width + 20, CELL_HEIGHT - gapY));
        }
        
		friendIDLabel->setAnchorPoint(ccp(0, 1));
        friendIDLabel->setTag(FRIEND_ID_LABEL_TAG);
        cell->addChild(friendIDLabel);
        
        // Friend Facebook Name
        CCLabelTTF *friendNameLabel = CCLabelTTF::create(friendName.c_str(), "Helvetica", 20.0 * MENU_FONT_SCALE);
        friendNameLabel->setPosition(ccp(friendIDLabel->getPositionX(),
                                         friendIDLabel->getPositionY() - friendIDLabel->getContentSize().height - gapY));
        
		friendNameLabel->setAnchorPoint(ccp(0, 1));
        friendNameLabel->setTag(FRIEND_NAME_LABEL_TAG);
        cell->addChild(friendNameLabel);
        
        
        // High Score
        CCLabelTTF *scoreLabel = CCLabelTTF::create(score.c_str(), "Helvetica", 20.0 * MENU_FONT_SCALE);
        scoreLabel->setPosition(ccp(SCREEN_WIDTH - 20, friendIDLabel->getPositionY()));
        
        scoreLabel->setAnchorPoint(ccp(1, 1));
        scoreLabel->setTag(FRIEND_SCORE_LABEL_TAG);
        cell->addChild(scoreLabel);
        
        // Installed String
        CCLabelTTF *installedLabel = CCLabelTTF::create(installed.c_str(), "Helvetica", 20.0 * MENU_FONT_SCALE);
        installedLabel->setPosition(ccp(SCREEN_WIDTH - 20, friendNameLabel->getPositionY()));
        
        installedLabel->setAnchorPoint(ccp(1, 1));
        installedLabel->setTag(FRIEND_INSTALLED_LABEL_TAG);
        cell->addChild(installedLabel);
        
    }
    else
    {
        
        // Set the Friend ID
        CCLabelTTF *friendIDLabel = (CCLabelTTF*)cell->getChildByTag(FRIEND_ID_LABEL_TAG);
        friendIDLabel->setString(friendID.c_str());
        
        // Set the Friend Name
        CCLabelTTF *friendNameLabel = (CCLabelTTF*)cell->getChildByTag(FRIEND_NAME_LABEL_TAG);
        friendNameLabel->setString(friendName.c_str());
        
        
        CCLabelTTF *highScoreLabel = (CCLabelTTF*)cell->getChildByTag(FRIEND_SCORE_LABEL_TAG);
        if (highScoreLabel != NULL)
        {
            highScoreLabel->setString(score.c_str());
        }
        
        highScoreLabel->setVisible(mEnableHighScoreDisplay);
        
        
        CCLabelTTF *installedLabel = (CCLabelTTF*)cell->getChildByTag(FRIEND_INSTALLED_LABEL_TAG);
        if (installedLabel != NULL)
        {
            installedLabel->setString(installed.c_str());
        }
        installedLabel->setVisible(mEnableInstalledDisplay);
        
        
        CCSprite* cellProfilePic = (CCSprite*)cell->getChildByTag(FRIEND_PHOTO_TAG);
        
        if (strcmp("", friendDetails->getPhotoPath()) != 0 )
        {
            const char*  picPath = friendDetails->getPhotoPath();
            profilePic = CCSprite::create(picPath);
            
        }
        else
        {
            profilePic = CCSprite::create(FB_DEFAULT_PHOTO);
        }
        
        
        cellProfilePic->setTexture(profilePic->getTexture());
    }
    
    
    return cell;
}

unsigned int FriendList::numberOfCellsInTableView(CCTableView *table)
{
    if (mFriendList == NULL)
    {
        return 0;
    }
    else
    {
        return mFriendList->count();
    }
}

// ---------------------------------------------
#pragma mark - Menu Callback Methods
// ---------------------------------------------


void FriendList::showHomePage()
{
    this->unscheduleUpdate();
    CCScene *pScene = HelloWorld::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void FriendList::showAllFriendsList()
{
    if (ALL_DOWNLOAD_COMPLETE == false)
    {
        CCMessageBox("Downloading of images in Progess. Try after sometime", "Downloading Images");
        return;
    }
    else
    {
        ALL_DOWNLOAD_COMPLETE = false;
    }
    this->showLoadingAction();
    this->unscheduleUpdate();
    EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, 20, 20);
}

void FriendList::showHighScoreList()
{
    if (ALL_DOWNLOAD_COMPLETE == false)
    {
        CCMessageBox("Downloading of images in Progess. Try after sometime", "Downloading Images");
        return;
    }
    else
    {
        ALL_DOWNLOAD_COMPLETE = false;
    }
    
    this->showLoadingAction();
    this->unscheduleUpdate();
    EziSocialObject::sharedObject()->getHighScores();
}

void FriendList::showInstalledList()
{
    if (ALL_DOWNLOAD_COMPLETE == false)
    {
        CCMessageBox("Downloading of images in Progess. Try after sometime", "Downloading Images");
        return;
    }
    else
    {
        ALL_DOWNLOAD_COMPLETE = false;
    }
    
    this->showLoadingAction();
    this->unscheduleUpdate();
    EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_INSTALLED, 0, 10);
}

void FriendList::showNotInstalledList()
{
    if (ALL_DOWNLOAD_COMPLETE == false)
    {
        CCMessageBox("Downloading of images in Progess. Try after sometime", "Downloading Images");
        return;
    }
    else
    {
        ALL_DOWNLOAD_COMPLETE = false;
    }
    
    this->showLoadingAction();
    this->unscheduleUpdate();
    EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_NOT_INSTALLED, 0, 10);
}

// ---------------------------------------------
#pragma mark - Helper Private Methods
// ---------------------------------------------

void FriendList::setFriendsData(cocos2d::CCArray* friendList)
{
    
    if (mFriendList != NULL)
    {
        mFriendList->release();
        mFriendList = NULL;
    }
    
    this->mFriendList = CCArray::createWithArray(friendList);
    this->mFriendList->retain();
    
    if (mFriendList->count() <= 0)
    {
        ALL_DOWNLOAD_COMPLETE = true;
        this->hideLoadingAction();
        return;
    }
    
    mPhotoLoadIndex = friendList->count();
    this->mTableView->reloadData();
    
    mReadyForNextDownload = true;
    this->scheduleUpdate();
}

void FriendList::downloadNextPhoto()
{
    if (mPhotoLoadIndex > 0)
    {
        mPhotoLoadIndex--;
        EziFacebookFriend* friendDetails = (EziFacebookFriend*)mFriendList->objectAtIndex(mPhotoLoadIndex);
        
        unsigned int size = 200;
        
        if (PHOTO_SCALE <= 0.5 && PHOTO_SCALE > 0.25)
        {
            size = 100;
        }
        else if (PHOTO_SCALE < 0.25)
        {
            size = 50;
        }
        
        EziSocialObject::sharedObject()->getProfilePicForID(friendDetails->getFBID(),
                                                            size, size, false);
    }
    else
    {
        this->unscheduleUpdate();
        ALL_DOWNLOAD_COMPLETE = true;
        this->hideLoadingAction();
    }
}

void FriendList::showLoadingAction()
{
    mLoadingImage->setVisible(true);
    CCRotateBy* rotateAction = CCRotateBy::create(1.0f, 360);
    CCRepeatForever* repeatAction = CCRepeatForever::create(rotateAction);
    repeatAction->retain();
    
    mLoadingImage->runAction(repeatAction);
    
}

void FriendList::hideLoadingAction()
{
    mLoadingImage->stopAllActions();
    mLoadingImage->setVisible(false);
}

// ---------------------------------------------
#pragma mark - CCLayer Delegate Methods
// ---------------------------------------------

void FriendList::update(float delta)
{
    if (mReadyForNextDownload)
    {
        this->mTableView->updateCellAtIndex(mPhotoLoadIndex);
        mReadyForNextDownload = false;
        this->downloadNextPhoto();
    }
}

// ---------------------------------------------
#pragma mark - Facebook Delegate Methods
// ---------------------------------------------


void FriendList::fbFriendsCallback(int responseCode, const char* responseMessage, CCArray *friends)
{
    if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_FRIEND_GET_ERROR || responseCode == EziSocialWrapperNS::RESPONSE_CODE::ERROR_INTERNET_NOT_AVAILABLE)
    {
        CCMessageBox(responseMessage, "Friends List - Error");
        this->hideLoadingAction();
        ALL_DOWNLOAD_COMPLETE = true;
    }
    else
    {
        
        
        if (friends != NULL && friends->count() > 0)
        {
            
            mEnableHighScoreDisplay = false;
            mEnableInstalledDisplay = true;
            this->setFriendsData(friends);
            CCMessageBox(responseMessage, "Friends List");
        }
        else
        {
            this->hideLoadingAction();
            ALL_DOWNLOAD_COMPLETE = true;
            CCMessageBox("Zero Friends Fetched", "Friends List");
        }
    }
}

void FriendList::fbHighScoresCallback(int responseCode, const char* responseMessage, CCArray *highScores)
{
    if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_HIGH_SCORE_GET_ERROR || responseCode == EziSocialWrapperNS::RESPONSE_CODE::ERROR_INTERNET_NOT_AVAILABLE)
    {
        CCMessageBox(responseMessage, "High Score - Error");
        this->hideLoadingAction();
        ALL_DOWNLOAD_COMPLETE = true;
    }
    else
    {
        
        if (highScores != NULL && highScores->count() > 0)
        {
            mEnableInstalledDisplay = false;
            mEnableHighScoreDisplay = true;
            this->setFriendsData(highScores);
            CCMessageBox(responseMessage, "High Score");
        }
        else
        {
            this->hideLoadingAction();
            ALL_DOWNLOAD_COMPLETE = true;
            CCMessageBox("None of your friend is playing", "High Score");
        }
    }
}

void FriendList::fbUserPhotoCallback(const char *userPhotoPath)
{
    EziFacebookFriend* friendDetails = (EziFacebookFriend*)mFriendList->objectAtIndex(mPhotoLoadIndex);
    friendDetails->setPhotoPath(userPhotoPath);
    
    mReadyForNextDownload = true;
}