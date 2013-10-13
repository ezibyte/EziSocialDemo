//
//  SplashScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__SplashScene__
#define __EziSocialPluginDevelopment__SplashScene__

#include "cocos2d.h"

class SplashScene :  public cocos2d::CCLayer
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
        
    // implement the "static node()" method manually
    CREATE_FUNC(SplashScene);
    
    void loadHomeScene();
    
};




#endif /* defined(__EziSocialPluginDevelopment__SplashScene__) */
