//
//  RequestListScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 10/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__RequestListScene__
#define __EziSocialPluginDevelopment__RequestListScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "EziSocialDelegate.h"
#include "EziFBIncomingRequest.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum FRIEND_CIRCLE {
    ALL_FRIENDS      = 0,
    INSTALLED_ONLY   = 1,
    NON_PLAYING_ONLY = 2,
    MY_DEVICE_ONLY   = 3
};

class RequestListScene :    public CCLayer,
public CCTableViewDataSource,
public CCTableViewDelegate,
public EziFacebookDelegate

{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(RequestListScene);
    
    // TableView Delegate Methods...
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    // Facebook Delegate Methods...
    //virtual void fbFriendsCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friends);
    virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
    
    virtual void keyBackClicked();
    virtual void onExit();
    
private:
    
    CCArray* _fbMessagesList;
    CCDictionary* _friendDictionary;
    CCTableView* _tableView;
    
    int _downloadCount;
    
    void useRequest(CCNode* sender);
    void backButtonPressed();
    void buildScene();
    void acceptAllCheckboxPressed();
    void acceptButtonPressed();
    
    std::string getFancyMessage(EziFBIncomingRequest *request);
    std::string getFancyTitle(EziFBIncomingRequest *request);
    std::string getRequetIconFrameName(EziFBIncomingRequest *request);
    
    CCMenuItemImage* _allButton;
    bool _allSelected;
    CCDictionary* _selectedRequestItems;
    
};

#endif /* defined(__EziSocialPluginDevelopment__RequestListScene__) */
