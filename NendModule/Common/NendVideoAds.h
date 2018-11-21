//
//  NendVideoAds.h
//  NendModuleProj
//

#ifndef NendVideoAds_h
#define NendVideoAds_h

#include <string>
#include <memory>
#include <functional>
#include <new>
#include <assert.h>

#include "NendVideoAdMacros.h"

#include "NendInterstitialVideoAd.h"
#include "NendRewardedVideoAd.h"
#include "NendUserFeature.h"
#include "NendLogger.h"

NS_NEND_BEGIN

template <class T>
class video_ad_container : public T {
public:
    using DefaultCallback = std::function<void(const video_ad_container<T>& vac)>;
    using ErrorCallback = std::function<void(const video_ad_container<T>& vac, int errorCode)>;

    video_ad_container(const std::string& spotId, const std::string& apiKey) : T(spotId, apiKey) {}

    virtual ~video_ad_container() = default;

    void loadAd() {
        T::getImpl()->loadAd();
    }

    void showAd() {
        T::getImpl()->showAd();
    }

    bool isReady() {
        return T::getImpl()->isReady();
    }

    void setUserid(const std::string& userId) {
        T::getImpl()->setUserId(userId);
    }
    
    void setUserFeature(NendUserFeature *userFeature) {
        T::getImpl()->setUserFeature(userFeature);
    }

    void setMediationName(const std::string& mediationName) {
        T::getImpl()->setMediationName(mediationName);
    }

    void destroy() {
        T::getImpl()->destroy();
    }
    
    void onLoaded(const DefaultCallback& callback) {
        T::getImpl()->setOnLoaded([=]() { callback(*this); });
    }

    void onFailedToLoad(const ErrorCallback& callback) {
        T::getImpl()->setOnFailedToLoad([=](int errorCode) { callback(*this, errorCode); });
    }

    void onShown(const DefaultCallback& callback) {
        T::getImpl()->setOnShown([=]() { callback(*this); });
    }

    void onStarted(const DefaultCallback& callback) {
        T::getImpl()->setOnStarted([=]() { callback(*this); });
    }

    void onStopped(const DefaultCallback& callback) {
        T::getImpl()->setOnStopped([=]() { callback(*this); });
    }

    void onCompleted(const DefaultCallback& callback) {
        T::getImpl()->setOnCompleted([=]() { callback(*this); });
    }

    void onClosed(const DefaultCallback& callback) {
        T::getImpl()->setOnClosed([=]() { callback(*this); });
    }

    void onFailedToPlay(const DefaultCallback& callback) {
        T::getImpl()->setOnFailedToPlay([=]() { callback(*this); });
    }

    void onAdClicked(const DefaultCallback& callback) {
        T::getImpl()->setOnAdClicked([=]() { callback(*this); });
    }

    void onInformationClicked(const DefaultCallback& callback) {
        T::getImpl()->setOnInformationClicked([=]() { callback(*this); });
    }
};

class interstitial_video_ad_impl {
public:
    virtual ~interstitial_video_ad_impl() = default;

    void addFallbackFullBoard(const std::string& spotId, const std::string& apiKey) {
        impl_->addFallbackFullBoard(spotId, apiKey);
    }
    
    void setFallbackFullBoardBackgroundColor4F(const cocos2d::Color4F& color) {
        impl_->setFallbackFullBoardBackgroundColor4F(color);
    }

protected:
    interstitial_video_ad_impl(const std::string& spotId, const std::string& apiKey) {
        impl_.reset(new InterstitialVideoAd(spotId, apiKey));
    }

    InterstitialVideoAd* getImpl() const {
        return impl_.get();
    }

private:
    std::unique_ptr<InterstitialVideoAd> impl_;
};

class rewarded_video_ad_impl {
public:
    virtual ~rewarded_video_ad_impl() = default;

    using RewardedCallback = std::function<void(const video_ad_container<rewarded_video_ad_impl>& vac,
                                                const RewardItem& item)>;

    void onRewarded(const RewardedCallback& callback) {
        impl_->setOnRewarded([=](const RewardItem& item) {
            callback(static_cast<const video_ad_container<rewarded_video_ad_impl>&>(*this), item);
        });
    }

protected:
    rewarded_video_ad_impl(const std::string& spotId, const std::string& apiKey) {
        impl_.reset(new RewardedVideoAd(spotId, apiKey));
    }

    RewardedVideoAd* getImpl() const {
        return impl_.get();
    }

private:
    std::unique_ptr<RewardedVideoAd> impl_;
};

struct VideoAds {

    using InterstitialVideoAd = video_ad_container<interstitial_video_ad_impl>;
    using RewardedVideoAd = video_ad_container<rewarded_video_ad_impl>;

    static InterstitialVideoAd* createInterstitialVideoAd(const std::string& spotId, const std::string& apiKey, bool isOutputLog = false) {
        assert(!spotId.empty() && !apiKey.empty());
        if (isOutputLog) {
            NendLogger::setLogLevel(NendLoggerLogLevel::DEBUG);
        }
        return new(std::nothrow) video_ad_container<interstitial_video_ad_impl>{spotId, apiKey};
    }

    static RewardedVideoAd* createRewardedVideoAd(const std::string& spotId, const std::string& apiKey, bool isOutputLog = false) {
        assert(!spotId.empty() && !apiKey.empty());
        if (isOutputLog) {
            NendLogger::setLogLevel(NendLoggerLogLevel::DEBUG);
        }
        return new(std::nothrow) video_ad_container<rewarded_video_ad_impl>{spotId, apiKey};
    }

};


NS_NEND_END

#endif /* NendVideoAds_h */
