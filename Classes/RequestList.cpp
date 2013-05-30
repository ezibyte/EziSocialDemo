//
//  RequestList.cpp
//  FacebookGameDemo
//
//  Created by Paras Mendiratta on 21/05/13.
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

#include "RequestList.h"
#include "HelloWorldScene.h"
#include "EziSocialObject.h"
#include "EziFacebookFriend.h"
#include "EziFBIncomingRequest.h"
#include "EziFBIncomingRequestManager.h"

static float SCREEN_WIDTH   = 480;
static float SCREEN_HEIGHT  = 320;
static float CELL_HEIGHT    = 320;
static float SCALE_FACTOR   =  1.0f;

static int FRIEND_ID_LABEL_TAG          = 100;
static int REQUEST_MESSAGE_LABEL_TAG    = FRIEND_ID_LABEL_TAG           + 1;
static int REQUEST_ID_LABEL_TAG         = REQUEST_MESSAGE_LABEL_TAG     + 1;
static int FRIEND_PHOTO_TAG             = REQUEST_ID_LABEL_TAG          + 1;
static int FRIEND_CONSUME_MENU_TAG      = FRIEND_PHOTO_TAG              + 1;
static int FRIEND_CONSUME_BUTTON_TAG    = FRIEND_CONSUME_MENU_TAG       + 1;
static int BACKGROUND_TAG               = FRIEND_CONSUME_BUTTON_TAG     + 1;

static const char* FB_DEFAULT_PHOTO     = "fb_user_icon.jpg";

static float MENU_FONT_SCALE     = 1.0f;
static float PHOTO_SCALE = 1.0f;
static bool ALL_DOWNLOAD_COMPLETE = true;

USING_NS_CC;
USING_NS_CC_EXT;

bool RequestList::init()
{
    if ( !CCLayer::init())
    {
        return false;
    }
    
    mPhotoLoadIndex         = 0;
    
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    SCREEN_WIDTH  = winSize.width;
    SCREEN_HEIGHT = winSize.height;
    
    CCLayerColor *backgroundLayer = CCLayerColor::create(ccc4(20, 100, 100, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
    
    this->addChild(backgroundLayer);
    
    
    MENU_FONT_SCALE = SCREEN_HEIGHT/320;
    
    
    // Back Menu
    
    CCMenuItemFont *itemBack = CCMenuItemFont::create("Back", this, menu_selector(RequestList::showHomePage));
    itemBack->setAnchorPoint(ccp(1, 0.5));
	itemBack->setPosition(ccp(SCREEN_WIDTH - 10, (itemBack->getContentSize().height/2 + 2) * MENU_FONT_SCALE));
	
    CCMenuItemFont *purgeItems = CCMenuItemFont::create("Clear Completed Requests",
                                                           this,
                                                           menu_selector(RequestList::clearCompletedRequest));
    purgeItems->setAnchorPoint(ccp(0, 0.5));
    purgeItems->setPosition(ccp(10, (itemBack->getContentSize().height/2 + 2) * MENU_FONT_SCALE));
    
    
    itemBack->setScale(MENU_FONT_SCALE);
    purgeItems->setScale(MENU_FONT_SCALE);
    
    CCMenu *menuBack = CCMenu::create(itemBack, purgeItems, NULL);
	menuBack->setPosition(CCPointZero);
	addChild(menuBack);
    
    float gap = itemBack->getContentSize().height * 2 * MENU_FONT_SCALE + (10 * MENU_FONT_SCALE);
    
    _fbIncomingRequestList = NULL;
    
    refreshList();

    SCALE_FACTOR  = SCREEN_HEIGHT / 768.0f;
    SCALE_FACTOR = MAX(0.5, SCALE_FACTOR);
    
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

void RequestList::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
    std::string messageToDisplay = "";
    
    EziFBIncomingRequest* fbRequest = (EziFBIncomingRequest*)_fbIncomingRequestList->objectAtIndex(cell->getIdx());
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = fbRequest->getRequestType();

    const char* senderName      = fbRequest->getSender()->getName();
    const char* requestTypeChar = "";
    const char* message         = "";
    
    message = fbRequest->getMessage();
    
    CCDictionary* giftDictionary = fbRequest->getDataDictionary();
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            requestTypeChar = "Invite";
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            requestTypeChar = "Gift";
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            requestTypeChar = "Challenge";
            break;
            
        default:
            requestTypeChar = "Unknown";
            break;
    }
    
    // Add the request Type
    messageToDisplay.append("Request Type = ").append(requestTypeChar).append("\n");
    
    // Add the sender Name
    messageToDisplay.append("Sender = ").append(senderName).append("\n");
    
    // Add the message.
    messageToDisplay.append("Message = ").append(message).append("\n");
    
    if (giftDictionary && giftDictionary->count()>0)
    {
        CCArray* allKeys = giftDictionary->allKeys();
        if (allKeys && allKeys->count() > 0)
        {
            messageToDisplay.append("Extra/Gift items sent:\n");
            for (int i=0; i<allKeys->count(); i++)
            {
                CCString* key   = (CCString*)allKeys->objectAtIndex(i);
                CCString* value = (CCString*)giftDictionary->objectForKey(key->getCString());
                messageToDisplay.append(key->getCString()).append(": ").append(value->getCString()).append("\n");
            }
        }
        
    }
    else
    {
        messageToDisplay.append("No Extra Items were sent by sender");
    }
    
    CCMessageBox(messageToDisplay.c_str(), "Request Details");
}

CCSize RequestList::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(SCREEN_WIDTH, CELL_HEIGHT + 2);
}

