#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "EziSocialObject.h"
#include "EziSocialDefinition.h"

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
    
    
    
    this->prepareScene();
    
    // Set Delegates
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    EziSocialObject::sharedObject()->setEmailDelegate(this);
    
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
    
    _facebookActionLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    CCMenuItemImage *fbPostMessageButton = CCMenuItemImage::create("FB_Post_Message_Button.png",
                                                                   "FB_Post_Message_Button_Pressed.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::postMessageOnFacebookWall));
    
    CCMenuItemImage *fbAutoPostMessageButton = CCMenuItemImage::create("FB_Auto_Post_Message_Button.png",
                                                                       "FB_Auto_Post_Message_Button_Pressed.png",
                                                                       this,
                                                                       menu_selector(HelloWorld::autoPostMessageOnFacebookWall));
    
    CCMenuItemImage *fbFriendsListButton = CCMenuItemImage::create("FB_Friends_Button.png",
                                                                   "FB_Friends_Button_Pressed.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::getListOfFriendsUsingThisApp));
    
    
    CCMenuItemImage *fbPageLikeButton = CCMenuItemImage::create("FB_Check_Page_Like_Button.png",
                                                                "FB_Check_Page_Like_Button_Pressed.png",
                                                                this,
                                                                menu_selector(HelloWorld::checkIfUserLikeMyFacebookPage));
    
    CCMenuItemImage *fbUserDetailsButton = CCMenuItemImage::create("FB_User_Details_Button.png",
                                                                   "FB_User_Details_Button_Pressed.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::fetchFBUserDetails));
    
    CCMenuItemImage *fbPostScoreButton = CCMenuItemImage::create("FB_Post_Score_Button.png",
                                                                 "FB_Post_Score_Button_Pressed.png",
                                                                 this,
                                                                 menu_selector(HelloWorld::postScore));
    
    
    CCMenuItemImage *fbHighScoreButton = CCMenuItemImage::create("FB_High_Scores_Button.png",
                                                                 "FB_High_Scores_Button_Pressed.png",
                                                                 this,
                                                                 menu_selector(HelloWorld::getHighScores));
    
    
    CCMenuItemImage *fbPageButton = CCMenuItemImage::create("FB_Like_My_Page_Button.png",
                                                            "FB_Like_My_Page_Button_Pressed.png",
                                                            this,
                                                            menu_selector(HelloWorld::openFacebookPage));
    
    
    CCMenu* fbMainActionMenu = CCMenu::create(fbUserDetailsButton,
                                              fbPageButton,
                                              fbPageLikeButton,
                                              fbFriendsListButton,
                                              NULL);
    
    
    CCMenu* fbMainActionMenu2 = CCMenu::create(fbPostMessageButton,
                                               fbAutoPostMessageButton,
                                               fbPostScoreButton,
                                               fbHighScoreButton,
                                               NULL);
    
    
    fbPostMessageButton->setScale(SCALE_FACTOR);
    fbAutoPostMessageButton->setScale(SCALE_FACTOR);
    fbFriendsListButton->setScale(SCALE_FACTOR);
    fbPageLikeButton->setScale(SCALE_FACTOR);
    fbUserDetailsButton->setScale(SCALE_FACTOR);
    fbHighScoreButton->setScale(SCALE_FACTOR);
    fbPostScoreButton->setScale(SCALE_FACTOR);
    fbPageButton->setScale(SCALE_FACTOR);
    
    fbMainActionMenu->alignItemsVerticallyWithPadding(GAP);
    fbMainActionMenu2->alignItemsVerticallyWithPadding(GAP);
    
    fbMainActionMenu->setPosition(ccp(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT/2));
    fbMainActionMenu2->setPosition(ccp(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT/2));
    
    _facebookActionLayer->addChild(fbMainActionMenu);
    _facebookActionLayer->addChild(fbMainActionMenu2);
    
    _facebookActionLayer->setPositionY(-SCREEN_HEIGHT);
    
    this->addChild(_facebookActionLayer);
    
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

// ---------------------------------------------------------
#pragma mark - Facebook Action Methods
// ---------------------------------------------------------

void HelloWorld::loginViaFacebook()
{
    EziSocialObject::sharedObject()->performLoginUsingFacebook();
}

void HelloWorld::logoutFromFacebook()
{
    EziSocialObject::sharedObject()->performLogoutFromFacebook();
}

void HelloWorld::postMessageOnFacebookWall()
{
    EziSocialObject::sharedObject()->postMessageOnWall("My Action Game",
                                                       "With Any Caption",
                                                       "I just scored 393,384 points on My Action Game for iOS",
                                                       "http://db.tt/ZhlVyMtp",
                                                       "https://itunes.apple.com/app/storm-the-train/id553852137?mt=8");
}

void HelloWorld::autoPostMessageOnFacebookWall()
{
    EziSocialObject::sharedObject()->autoPostMessageOnWall("My Action Game - Auto Post Test",
                                                           "With Any Caption",
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
    EziSocialObject::sharedObject()->getListOfFriendsUsingFBApp();
}

void HelloWorld::postScore()
{
    EziSocialObject::sharedObject()->postScore(rand()%1000000);
}

void HelloWorld::getHighScores()
{
    EziSocialObject::sharedObject()->getHighScores();
}

void HelloWorld::fetchFBUserDetails()
{
    EziSocialObject::sharedObject()->fetchFBUserDetails();
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
#pragma mark - Twitter Action Methods
// ---------------------------------------------------------


void HelloWorld::loginViaTwitter()
{
    EziSocialObject::sharedObject()->tweet("Twitter Test Message", "");
    //CCMessageBox("Menu Action: Login Via Twitter pressed", "Login - Twitter");
}

// ---------------------------------------------------------
#pragma mark - Email Action Methods
// ---------------------------------------------------------

void HelloWorld::sendEmail()
{
    EziSocialObject::sharedObject()->sendEmail("Email Subject Line", "<HTML><BODY><B>Title of the mail</B><BR>This is a test email</BODY></HTML>", "testUserOne@gmail.com;testUserTwo@gmail.com");
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


void HelloWorld::fbSessionCallback(int responseCode)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_NO_TOKEN:
        {
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL:
        {
            this->showFacebookActionPage();
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_FAILED:
        {
            CCLOG("FB_LOGIN_FAILED or User cancelled Login.");
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

void HelloWorld::fbUserDetailCallback(CCDictionary* data)
{
    if (data)
    {
        mUsername->setString(((CCString*)(data->objectForKey(KEY_FB_USER_NAME)))->getCString());
        mGender->setString(((CCString*)(data->objectForKey(KEY_FB_USER_GENDER)))->getCString());
        mHometown->setString(((CCString*)(data->objectForKey(KEY_FB_USER_HOMETOWN)))->getCString());
        mPresentLocation->setString(((CCString*)(data->objectForKey(KEY_FB_USER_PRESENT_LOCATION)))->getCString());
        mProfileID->setString(((CCString*)(data->objectForKey(KEY_FB_USER_PROFILE_ID)))->getCString());
        mFirstName->setString(((CCString*)(data->objectForKey(KEY_FB_USER_FIRST_NAME)))->getCString());
        mLastName->setString(((CCString*)(data->objectForKey(KEY_FB_USER_LAST_NAME)))->getCString());
        mAccessToken->setString(((CCString*)(data->objectForKey(KEY_FB_USER_ACCESS_TOKEN)))->getCString());
        
        CCLOG("Access Token = %s", ((CCString*)(data->objectForKey(KEY_FB_USER_ACCESS_TOKEN)))->getCString());
        
        CCString* name = CCString::createWithFormat("%s %s", mFirstName->getString(), mLastName->getString(), NULL);
        mName->setString(name->getCString());
        
        this->showUserDetailPage();
    }
    else
    {
        CCMessageBox("Sorry, user details not available", "fbUserDetailCallback");
    }
}

void HelloWorld::fbMessageCallback(int responseCode)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_MESSAGE_PUBLISHED:
        {
            CCMessageBox("Message posted on user wall successfully.", "Post Messgae on Wall");
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_MESSAGE_CANCELLLED:
        {
            CCMessageBox("User cancelled the message.", "Post Messgae on Wall");
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_MESSAGE_PUBLISHING_ERROR:
        {
            CCMessageBox("Got error while publishing the message.", "Post Messgae on Wall");
            break;
        }
        default:
            break;
    }
}

void HelloWorld::fbChallengeCallback(int responseCode)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_CHALLEGE_CANCELLED:
        {
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_CHALLEGE_PUBLISHED:
        {
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_CHALLENGE_SENDING_ERROR:
        {
            break;
        }
        default:
            break;
    }
}

void HelloWorld::fbGiftCallback(int responseCode)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_GIFT_SENDING_CANCELLED:
        {
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_GIFT_SENDING_ERROR:
        {
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_GIFT_SENT:
        {
            break;
        }
        default:
            break;
    }
}

void HelloWorld::fbPageLikeCallback(int responseCode)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_PAGELIKE_ERROR:
        {
            CCMessageBox("Got error while checking page like.", "Error - Page Like");
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_PAGELIKE_NEGATIVE:
        {
            CCMessageBox("Ah! He don't like my page", "PageLike - Negative");
            break;
        }
        case EziSocialWrapperNS::RESPONSE_CODE::FB_PAGELIKE_POSITIVE:
        {
            CCMessageBox("Hurray! he like my page", "PageLike - Positive");
            break;
        }
        default:
            break;
    }
}

void HelloWorld::fbFriendsCallback(cocos2d::CCArray* friends)
{
    if (friends && friends->count() > 0)
    {
        CCLOG("Total Friends playing this game = %d", friends->count());
        for (int i=0; i<friends->count(); i++)
        {
            CCDictionary *friendDetails = (CCDictionary *)friends->objectAtIndex(i);
            CCString* friendID = (CCString*)friendDetails->objectForKey("id");
            CCString* friendName = (CCString*)friendDetails->objectForKey("name");
            CCLOG("%d. ID = %s, Name = %s", (i+1), friendID->getCString() ,friendName->getCString());
        }
        CCMessageBox((CCString::createWithFormat("Total friends playing this game = %d", friends->count()))->getCString(),
                     "Friends Playing This Game");
    }
    else
    {
        CCMessageBox("None of your friend playing this game.", "Friends Playing this game.");
        
        //CCUserDefault::sharedUserDefault()->setBoolForKey("", true);
        
    }
}

void HelloWorld::fbHighScoresCallback(cocos2d::CCArray* highScores)
{
    if (highScores && highScores->count() > 0)
    {
        CCLOG("Total High Scores available = %d", highScores->count());
        showHighScorePage(highScores);
    }
    else
    {
        CCMessageBox("High Scores are not available.", "Game High Scores.");
    }
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

