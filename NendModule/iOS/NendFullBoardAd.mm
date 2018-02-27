//
//  NendFullBoardAd.mm
//
//  Created by F@N Communications, Inc.
//
//

#import <Foundation/Foundation.h>
#import <NendAd/NADFullBoardLoader.h>
#import <NendAd/NADFullBoard.h>
#include "NendFullBoardAd.h"
#include "NendNativeAdLog.h"

@interface NadFullBoardDelegate : NSObject<NADFullBoardDelegate>

@property (nonatomic, copy) void(^AdShown)();
@property (nonatomic, copy) void(^AdClicked)();
@property (nonatomic, copy) void(^AdDismissed)();

@end

using namespace nend_module;
class NendFullBoardAd::NendFullBoardAdInner
{
private:
    NADFullBoardLoader *m_nadFullBoardLoader;
    NADFullBoard *m_nadFullBoard;
    NadFullBoardDelegate *m_Delegate;
    
public:
    NendFullBoardAdInner(NendFullBoardAd *thisPtr, NSString *spotId, NSString *apiKey)
    {
        m_nadFullBoardLoader = [[NADFullBoardLoader alloc] initWithSpotId:spotId apiKey:apiKey];
        m_nadFullBoard = nil;
        m_Delegate = [[NadFullBoardDelegate alloc] init];
        m_Delegate.AdShown = ^() {
            if (thisPtr->onFullBoardAdShown) {
                thisPtr->onFullBoardAdShown(*thisPtr);
            }
        };
        m_Delegate.AdClicked = ^() {
            if (thisPtr->onFullBoardAdClicked) {
                thisPtr->onFullBoardAdClicked(*thisPtr);
            }
        };
        m_Delegate.AdDismissed = ^() {
            if (thisPtr->onFullBoardAdDismissed) {
                thisPtr->onFullBoardAdDismissed(*thisPtr);
            }
        };
    }
    
    virtual ~NendFullBoardAdInner()
    {
        
        NendNativeAdLog::logDebug(__FUNCTION__);
        [m_Delegate release];
        [m_nadFullBoard release];
        [m_nadFullBoardLoader release];
    }
    
    void setNADFullBoard(NADFullBoard *nadFullBoard)
    {
        if (m_nadFullBoard) {
            [m_nadFullBoard release];
        }
        m_nadFullBoard = nadFullBoard;
        m_nadFullBoard.delegate = m_Delegate;
        [m_nadFullBoard retain];
    }
    
    NADFullBoard *getNADFullBoard()
    {
        return m_nadFullBoard;
    }

    NADFullBoardLoader* getNADFullBoardLoader()
    {
        return m_nadFullBoardLoader;
    }
};

@implementation NadFullBoardDelegate
#pragma mark - NADFullBoardDelegate
- (void)NADFullBoardDidShowAd:(NADFullBoard *)ad
{
    if (self.AdShown) {
        self.AdShown();
    }
}

- (void)NADFullBoardDidClickAd:(NADFullBoard *)ad
{
    if (self.AdClicked) {
        self.AdClicked();
    }
}

- (void)NADFullBoardDidDismissAd:(NADFullBoard *)ad
{
    if (self.AdDismissed) {
        self.AdDismissed();
    }
}

- (void)dealloc
{
    self.AdShown = nil;
    self.AdClicked = nil;
    self.AdDismissed = nil;
    [super dealloc];
}
@end

NendFullBoardAd::NendFullBoardAd(const std::string& spotId, const std::string& apiKey)
{
    if (!m_Inner) {
        NSString *apiKeyString = [NSString stringWithUTF8String:apiKey.c_str()];
        NSString *spotIDString = [NSString stringWithUTF8String:spotId.c_str()];
        m_Inner = new NendFullBoardAdInner(this, spotIDString, apiKeyString);
    }
}

NendFullBoardAd::~NendFullBoardAd()
{
    NendNativeAdLog::logDebug(__FUNCTION__);
    CC_SAFE_DELETE(m_Inner);
}

void NendFullBoardAd::load() {
    [m_Inner->getNADFullBoardLoader() loadAdWithCompletionHandler:^(NADFullBoard *ad, NADFullBoardLoaderError error) {
        if (ad) {
            if (onFullBoardAdLoaded)
            {
                onFullBoardAdLoaded(*this);
            }
            m_Inner->setNADFullBoard(ad);
        } else {
            ErrorType errorType;
            switch (error) {
                case NADFullBoardLoaderErrorFailedAdRequest:
                    errorType = ErrorType::FAILED_TO_AD_REQUEST;
                    break;
                case NADFullBoardLoaderErrorFailedDownloadImage:
                    errorType = ErrorType::FAILED_TO_DOWNLOAD_IMAGE;
                    break;
                case NADFullBoardLoaderErrorInvalidAdSpaces:
                    errorType = ErrorType::INVALID_AD_SPACES;
                    break;
                default:
                    break;
            }
            if (onFullBoardAdFailedToLoad)
            {
                onFullBoardAdFailedToLoad(*this, errorType);
            }
        }
    }];
}

void NendFullBoardAd::show() {
    if(m_Inner->getNADFullBoard()) {
        m_Inner->getNADFullBoard().backgroundColor = [UIColor colorWithRed:m_backgroundColor4F.r green:m_backgroundColor4F.g blue:m_backgroundColor4F.b alpha:m_backgroundColor4F.a];
        [m_Inner->getNADFullBoard() showFromViewController:[UIApplication sharedApplication].keyWindow.rootViewController];
    } else {
        NendNativeAdLog::logDebug("Ad has not downloaded yet.");
    }
}
