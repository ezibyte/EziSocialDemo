//
//  BaseScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "BaseScene.h"
#include "AppDelegate.h"
#include "GameConstants.h"
#include "HelpScene.h"
#include "EziFacebookFriend.h"
#include "EziFBIncomingRequestManager.h"

void BaseScene::onExit()
{
    CCLayer::onExit();
    EziSocialObject::sharedObject()->setFacebookDelegate(NULL);
}

void BaseScene::buildScene()
{
    // Add the background.
    
    this->setKeypadEnabled(true);
    
    CCLayerColor* bgColorLayer = CCLayerColor::create(ccc4(41, 69, 66, 255), AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT);
    this->addChild(bgColorLayer);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_BG));
    
    CCSprite* backgroundSprite = CCSprite::createWithSpriteFrameName("background");
    this->addChild(backgroundSprite);
    PlaceNode(backgroundSprite, 0.5, 0.5)
 
    CCSpriteBatchNode* commonBatchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_COMMON));
    this->addChild(commonBatchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_COMMON));
    
    
    // Create tile menu
    BaseScene::buildTileMenuSkelton();
    this->addChild(_tileMenu);
    
    // Create Navigation Buttons
    this->addNavigationButtons();
    
    if (AppDelegate::SCREEN_HEIGHT > 640.0f)
    {
        //EziAdMobObject::sharedObject()->loadBannerAd();
    }
    
}

CCMenuItemImage* BaseScene::buildAndPlaceTile(const char *normalFrameName, const char *selectedFrameName, int tilePosX, int tilePosY)
{
    CCMenuItemImage* menuButton = CCMenuItemImage::create();
    menuButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(normalFrameName));
    menuButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selectedFrameName));
    menuButton->setDisabledSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selectedFrameName));
    //menuButton->setTarget(this, menu_selector(HomeScene::getZOrder));
    menuButton->setAnchorPoint(ccp(0.0, 1.0));
    
    _tileMenu->addChild(menuButton);
    
    
    float onePosX = AppDelegate::SCREEN_WIDTH  * 0.348;
    float onePosY = AppDelegate::SCREEN_HEIGHT * 0.764;
    float tileHeight = menuButton->getContentSize().height;
    float gap     = 7 * AppDelegate::SCREEN_SCALE_FACTOR;
    
    float desiredPosX = onePosX + ((gap + tileHeight) * tilePosX);
    float desiredPosY = onePosY - ((gap + tileHeight) * tilePosY);
    
    menuButton->setPosition(desiredPosX, desiredPosY);

    return menuButton;
}

void BaseScene::buildHeader(int highlightIndex, const char* title, const char* description)
{
    _currentMenuIndex = highlightIndex;
    
    CCMenuItemImage* homeButton = CCMenuItemImage::create();
    homeButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_home_nrl"));
    homeButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_home_prd"));
    homeButton->setDisabledSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_home_prd"));
    homeButton->setTarget(this, menu_selector(BaseScene::loadHomeScene));
    homeButton->setAnchorPoint(ccp(1.0f, 1.0f));
    
    CCMenuItemImage* fbActionButton = CCMenuItemImage::create();
    fbActionButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_fb_action_nrl"));
    fbActionButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_fb_action_prd"));
    fbActionButton->setDisabledSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_fb_action_dis"));
    fbActionButton->setTarget(this, menu_selector(BaseScene::loadFBActionScene));
    fbActionButton->setAnchorPoint(ccp(1.0f, 1.0f));
    
    if (EziSocialObject::sharedObject()->isFacebookSessionActive() == false)
    {
        fbActionButton->setEnabled(false);
    }
    
    CCMenuItemImage* helpButton = CCMenuItemImage::create();
    helpButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_help_nrl"));
    helpButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_help_prd"));
    helpButton->setDisabledSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_help_prd"));
    helpButton->setTarget(this, menu_selector(BaseScene::loadHelpScene));
    
    helpButton->setAnchorPoint(ccp(1.0f, 1.0f));
    
    switch (highlightIndex)
    {
        case 0:
            homeButton->setEnabled(false);
            break;
            
        case 1:
            fbActionButton->setEnabled(false);
            break;
            
        case 2:
            helpButton->setEnabled(false);
            break;
            
        default:
            break;
    }
    
    
    CCMenu* homeMenu = CCMenu::create(homeButton, fbActionButton, helpButton, NULL);
    homeMenu->alignItemsHorizontally();
    
    this->addChild(homeMenu);
    PlaceNode(homeMenu, 0.88, 0.98);
    
    // Add a line.
    CCLayerColor* lineLayer = CCLayerColor::create(ccc4(204, 204, 204, 204),
                                                   CCDirector::sharedDirector()->getVisibleSize().width,
                                                   1.0 * AppDelegate::SCREEN_SCALE_FACTOR);
    
    this->addChild(lineLayer);
    PlaceNode(lineLayer, 0.0, 0.8);
    lineLayer->setPositionY(homeMenu->getPositionY() - homeButton->getContentSize().height - (7 * AppDelegate::SCREEN_SCALE_FACTOR));
    
    // Add Title
    CCLabelTTF* titleLabel = CCLabelTTF::create(title, "Arial", 50.0 * AppDelegate::SCREEN_SCALE_FACTOR);
    this->addChild(titleLabel);
    titleLabel->setAnchorPoint(ccp(0.0f, 0.0f));
    //PlaceNode(titleLabel, 0.02, 0.98);
    
    CCLabelTTF* descriptionLabel = CCLabelTTF::create(description,
                                                      "Arial", SCALED_VALUE(22.0f));
    this->addChild(descriptionLabel);
    descriptionLabel->setAnchorPoint(ccp(0.0f, 0.0f));
    PlaceNode(descriptionLabel, 0.02, 0.98);
    descriptionLabel->setPositionY(homeMenu->getPositionY() - homeButton->getContentSize().height);
    descriptionLabel->setColor(ccc3(204, 204, 204));
    
    titleLabel->cocos2d::CCNode::setPosition(descriptionLabel->getPositionX(),
                                             descriptionLabel->getPositionY() + descriptionLabel->getContentSize().height - 2.0f);
    
}

