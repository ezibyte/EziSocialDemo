//
//  ProfileScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__ProfileScene__
#define __EziSocialPluginDevelopment__ProfileScene__

#include "BaseScene.h"

class ProfileScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(ProfileScene);
    
    virtual void buildScene();
    virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
    virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);
    
    
private:

    CCSprite* _userSprite;

};

#endif /* defined(__EziSocialPluginDevelopment__ProfileScene__) */
