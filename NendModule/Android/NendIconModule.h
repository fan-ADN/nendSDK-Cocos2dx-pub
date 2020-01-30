//
//  NendIconModule.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef __NendIconModule__
#define __NendIconModule__

#include "cocos2d.h"

#define NADIconLoaderDelegateNotification     "NEND_ICON_DELEGATE_NOTIFICATION"
#define NADIconLoaderDelegateWithSpotIdNotification     "NEND_ICON_DELEGATE_WITH_SPOTID_NOTIFICATION"

typedef enum {
    FINISH_LOAD_AD_ICON,
    RECEIVE_AD_ICON,
    FAIL_TO_RECEIVE_AD_ICON,
    CLICK_AD_ICON,
    CLICK_INFORMATION_ICON
} NADIconLoaderDelegateStatusCode;

using namespace cocos2d;

class NendIconModule
{
private:
    NendIconModule();
    //シングルトン用クラス保持
    static NendIconModule* m_mySingleton;
    static cocos2d::Point getIconViewPosition(cocos2d::Point pos);
public:
    //クラス取得
    static NendIconModule* sharedNendIconModule();
    //NADIconLoader生成
    static void createNADIconLoader(char* apiKey, char* spotID);

    //NADIconView生成 (位置指定)
    static void createNADIconView(cocos2d::Point pos);
    //NADIconView生成 (位置指定、spotID指定、タイトル表示指定、余白指定、文字色指定)
    static void createNADIconView(cocos2d::Point pos, char* spotID, bool isVisible, bool isSpace, Color3B titleColor);

    //NADIconView生成、画面下部
    static void createNADIconViewBottom();
    //NADIconView生成、画面下部(アイコン表示個数指定)
    static void createNADIconViewBottom(int iconCount);
    //NADIconView生成、画面下部(spotID指定、タイトル表示指定、余白指定、文字色指定)
    static void createNADIconViewBottom(char* spotID, bool isVisible, bool isSpace, Color3B titleColor);
    //NADIconView生成、画面下部(アイコン表示個数指定、spotID指定、タイトル表示指定、余白指定、文字色指定)
    static void createNADIconViewBottom(int iconCount, char* spotID, bool isVisible, bool isSpace, Color3B titleColor);

    //NADIconView生成、画面上部
    static void createNADIconViewTop();
    //NADIconView生成、画面上部(アイコン表示個数指定)
    static void createNADIconViewTop(int iconCount);
    //NADIconView生成、画面上部(spotID指定)
    static void createNADIconViewTop(char* spotID, bool isVisible, bool isSpace, Color3B titleColor);
    //NADIconView生成、画面上部(アイコン表示個数指定、spotID指定、タイトル表示指定、余白指定、文字色指定)
    static void createNADIconViewTop(int iconCount, char* spotID, bool isVisible, bool isSpace, Color3B titleColor);

    //NADIconView非表示
    static void hideNADIconView();
    static void hideNADIconView(char* spotID);

    //NADIconView表示
    static void showNADIconView();
    static void showNADIconView(char* spotID);

    //広告のload
    static void load();
    static void load(char* spotID);

    //広告のload一時停止
    static void pause();
    static void pause(char* spotID);

    //広告のload再開
    static void resume();
    static void resume(char* spotID);

    //解放
    static void release();
    static void release(char* spotID);

};

#endif
