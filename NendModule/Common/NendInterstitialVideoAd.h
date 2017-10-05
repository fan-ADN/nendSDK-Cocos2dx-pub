//
//  NendInterstitialVideoAd.h
//  NendModuleProj
//

#ifndef NendInterstitialVideoAd_h
#define NendInterstitialVideoAd_h

#include <memory>

#include "NendVideoAd.h"

NS_NEND_BEGIN

class InterstitialVideoAd : public VideoAd<InterstitialVideoAd> {
public:
    InterstitialVideoAd(const std::string& spotId, const std::string& apiKey, bool isOutputLog);

    virtual ~InterstitialVideoAd();

    void loadAdImpl();

    void showAdImpl();

    bool isReadyImpl();

    void setUserIdImpl(const std::string& userId);

    void setMediationNameImpl(const std::string& mediationName);

    void destroyImpl();

    void addFallbackFullBoard(const std::string& spotId, const std::string& apiKey);

    class Impl;

private:
    std::unique_ptr<Impl> impl_;
};

NS_NEND_END

#endif /* NendInterstitialVideoAd_h */
