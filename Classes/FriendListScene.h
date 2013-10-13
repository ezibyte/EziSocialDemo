//
//  FriendListScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 05/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__FriendListScene__
#define __EziSocialPluginDevelopment__FriendListScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "EziSocialDelegate.h"


USING_NS_CC;
USING_NS_CC_EXT;

enum FRIEND_CIRCLE {
    ALL_FRIENDS      = 0,
    INSTALLED_ONLY   = 1,
    NON_PLAYING_ONLY = 2,
    MY_DEVICE_ONLY   = 3
    };

class FriendListScene :    public CCLayer,
public CCTableViewDataSource,
public CCTableViewDelegate,
public EziFacebookDelegate

{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene(FRIEND_CIRCLE friendCircle);
    
    // implement the "static node()" method manually
    CREATE_FUNC(FriendListScene);

    void setSelectedFriendCircle(FRIEND_CIRCLE selectedFriendCircle);
    
    // TableView Delegate Methods...
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    // Facebook Delegate Methods...
    virtual void fbFriendsCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friends);
    virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
    
    virtual void keyBackClicked();
    
private:
    
    FRIEND_CIRCLE _selectedFriendCircle;
    
    CCArray* _friendList;
    CCDictionary* _friendDictionary;
    CCTableView* _tableView;
    
    int _downloadCount;

    void useRequest(CCNode* sender);
    void backButtonPressed();
    void buildScene();    
};
#endif /* defined(__EziSocialPluginDevelopment__FriendListScene__) */
