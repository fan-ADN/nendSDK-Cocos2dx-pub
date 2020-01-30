//
//  NendFullBoardAd.h
//
//  Created by FAN Communications, Inc.
//
//
#ifndef NendFullBoardAd_h
#define NendFullBoardAd_h
#include <string>
#include <functional>
#include "cocos2d.h"

namespace nend_module
{
    class NendFullBoardAd final {
        class NendFullBoardAdInner;
    public:
        NendFullBoardAd(const std::string& spotId, const std::string& apiKey);
        virtual ~NendFullBoardAd();

        enum struct ErrorType : int {
            FAILED_TO_AD_REQUEST = 1,
            FAILED_TO_DOWNLOAD_IMAGE = 2,
            INVALID_AD_SPACES = 3
        };

        std::function<void(const NendFullBoardAd& ad)> onFullBoardAdLoaded;
        std::function<void(const NendFullBoardAd& ad, const ErrorType error)> onFullBoardAdFailedToLoad;
        std::function<void(const NendFullBoardAd& ad)> onFullBoardAdShown;
        std::function<void(const NendFullBoardAd& ad)> onFullBoardAdDismissed;
        std::function<void(const NendFullBoardAd& ad)> onFullBoardAdClicked;

        void load();
        void show();
        NendFullBoardAdInner *m_Inner = NULL;

        void setBackgroundColor4F(const cocos2d::Color4F& color) { m_backgroundColor4F = color; }
    private:
        cocos2d::Color4F m_backgroundColor4F = cocos2d::Color4F::BLACK;
    };
}

#endif /* NendFullBoardAd_h */
