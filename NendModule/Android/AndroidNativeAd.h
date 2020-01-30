//
//  AndroidNativeAd.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef AndroidNativeAd_h
#define AndroidNativeAd_h

#include "NendNativeAd.h"
#include "AndroidNativeJNI.h"
#include <jni.h>

namespace nend_module
{
    namespace internal
    {
        typedef enum {
            NEND_AD_CLICK,
            NEND_INFO_CLICK,
            NEND_IMPRESSION
        } NativeAdConnectorMethod;

        class AndroidNativeAd : public NendNativeAd, public AndroidNativeJNI
        {
        private:
            void callJniMethod(NativeAdConnectorMethod methodType);
            std::string m_advertisingExplicitly;
        public:
            static AndroidNativeAd* create();
            AndroidNativeAd();
            ~AndroidNativeAd();
            jobject m_connector;
            jobject m_activity;
            virtual void onAdClick() override;
            virtual void onInformationClick() override;
            virtual void onImpression() override;
            virtual std::string prTextForAdvertisingExplicitly(NendNativeAdvertisingExplicitly explicitly) override;
        };
    }
}
#endif /* AndroidNativeAd_h */
