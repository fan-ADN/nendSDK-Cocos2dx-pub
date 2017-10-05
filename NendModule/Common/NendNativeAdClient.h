//
//  NendNativeAdClient.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef NendNativeAdClient_h
#define NendNativeAdClient_h

#include "cocos2d.h"
#include "NendNativeLabel.h"
#include "NendNativeSprite.h"
#include "NendNativeClippingNode.h"
#include "INativeAdClient.h"
#include "NendNativeAd.h"

USING_NS_CC;

namespace nend_module
{
    typedef enum {
        NEND_FAILED_TO_REQUEST = 340,
        NEND_INVALID_RESPONSE_TYPE = 341,
        NEND_SUCCESS_LOAD_AD = 200,
        NEND_INVALID_IMAGE_FORMAT = 343
    } NendNativeLoadResultCode;
    
    class NendNativeAdClient : public cocos2d::Ref
    {
    public:
        NendNativeAdClient(std::string apiKey, std::string spotID);
        virtual ~NendNativeAdClient();

        static nend_module::NendNativeAdClient* create(std::string apiKey, std::string spotID);
        void loadAd(const std::function<void (nend_module::NendNativeAd*, NendNativeLoadResultCode, std::string)> &callback);
        void enableAutoReload(const int interval, const std::function<void (nend_module::NendNativeAd*, NendNativeLoadResultCode, std::string)> &callback);
        void disableAutoReload();
        
        NendNativeAdClient(std::string apiKey, std::string spotID, NendNativeAdvertisingExplicitly explicitly) __attribute__((deprecated("Now deprecated: please use `NendNativeAdClient(std::string apiKey, std::string spotID);`.")));
        void loadAd(const std::function<void (NendNativeLoadResultCode, std::string)> &callback) __attribute__((deprecated("Now deprecated: please use `void loadAd(const std::function<void (nend_module::NendNativeAd*, NendNativeLoadResultCode, std::string)> &callback);`.")));
        void renderAdViews(Node* container, NendNativeAdBinder* binder) __attribute__((deprecated("Now deprecated: please use `void renderAdViews(Node* node, NendNativeAdBinder* binder, NendNativeAdvertisingExplicitly explicitly);` of NendNativeAd.")));
        void setRenderAdViewSuccessCallback(const std::function<void (Node*)> &callback) __attribute__((deprecated("will be removed.")));
        void setRenderAdViewFailedCallback(const std::function<void (Node*)> &callback) __attribute__((deprecated("not used. will be removed."))){return;}
        void setAdClickCallback(const std::function<void (Node*)> &callback) __attribute__((deprecated("Now deprecated: please use `void setAdClickCallback(const std::function<void(NendNativeAd *, Node *)> &callback);` of NendNativeAd.")));
        
    private:
        void initialize(const std::string apiKey, const std::string spotID);
        nend_module::internal::INativeAdClient* m_client;
        nend_module::NendNativeAd* m_nativeAd;
        std::function<void (Node*)> m_renderAdViewSuccessCallback;
        std::function<void (Node*)> m_adClickCallback;
        NendNativeAdvertisingExplicitly m_explicitly;
        cocos2d::Vector<nend_module::NendNativeAd*> nativeAds;
        
        void callbackLoadResult(const std::function<void (nend_module::NendNativeAd*, NendNativeLoadResultCode, std::string)> &callback, nend_module::NendNativeAd* ad, int* errorCode, std::string errorMessage);
        bool isGifImageFormat(const std::string &imageUrl);
    };
}
#endif /* NendNativeAdClient_h */
