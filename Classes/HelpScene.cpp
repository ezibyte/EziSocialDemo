//
//  HelpScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#include "HelpScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelpScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelpScene *layer = HelpScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool HelpScene::init()
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
void HelpScene::onEnterTransitionDidFinish()
{
    CCLog("Enter Transition Finished.");
}

void HelpScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that EziByte team provides you personalize support for all your queries? Just try sending email to them at support@ezibyte.com");

    buildHeader(2, "Help", "Get all the help you need.");
    
    const char* helpText = "What is EziSocial?\n\nEziSocial is a powerful social networking plugin designed for Cocos2D-X platform to meet the social networking capabilities required in a game.  The tool is written in Java, Objective-C and C++. EziSocial plugin uses common development code for both Android and iOS Cocos2D-x project. So, there is no duplicity of code and you need not to worry about portability issue at all.\n\nFor more details you can visit our website. http:://www.ezibyte.com";
    
    CCLabelTTF* helpTextLabel = CCLabelTTF::create("", "Arial", SCALED_VALUE(20.0f));
    helpTextLabel->setDimensions(CCSizeMake(AppDelegate::SCREEN_WIDTH * 0.60, AppDelegate::SCREEN_HEIGHT * 0.72));
    
    helpTextLabel->setString(helpText);
    
    this->addChild(helpTextLabel);
    helpTextLabel->setAnchorPoint(ccp(0.0, 1.0));
    helpTextLabel->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    float py = 1 - (1 - 0.764);
    PlaceNode(helpTextLabel, 0.38, py);
}

