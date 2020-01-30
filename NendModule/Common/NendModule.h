//
//  NendModule.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef __NendModule__
#define __NendModule__

#include "cocos2d.h"

#define NADViewDelegateNotification               "NENDVIEW_DELEGATE_NOTIFICATION"
#define NADViewDelegateWithSpotIdNotification     "NENDVIEW_DELEGATE_WITH_SPOTID_NOTIFICATION"

typedef enum {
    FINISH_LOAD_AD,
    RECEIVE_AD,
    FAIL_TO_RECEIVE_AD,
    CLICK_AD,
    CLICK_INFORMATION
} NADViewDelegateStatusCode;

using namespace cocos2d;

class NendModule
{
private:
    NendModule();
    //シングルトン用クラス保持
    static NendModule* m_mySingleton;
    static void releaseNADView(char* spotID);
public:
    //クラス取得
    static NendModule* sharedNendModule();

    //NADView生成、API Key、NendID、位置を設定
    static void createNADView(char* apiKey, char* spotID, cocos2d::Point pos);
    //NADView生成、API Key、NendID、位置、バナーサイズ自動調整を設定
    static void createNADView(char* apiKey, char* spotID, cocos2d::Point pos, bool isAdjust);

    //NADView生成、画面上部、API Key、NendIDを設定
    static void createNADViewTop(char* apiKey, char* spotID);
    //NADView生成、画面上部、API Key、NendID、バナーサイズ自動調整を設定
    static void createNADViewTop(char* apiKey, char* spotID, bool isAdjust);

    //NADView生成、画面下部、API Key、NendIDを設定
    static void createNADViewBottom(char* apiKey, char* spotID);
    //NADView生成、画面下部、API Key、NendID、バナーサイズ自動調整を設定
    static void createNADViewBottom(char* apiKey, char* spotID, bool isAdjust);

    //NADView非表示
    static void hideNADView();
    static void hideNADView(char* spotID);

    //NADView表示
    static void showNADView();
    static void showNADView(char* spotID);

    //広告のload一時停止
    static void pause();
    static void pause(char* spotID);

    //広告のload再開
    static void resume();
    static void resume(char* spotID);

    //解放
    static void release();
    static void release(char* spotID);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // EventDispatcherでデリゲート結果を通知する
    static void sendEventDispatcher(int resultCode, char* spotID);
    // NADViewの表示位置を調整する
    static void layoutNADView(char* spotID);
#endif

};

#endif