void BaseScene::addTipBoard(const char* tip)
{
    CCSprite* tipBoard = CCSprite::createWithSpriteFrameName("tip_board");
    tipBoard->setAnchorPoint(ccp(0.0f, 1.0f));
    
    //tipBoard->cocos2d::CCNode::setPosition(SCALED_VALUE(12.0f), SCALED_VALUE(368.0f));
    PlaceNode(tipBoard, 0.02, 0.764);
    this->addChild(tipBoard);
    
    CCLabelTTF* tipLabel = CCLabelTTF::create(tip, "Arial", SCALED_VALUE(20.0f));
    tipLabel->setColor(ccc3(33, 33, 33));
    tipLabel->setAnchorPoint(ccp(0.5f, 1.0f));
    tipLabel->setDimensions(CCSizeMake(tipBoard->getContentSize().width * 0.8, tipBoard->getContentSize().height * 0.8));
    
    float py = 0.0f;
    py = tipBoard->getPositionY();
    py = py - SCALED_VALUE(127.0f);
    tipLabel->setPositionY(py);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    tipBoard->setScaleY(1.20f);
    
    if (AppDelegate::SCREEN_HEIGHT > 400.0f)
    {
        tipLabel->setPositionY(py + SCALED_VALUE(10.0f));
    }
    else
    {
        tipLabel->setPositionY(py - SCALED_VALUE(5.0f));
    }
    
    
#else
    //PlaceNode(tipLabel, 0.04, 0.55);
#endif
    
    //tipLabel->setPositionY(py);
    
    float pX = tipBoard->getPositionX() + tipBoard->getContentSize().width/2;
    tipLabel->setPositionX(pX);
    
    this->addChild(tipLabel);
}

void BaseScene::buildTileMenuSkelton()
{
    _tileMenu = NULL;
    _tileMenu = CCMenu::create();
    _tileMenu->setContentSize(CCSizeMake(AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT));
    PlaceNode(_tileMenu, 0.0, 0.0);
}

void BaseScene::loadFBActionScene()
{
    CCScene *pScene = FBActionScene::scene();
    
    if (_currentMenuIndex == 0)
    {
        ReplaceToRight(pScene);
    }
    else if (_currentMenuIndex == -1)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFadeTR::create(1.0f, pScene));
    }
    else
    {
        ReplaceToLeft(pScene);
    }
    
}

void BaseScene::addNavigationButtons()
{
    /*
    _backButton = CCMenuItemImage::create();
    _backButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_back_nrl"));
    _backButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_back_prd"));
    _backButton->setTarget(this, menu_selector(BaseScene::loadHomeScene));
    _backButton->setAnchorPoint(ccp(0.0f, 1.0f));
    
    _forwardButton = CCMenuItemImage::create();
    _forwardButton->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_forward_nrl"));
    _forwardButton->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_forward_prd"));
    _forwardButton->setTarget(this, menu_selector(BaseScene::loadFBActionScene));
    _forwardButton->setAnchorPoint(ccp(0.0f, 1.0f));
    
    _navigationMenu = CCMenu::create(_backButton, _forwardButton, NULL);
    _navigationMenu->alignItemsHorizontallyWithPadding(20 * AppDelegate::SCREEN_SCALE_FACTOR);
    PlaceNode(_navigationMenu, 0.11, 0.764);
    this->addChild(_navigationMenu, 5);
    
    hideNavigationBackButton(true);
    */
}

void BaseScene::hideNavigationMenu(bool toHide)
{
    if (_navigationMenu != NULL)
    {
        _forwardButton->setVisible(!toHide);
        _backButton->setVisible(!toHide);
    }
}

