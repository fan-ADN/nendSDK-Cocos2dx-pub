//
//  iOSNativeAd.m
//
//  Created by F@N Communications, Inc.
//
//
#import <NendAd/NADNative.h>
#include "NendLogger.h"
#include "iOSNativeAd.h"

using namespace nend_module::internal;

#pragma mark - Inner class
class iOSNativeAd::NativeInner : public cocos2d::Ref
{
   private:
    NADNative *m_nadNative;

   public:
    NativeInner()
    {
    }
    static NativeInner *create()
    {
        auto ret = new (std::nothrow) NativeInner;
        if (ret) {
            ret->autorelease();
        }
        return ret;
    }
    virtual ~NativeInner()
    {
        NendLogger::logDebug(__FUNCTION__);
        [m_nadNative release];
    }

    void setNadNative(NADNative *nadNative)
    {
        m_nadNative = nadNative;
        [m_nadNative retain];
    }
    NADNative *getNadNative()
    {
        return m_nadNative;
    }
};

iOSNativeAd *iOSNativeAd::create()
{
    auto ret = new (std::nothrow) iOSNativeAd;
    if (ret) {
        ret->autorelease();
    }
    return ret;
}

#pragma mark - Constructor / Destructor
iOSNativeAd::iOSNativeAd()
{
    m_inner = nullptr;
}

iOSNativeAd::~iOSNativeAd()
{
    NendLogger::logDebug(__FUNCTION__);
    CC_SAFE_DELETE(m_inner);
}

#pragma mark - holdNadNative
void iOSNativeAd::holdNadNative(void *nadNative)
{
    m_inner = new NativeInner();
    NADNative *nadNative_ = (NADNative *)nadNative;
    m_inner->setNadNative(nadNative_);
}

#pragma mark - override method
void iOSNativeAd::onAdClick()
{
    [m_inner->getNadNative() performSelector:@selector(performAdClick)];
}
void iOSNativeAd::onInformationClick()
{
    [m_inner->getNadNative() performSelector:@selector(performInformationClick)];
}
void iOSNativeAd::onImpression()
{
    [m_inner->getNadNative() performSelector:@selector(sendImpression)];
}
std::string iOSNativeAd::prTextForAdvertisingExplicitly(nend_module::NendNativeAdvertisingExplicitly explicitly)
{
    NADNativeAdvertisingExplicitly nativeExplicitly;

    switch (explicitly) {
        case NAD_NATIVE_ADVERTISING_EXPLIICITY_SPONSORED:
            nativeExplicitly = NADNativeAdvertisingExplicitlySponsored;
            break;
        case NAD_NATIVE_ADVERTISING_EXPLIICITY_AD:
            nativeExplicitly = NADNativeAdvertisingExplicitlyAD;
            break;
        case NAD_NATIVE_ADVERTISING_EXPLIICITY_PROMOTION:
            nativeExplicitly = NADNativeAdvertisingExplicitlyPromotion;
            break;
        default:
            nativeExplicitly = NADNativeAdvertisingExplicitlyPR;
            break;
    }

    NSString *explicitylyString = [m_inner->getNadNative() prTextForAdvertisingExplicitly:nativeExplicitly];

    if ([explicitylyString length] > 0) {
        return [explicitylyString UTF8String];
    }
    return "";
}
