//
//  ScoreAPIScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__ScoreAPIScene__
#define __EziSocialPluginDevelopment__ScoreAPIScene__

#include "BaseScene.h"

class ScoreAPIScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(ScoreAPIScene);
    
    virtual void buildScene();
    
    // Facebook Callback methods...
    virtual void fbMessageCallback(int responseCode, const char* responseMessage);
    virtual void fbAchievementCallback(int responseCode, const char* responseMessage);
    
private:
    
    void resetUserScore();
    void resetTournamentScore();
    void submitUserScore();
    void publishAnAchievement();
    
};

#endif /* defined(__EziSocialPluginDevelopment__ScoreAPIScene__) */
