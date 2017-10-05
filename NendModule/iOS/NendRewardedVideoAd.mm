//
//  NendRewardedVideoAd.m
//  NendModuleProj
//
//  Created by F@N Communications, Inc.
//
//

#include "NendRewardedVideoAd.h"

#import <Foundation/Foundation.h>
#import <NendAd/NADRewardedVideo.h>

using Event = nend_module::video::VideoAd<nend_module::video::RewardedVideoAd>::EventType;

@interface RewardedVideoAdDelegate : NSObject<NADRewardedVideoDelegate>

@property (nonatomic, copy) void(^callback)(Event);
@property (nonatomic) NSInteger errorCode;
@property (nonatomic, copy) NSString *currencyName;
@property (nonatomic) NSInteger currencyAmount;

@end

@implementation RewardedVideoAdDelegate

- (void)dealloc
{
    CCLOG("Destruct RewardedVideoAdDelegate %p", self);
    self.callback = nil;
    self.currencyName = nil;
    [super dealloc];
}

#pragma mark - NADRewardedVideoDelegate

- (void)nadRewardVideoAd:(NADRewardedVideo *)nadRewardedVideoAd didReward:(NADReward *)reward
{
    self.currencyName = reward.name;
    self.currencyAmount = reward.amount;
    if (self.callback) {
        self.callback(Event::REWARDED);
    }
}

- (void)nadRewardVideoAdDidReceiveAd:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::LOADED);
    }
}

- (void)nadRewardVideoAd:(NADRewardedVideo *)nadRewardedVideoAd didFailToLoadWithError:(NSError *)error
{
    self.errorCode = error.code;
    if (self.callback) {
        self.callback(Event::FAILED_TO_LOAD);
    }
}

- (void)nadRewardVideoAdDidFailedToPlay:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::FAILED_TO_PLAY);
    }
}

- (void)nadRewardVideoAdDidOpen:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::SHOWN);
    }
}

- (void)nadRewardVideoAdDidClose:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::CLOSED);
    }
}

- (void)nadRewardVideoAdDidStartPlaying:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::STARTED);
    }
}

- (void)nadRewardVideoAdDidStopPlaying:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::STOPPED);
    }
}

- (void)nadRewardVideoAdDidCompletePlaying:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::COMPLETED);
    }
}

- (void)nadRewardVideoAdDidClickAd:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::AD_CLICKED);
    }
}

- (void)nadRewardVideoAdDidClickInformation:(NADRewardedVideo *)nadRewardedVideoAd
{
    if (self.callback) {
        self.callback(Event::INFORMATION_CLICKED);
    }
}

@end

NS_NEND_BEGIN

#pragma mark - RewardedVideoAd::Impl

class RewardedVideoAd::Impl {
public:
    Impl(const std::string& spotId, const std::string& apiKey);
    virtual ~Impl();

    NADRewardedVideo* getRewardedVideoAd() const {
        return rewardedVideoAd_;
    };
    RewardedVideoAdDelegate* getRewardedVideoAdDelegate() const {
        return rewardedVideoAdDelegate_;
    };
    
private:
    NADRewardedVideo* rewardedVideoAd_;
    RewardedVideoAdDelegate* rewardedVideoAdDelegate_;
};

RewardedVideoAd::Impl::Impl(const std::string& spotId, const std::string& apiKey) {
    rewardedVideoAd_ = [[NADRewardedVideo alloc] initWithSpotId:@(spotId.c_str()) apiKey:@(apiKey.c_str())];
    rewardedVideoAdDelegate_ = [RewardedVideoAdDelegate new];
    rewardedVideoAd_.delegate = rewardedVideoAdDelegate_;
}

RewardedVideoAd::Impl::~Impl() {
    CCLOG("Destruct RewardedVideoAd::Impl %p", this);
    
    rewardedVideoAd_.delegate = nil;
    [rewardedVideoAd_ release];
    [rewardedVideoAdDelegate_ release];
    
    rewardedVideoAd_ = nullptr;
    rewardedVideoAdDelegate_ = nullptr;
}

#pragma mark - RewardedVideoAd

RewardedVideoAd::RewardedVideoAd(const std::string& spotId, const std::string& apiKey, bool isOutputLog) : VideoAd<RewardedVideoAd>(), onRewarded_(nullptr) {
    impl_.reset(new RewardedVideoAd::Impl(spotId, apiKey));
    
    impl_->getRewardedVideoAd().isOutputLog = isOutputLog;
    impl_->getRewardedVideoAdDelegate().callback = ^(Event event) {
        switch (event) {
            case Event::LOADED:
                if (onLoaded_ != nullptr) {
                    onLoaded_();
                }
                break;
            case Event::FAILED_TO_LOAD:
            {
                if (onFailedToLoad_ != nullptr) {
                    RewardedVideoAdDelegate *delegate = impl_->getRewardedVideoAdDelegate();
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
            case Event::REWARDED:
            {
                if (onRewarded_ != nullptr) {
                    RewardedVideoAdDelegate *delegate = impl_->getRewardedVideoAdDelegate();
                    std::string name = delegate.currencyName.UTF8String;
                    int amount = @(delegate.currencyAmount).intValue;
                    onRewarded_({name, amount});
                }
            }
                break;
            default:
                break;
        }
    };
}

RewardedVideoAd::~RewardedVideoAd() {
    CCLOG("Destruct RewardedVideoAd %p", this);
    destroyImpl();
}

void RewardedVideoAd::loadAdImpl() {
    [impl_->getRewardedVideoAd() loadAd];
}

void RewardedVideoAd::showAdImpl() {
    [impl_->getRewardedVideoAd() showAdFromViewController:UIApplication.sharedApplication.keyWindow.rootViewController];
}

bool RewardedVideoAd::isReadyImpl() {
    return impl_->getRewardedVideoAd().isReady;
}

void RewardedVideoAd::setUserIdImpl(const std::string &userId) {
    impl_->getRewardedVideoAd().userId = @(userId.c_str());
}

void RewardedVideoAd::setMediationNameImpl(const std::string &mediationName) {
    impl_->getRewardedVideoAd().mediationName = @(mediationName.c_str());
}

void RewardedVideoAd::destroyImpl() {
    if (isDestroyNeeded()) {
        [impl_->getRewardedVideoAd() releaseVideoAd];
    }
}

NS_NEND_END
