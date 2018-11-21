//
//  NendInterstitialVideoAd.h
//  NendModuleProj
//

#ifndef NendInterstitialVideoAd_h
#define NendInterstitialVideoAd_h

#include <memory>

#include "NendVideoAd.h"
#include "NendUserFeature.h"
#include "cocos2d.h"

NS_NEND_BEGIN

class InterstitialVideoAd : public VideoAd<InterstitialVideoAd> {
public:
    InterstitialVideoAd(const std::string& spotId, const std::string& apiKey);

    virtual ~InterstitialVideoAd();

    void loadAdImpl();

    void showAdImpl();

    bool isReadyImpl();

    void setUserIdImpl(const std::string& userId);

    void setUserFeatureImpl(NendUserFeature *userFeature);

    void setMediationNameImpl(const std::string& mediationName);

    void destroyImpl();

    void addFallbackFullBoard(const std::string& spotId, const std::string& apiKey);

    void setFallbackFullBoardBackgroundColor4F(const cocos2d::Color4F& color) { m_fallbackFullboardBackgroundColor4F = color; }

    class Impl;

private:
    std::unique_ptr<Impl> impl_;
    cocos2d::Color4F m_fallbackFullboardBackgroundColor4F = cocos2d::Color4F::BLACK;
};

NS_NEND_END

#endif /* NendInterstitialVideoAd_h */
