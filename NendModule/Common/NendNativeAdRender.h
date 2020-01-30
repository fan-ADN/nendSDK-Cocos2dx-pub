//
//  NendNativeAdRender.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef NendNativeAdRender_h
#define NendNativeAdRender_h

#include "cocos2d.h"
#include "NendNativeAd.h"

USING_NS_CC;

namespace nend_module
{
    namespace internal
    {
        class NendNativeAdRender
        {
        public:
            static std::string createFindName(const std::string name) { return "//" + name; };
            static Label* findLabelFromContainer(Node* container, std::string findName);
            static void renderNativeAdView(Node* container, NendNativeAd* nativeAd, NendNativeAdBinder* binder, NendNativeAdvertisingExplicitly explicitly);
        private:
            static void renderAdElements(Node* container, NendNativeAd* nativeAd, NendNativeAdBinder* binder, NendNativeAdvertisingExplicitly explicitly);
            static void clearRenderNativeAdView(Node* container, NendNativeAdBinder* binder);
        };
    }
}


#endif /* NendNativeAdRender_h */
