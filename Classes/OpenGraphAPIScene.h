//
//  OpenGraphAPIScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__OpenGraphAPIScene__
#define __EziSocialPluginDevelopment__OpenGraphAPIScene__

#include "BaseScene.h"

class OpenGraphAPIScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(OpenGraphAPIScene);
    
    virtual void buildScene();
    
    virtual void fbOpenGraphCallback(int responseCode, const char* responseMessage);
    
private:
    
    void earnAchievement();
    void defeatAFriend();
    void findAGuitarItem();
    void completeALevel();
    
};


#endif /* defined(__EziSocialPluginDevelopment__OpenGraphAPIScene__) */
