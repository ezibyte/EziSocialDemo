//
//  RequestListScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 10/09/13.
//
//

#include "RequestListScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "EziFacebookFriend.h"
#include "GameConstants.h"
#include "RequestAPIScene.h"
#include "EziFBIncomingRequestManager.h"
#include "EziFBIncomingRequest.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define TAG_TICK_ICON     100
#define TAG_PHOTO         200
#define TAG_TTTLE         300
#define TAG_MESSAGE       400
#define TAG_ITEM_ICON     500

CCScene* RequestListScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    RequestListScene *layer = RequestListScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool RequestListScene::init()
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
void RequestListScene::onEnterTransitionDidFinish()
{
    //CCLog("[HomeScene]: Enter Transition Finished.");
    //EziSocialObject::sharedObject()->performLoginUsingFacebook(true);
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
}

void RequestListScene::onExit()
{
    CCLayer::onExit();
    _fbMessagesList->release();
    _selectedRequestItems->release();
    EziSocialObject::sharedObject()->setFacebookDelegate(NULL);
}

void RequestListScene::buildScene()
{
    _fbMessagesList = NULL;
    _tableView      = NULL;
    _downloadCount  = 0;
    _friendDictionary = NULL;
    _selectedRequestItems = NULL;
    _allSelected = false;
    
    _fbMessagesList = EziFBIncomingRequestManager::sharedManager()->getAllRequests();
    _fbMessagesList->retain();
    
    _selectedRequestItems = CCDictionary::create();
    _selectedRequestItems->retain();
    
    CCLOG("ALL Request count = %d", _fbMessagesList->count());
    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_REQUEST));
    this->addChild(batchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_REQUEST));
    
    
    const char* headerString = "Your Messages";
    
    
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
    backButton->setTarget(this, menu_selector(RequestListScene::backButtonPressed));
    backButton->setAnchorPoint(ccp(1.0f, 0.5));
    CCMenu* backMenu = CCMenu::create(backButton, NULL);
    header->addChild(backMenu);
    backMenu->setPositionX(header->getContentSize().width - SCALED_VALUE(20.0f));
    backMenu->setPositionY(header->getContentSize().height/2);
    
    // Set the footer.
    CCLayerColor *footer = CCLayerColor::create(ccc4(66, 66, 66, 255),
                                                AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f),
                                                SCALED_VALUE(100.0f));
    footer->setAnchorPoint(ccp(0, 0));
    footer->setPosition(ccp(SCALED_VALUE(10.0f), SCALED_VALUE(0.0f)));
    this->addChild(footer);
    
    // Add buttons to the footer.
    CCMenuItemImage *acceptButton = CCMenuItemImage::create();
    acceptButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_accept_nrl"));
    acceptButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_accept_prd"));
    acceptButton->setTarget(this, menu_selector(RequestListScene::acceptButtonPressed));
    acceptButton->setAnchorPoint(ccp(0.0f, 0.5));
    
    _allButton = CCMenuItemImage::create();
    _allButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("check_all_unslected"));
    _allButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("check_all_prd"));
    _allButton->setTarget(this, menu_selector(RequestListScene::acceptAllCheckboxPressed));
    _allButton->setAnchorPoint(ccp(1.0f, 0.5));

    
    CCMenu* footerMenu = CCMenu::create(_allButton, acceptButton, NULL);
    footerMenu->alignItemsHorizontallyWithPadding(SCALED_VALUE(20.0f));
    footer->addChild(footerMenu);
    footerMenu->setPositionX(footer->getContentSize().width/2);
    footerMenu->setPositionY(footer->getContentSize().height/2);
    
    // Create the table view.
    float tableHeight = bg->getContentSize().height - secondBG->getPositionY() - (2 * header->getContentSize().height) - SCALED_VALUE(10);
    _tableView = CCTableView::create(this, CCSizeMake(secondBG->getContentSize().width, tableHeight));
    _tableView->setAnchorPoint(ccp(0, 0.0));
    _tableView->setDirection(kCCScrollViewDirectionVertical);
    _tableView->setPosition(ccp(SCALED_VALUE(10.0f), SCALED_VALUE(10) + (header->getContentSize().height)));
    _tableView->setDelegate(this);
    this->addChild(_tableView);

}

void RequestListScene::acceptAllCheckboxPressed()
{
    CCLOG("Accept All button pressed.");
    _allSelected = !_allSelected;
    
    if (_allSelected)
    {
        _allButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("check_all_selected"));
        int totalRequest = _fbMessagesList->count();
        bool anyChanges = false;
        
        for (int i=0; i<totalRequest; i++)
        {
            EziFBIncomingRequest* message = (EziFBIncomingRequest*)_fbMessagesList->objectAtIndex(i);
            if (_selectedRequestItems->objectForKey(message->getRequestID()) == NULL)
            {
                anyChanges = true;
                _selectedRequestItems->setObject(message, message->getRequestID());
            }
        }
        
        if (anyChanges)
        {
            _tableView->reloadData();
        }

    }
    else
    {
        _allButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("check_all_unslected"));
        bool needsReload = false;
        if (_selectedRequestItems->count() > 0)
        {
            needsReload = true;
        }
        
        _selectedRequestItems->removeAllObjects();
        
        if (needsReload)
        {
            _tableView->reloadData();    
        }
        
    }
}

