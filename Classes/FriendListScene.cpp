//
//  FriendListScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 05/09/13.
//
//

#include "FriendListScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "EziFacebookFriend.h"
#include "GameConstants.h"
#include "FriendsScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define TAG_CONSUMED_ICON 100
#define TAG_USE_MENU      200
#define TAG_MESSAGE       300
#define TAG_PHOTO         400

CCScene* FriendListScene::scene(FRIEND_CIRCLE friendCircle)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    FriendListScene *layer = FriendListScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    layer->setSelectedFriendCircle(friendCircle);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool FriendListScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    return true;
}

void FriendListScene::setSelectedFriendCircle(FRIEND_CIRCLE selectedFriendCircle)
{
    _selectedFriendCircle = selectedFriendCircle;
    this->buildScene();
}

/**
 *  Use this method for autologin in facebook account.
 */
void FriendListScene::onEnterTransitionDidFinish()
{
    //CCLog("[HomeScene]: Enter Transition Finished.");
    //EziSocialObject::sharedObject()->performLoginUsingFacebook(true);
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
    switch (_selectedFriendCircle)
    {
        case ALL_FRIENDS:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, 0, 40);
            break;
            
        case INSTALLED_ONLY:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_INSTALLED, 0, 500);
            break;
            
        case NON_PLAYING_ONLY:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_NOT_INSTALLED, 10, 40);
            break;
            
        case MY_DEVICE_ONLY:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_MY_DEVICES, 0, 500);
            break;
            
        default:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, 0, 40);
            break;
    }
}

void FriendListScene::buildScene()
{
    _friendDictionary = NULL;
    _friendList = NULL;
    _tableView = NULL;
    _downloadCount = 0;
    
    const char* headerString = "";
    
    switch (_selectedFriendCircle)
    {
        case ALL_FRIENDS:
            headerString = "All Friends";
            break;
        
        case INSTALLED_ONLY:
            headerString = "Friends Playing Game";
            break;
            
        case NON_PLAYING_ONLY:
            headerString = "Friends not Playing Game";
            break;
            
        case MY_DEVICE_ONLY:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            headerString = "Friends playing on iOS Device";
#else
            headerString = "Friends playing on Android Device";
#endif
        default:
            break;
    }
    
    _friendList = CCArray::createWithCapacity(5);
    _friendList->retain();
    CCLOG("ALL Request count = %d", _friendList->count());
    
    // Add background layers to the scene.
    CCLayerColor *bg = CCLayerColor::create(ccc4(255, 255, 255, 255), AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT);
    this->addChild(bg);
    
    CCLayerColor *secondBG = CCLayerColor::create(ccc4(95, 95, 95, 255),
                                                  AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f),
                                                  AppDelegate::SCREEN_HEIGHT - SCALED_VALUE(20.0f));
    
    secondBG->setPosition(ccp(SCALED_VALUE(10.0f), SCALED_VALUE(10.0f)));
    this->addChild(secondBG);
    
    // Set the table header.
    CCLayerColor *header = CCLayerColor::create(ccc4(66, 66, 66, 255),
                                                AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f),
                                                SCALED_VALUE(100.0f));
    header->setAnchorPoint(ccp(0, 0));
    header->setPosition(ccp(SCALED_VALUE(10.0f), AppDelegate::SCREEN_HEIGHT - SCALED_VALUE(110.0f)));
    this->addChild(header);
    
    // Set the header text.
    CCLabelTTF* headerText = CCLabelTTF::create(headerString, "Arial", SCALED_VALUE(40.0f));
    headerText->setAnchorPoint(ccp(0.0f, 0.5f));
    header->addChild(headerText);
    headerText->setPositionX(SCALED_VALUE(28.0f));
    headerText->setPositionY(header->getContentSize().height/2);
    
    // Add back button go back to previous scene
    CCMenuItemImage *backButton = CCMenuItemImage::create();
    backButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_table_back_nrl"));
    backButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_table_back_prd"));
    backButton->setTarget(this, menu_selector(FriendListScene::backButtonPressed));
    backButton->setAnchorPoint(ccp(1.0f, 0.5));
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    header->addChild(backMenu);
    backMenu->setPositionX(header->getContentSize().width - SCALED_VALUE(20.0f));
    backMenu->setPositionY(header->getContentSize().height/2);
    
    
    // Create the table view.
    float tableHeight = bg->getContentSize().height - secondBG->getPositionY() - (header->getContentSize().height) - SCALED_VALUE(10);
    _tableView = CCTableView::create(this, CCSizeMake(secondBG->getContentSize().width, tableHeight));
    _tableView->setAnchorPoint(ccp(0, 0.0));
    _tableView->setDirection(kCCScrollViewDirectionVertical);
    _tableView->setPosition(ccp(SCALED_VALUE(10.0f), SCALED_VALUE(10)));
    _tableView->setDelegate(this);
    this->addChild(_tableView);
}

