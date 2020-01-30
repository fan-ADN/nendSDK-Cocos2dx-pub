//
//  AndroidNativeAdClient.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef AndroidNativeAdClient_h
#define AndroidNativeAdClient_h

#include "cocos2d.h"
#include "NendNativeAdBinder.h"
#include "INativeAdClient.h"
#include "AndroidNativeAd.h"
#include "AndroidNativeJNI.h"
#include <jni.h>

USING_NS_CC;

namespace nend_module
{
    namespace internal
    {
        class AndroidNativeAdClient : public INativeAdClient, public AndroidNativeJNI
        {
        private:
            jobject m_nativeClient;
        public:
            static AndroidNativeAdClient* create(std::string apiKey, std::string spotID);
            AndroidNativeAdClient(std::string apiKey, std::string spotID);
            ~AndroidNativeAdClient();
            virtual void loadAd(const std::function<void (NendNativeAd*, int*, std::string)> &callback) override;
            virtual void enableAutoReload(const int interval, const std::function<void (NendNativeAd*, int*, std::string)> &callback) override;
            virtual void disableAutoReload() override;
        };
    }
}
#endif /* AndroidNativeAdClient_h */
