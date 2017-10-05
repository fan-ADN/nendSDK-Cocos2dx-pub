//
//  NendModule.mm
//
//  Created by F@N Communications, Inc.
//
//

#include "NendModule.h"
#include "AppController.h"
#import "RootViewController.h"
#import <NendAd/NADView.h>

@interface NadViewDelegate : NSObject<NADViewDelegate>
{
}
@end

@implementation NadViewDelegate

#pragma mark - NADViewDelegateの設定

// 初回ロード終了後の処理を記述
-(void)nadViewDidFinishLoad:(NADView *)adView{
    NendModule::sendEventDispatcher(FINISH_LOAD_AD, (char *)[adView.nendSpotID UTF8String]);
    NendModule::layoutNADView((char *)[adView.nendSpotID UTF8String]);
}

// 広告受信成功後処理を記述
-(void)nadViewDidReceiveAd:(NADView *)adView{
    NendModule::sendEventDispatcher(RECEIVE_AD, (char *)[adView.nendSpotID UTF8String]);
}

// 広告受信失敗の処理を記述
-(void)nadViewDidFailToReceiveAd:(NADView *)adView{
    NendModule::sendEventDispatcher(FAIL_TO_RECEIVE_AD, (char *)[adView.nendSpotID UTF8String]);
}

// 広告クリック時の処理を記述
-(void)nadViewDidClickAd:(NADView *)adView{
    NendModule::sendEventDispatcher(CLICK_AD, (char *)[adView.nendSpotID UTF8String]);
}

// インフォメーションボタンクリック時の処理を記述
-(void)nadViewDidClickInformation:(NADView *)adView{
    NendModule::sendEventDispatcher(CLICK_INFORMATION, (char *)[adView.nendSpotID UTF8String]);
}


@end


#pragma mark -

typedef enum {
    TOP,
    BOTTOM,
    USER
} NADViewLayoutType;

//シングルトンの設定
NendModule* NendModule::m_mySingleton = NULL;
NSMutableDictionary *spotIdDictionary = nil;
NendModule::NendModule()
{
    
}

#pragma mark -
NendModule* NendModule::sharedNendModule(){
    
    if (NULL == m_mySingleton) {
        //クラス未生成の場合は生成する
        m_mySingleton = new NendModule();
    }
    return m_mySingleton;
}

#pragma mark - 保持しているNADViewを取得する
NADView* getHoldNADView(char* spotID)
{
    NSString *nsstrDst = [NSString stringWithUTF8String: spotID];
    NSDictionary *nadViewDictionary = spotIdDictionary[nsstrDst];
    NADView *holdNADView = nadViewDictionary[@"nadView"];
    return holdNADView;
}

#pragma mark - 保持しているNADViewの座標を取得する
CGPoint getHoldPoint(char* spotID)
{
    NSString *nsstrDst = [NSString stringWithUTF8String: spotID];
    NSDictionary *nadViewDictionary = spotIdDictionary[nsstrDst];
    CGPoint holdPoint = [nadViewDictionary[@"position"] CGPointValue];
    return holdPoint;
}

#pragma mark - load
void load(char* spotID){
    NADView *holdNADView = getHoldNADView(spotID);
    if (holdNADView) {
        [holdNADView load];
    }
}

#pragma mark - NADView生成、API Key, NendIDを設定
void createNADViewInternal(char* apiKey, char* spotID, bool isAdjust, NADViewLayoutType layoutType, cocos2d::Point pos){
    
    if (getHoldNADView(spotID)) {
        // すでにNADViewを生成済み
        return;
    }
    
    NendModule::sharedNendModule();
    
    //NADViewの生成
    NADView *nadView = [[NADView alloc] initWithIsAdjustAdSize:isAdjust];
    
    //NADViewDelegateクラスを生成
    nadView.delegate = (id<NADViewDelegate>)[[NadViewDelegate alloc] init];
    
    AppController* app = [UIApplication sharedApplication].delegate;
    RootViewController* viewController = app.viewController;
    [viewController.view addSubview:nadView];
    
    NSString *nendIDString = [NSString stringWithCString:apiKey encoding:NSUTF8StringEncoding];
    NSString *spotIDString = [NSString stringWithCString:spotID encoding:NSUTF8StringEncoding];
    [nadView setNendID:nendIDString spotID:spotIDString];
    
    if (!spotIdDictionary) {
        spotIdDictionary = [[NSMutableDictionary alloc] init];
    }
    NSDictionary *holdDictionary = @{@"nadView": nadView, @"layoutType": @(layoutType), @"position": [NSValue valueWithCGPoint:CGPointMake(pos.x, pos.y)]};
    [spotIdDictionary setObject:holdDictionary forKey:spotIDString];
    
    load(spotID);
}

