//
//  iOSNativeAd.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef iOSNativeAd_h
#define iOSNativeAd_h

#include "NendNativeAd.h"

namespace nend_module
{
    namespace internal
    {
        class iOSNativeAd : public NendNativeAd
        {
            class NativeInner;
        public:
            static iOSNativeAd* create();
            iOSNativeAd();
            ~iOSNativeAd();
            NativeInner* m_inner;
            void holdNadNative(void* nadNative);
            virtual void onAdClick() override;
            virtual void onInformationClick() override;
            virtual void onImpression() override;
            virtual std::string prTextForAdvertisingExplicitly(NendNativeAdvertisingExplicitly explicitly) override;
        };
    }
}
#endif /* iOSNativeAd_h */
