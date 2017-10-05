//
//  INativeAdClient.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef INativeAdClient_h
#define INativeAdClient_h

#include "NendNativeAd.h"

namespace nend_module
{
    namespace internal
    {        
        class INativeAdClient : public cocos2d::Ref
        {
        protected:
            INativeAdClient(const std::string apiKey, const std::string spotID, const NendNativeAdvertisingExplicitly explicitly){}
            INativeAdClient(const std::string apiKey, const std::string spotID){}
        public:
            virtual ~INativeAdClient(){}
            virtual void loadAd(const std::function<void (NendNativeAd*, int*, std::string)> &callback) = 0;
            virtual void enableAutoReload(const int interval, const std::function<void (NendNativeAd*, int*, std::string)> &callback) = 0;
            virtual void disableAutoReload() = 0;
        };
    }
}
#endif /* INativeAdClient_h */