#pragma mark - NADView生成、API Key, NendID、位置を設定
void NendModule::createNADView(char* apiKey, char* spotID, cocos2d::Point pos){

    NendModule::createNADView(apiKey, spotID, pos, false);
}

void NendModule::createNADView(char* apiKey, char* spotID, cocos2d::Point pos, bool isAdjust){
    createNADViewInternal(apiKey, spotID, isAdjust, USER, pos);
}

#pragma mark - NADView生成、画面上部、API Key, NendIDを設定
void NendModule::createNADViewTop(char *apiKey, char *spotID){
    
    NendModule::createNADViewTop(apiKey, spotID, false);
}

void NendModule::createNADViewTop(char *apiKey, char *spotID, bool isAdjust){
    
    createNADViewInternal(apiKey, spotID, isAdjust, TOP, cocos2d::Point(0,0));
}

#pragma mark - NADView生成、画面下部、API Key, NendIDを設定
void NendModule::createNADViewBottom(char *apiKey, char *spotID){
    
    NendModule::createNADViewBottom(apiKey, spotID, false);
}

void NendModule::createNADViewBottom(char *apiKey, char *spotID, bool isAdjust){
    
    createNADViewInternal(apiKey, spotID, isAdjust, BOTTOM, cocos2d::Point(0,0));
}

#pragma mark - setPositionTop
//NADViewを画面上部・中央に移動する
void setPositionTop(char *spotID){
    NADView *holdNADView = getHoldNADView(spotID);
    if (!holdNADView) {
        return;
    }
    //Windowサイズを取得
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    UIView *baseView = (UIView *) view->getEAGLView();
    CGRect mainViewRect = baseView.bounds;
    
    //NADViewの位置を変更
    [holdNADView setFrame:CGRectMake((mainViewRect.size.width - holdNADView.frame.size.width) /2, 0, holdNADView.frame.size.width, holdNADView.frame.size.height)];
    
}

#pragma mark - setPositionButtom
//NADViewを画面下部・中央に移動する
void setPositionButtom(char *spotID){
    NADView *holdNADView = getHoldNADView(spotID);
    if (!holdNADView) {
        return;
    }
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    UIView *baseView = (UIView *) view->getEAGLView();
    CGRect mainViewRect = baseView.bounds;
    
    [holdNADView setFrame:CGRectMake((mainViewRect.size.width - holdNADView.frame.size.width) /2, mainViewRect.size.height - holdNADView.frame.size.height, holdNADView.frame.size.width, holdNADView.frame.size.height)];
    
}

#pragma mark - setPositionUser
//NADViewを指定位置に配置する
void setPositionUser(char *spotID) {
    NADView *holdNADView = getHoldNADView(spotID);
    if (!holdNADView) {
        return;
    }
    //cocos2d-x上の座標をiOSの座標に変換
    //cocos2d-xで管理するwindowサイズを取得
    cocos2d::Size size = Director::getInstance()->getWinSize();
    
    //座標を割合で取得
    CGPoint holdPoint = getHoldPoint(spotID);
    cocos2d::Point pointRate;
    pointRate.x = (holdPoint.x / size.width);
    pointRate.y = (holdPoint.y / size.height);
    
    //iOSで管理するViewのサイズを取得
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    UIView *baseView = (UIView *) view->getEAGLView();
    CGRect mainViewRect = baseView.bounds;
    
    //高さをiOS基準に変換する
    CGPoint newPosition;
    newPosition.x = mainViewRect.size.width * pointRate.x;
    newPosition.y = mainViewRect.size.height - (mainViewRect.size.height * pointRate.y);
    
    //座標の設定
    [holdNADView setFrame:CGRectMake(newPosition.x, newPosition.y, holdNADView.frame.size.width, holdNADView.frame.size.height)];
    
}