void BaseScene::hideNavigationForwardButton(bool toHide)
{
    if (_forwardButton != NULL)
    {
        _forwardButton->setVisible(!toHide);
    }
}

void BaseScene::hideNavigationBackButton(bool toHide)
{
    if (_backButton != NULL)
    {
        _backButton->setVisible(!toHide);
    }
}

void BaseScene::forwardKeyAction()
{
    
}

void BaseScene::backKeyAction()
{
    
}

void BaseScene::keyBackClicked()
{
    CCDirector::sharedDirector()->end();
}

void BaseScene::loadHomeScene()
{
    CCScene *pScene = HomeScene::scene();
    //EziSocialObject::sharedObject()->perfromLogoutFromFacebook();
    ReplaceToLeft(pScene);
}

void BaseScene::loadHelpScene()
{
    CCScene *pScene = HelpScene::scene();
    ReplaceToRight(pScene);
}

//---------------------------------------------
    #pragma mark - EziSocial Delegate Methods
//---------------------------------------------

void BaseScene::fbSessionCallback(int responseCode, const char* responseMessage)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL:
        {
            CCLOG("Login successful %d", CCUserDefault::sharedUserDefault()->getIntegerForKey("FRIENDS_COUNT", 0));
            if (CCUserDefault::sharedUserDefault()->getIntegerForKey("FRIENDS_COUNT", 0) <= 0)
            {
                EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, 0, 40);
            }
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_USER_PERMISSION_REVOKED:
        {
            CCLOG("Login Failed due to %s", responseMessage);
            CCMessageBox(responseMessage, "Login Failed");
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGOUT_SUCCESSFUL:
        {
            CCLOG("Logout successful...");
            break;
        }
            
        default:
            CCMessageBox(responseMessage, "Login failed");
            break;
    }
}

void BaseScene::mailCallback(int responseCode)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_ERROR:
            CCMessageBox("There is error while sending the email", "Email Delegate Response ");
            break;

        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_SUCCESS:
            CCMessageBox("Email sent sucessfully", "Email Delegate Response ");
            break;
        
        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_CANCLLED:
            CCMessageBox("You have cancelled email.", "Email Delegate Response ");
            break;

        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_STORED:
            CCMessageBox("Your email is stored in draft.", "Email Delegate Response ");
            break;

        default:
            CCMessageBox("Unknown message code in email response. Please report back at support@ezibyte.com", "[Email]: Unknown Response.");
            break;
    }
}

void BaseScene::twitterCallback(int responseCode)
{   
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::TWIT_SEND:
            CCMessageBox("Tweeted! :)", "Tweet Delegate Response ");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::TWIT_CANCEL:
            CCMessageBox("Sorry! You have cancelled the tweet.", "Tweet Delegate Response ");
            break;
                        
        default:
            CCMessageBox("Unknown message code in email response. Please report back at support@ezibyte.com", "[Email]: Unknown Response.");
            break;

    }
}

void BaseScene::fbFriendsCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friends)
{
    if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_FRIEND_GET_SUCCESS)
    {
        for (int i=0; i<friends->count(); i++)
        {
            EziFacebookFriend* myFriend = (EziFacebookFriend*)(friends->objectAtIndex(i));
            CCString* friendKey = CCString::createWithFormat("Friend_ID_%d", (i+1), NULL);
            CCUserDefault::sharedUserDefault()->setStringForKey(friendKey->getCString(), myFriend->getFBID());
        }
        
        if (friends->count() > 0)
        {
            CCLOG("[EziSocial Log]: %d friends saved in DB for future use.", friends->count());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("FRIENDS_COUNT", friends->count());
            
            // Save these friends for future use ;)
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
}

void BaseScene::fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests)
{
    if (totalIncomingRequests > 0)
    {
        int pendingRequest = EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount();
        CCString* message = CCString::createWithFormat("Total new requests = %d\n Total pending requests = %d.\nGo to Request API Scene to see the list of incoming requets.", totalIncomingRequests, pendingRequest);
        CCMessageBox(message->getCString(), "fbIncomingRequestCallback");
    }
    else
    {
        CCLOG("No new incoming request...");
    }
}

CCArray* BaseScene::getRandomFriends(int howMany)
{
    CCArray* friendArray = CCArray::createWithCapacity(howMany);
    
    int storedFriends = CCUserDefault::sharedUserDefault()->getIntegerForKey("FRIENDS_COUNT");
    
    if (storedFriends > howMany)
    {
        for (int i=0; i<howMany; i++)
        {
            int randID = rand() % storedFriends;
            
            CCString* friendKey = CCString::createWithFormat("Friend_ID_%d", randID, NULL);
            std::string friendID = CCUserDefault::sharedUserDefault()->getStringForKey(friendKey->getCString());
            
            CCString* friendIDString = CCString::create(friendID);
            friendArray->addObject(friendIDString);
        }
        
        return friendArray;
    }
    else
    {
        return NULL;
    }
    
}

