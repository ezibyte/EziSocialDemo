//
//  FeedAPIScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__FeedAPIScene__
#define __EziSocialPluginDevelopment__FeedAPIScene__

#include "BaseScene.h"

class FeedAPIScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(FeedAPIScene);
    
    virtual void buildScene();
    
    // Facebook Delegate Methods
    virtual void fbMessageCallback(int responseCode, const char* responseMessage);
    virtual void fbPostPhotoCallback(int responseCode, const char* responseMessage);
    
private:
    
    void postMessageOnUserWall();
    void autoPostMessageOnUserWall();
    void postPhotoInUserAlbum();
    
};

#endif /* defined(__EziSocialPluginDevelopment__FeedAPIScene__) */
