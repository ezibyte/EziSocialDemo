//
//  HelpScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__HelpScene__
#define __EziSocialPluginDevelopment__HelpScene__

#include "cocos2d.h"
#include "BaseScene.h"

class HelpScene :  public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelpScene);
    
    virtual void buildScene();
    
};

#endif /* defined(__EziSocialPluginDevelopment__HelpScene__) */
