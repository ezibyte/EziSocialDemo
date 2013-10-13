//
//  EventsAPIScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__EventsAPIScene__
#define __EziSocialPluginDevelopment__EventsAPIScene__

#include "BaseScene.h"

class EventsAPIScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(EventsAPIScene);
    
    virtual void buildScene();
    
private:
    
    bool _storeOpen;
    bool _loggingActiveness;
    
    CCMenuItemImage* _carButton;
    CCMenuItemImage* _shieldButton;
    CCMenuItemImage* _lifeButton;
    CCMenuItemImage* _storeButton;
    CCMenuItemImage* _logActiveButton;
    
    void openStore();
    void buyCar();
    void buyShield();
    void buyLife();
    void logActiveness();
    void showPopup();
};


#endif /* defined(__EziSocialPluginDevelopment__EventsAPIScene__) */
