#include "AppDelegate.h"

#include "cocos2d.h"
#include "SplashScene.h"
#include "EziSocialObject.h"

USING_NS_CC;

float AppDelegate::SCREEN_WIDTH         = 640;
float AppDelegate::SCREEN_HEIGHT        = 960;
float AppDelegate::SCREEN_SCALE_FACTOR  = 1.0f;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());


    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    AppDelegate::SCREEN_SCALE_FACTOR = CCDirector::sharedDirector()->getVisibleSize().width / 960.0f;
    AppDelegate::SCREEN_WIDTH  = CCDirector::sharedDirector()->getWinSizeInPixels().width;
    AppDelegate::SCREEN_HEIGHT = CCDirector::sharedDirector()->getWinSize().height;
    
    CCLOG("W, H = %f, %f", AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT);
    
    //pDirector->setContentScaleFactor(SCREEN_SCALE_FACTOR);
    
    std::vector<std::string> searchPath;
    
    if (AppDelegate::SCREEN_HEIGHT > 768)
    {
        searchPath.push_back("HDR");
        CCLOG("Using HDR Directory");
        //pDirector->setContentScaleFactor(MAX(AppDelegate::SCREEN_WIDTH/2048.0f, AppDelegate::SCREEN_HEIGHT/1536.0f));
    }
    else if (AppDelegate::SCREEN_HEIGHT > 320)
    {
        searchPath.push_back("HD");
        //pDirector->setContentScaleFactor(MIN(AppDelegate::SCREEN_WIDTH/1024.0f, AppDelegate::SCREEN_HEIGHT/768.0f));
        CCLOG("Using HD Directory with scaleFactor = %f", pDirector->getContentScaleFactor());
        //CCEGLView::sharedOpenGLView()->setDesignResolutionSize(AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT, kResolutionNoBorder);
    }
    else
    {
        searchPath.push_back("SD");
        CCLOG("Using SD Directory");
        //pDirector->setContentScaleFactor(AppDelegate::SCREEN_HEIGHT/320.0f);
    }
    
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    
    // create a scene. it's an autorelease object
    CCScene *pScene = SplashScene::scene();

    // run
    pDirector->runWithScene(pScene);
    
    EziSocialObject::sharedObject()->setAutoCheckIncomingRequestsOnAppLaunch(true);

    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