#pragma mark - NADViewの表示位置を調整する
void NendModule::layoutNADView(char *spotID) {
    
    NSString *nsstrDst = [NSString stringWithUTF8String: spotID];
    NSDictionary *nadViewDictionary = spotIdDictionary[nsstrDst];
    NSInteger layoutValue = [nadViewDictionary[@"layoutType"] integerValue];
    
    switch (layoutValue) {
        case TOP:
            setPositionTop(spotID);
            break;
        case BOTTOM:
            setPositionButtom(spotID);
            break;
        case USER:
            setPositionUser(spotID);
            break;
        default:
            break;
    }
}

#pragma mark - NADView非表示
void NendModule::hideNADView(){
    NendModule::hideNADView(NULL);
}
void NendModule::hideNADView(char* spotID){
    if (spotID) {
        NADView *holdNADView = getHoldNADView(spotID);
        if (holdNADView) {
            [holdNADView setHidden:YES];
        }
    }else{
        NSArray *allSpotID = [spotIdDictionary allKeys];
        for (NSString *holdSpotID in allSpotID) {
            NADView *holdNADView = getHoldNADView((char *)[holdSpotID UTF8String]);
            if (holdNADView) {
                [holdNADView setHidden:YES];
            }
        }
    }
}

#pragma mark - NADView再表示
void NendModule::showNADView(){
    NendModule::showNADView(NULL);
}

void NendModule::showNADView(char* spotID){
    if (spotID) {
        NADView *holdNADView = getHoldNADView(spotID);
        if (holdNADView) {
            [holdNADView setHidden:NO];
        }
    }else {
        NSArray *allSpotID = [spotIdDictionary allKeys];
        for (NSString *holdSpotID in allSpotID) {
            NADView *holdNADView = getHoldNADView((char *)[holdSpotID UTF8String]);
            if (holdNADView) {
                [holdNADView setHidden:NO];
            }
        }
    }
}

#pragma mark - pause
void NendModule::pause(){
    NendModule::pause(NULL);
}

void NendModule::pause(char* spotID){
    if (spotID) {
        NADView *holdNADView = getHoldNADView(spotID);
        if (holdNADView) {
            [holdNADView pause];
        }
    }else {
        NSArray *allSpotID = [spotIdDictionary allKeys];
        for (NSString *holdSpotID in allSpotID) {
            NADView *holdNADView = getHoldNADView((char *)[holdSpotID UTF8String]);
            if (holdNADView) {
                [holdNADView pause];
            }
        }
    }
}

#pragma mark - resume
void NendModule::resume(){
    NendModule::resume(NULL);
}

void NendModule::resume(char* spotID){
    if (spotID) {
        NADView *holdNADView = getHoldNADView(spotID);
        if (holdNADView) {
            [holdNADView resume];
        }
    }else {
        NSArray *allSpotID = [spotIdDictionary allKeys];
        for (NSString *holdSpotID in allSpotID) {
            NADView *holdNADView = getHoldNADView((char *)[holdSpotID UTF8String]);
            if (holdNADView) {
                [holdNADView resume];
            }
        }
    }
}

#pragma mark - release
void NendModule::releaseNADView(char* spotID)
{
    NADView *holdNADView = getHoldNADView(spotID);

    if (holdNADView) {
        [holdNADView pause];
        [holdNADView.delegate release];
        holdNADView.delegate = nil;
        [holdNADView removeFromSuperview];
        [holdNADView release];
        holdNADView = nil;
        
        if (spotIdDictionary) {
            NSString *nsstrDst = [NSString stringWithUTF8String: spotID];
            [spotIdDictionary removeObjectForKey:nsstrDst];
        }
    }
}
void NendModule::release(){
    NendModule::release(NULL);
}

void NendModule::release(char* spotID){
    if (spotID) {
        NendModule::releaseNADView(spotID);
    }else {
        NSArray *allSpotID = [spotIdDictionary allKeys];
        for (NSString *holdSpotID in allSpotID) {
            NendModule::releaseNADView((char *)[holdSpotID UTF8String]);
        }
        if (spotIdDictionary) {
            [spotIdDictionary removeAllObjects];
            [spotIdDictionary release];
            spotIdDictionary = nil;
        }
    }
}

#pragma mark - sendEventDispatcher
void NendModule::sendEventDispatcher(int resultCode, char* spotID){
    int intSpotId;
    intSpotId = atoi(spotID);
    int intArray[] = {resultCode, intSpotId};
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADViewDelegateNotification,&resultCode);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADViewDelegateWithSpotIdNotification, intArray);
}
