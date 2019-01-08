//
//  NendRewardedVideoAd.h
//  NendModuleProj
//

#ifndef NendRewardedVideoAd_h
#define NendRewardedVideoAd_h

#include <memory>

#include "NendVideoAd.h"
#include "NendRewardItem.h"
#include "NendUserFeature.h"

NS_NEND_BEGIN

class RewardedVideoAd : public VideoAd<RewardedVideoAd> {
public:
    RewardedVideoAd(const std::string& spotId, const std::string& apiKey);

    virtual ~RewardedVideoAd();

    void loadAdImpl();

    void showAdImpl();

    bool isReadyImpl();

    void setUserIdImpl(const std::string& userId);

    void setUserFeatureImpl(NendUserFeature *userFeature);
    
    void setLocationEnabledImpl(bool enabledLocation);

    void setMediationNameImpl(const std::string& mediationName);

    void destroyImpl();

    using RewardedCallback = std::function<void(const RewardItem&)>;

    void setOnRewarded(const RewardedCallback& callback) {
        onRewarded_ = callback;
    };

    class Impl;

private:
    RewardedCallback onRewarded_;
    std::unique_ptr<Impl> impl_;
};

NS_NEND_END

#endif /* NendRewardedVideoAd_h */
