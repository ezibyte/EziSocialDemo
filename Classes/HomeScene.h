//
//  HomeScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__HomeScene__
#define __EziSocialPluginDevelopment__HomeScene__

#include "cocos2d.h"
#include "BaseScene.h"

class HomeScene :   public BaseScene

{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(HomeScene);
    
    virtual void buildScene();
    
    // EziSocial Delegate Methods
    virtual void fbSessionCallback(int responseCode, const char* responseMessage);

    
private:

    void facebookButtonPressed();
    void tweetButtonPressed();
    void emailButtonPressed();
    

};

#endif /* defined(__EziSocialPluginDevelopment__HomeScene__) */
