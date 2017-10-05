//
//  NendInterstitialModule.m
//
//  Created by F@N Communications, Inc.
//
//

#include "AppController.h"
#import <NendAd/NADInterstitial.h>
#import "NendInterstitialModule.h"
#import "RootViewController.h"

@interface NadInterstitialDelegate : NSObject <NADInterstitialDelegate> {
}
@end

@implementation NadInterstitialDelegate

- (void)resultForDidFinishLoadInterstitialAdWithStatus:(NADInterstitialStatusCode)status withSpotId:(NSString *)spotId
{
    switch (status) {
        case SUCCESS:
            NendInterstitialModule::sendEventDispatcherLoadResult(FINISH_LOAD_AD_INTERSTITIAL, (char *)[spotId UTF8String]);
            break;
        case INVALID_RESPONSE_TYPE:
            NendInterstitialModule::sendEventDispatcherLoadResult(INVALID_RESPONSE_TYPE_INTERSTITIAL, (char *)[spotId UTF8String]);
            break;
        case FAILED_AD_REQUEST:
            NendInterstitialModule::sendEventDispatcherLoadResult(FAIL_AD_REQUEST_INTERSTITIAL, (char *)[spotId UTF8String]);
            break;
        case FAILED_AD_DOWNLOAD:
            NendInterstitialModule::sendEventDispatcherLoadResult(FAIL_AD_DOWNLOAD_INTERSTITIAL, (char *)[spotId UTF8String]);
            break;
        default:
            break;
    }
}

- (void)resultForNADInterstitialClickType:(NADInterstitialClickType)type withSpotId:(NSString *)spotId
{
    switch (type) {
        case DOWNLOAD:
            // ダウンロードボタンがクリックされました。
            NendInterstitialModule::sendEventDispatcherClickResult(CLICK_DOWNLOAD, (char *)[spotId UTF8String]);
            break;
        case CLOSE:
            // 閉じるボタンあるいは広告範囲外の領域がクリックされました。
            NendInterstitialModule::sendEventDispatcherClickResult(CLICK_CLOSE, (char *)[spotId UTF8String]);
            break;
        case INFORMATION:
            // インフォメーションボタンがクリックされました。
            NendInterstitialModule::sendEventDispatcherClickResult(CLICK_INFORMATION_INTERSTITIAL, (char *)[spotId UTF8String]);
            break;
        default:
            break;
    }
}

//NADViewDelegateの設定
- (void)didFinishLoadInterstitialAdWithStatus:(NADInterstitialStatusCode)status spotId:(NSString *)spotId
{
    [self resultForDidFinishLoadInterstitialAdWithStatus:status withSpotId:spotId];
}

- (void)didClickWithType:(NADInterstitialClickType)type spotId:(NSString *)spotId
{
    [self resultForNADInterstitialClickType:type withSpotId:spotId];
}
@end

//シングルトンの設定
NendInterstitialModule *NendInterstitialModule::m_mySingleton = NULL;
NADInterstitial *interstitial = nil;
NSString *nadInterstitialApiKey;
NSString *nadInterstitialSpotID;
bool enableAutoReload = true;

NendInterstitialModule::NendInterstitialModule()
{
}

NendInterstitialModule *NendInterstitialModule::sharedNendInterstitialModule()
{
    if (NULL == m_mySingleton) {
        //クラス未生成の場合は生成する
        m_mySingleton = new NendInterstitialModule();
    }

    return m_mySingleton;
}

void NendInterstitialModule::createNADInterstitial(char *apiKey, char *spotID)
{
    if (interstitial) {
        return;
    }

    NendInterstitialModule::sharedNendInterstitialModule();

    [[NADInterstitial sharedInstance] setDelegate:(id<NADInterstitialDelegate>)[[NadInterstitialDelegate alloc] init]];

    nadInterstitialApiKey = [NSString stringWithCString:apiKey encoding:NSUTF8StringEncoding];
    nadInterstitialSpotID = [NSString stringWithCString:spotID encoding:NSUTF8StringEncoding];

    [[NADInterstitial sharedInstance] loadAdWithApiKey:nadInterstitialApiKey
                                                spotId:nadInterstitialSpotID];
}

void NendInterstitialModule::dismissNADInterstitialView()
{
    [[NADInterstitial sharedInstance] dismissAd];
}