void FriendListScene::backButtonPressed()
{
    CCScene* pScene = FriendsScene::scene();
    ReplaceToLeft(pScene);
}

void FriendListScene::keyBackClicked()
{
    backButtonPressed();
}

void FriendListScene::useRequest(cocos2d::CCNode *sender)
{
    CCLOG("Use Button Tag = %d", sender->getTag());
}

//-------------------------------------
#pragma mark - Tableview delegates
//-------------------------------------

void FriendListScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}
CCSize FriendListScene::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f), SCALED_VALUE(124.0f));
}

CCTableViewCell* FriendListScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    int index = idx;
    EziFacebookFriend* myFriend = (EziFacebookFriend*)_friendList->objectAtIndex(index);
    std::string friendName = myFriend->getName();
    const char* photoPath = myFriend->getPhotoPath();
    bool toRequestForPhoto = false;
    
    if (strcmp(photoPath, "") == 0)
    {
        toRequestForPhoto = true;
        //EziSocialObject::sharedObject()->getProfilePicForID(this, myFriend->getFBID(), SCALED_VALUE(85.0f), SCALED_VALUE(85.0f), false);
    }
    
    // Build the table cell.
    CCTableViewCell *cell = table->cellAtIndex(idx);
    
    if (cell == NULL)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
        
        CCLayerColor* colorLayer = CCLayerColor::create(ccc4(100, 100, 100, 200),
                                                        AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f),
                                                        SCALED_VALUE(124.0f));
        //cell->addChild(colorLayer);
        
        CCLayerColor* photoLayer = CCLayerColor::create(ccc4(255, 255, 255, 255),
                                                        SCALED_VALUE(100.0f),
                                                        SCALED_VALUE(100.0f));
        photoLayer->setPosition(SCALED_VALUE(40.0f), SCALED_VALUE(12.0f));
        cell->addChild(photoLayer);
        CCSprite* userPhotoSprite = NULL;
        
        if (toRequestForPhoto)
        {
            userPhotoSprite = CCSprite::create();
            userPhotoSprite->setContentSize(CCSizeMake(SCALED_VALUE(85.0f), SCALED_VALUE(85.0f)));
        }
        else
        {
            userPhotoSprite = CCSprite::create(myFriend->getPhotoPath());
        }
        
        cell->addChild(userPhotoSprite);
        userPhotoSprite->setAnchorPoint(ccp(0.0f, 0.0));
        userPhotoSprite->cocos2d::CCNode::setPosition(SCALED_VALUE(47.0f), SCALED_VALUE(18.0f));
        userPhotoSprite->setTag(TAG_PHOTO);
        
        CCLabelTTF* message = CCLabelTTF::create(friendName.c_str(), "Arial", SCALED_VALUE(30.0f));
        message->setAnchorPoint(ccp(0, 0.5));
        message->setPosition(ccp(photoLayer->getContentSize().width + photoLayer->getPositionX() + SCALED_VALUE(20.0f),
                                 colorLayer->getContentSize().height/2));
        cell->addChild(message);
        message->setTag(TAG_MESSAGE);
        
        
        /*
        CCSprite* consumedIcon = CCSprite::createWithSpriteFrameName("green_tick");
        consumedIcon->setAnchorPoint(ccp(1.0, 0.5));
        cell->addChild(consumedIcon);
        consumedIcon->setPosition(ccp(colorLayer->getContentSize().width - SCALED_VALUE(20.0f),
                                      colorLayer->getContentSize().height/2));
        message->setTag(TAG_CONSUMED_ICON);
        
        CCMenuItemImage* useButton = CCMenuItemImage::create();
        useButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_use_prd"));
        useButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_use_nrl"));
        useButton->setAnchorPoint(ccp(1.0, 0.5));
        useButton->setTarget(this, menu_selector(FriendListScene::useRequest));
        
        CCMenu* useMenu = CCMenu::create(useButton, NULL);
        cell->addChild(useMenu);
        useMenu->setPosition(ccp(colorLayer->getContentSize().width - SCALED_VALUE(20.0f),
                                 colorLayer->getContentSize().height/2));
        
        useButton->setTag(index);
        useMenu->setTag(TAG_USE_MENU);
        */
        
    }
    else
    {
        CCSprite* cellProfilePic = (CCSprite*)cell->getChildByTag(TAG_PHOTO);
        
        if (toRequestForPhoto == false)
        {
            cellProfilePic->setTexture(CCSprite::create(myFriend->getPhotoPath())->getTexture());
        }
        
        // Update the message Label.
        CCLabelTTF *messageLabel = (CCLabelTTF*)cell->getChildByTag(TAG_MESSAGE);
        CCLOG("Friend Name = %s", friendName.c_str());
        
        if (messageLabel)
        {
            messageLabel->setString(friendName.c_str());    
        }
        
        
    }
    
    return cell;
}

