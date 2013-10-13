//
//  ScoreAPIScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "ScoreAPIScene.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* ScoreAPIScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ScoreAPIScene *layer = ScoreAPIScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool ScoreAPIScene::init()
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
void ScoreAPIScene::onEnterTransitionDidFinish()
{
    CCLog("[ScoreAPIScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void ScoreAPIScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that store user score on Facebook? Compare friends score and build a leaderboard in just 1 function call");

    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PNG_FILE(kRESOURCE_SCORE));
    this->addChild(batchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_SCORE));
    buildHeader(-1, "Score APIs", "Generate interesting stories by updating scores");
    
    CCMenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_reset_score_nrl",      "btn_reset_score_prd",      1, 0);
    menuItem->setTarget(this, menu_selector(ScoreAPIScene::resetUserScore));
    
    menuItem = this->buildAndPlaceTile("btn_submit_score_nrl",     "btn_submit_score_prd",     1, 1);
    menuItem->setTarget(this, menu_selector(ScoreAPIScene::submitUserScore));
    
    //menuItem = this->buildAndPlaceTile("btn_reset_tournament_nrl", "btn_reset_tournament_prd", 1, 1);
    //menuItem->setTarget(this, menu_selector(ScoreAPIScene::resetTournamentScore));
    
    menuItem = this->buildAndPlaceTile("btn_achievement_nrl",      "btn_achievement_prd",      0, 2);
    menuItem->setTarget(this, menu_selector(ScoreAPIScene::publishAnAchievement));
}

void ScoreAPIScene::resetUserScore()
{
    EziSocialObject::sharedObject()->deleteScore();
}

void ScoreAPIScene::resetTournamentScore()
{
    //CCMessageBox("Not implemented yet", <#const char *pszTitle#>)
}

void ScoreAPIScene::submitUserScore()
{
    int score = rand() % 1000;
    CCLOG("Posting Score = %d", score);
    EziSocialObject::sharedObject()->postScore(score);
}

void ScoreAPIScene::publishAnAchievement()
{
    EziSocialObject::sharedObject()->postAchievement("https://dl.dropboxusercontent.com/u/37517735/achievementtest.html");
}


#pragma mark - Facebook Callback method

void ScoreAPIScene::fbMessageCallback(int responseCode, const char* responseMessage)
{
    CCMessageBox(responseMessage, "Score API Response");
}

void ScoreAPIScene::fbAchievementCallback(int responseCode, const char* responseMessage)
{
    CCMessageBox(responseMessage, "Achievement Submission Response");
}
