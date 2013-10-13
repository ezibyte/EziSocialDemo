//
//  RequestAPIScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__RequestAPIScene__
#define __EziSocialPluginDevelopment__RequestAPIScene__

#include "BaseScene.h"

class RequestAPIScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(RequestAPIScene);

    virtual void fbSendRequestCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests);
    virtual void fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests);

private:
    
    CCLabelTTF* _pendingRequestLabel;
    
    void sendGifts();
    void inviteFriends();
    void challengeFriends();
    void refreshIncomingRequests();
    void viewOpenRequestItems();

    
    virtual void buildScene();
};


#endif /* defined(__EziSocialPluginDevelopment__RequestAPIScene__) */
