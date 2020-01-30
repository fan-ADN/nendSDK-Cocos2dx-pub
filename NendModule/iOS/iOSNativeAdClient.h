//
//  iOSNativeAdClient.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef iOSNativeAdClient_h
#define iOSNativeAdClient_h

#include "cocos2d.h"
#include "INativeAdClient.h"

USING_NS_CC;

namespace nend_module
{
    namespace internal
    {
        class iOSNativeAdClient : public INativeAdClient
        {
            class NativeInner;

        public:
            static iOSNativeAdClient* create(std::string apiKey, std::string spotID);
            iOSNativeAdClient(std::string apiKey, std::string spotID);
            ~iOSNativeAdClient();
            NativeInner* m_inner;
            virtual void loadAd(const std::function<void (NendNativeAd*, int*, std::string)> &callback) override;
            virtual void enableAutoReload(const int interval, const std::function<void (NendNativeAd*, int*, std::string)> &callback) override;
            virtual void disableAutoReload() override;
        };
    }
}
#endif /* iOSNativeAdClient_h */