CCTableViewCell* RequestList::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    int index = idx;    
    EziFBIncomingRequest* fbRequest = (EziFBIncomingRequest*)_fbIncomingRequestList->objectAtIndex(index);
    EziFacebookFriend* sender       = fbRequest->getSender();
    
    const char* senderName = sender->getName();
    
    // Create the fancy test.
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = fbRequest->getRequestType();
    std::string messageToDisplay = "";
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            messageToDisplay.append(senderName).append(" has sent invitation to you.");
            break;

        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            messageToDisplay.append(senderName).append(" has sent gift to you.");
            break;

        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            messageToDisplay.append(senderName).append(" has challenged to you.");
            break;
            
        default:
            messageToDisplay.append("Unknown message");
            break;
    }
    
    // Means users have already used this request.
    bool requestConsumed = fbRequest->isConsumed();
    
    CCSprite* profilePic            = NULL;
    ccColor4B bgColor = ccc4(20, 0, 40, 255);
    
    if (requestConsumed == false)
    {
        bgColor = ccc4(50, 50, 20, 255);
    }

    
    // Build the table cell.
    CCTableViewCell *cell = table->dequeueCell();
    
    if (cell == NULL)
    {
        cell = new CCTableViewCell();
        cell->autorelease();

                
        // Create the back layer of the cell.
        CCLayerColor* colorLayer = CCLayerColor::create(bgColor, SCREEN_WIDTH, CELL_HEIGHT);
        colorLayer->setTag(BACKGROUND_TAG);
        
        cell->addChild(colorLayer);
        
        // Get the sender profile picture path. Create it if path is available.
        if (sender != NULL && strcmp(sender->getPhotoPath(), "") != 0)
        {
            const char*  picPath = sender->getPhotoPath();
            profilePic = CCSprite::create(picPath);
        }
        else
        {
            profilePic = CCSprite::create(FB_DEFAULT_PHOTO);
        }

        // Add the profile pic to the cell row.
        profilePic->setAnchorPoint(ccp(0, 0.5));
        profilePic->setPosition(ccp(20, CELL_HEIGHT/2));
        cell->addChild(profilePic);
        profilePic->setTag(FRIEND_PHOTO_TAG);
        
        // Set the message.
        CCLabelTTF *messageLabel = CCLabelTTF::create(messageToDisplay.c_str(), "Helvetica", 20.0 * MENU_FONT_SCALE);
        messageLabel->setAnchorPoint(ccp(0, 0.5));
        messageLabel->setPosition(ccp(20 + 20 + profilePic->getContentSize().width, CELL_HEIGHT/2));
        messageLabel->setTag(REQUEST_MESSAGE_LABEL_TAG);
        messageLabel->setDimensions(CCSizeMake(SCREEN_WIDTH - (profilePic->getContentSize().width * 2) - 60, CELL_HEIGHT-5));
        messageLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        cell->addChild(messageLabel);

        CCMenuItemImage* constumeItemButton = NULL;
        constumeItemButton = CCMenuItemImage::create("use.png", "use_pressed.png");
        

        constumeItemButton->setTag(idx);
        constumeItemButton->setTarget(this, menu_selector(RequestList::useItem));
        constumeItemButton->setScale(SCALE_FACTOR * 0.8);
        constumeItemButton->setTag(FRIEND_CONSUME_BUTTON_TAG);
        
        CCMenu* consumeMenu = CCMenu::create(constumeItemButton, NULL);
        consumeMenu->setTag(FRIEND_CONSUME_MENU_TAG);
        consumeMenu->setPosition(ccp(SCREEN_WIDTH - (constumeItemButton->getContentSize().width*SCALE_FACTOR*0.4),
                                       (CELL_HEIGHT - (constumeItemButton->getContentSize().height)*SCALE_FACTOR*0.4)));
        
        
        cell->addChild(consumeMenu);
        
        
    }
    else
    {
        // Refresh the cell data based upon idx.
        
        // 1. Refresh the profile picture.
        CCSprite* cellProfilePic = (CCSprite*)cell->getChildByTag(FRIEND_PHOTO_TAG);
        
        if (strcmp("", sender->getPhotoPath()) != 0 )
        {
            const char*  picPath = sender->getPhotoPath();
            profilePic = CCSprite::create(picPath);
            
        }
        else
        {
            profilePic = CCSprite::create(FB_DEFAULT_PHOTO);
        }
        
        cellProfilePic->setTexture(profilePic->getTexture());
        
        // Update the message Label.
        CCLabelTTF *messageLabel = (CCLabelTTF*)cell->getChildByTag(REQUEST_MESSAGE_LABEL_TAG);
        messageLabel->setString(messageToDisplay.c_str());
        
        CCMenu* consumeMenu = (CCMenu*)cell->getChildByTag(FRIEND_CONSUME_MENU_TAG);
        
        CCArray* children = consumeMenu->getChildren();
        
        if (children)
        {
            CCMenuItemImage* consumenButton = (CCMenuItemImage*)children->objectAtIndex(0);
            
            if (consumenButton)
            {
                consumenButton->setTag(index);
            }
            else
            {
                CCLOG("No Consume Button");
            }
            
        }
        
        CCLayerColor* bgLayer = (CCLayerColor*)cell->getChildByTag(BACKGROUND_TAG);
        bgLayer->setColor(ccc3(bgColor.r, bgColor.g, bgColor.b));
    }
    
    return cell;
    
}