void RequestListScene::acceptButtonPressed()
{
    CCLOG("accept button pressed.");
    if (_selectedRequestItems->count() == 0)
    {
        CCMessageBox("Please select an message to use.", "No message selected.");
    }
    else
    {
        int totalRequest = _fbMessagesList->count();
        bool anyChanges = false;
        CCArray* objectToRemove = CCArray::create();
        
        for (int i=0; i<totalRequest; i++)
        {
            EziFBIncomingRequest* message = (EziFBIncomingRequest*)_fbMessagesList->objectAtIndex(i);
            if (_selectedRequestItems->objectForKey(message->getRequestID()) != NULL)
            {
                anyChanges = true;
                objectToRemove->addObject(message);
                EziFBIncomingRequestManager::sharedManager()->consumeItem(message);
            }
        }
        
        if (objectToRemove->count() > 0)
        {
            _fbMessagesList->removeObjectsInArray(objectToRemove);
            EziFBIncomingRequestManager::sharedManager()->clearCompletedRequestList();
            _tableView->reloadData();
        }
    }
}

void RequestListScene::backButtonPressed()
{
    CCScene* pScene = RequestAPIScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFlipAngular::create(1.0f, pScene));
}

void RequestListScene::keyBackClicked()
{
    backButtonPressed();
}

void RequestListScene::useRequest(cocos2d::CCNode *sender)
{
    CCLOG("Use Button Tag = %d", sender->getTag());
}

//-------------------------------------
#pragma mark - Tableview delegates
//-------------------------------------

void RequestListScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("Table cell touched");
    CCSprite* selection = (CCSprite*)cell->getChildByTag(TAG_TICK_ICON);
    EziFBIncomingRequest* selectedRequestItem = (EziFBIncomingRequest*)selection->getUserData();
    
    if (selectedRequestItem == NULL)
    {
        return;
    }
    
    if (_selectedRequestItems->objectForKey(selectedRequestItem->getRequestID()) == NULL)
    {
        selection->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("row_selected"));
        _selectedRequestItems->setObject(selectedRequestItem, selectedRequestItem->getRequestID());
    }
    else
    {
        _allButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("check_all_unslected"));
        _allSelected = false;
        selection->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("row_not_selected"));
        _selectedRequestItems->removeObjectForKey(selectedRequestItem->getRequestID());
    }
    
    
}


CCSize RequestListScene::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f), SCALED_VALUE(124.0f));
}

