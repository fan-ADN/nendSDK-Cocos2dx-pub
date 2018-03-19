//
//  NendInterstitialVideoAd.mm
//  NendModuleProj
//

#include "NendInterstitialVideoAd.h"

#import <Foundation/Foundation.h>
#import <NendAd/NADInterstitialVideo.h>

using Event = nend_module::video::VideoAd<nend_module::video::InterstitialVideoAd>::EventType;

@interface InterstitialVideoAdDelegate : NSObject<NADInterstitialVideoDelegate>

@property (nonatomic, copy) void(^callback)(Event);
@property (nonatomic) NSInteger errorCode;

@end

@implementation InterstitialVideoAdDelegate

- (void)dealloc
{
    CCLOG("Destruct InterstitialVideoAdDelegate %p", self);
    self.callback = nil;
    [super dealloc];
}

#pragma mark - NADInterstitialVideoDelegate

- (void)nadInterstitialVideoAdDidReceiveAd:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::LOADED);
    }
}

- (void)nadInterstitialVideoAd:(NADInterstitialVideo *)nadInterstitialVideoAd didFailToLoadWithError:(NSError *)error
{
    self.errorCode = error.code;
    if (self.callback) {
        self.callback(Event::FAILED_TO_LOAD);
    }
}

- (void)nadInterstitialVideoAdDidFailedToPlay:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::FAILED_TO_PLAY);
    }
}

- (void)nadInterstitialVideoAdDidOpen:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::SHOWN);
    }
}

- (void)nadInterstitialVideoAdDidClose:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::CLOSED);
    }
}

- (void)nadInterstitialVideoAdDidStartPlaying:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::STARTED);
    }
}

- (void)nadInterstitialVideoAdDidStopPlaying:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::STOPPED);
    }
}

- (void)nadInterstitialVideoAdDidCompletePlaying:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::COMPLETED);
    }
}

- (void)nadInterstitialVideoAdDidClickAd:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::AD_CLICKED);
    }
}

- (void)nadInterstitialVideoAdDidClickInformation:(NADInterstitialVideo *)nadInterstitialVideoAd
{
    if (self.callback) {
        self.callback(Event::INFORMATION_CLICKED);
    }
}

@end

NS_NEND_BEGIN

#pragma mark - InterstitialVideoAd::Impl

class InterstitialVideoAd::Impl {
public:
    Impl(const std::string& spotId, const std::string& apiKey);
    virtual ~Impl();
    
    NADInterstitialVideo* getInterstitialVideoAd() const {
        return interstitialVideoAd_;
    };
    InterstitialVideoAdDelegate* getInterstitialVideoAdDelegate() const {
        return interstitialVideoAdDelegate_;
    };

private:
    NADInterstitialVideo* interstitialVideoAd_;
    InterstitialVideoAdDelegate* interstitialVideoAdDelegate_;
};

InterstitialVideoAd::Impl::Impl(const std::string& spotId, const std::string& apiKey) {
    interstitialVideoAd_ = [[NADInterstitialVideo alloc] initWithSpotId:@(spotId.c_str()) apiKey:@(apiKey.c_str())];
    interstitialVideoAdDelegate_ = [InterstitialVideoAdDelegate new];
    interstitialVideoAd_.delegate = interstitialVideoAdDelegate_;
}

InterstitialVideoAd::Impl::~Impl() {
    CCLOG("Destruct InterstitialVideoAd::Impl %p", this);
    
    interstitialVideoAd_.delegate = nil;
    [interstitialVideoAd_ release];
    [interstitialVideoAdDelegate_ release];
    
    interstitialVideoAd_ = nullptr;
    interstitialVideoAdDelegate_ = nullptr;
}

#pragma mark - InterstitialVideoAd

InterstitialVideoAd::InterstitialVideoAd(const std::string& spotId, const std::string& apiKey, bool isOutputLog) : VideoAd<InterstitialVideoAd>() {
    impl_.reset(new InterstitialVideoAd::Impl(spotId, apiKey));
    
    impl_->getInterstitialVideoAd().isOutputLog = isOutputLog;
    impl_->getInterstitialVideoAdDelegate().callback = ^(Event event) {
        switch (event) {
            case Event::LOADED:
                if (onLoaded_ != nullptr) {
                    onLoaded_();
                }
                break;
            case Event::FAILED_TO_LOAD:
            {
                if (onFailedToLoad_ != nullptr) {
                    InterstitialVideoAdDelegate *delegate = impl_->getInterstitialVideoAdDelegate();
                    int errorCode = @(delegate.errorCode).intValue;
                    onFailedToLoad_(errorCode);
                }
            }
                break;
            case Event::SHOWN:
                if (onShown_ != nullptr) {
                    onShown_();
                }
                break;
            case Event::STARTED:
                if (onStarted_ != nullptr) {
                    onStarted_();
                }
                break;
            case Event::STOPPED:
                if (onStopped_ != nullptr) {
                    onStopped_();
                }
                break;
            case Event::COMPLETED:
                if (onCompleted_ != nullptr) {
                    onCompleted_();
                }
                break;
            case Event::CLOSED:
                if (onClosed_ != nullptr) {
                    onClosed_();
                }
                break;
            case Event::FAILED_TO_PLAY:
                if (onFailedToPlay_ != nullptr) {
                    onFailedToPlay_();
                }
                break;
            case Event::AD_CLICKED:
                if (onAdClicked_ != nullptr) {
                    onAdClicked_();
                }
                break;
            case Event::INFORMATION_CLICKED:
                if (onInformationClicked_ != nullptr) {
                    onInformationClicked_();
                }
                break;
            default:
                break;
        }
    };
}

InterstitialVideoAd::~InterstitialVideoAd() {
    CCLOG("Destruct InterstitialVideoAd %p", this);
    destroyImpl();
}

void InterstitialVideoAd::loadAdImpl() {
    [impl_->getInterstitialVideoAd() loadAd];
}

void InterstitialVideoAd::showAdImpl() {
    impl_->getInterstitialVideoAd().fallbackFullboardBackgroundColor = [UIColor colorWithRed:m_fallbackFullboardBackgroundColor4F.r green:m_fallbackFullboardBackgroundColor4F.g blue:m_fallbackFullboardBackgroundColor4F.b alpha:m_fallbackFullboardBackgroundColor4F.a];
    [impl_->getInterstitialVideoAd() showAdFromViewController:UIApplication.sharedApplication.keyWindow.rootViewController];
}

bool InterstitialVideoAd::isReadyImpl() {
    return impl_->getInterstitialVideoAd().isReady;
}

void InterstitialVideoAd::setUserIdImpl(const std::string &userId) {
    impl_->getInterstitialVideoAd().userId = @(userId.c_str());
}

void InterstitialVideoAd::setUserFeatureImpl(NendUserFeature *userFeature) {
    impl_->getInterstitialVideoAd().userFeature = (NADUserFeature *)userFeature->getUserFeature();
}

void InterstitialVideoAd::setMediationNameImpl(const std::string &mediationName) {
    impl_->getInterstitialVideoAd().mediationName = @(mediationName.c_str());
}

void InterstitialVideoAd::destroyImpl() {
    if (isDestroyNeeded()) {
        [impl_->getInterstitialVideoAd() releaseVideoAd];
    }
}

void InterstitialVideoAd::addFallbackFullBoard(const std::string &spotId, const std::string &apiKey) {
    [impl_->getInterstitialVideoAd() addFallbackFullboardWithSpotId:@(spotId.c_str()) apiKey:@(apiKey.c_str())];
}

NS_NEND_END
