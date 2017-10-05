//
//  NendNativeAd.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef NendNativeAd_h
#define NendNativeAd_h

#include "cocos2d.h"
#include "NendNativeAdBinder.h"
#include "NendHttpHelper.h"
#include "base/CCVector.h"

namespace nend_module
{
    typedef enum {
        NAD_NATIVE_ADVERTISING_EXPLIICITY_PR,
        NAD_NATIVE_ADVERTISING_EXPLIICITY_SPONSORED,
        NAD_NATIVE_ADVERTISING_EXPLIICITY_AD,
        NAD_NATIVE_ADVERTISING_EXPLIICITY_PROMOTION
    } NendNativeAdvertisingExplicitly;

    class NendNativeAd : public cocos2d::Ref
    {
    public:
        NendNativeAd(){}
        
        virtual ~NendNativeAd();
        
        void setShortText(std::string text){m_shortText = text;}
        std::string getShortText(){return m_shortText;}
        
        void setLongText(std::string text){m_longText = text;}
        std::string getLongText(){return m_longText;}
        
        void setPromotionNameText(std::string text){m_promotionNameText = text;}
        std::string getPromotionNameText(){return m_promotionNameText;}
        
        void setPromotionUrlText(std::string text){m_promotionUrlText = text;}
        std::string getPromotionUrlText(){return m_promotionUrlText;}
        
        void setActionButtonText(std::string text){m_actionButtonText = text;}
        std::string getActionButtonText(){return m_actionButtonText;}
        
        void setAdImageUrl(std::string url){m_adImageUrl = url;}
        std::string getAdImageUrl(){return m_adImageUrl;}
        
        void setLogoImageUrl(std::string url){m_logoImageUrl = url;}
        std::string getLogoImageUrl(){return m_logoImageUrl;}
        
        void setDidImpression(bool didImpression){m_didImpression = didImpression;}
        bool getDidImpression(){return m_didImpression;}
        
        virtual void onAdClick(){}
        virtual void onInformationClick(){}
        virtual void onImpression(){}
        
        void activateAdView(Node* node, Label* prLabel);
        void downloadAdImage(const std::function<void (Texture2D*, std::string)> &callback);
        void downloadLogoImage(const std::function<void (Texture2D*, std::string)> &callback);
        virtual std::string prTextForAdvertisingExplicitly(NendNativeAdvertisingExplicitly explicitly){return "";};
        
        void renderAdViews(Node* node, NendNativeAdBinder* binder, NendNativeAdvertisingExplicitly explicitly);
        
        void setAdClickCallback(const std::function<void (NendNativeAd*, Node*)> &callback);
    private:
        std::string m_shortText;
        std::string m_longText;
        std::string m_promotionNameText;
        std::string m_promotionUrlText;
        std::string m_actionButtonText;
        std::string m_adImageUrl;
        std::string m_logoImageUrl;
        bool m_didImpression;
        
        std::function<void (NendNativeAd*, Node*)> m_adClickCallback;
        void downloadImageData(const std::string imageUrl, const std::function<void (Texture2D*, std::string)> &callback);
        
        void registerEventListener(EventListenerTouchOneByOne* eventListener, Node* container, bool isAdView);
        void tryClickAd(Touch* touch, Node* container);
        void tryClickPR(Touch* touch, Label* prLabel);
        
        void addClickEventToAdView(EventListenerTouchOneByOne* listener, Node* container);
        void addClickEventToPrLabel(EventListenerTouchOneByOne* listener, Label* prLabel);
        
    };
}
#endif /* NendNativeAd_h */