unsigned int RequestList::numberOfCellsInTableView(CCTableView *table)
{
    int count = _fbIncomingRequestList->count();
    return count;
}

// ---------------------------------------------
#pragma mark - Menu Callback Methods
// ---------------------------------------------


void RequestList::showHomePage()
{
    this->unscheduleUpdate();
    CCScene *pScene = HelloWorld::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void RequestList::clearCompletedRequest()
{
    EziFBIncomingRequestManager::sharedManager()->clearCompletedRequestList();
    // To do.. Need to remove the list of items that are completed.
    //mTableView->reloadData();
    
    //_fbIncomingRequestList = NULL;
    CCScene *pScene = CCScene::create();
	RequestList *pLayer = RequestList::create();
	pScene->addChild(pLayer);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

// ---------------------------------------------
    #pragma mark - Helper Private Methods
// ---------------------------------------------

void RequestList::downloadNextPhoto()
{
    if (mPhotoLoadIndex > 0)
    {
        mPhotoLoadIndex--;
        int index = mPhotoLoadIndex;
        EziFBIncomingRequest* incomingRequest = NULL;
        EziFacebookFriend *friendObject = NULL;
        
        incomingRequest = (EziFBIncomingRequest*)_fbIncomingRequestList->objectAtIndex(index);
                
        friendObject = incomingRequest->getSender();
        
        unsigned int size = 200;
        
        if (PHOTO_SCALE <= 0.5 && PHOTO_SCALE > 0.25)
        {
            size = 100;
        }
        else if (PHOTO_SCALE < 0.25)
        {
            size = 50;
        }
        
        EziSocialObject::sharedObject()->getProfilePicForID(friendObject->getFBID(),
                                                            size, size, false);
    }
    else
    {
        this->unscheduleUpdate();
        ALL_DOWNLOAD_COMPLETE = true;
        this->hideLoadingAction();
    }
}

void RequestList::showLoadingAction()
{
    mLoadingImage->setVisible(true);
    CCRotateBy* rotateAction = CCRotateBy::create(1.0f, 360);
    CCRepeatForever* repeatAction = CCRepeatForever::create(rotateAction);
    repeatAction->retain();
    
    mLoadingImage->runAction(repeatAction);
    
}

void RequestList::hideLoadingAction()
{
    mLoadingImage->stopAllActions();
    mLoadingImage->setVisible(false);
}

void RequestList::useItem(CCObject* pSender)
{
    if (pSender == NULL)
    {
        return;
    }
    
    CCMenuItemImage* sender = (CCMenuItemImage*)pSender;
    CCLOG("Call for useItem for tag = %d", sender->getTag());
    
    if (sender->getTag() > _fbIncomingRequestList->count())
    {
        return;
    }
    
    std::string messageToDisplay = "";
    
    EziFBIncomingRequest* fbRequest = (EziFBIncomingRequest*)_fbIncomingRequestList->objectAtIndex(sender->getTag());
    
    if (fbRequest->isConsumed())
    {
        CCMessageBox("This item is already consumed.", "Consume Item Status");
        return;
    }
    
    
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = fbRequest->getRequestType();
    
    const char* senderName      = fbRequest->getSender()->getName();
    const char* requestTypeChar = "";
    const char* message         = "";
    
    message = fbRequest->getMessage();
    
    CCDictionary* giftDictionary = fbRequest->getDataDictionary();
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            requestTypeChar = "Invite";
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            requestTypeChar = "Gift";
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            requestTypeChar = "Challenge";
            break;
            
        default:
            requestTypeChar = "Unknown";
            break;
    }
    
    // Add the request Type
    messageToDisplay.append("Request Type = ").append(requestTypeChar).append("\n");
    
    // Add the sender Name
    messageToDisplay.append("Sender = ").append(senderName).append("\n");
    
    // Add the message.
    messageToDisplay.append("Message = ").append(message).append("\n");
    
    if (giftDictionary && giftDictionary->count()>0)
    {
        CCArray* allKeys = giftDictionary->allKeys();
        if (allKeys && allKeys->count() > 0)
        {
            messageToDisplay.append("Extra/Gift items sent:\n");
            for (int i=0; i<allKeys->count(); i++)
            {
                CCString* key   = (CCString*)allKeys->objectAtIndex(i);
                CCString* value = (CCString*)giftDictionary->objectForKey(key->getCString());
                messageToDisplay.append(key->getCString()).append(": ").append(value->getCString()).append("\n");
            }
        }
        
    }
    else
    {
        messageToDisplay.append("No Extra Items were sent by sender");
    }
    
    CCMessageBox(messageToDisplay.c_str(), "Item Consumed!!! :)))");
    
    EziFBIncomingRequestManager::sharedManager()->consumeItem(fbRequest);
    
    CCScene *pScene = CCScene::create();
	RequestList *pLayer = RequestList::create();
	pScene->addChild(pLayer);
	CCDirector::sharedDirector()->replaceScene(pScene);
    
}

// ---------------------------------------------
#pragma mark - CCLayer Delegate Methods
// ---------------------------------------------

void RequestList::update(float delta)
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

void RequestList::fbRecieveRequestCallback(int responseCode,
                                      const char* message,
                                      const char* senderName, cocos2d::CCDictionary* dataDictionary)
{
    CCMessageBox("Got new request. Refresh List", "New Incoming Request");
    this->refreshList();

}

void RequestList::fbIncomingRequestCallback(int totalIncomingRequests)
{
    CCLOG("Got new requests = %d", totalIncomingRequests);
}

void RequestList::refreshList()
{
    this->unscheduleUpdate();
    
    if (_fbIncomingRequestList != NULL)
    {
        _fbIncomingRequestList->release();
    }
    
    _fbIncomingRequestList = NULL;
    _fbIncomingRequestList = CCArray::create();
    _fbIncomingRequestList->retain();
    
    CCDictionary* completedDictionary = EziFBIncomingRequestManager::sharedManager()->getCompletedRequest();
    
    if (completedDictionary)
    {
        CCArray* completedKeys = completedDictionary->allKeys();
        
        if (completedKeys)
        {
            for (int i=0; i<completedKeys->count(); i++)
            {
                CCString *cKey = (CCString*)completedKeys->objectAtIndex(i);
                _fbIncomingRequestList->addObject(completedDictionary->objectForKey(cKey->getCString()));
            }
        }
    }
    
    CCDictionary* pendingDictionary = EziFBIncomingRequestManager::sharedManager()->getPendingRequests();
    
    if (pendingDictionary)
    {
        CCArray* pendingKeys = pendingDictionary->allKeys();
        
        if (pendingKeys)
        {
            for (int i=0; i<pendingKeys->count(); i++)
            {
                CCString *pKey = (CCString*)pendingKeys->objectAtIndex(i);
                _fbIncomingRequestList->addObject(pendingDictionary->objectForKey(pKey->getCString()));
            }
        }
    }
    
    
    
    
    mPhotoLoadIndex = this->numberOfCellsInTableView(mTableView);
    mReadyForNextDownload = true;
    this->scheduleUpdate();
}

void RequestList::fbUserPhotoCallback(const char *userPhotoPath)
{
    int index = mPhotoLoadIndex;
    EziFBIncomingRequest* incomingRequest = NULL;
    EziFacebookFriend *friendObject = NULL;
    
    incomingRequest = (EziFBIncomingRequest*)_fbIncomingRequestList->objectAtIndex(index);
        
    friendObject = incomingRequest->getSender();
    friendObject->setPhotoPath(userPhotoPath);
    
    mReadyForNextDownload = true;
}