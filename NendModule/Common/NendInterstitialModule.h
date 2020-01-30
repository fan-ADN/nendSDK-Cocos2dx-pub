//
//  NendInterstitialModule.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef cocos2dxv32test2_NendInterstitialModule_h
#define cocos2dxv32test2_NendInterstitialModule_h

#include "cocos2d.h"
#include <stdarg.h>

#define NADInterstitialDelegateLoadResultNotification               "NEND_INTERSTITIAL_DELEGATE_LOAD_RESULT_NOTIFICATION"
#define NADInterstitialDelegateLoadResultWithSpotIdNotification     "NEND_INTERSTITIAL_DELEGATE_LOAD_RESULT_WITH_SPOTID_NOTIFICATION"
#define NADInterstitialDelegateShowResultNotification               "NEND_INTERSTITIAL_DELEGATE_SHOW_RESULT_NOTIFICATION"
#define NADInterstitialDelegateShowResultWithSpotIdNotification     "NEND_INTERSTITIAL_DELEGATE_SHOW_RESULT_WITH_SPOTID_NOTIFICATION"
#define NADInterstitialDelegateClickResultNotification              "NEND_INTERSTITIAL_DELEGATE_CLICK_RESULT_NOTIFICATION"
#define NADInterstitialDelegateClickResultWithSpotIdNotification    "NEND_INTERSTITIAL_DELEGATE_CLICK_RESULT_WITH_SPOTID_NOTIFICATION"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
typedef enum {
    FINISH_LOAD_AD_INTERSTITIAL,
    INVALID_RESPONSE_TYPE_INTERSTITIAL,
    FAIL_AD_REQUEST_INTERSTITIAL,
    FAIL_AD_DOWNLOAD_INTERSTITIAL,
    FAIL_AD_INCOMPLETE_INTERSTITIAL
} NADInterstitialDelegateLoadResultStatusCode;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
typedef enum {
    FINISH_LOAD_AD_INTERSTITIAL,
    INVALID_RESPONSE_TYPE_INTERSTITIAL,
    FAIL_AD_REQUEST_INTERSTITIAL,
    FAIL_AD_INCOMPLETE_INTERSTITIAL,
    FAIL_AD_DOWNLOAD_INTERSTITIAL
} NADInterstitialDelegateLoadResultStatusCode;

#endif

typedef enum {
    SHOW_SUCCESS,
    LOAD_INCOMPLETE,
    REQUEST_INCOMPLETE,
    DOWNLOAD_INCOMPLETE,
    SHOW_FREQUENCY_NOT_REACHABLE,
    SHOW_ALREADY,
    CANNOT_DISPLAY
} NADInterstitialDelegateShowResultStatusCode;

typedef enum {
    CLICK_DOWNLOAD,
    CLICK_CLOSE,
    CLICK_INFORMATION_INTERSTITIAL
} NADInterstitialDelegateClickResultStatusCode;

using namespace cocos2d;

class NendInterstitialModule
{
private:
    NendInterstitialModule();
    // シングルトン用クラス保持
    static NendInterstitialModule* m_mySingleton;

public:
    //クラス取得
    static NendInterstitialModule* sharedNendInterstitialModule();
    // NADInterstitial生成、API Key, NendIDを設定
    static void createNADInterstitial(char* apiKey, char* spotID);
    // NADInterstitial表示
    static void showNADInterstitialView();
    static void showNADInterstitialView(char* spotID);
    // NADInterstitial非表示
    static void dismissNADInterstitialView();
    static void setEnableAutoReload(bool isEnable);
    static bool getEnableAutoReload();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // EventDispatcherでデリゲート結果を通知する
    static void sendEventDispatcherLoadResult(int resultCode, char* spotID);
    static void sendEventDispatcherShowResult(int resultCode, char* spotID);
    static void sendEventDispatcherClickResult(int resultCode, char* spotID);
#endif

};

#endif