void NendInterstitialModule::showNADInterstitialView()
{
    NADInterstitialShowResult result = [[NADInterstitial sharedInstance] showAdFromViewController:[UIApplication sharedApplication].keyWindow.rootViewController];

    char spotID[] = "0";
    switch (result) {
        case AD_SHOW_SUCCESS:
            // 広告の表示に成功しました。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_SUCCESS, spotID);
            break;
        case AD_SHOW_ALREADY:
            // 既に広告が表示されています。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_ALREADY, spotID);
            break;
        case AD_FREQUENCY_NOT_REACHABLE:
            // 広告のフリークエンシーカウントに達していません。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_FREQUENCY_NOT_REACHABLE, spotID);
            break;
        case AD_REQUEST_INCOMPLETE:
            // 抽選リクエストに失敗しています。
            NendInterstitialModule::sendEventDispatcherShowResult(REQUEST_INCOMPLETE, spotID);
            break;
        case AD_LOAD_INCOMPLETE:
            // 抽選リクエストが実行されていない、もしくは実行中です。
            NendInterstitialModule::sendEventDispatcherShowResult(LOAD_INCOMPLETE, spotID);
            break;
        case AD_DOWNLOAD_INCOMPLETE:
            // 広告のダウンロードが完了していません。
            NendInterstitialModule::sendEventDispatcherShowResult(DOWNLOAD_INCOMPLETE, spotID);
            break;
        case AD_CANNOT_DISPLAY:
            // 指定されたViewControllerに広告が表示できませんでした。
            NendInterstitialModule::sendEventDispatcherShowResult(CANNOT_DISPLAY, spotID);
            break;
    }
}

void NendInterstitialModule::showNADInterstitialView(char *spotID)
{
    NADInterstitialShowResult result = [[NADInterstitial sharedInstance] showAdFromViewController:[UIApplication sharedApplication].keyWindow.rootViewController
                                                                                           spotId:[NSString stringWithCString:spotID encoding:NSUTF8StringEncoding]];

    int intSpotId;
    intSpotId = atoi(spotID);
    switch (result) {
        case AD_SHOW_SUCCESS:
            // 広告の表示に成功しました。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_SUCCESS, spotID);
            break;
        case AD_SHOW_ALREADY:
            // 既に広告が表示されています。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_ALREADY, spotID);
            break;
        case AD_FREQUENCY_NOT_REACHABLE:
            // 広告のフリークエンシーカウントに達していません。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_FREQUENCY_NOT_REACHABLE, spotID);
            break;
        case AD_REQUEST_INCOMPLETE:
            // 抽選リクエストに失敗しています。
            NendInterstitialModule::sendEventDispatcherShowResult(REQUEST_INCOMPLETE, spotID);
            break;
        case AD_LOAD_INCOMPLETE:
            // 抽選リクエストが実行されていない、もしくは実行中です。
            NendInterstitialModule::sendEventDispatcherShowResult(LOAD_INCOMPLETE, spotID);
            break;
        case AD_DOWNLOAD_INCOMPLETE:
            // 広告のダウンロードが完了していません。
            NendInterstitialModule::sendEventDispatcherShowResult(DOWNLOAD_INCOMPLETE, spotID);
            break;
        case AD_CANNOT_DISPLAY:
            // 指定されたViewControllerに広告が表示できませんでした。
            NendInterstitialModule::sendEventDispatcherShowResult(CANNOT_DISPLAY, spotID);
            break;
    }
}

void NendInterstitialModule::setEnableAutoReload(bool isEnable)
{
    enableAutoReload = isEnable;
    [[NADInterstitial sharedInstance] setEnableAutoReload:enableAutoReload];
}

bool NendInterstitialModule::getEnableAutoReload()
{
    return enableAutoReload;
}

void NendInterstitialModule::sendEventDispatcherLoadResult(int resultCode, char *spotID)
{
    int intSpotId;
    intSpotId = atoi(spotID);
    int intArray[] = {resultCode, intSpotId};
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateLoadResultNotification, &resultCode);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateLoadResultWithSpotIdNotification, intArray);
}

void NendInterstitialModule::sendEventDispatcherShowResult(int resultCode, char *spotID)
{
    int intSpotId;
    intSpotId = atoi(spotID);
    int intArray[] = {resultCode, intSpotId};
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateShowResultNotification, &resultCode);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateShowResultWithSpotIdNotification, intArray);
}

void NendInterstitialModule::sendEventDispatcherClickResult(int resultCode, char *spotID)
{
    int intSpotId;
    intSpotId = atoi(spotID);
    int intArray[] = {resultCode, intSpotId};
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateClickResultNotification, &resultCode);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateClickResultWithSpotIdNotification, intArray);
}