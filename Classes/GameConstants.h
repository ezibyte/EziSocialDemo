//
//  GameConstants.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#ifndef EziSocialPluginDevelopment_GameConstants_h
#define EziSocialPluginDevelopment_GameConstants_h

#include "HomeScene.h"
#include "FBActionScene.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

#define PlaceNode(sprite, x, y) sprite->setPosition(CCPointMake(AppDelegate::SCREEN_WIDTH * x, AppDelegate::SCREEN_HEIGHT * y));
#define ScaleByScreen(sprite) sprite->setScale(AppDelegate::SCREEN_SCALE_FACTOR);
#define ReplaceScene(sceneObject) CCDirector::sharedDirector()->replaceScene(CCTransitionFlipX::create(1.0f, sceneObject));
#define ReplaceToLeft(sceneObject) CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1.0f, sceneObject));
#define ReplaceToRight(sceneObject) CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1.0f, sceneObject));
#define SCALED_VALUE(valueToScale) AppDelegate::SCREEN_SCALE_FACTOR * valueToScale
#define PNG_FILE(filename)   filename ".png"
#define PLIST_FILE(filename) filename ".plist"

#define FACEBOOK_PAGE_ID "377572839023945"

#define kSPALSH_WAIT_TIME 3.0f

// Resources Names
#define kRESOURCE_BG                "bg"
#define kRESOURCE_COMMON            "common"
#define kRESOURCE_EVENTS             "events"
#define kRESOURCE_FBACTION           "fb_action"
#define kRESOURCE_FEED_API           "feed_api"
#define kRESOURCE_FRIENDS            "friends"
#define kRESOURCE_HELP               "help"
#define kRESOURCE_HOME               "home"
#define kRESOURCE_MORE               "more"
#define kRESOURCE_OG                 "og"
#define kRESOURCE_PROFILE            "profile"
#define kRESOURCE_REQUEST            "request_api"
#define kRESOURCE_SCORE              "score_api"
#define kRESOURCE_SHARE_DIALOG       "share_dialogs"
#define kRESOURCE_SPLASH             "splash"
    
#endif