CCTableViewCell* RequestListScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    int index = idx;
    EziFBIncomingRequest* messageRequest = (EziFBIncomingRequest*)_fbMessagesList->objectAtIndex(index);
    EziFacebookFriend* myFriend = messageRequest->getSender();
    
    std::string friendName = myFriend->getName();
    std::string messageTitle = this->getFancyTitle(messageRequest);
    std::string messageToDisplay = this->getFancyMessage(messageRequest);
    
    const char* photoPath = myFriend->getPhotoPath();
    bool toRequestForPhoto = false;
    
    if (strcmp(photoPath, "") == 0)
    {
        toRequestForPhoto = true;
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
        
        CCSprite* tickSprite = CCSprite::createWithSpriteFrameName("row_not_selected");
        
        if (_selectedRequestItems->objectForKey(messageRequest->getRequestID()) != NULL)
        {
            tickSprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("row_selected"));
        }
        
        cell->addChild(tickSprite);
        tickSprite->setAnchorPoint(ccp(0.0, 0.5));
        tickSprite->setPositionX(SCALED_VALUE(27.0f));
        tickSprite->setScale(0.5);
        tickSprite->setPositionY(colorLayer->getContentSize().height/2);
        tickSprite->setTag(TAG_TICK_ICON);
        tickSprite->setUserData(messageRequest);
        
        CCLayerColor* photoLayer = CCLayerColor::create(ccc4(255, 255, 255, 255),
                                                        SCALED_VALUE(00.0f),
                                                        SCALED_VALUE(100.0f));
        photoLayer->setPosition(SCALED_VALUE(105.0f), SCALED_VALUE(12.0f));
        cell->addChild(photoLayer);
        CCSprite* userPhotoSprite = NULL;
        
        if (toRequestForPhoto)
        {
            userPhotoSprite = CCSprite::create();
            userPhotoSprite->setContentSize(CCSizeMake(SCALED_VALUE(0.0f), SCALED_VALUE(85.0f)));
        }
        else
        {
            userPhotoSprite = CCSprite::create(myFriend->getPhotoPath());
        }
        
        cell->addChild(userPhotoSprite);
        userPhotoSprite->setAnchorPoint(ccp(0.0f, 0.0));
        userPhotoSprite->cocos2d::CCNode::setPosition(SCALED_VALUE(47.0f), SCALED_VALUE(18.0f));
        userPhotoSprite->setTag(TAG_PHOTO);
        
        // Set the message text.
        CCLabelTTF* message = CCLabelTTF::create(messageToDisplay.c_str(), "Arial", SCALED_VALUE(20.0f));
        message->setAnchorPoint(ccp(0, 1.0));
        message->setHorizontalAlignment(kCCTextAlignmentLeft);
        message->setDimensions(CCSizeMake(colorLayer->getContentSize().width * 0.5, colorLayer->getContentSize().height));
        message->setPosition(ccp(photoLayer->getContentSize().width + photoLayer->getPositionX() + SCALED_VALUE(20.0f),
                                 colorLayer->getContentSize().height/2));
        cell->addChild(message);
        message->setTag(TAG_MESSAGE);
        
        // Set the title text
        CCLOG("Message title = %s", messageTitle.c_str());
        
        CCLabelTTF* titleLabel = CCLabelTTF::create(messageTitle.c_str(), "Arial", SCALED_VALUE(30.0f));
        titleLabel->setAnchorPoint(ccp(0, 0.0));
        titleLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        //titleLabel->setDimensions(CCSizeMake(colorLayer->getContentSize().width * 0.5, colorLayer->getContentSize().height));
        titleLabel->setPosition(ccp(message->getPositionX(),
                                 colorLayer->getContentSize().height/2));
        cell->addChild(titleLabel);
        titleLabel->setTag(TAG_TTTLE);
        
        CCSprite* consumedIcon = CCSprite::createWithSpriteFrameName(getRequetIconFrameName(messageRequest).c_str());
        consumedIcon->setAnchorPoint(ccp(1.0, 0.5));
        cell->addChild(consumedIcon);
        consumedIcon->setPosition(ccp(colorLayer->getContentSize().width - SCALED_VALUE(20.0f),
                                      colorLayer->getContentSize().height/2));
        consumedIcon->setTag(TAG_ITEM_ICON);
        
    }
    else
    {
        CCSprite* cellProfilePic = (CCSprite*)cell->getChildByTag(TAG_PHOTO);
        
        if (toRequestForPhoto == false)
        {
            //cellProfilePic->setTexture(CCSprite::create(myFriend->getPhotoPath())->getTexture());
        }
        
        CCSprite *tick = (CCSprite*)cell->getChildByTag(TAG_TICK_ICON);
        if (tick)
        {
            if (_selectedRequestItems->objectForKey(messageRequest->getRequestID()) != NULL)
            {
                tick->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("row_selected"));
            }
            else
            {
                tick->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("row_not_selected"));
            }

        }
        
        // Update the title Label.
        CCLabelTTF *titleLabel = (CCLabelTTF*)cell->getChildByTag(TAG_TTTLE);
        if (titleLabel)
        {
            titleLabel->setString(messageTitle.c_str());
        }

        // Update the message Label.
        CCLabelTTF *messageLabel = (CCLabelTTF*)cell->getChildByTag(TAG_MESSAGE);
        if (messageLabel)
        {
            messageLabel->setString(messageToDisplay.c_str());
        }
        
        // Update the message Label.
        CCSprite *itemIcon = (CCSprite*)cell->getChildByTag(TAG_ITEM_ICON);
        if (itemIcon)
        {
            itemIcon->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(getRequetIconFrameName(messageRequest).c_str()));
        }

        
    }
    
    return cell;
}

std::string RequestListScene::getRequetIconFrameName(EziFBIncomingRequest *request)
{
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = request->getRequestType();
    std::string frameName = "invite";
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            frameName = "invite";
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            frameName = "gift";
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            frameName = "challenge";
            break;
            
        default:
            frameName = "invite";
            break;
    }
    
    return frameName;
}

std::string RequestListScene::getFancyTitle(EziFBIncomingRequest *request)
{
    // Create the fancy test.
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = request->getRequestType();
    
    std::string messageToDisplay = "";
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            messageToDisplay.append("You got an invitation!");
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            messageToDisplay.append("You got a gift!");
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            messageToDisplay.append("You got a challenge!");
            break;
            
        default:
            messageToDisplay.append("Unknown message");
            break;
    }
    
    return messageToDisplay;
}


std::string RequestListScene::getFancyMessage(EziFBIncomingRequest *request)
{
    // Create the fancy test.
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = request->getRequestType();
    EziFacebookFriend* sender                        = request->getSender();
    const char* senderName = sender->getName();
    
    std::string messageToDisplay = "";
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            messageToDisplay.append(senderName).append(" has invited you to play with him.");
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            messageToDisplay.append(senderName).append(" has gave you a life.");
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            messageToDisplay.append(senderName).append(" has challenged your skills.");
            break;
            
        default:
            messageToDisplay.append("Unknown message");
            break;
    }
    
    return messageToDisplay;
}

unsigned int RequestListScene::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return _fbMessagesList->count();
}

//-------------------------------------
#pragma mark - eziSocial delegates
//-------------------------------------

/*
void RequestListScene::fbFriendsCallback(int responseCode, const char *responseMessage, cocos2d::CCArray *friends)
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
*/

void RequestListScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
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
    if (_downloadCount == _fbMessagesList->count())
    {
        // this is the last we fetched. We can refresh the table view now.
        CCLOG("Download completed...");
        _tableView->reloadData();
    }
    
}