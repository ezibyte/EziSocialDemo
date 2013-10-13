//
//  FBActionScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__FBActionScene__
#define __EziSocialPluginDevelopment__FBActionScene__

#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class FBActionScene :  public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(FBActionScene);
    
    // EziSocial Delegate Methods...
    virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);
    virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
    
private:
    
    virtual void buildScene();
    
    void loadProfileScene();
    void loadFeedAPIScene();
    void loadScoreAPIScene();
    void loadeventAPIScene();
    void loadOpenGraphAPIScene();
    void loadRequestAPIScene();
    void loadFriendsAPIScene();
    void loadShareDialogScene();
    
    void showCurrentUserData();
    void showCurrentUserPhoto();
    
    CCMenuItemImage* _profileMenuItem;
    CCSprite*   _profileSprite;
    CCLabelTTF* _firstName;
    CCLabelTTF* _lastName;
    
};

#endif /* defined(__EziSocialPluginDevelopment__FBActionScene__) */
