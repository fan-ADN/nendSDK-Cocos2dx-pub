//
//  iOSNativeAdClient.m
//
//  Created by F@N Communications, Inc.
//
//

#import <NendAd/NADNativeClient.h>
#include "NendNativeAd.h"
#include "NendNativeAdLog.h"
#include "NendNativeAdRender.h"
#include "iOSNativeAd.h"
#include "iOSNativeAdClient.h"

using namespace nend_module;
using namespace nend_module::internal;

#pragma mark - Inner class
class iOSNativeAdClient::NativeInner : public cocos2d::Ref
{
   private:
    NADNativeClient *m_nadNativeClient;

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
        NendNativeAdLog::logDebug(__FUNCTION__);
        [m_nadNativeClient release];
    }

    void setNadNativeClient(NADNativeClient *nativeClient)
    {
        m_nadNativeClient = nativeClient;
    }
    NADNativeClient *getNadNativeClient()
    {
        return m_nadNativeClient;
    }
};

iOSNativeAdClient *iOSNativeAdClient::create(const std::string apiKey, const std::string spotID)
{
    auto ret = new (std::nothrow) iOSNativeAdClient(apiKey, spotID);

    if (ret) {
        ret->autorelease();
    }

    return ret;
}

#pragma mark - Constructor / Destructor
iOSNativeAdClient::iOSNativeAdClient(const std::string apiKey, const std::string spotID)
    : INativeAdClient(apiKey, spotID)
{
    NSString *apiKeyString = [NSString stringWithUTF8String:apiKey.c_str()];
    NSString *spotIDString = [NSString stringWithUTF8String:spotID.c_str()];

    NADNativeClient *nadClient = [[NADNativeClient alloc] initWithSpotId:spotIDString apiKey:apiKeyString];

    m_inner = new NativeInner();
    m_inner->setNadNativeClient(nadClient);
}

iOSNativeAdClient::~iOSNativeAdClient()
{
    NendNativeAdLog::logDebug(__FUNCTION__);
    CC_SAFE_DELETE(m_inner);
}

#pragma mark - Internal
std::string convertToString(NSString *nsString)
{
    if ([nsString length] > 0) {
        return [nsString UTF8String];
    }
    return "";
}

iOSNativeAd *createNendNativeAd(NADNative *nadNative)
{
    iOSNativeAd *nativeAd = iOSNativeAd::create();
    CC_SAFE_RETAIN(nativeAd);
    nativeAd->setDidImpression(false);
    nativeAd->holdNadNative(nadNative);
    nativeAd->setShortText(convertToString(nadNative.shortText));
    nativeAd->setLongText(convertToString(nadNative.longText));
    nativeAd->setPromotionNameText(convertToString(nadNative.promotionName));
    nativeAd->setPromotionUrlText(convertToString(nadNative.promotionUrl));
    nativeAd->setActionButtonText(convertToString(nadNative.actionButtonText));
    nativeAd->setAdImageUrl(convertToString(nadNative.imageUrl));
    nativeAd->setLogoImageUrl(convertToString(nadNative.logoUrl));
    return nativeAd;
}

void callbackLoadResult(std::function<void(iOSNativeAd *nativeAd, int *errorCode, std::string errorMessage)> callback, NADNative *ad, NSError *error)
{
    if (ad) {
        iOSNativeAd *nativeAd_ = createNendNativeAd(ad);
        if (callback) {
            callback(nativeAd_, nullptr, "");
        }
    } else {
        if (error) {
            NSNumber *errorNumber = [NSNumber numberWithInteger:error.code];
            int errorCode = [errorNumber intValue];
            if (callback) {
                callback(nullptr, &errorCode, [error.description UTF8String]);
            }
            NendNativeAdLog::logError(StringUtils::format("loadAd error: %d, description: %s", errorCode, [error.description UTF8String]));
        }
    }
}

#pragma mark - loadAd
void iOSNativeAdClient::loadAd(const std::function<void(NendNativeAd *, int *, std::string)> &callback)
{
    std::function<void(iOSNativeAd * nativeAd, int *errorCode, std::string errorMessage)> callback_ = callback;

    NADNativeClient *nativeClient = m_inner->getNadNativeClient();
    [nativeClient loadWithCompletionBlock:^(NADNative *ad, NSError *error) {
        callbackLoadResult(callback_, ad, error);
    }];
}

void iOSNativeAdClient::enableAutoReload(const int interval, const std::function<void(NendNativeAd *, int *, std::string)> &callback)
{
    std::function<void(iOSNativeAd * nativeAd, int *errorCode, std::string errorMessage)> callback_ = callback;

    NADNativeClient *nativeClient = m_inner->getNadNativeClient();
    [nativeClient enableAutoReloadWithInterval:interval completionBlock:^(NADNative *ad, NSError *error) {
        callbackLoadResult(callback_, ad, error);
    }];
}

void iOSNativeAdClient::disableAutoReload()
{
    NADNativeClient *nativeClient = m_inner->getNadNativeClient();
    [nativeClient disableAutoReload];
}