unsigned int FriendListScene::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return _friendList->count();
}



//-------------------------------------
#pragma mark - eziSocial delegates
//-------------------------------------

void FriendListScene::fbFriendsCallback(int responseCode, const char *responseMessage, cocos2d::CCArray *friends)
{
    CCMessageBox(responseMessage, "Friends Callback");
    
    if (_friendList)
    {
        _friendList->release();
        _friendList = NULL;
    }
    
    _friendList = CCArray::createWithArray(friends);
    _friendList->retain();
    
    if (_friendDictionary)
    {
        _friendDictionary->release();
    }
    
    _friendDictionary = NULL;
    _friendDictionary = CCDictionary::create();
    
    int totalFriends = _friendList->count()-1;
    _downloadCount = 0;
    
    for (int i=totalFriends; i >= 0; i--)
    {
        EziFacebookFriend* myFriend = (EziFacebookFriend*)_friendList->objectAtIndex(i);
        _friendDictionary->setObject(myFriend, myFriend->getFBID());
        EziSocialObject::sharedObject()->getProfilePicForID(this, myFriend->getFBID(), SCALED_VALUE(85.0f), SCALED_VALUE(85.0f), false);
    }
    
    
    _friendDictionary->retain();
    _tableView->reloadData();
}

void FriendListScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
    if (_friendDictionary == NULL)
    {
        return;
    }
    
    EziFacebookFriend* myFriend = (EziFacebookFriend*)_friendDictionary->objectForKey(fbID);
    
    if (myFriend)
    {
        myFriend->setPhotoPath(userPhotoPath);
        CCLOG("Downloaded photo for = %s", myFriend->getName());
    }
    else
    {
        CCLOG("Friend with ID = %s not found.", fbID);
    }
    
    _downloadCount++;
    if (_downloadCount == _friendList->count())
    {
        // this is the last we fetched. We can refresh the table view now.
        CCLOG("Download completed...");
        _tableView->reloadData();
    }
    
}