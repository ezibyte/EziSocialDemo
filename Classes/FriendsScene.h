//
//  FriendsScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__FriendsScene__
#define __EziSocialPluginDevelopment__FriendsScene__

#include "BaseScene.h"

class FriendsScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(FriendsScene);
    
    virtual void buildScene();
    
private:
    
    void showAllFriends();
    void showNonPlayingFriends();
    void showOnlyPlayingFriends();
    void showOnlyFriendsPlayingWithMyKindOfDevice();
    

};


#endif /* defined(__EziSocialPluginDevelopment__FriendsScene__) */
