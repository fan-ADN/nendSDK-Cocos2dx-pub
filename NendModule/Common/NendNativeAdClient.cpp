//
//  NendNativeAdClient.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAdClient.h"
#include "NendNativeAdLog.h"
#include "NendNativeImpressionTracker.h"

#include <algorithm>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSNativeAdClient.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AndroidNativeAdClient.h"
#endif

using namespace nend_module;
using namespace nend_module::internal;

NendNativeAdClient *NendNativeAdClient::create(std::string apiKey, std::string spotID)
{
    auto ret = new (std::nothrow) NendNativeAdClient(apiKey, spotID);

    if (ret) {
        ret->autorelease();
    }

    return ret;
}

#pragma mark - Constructor / Destructor
NendNativeAdClient::NendNativeAdClient(const std::string apiKey, const std::string spotID)
{
    this->initialize(apiKey, spotID);
}

NendNativeAdClient::NendNativeAdClient(const std::string apiKey, const std::string spotID, NendNativeAdvertisingExplicitly explicitly)
{
    this->initialize(apiKey, spotID);
    this->m_explicitly = explicitly;
}

void NendNativeAdClient::initialize(const std::string apiKey, const std::string spotID)
{
    if (apiKey.empty()) {
        throw std::range_error("apiKey is empty");
    }
    if (spotID.empty()) {
        throw std::range_error("spodID is empty");
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->m_client = new iOSNativeAdClient(apiKey, spotID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    this->m_client = new AndroidNativeAdClient(apiKey, spotID);
#endif

    m_nativeAd = nullptr;

    m_renderAdViewSuccessCallback = nullptr;
    m_adClickCallback = nullptr;
}

NendNativeAdClient::~NendNativeAdClient()
{
    NendNativeAdLog::logDebug(__FUNCTION__);

    this->disableAutoReload();

    if (m_nativeAd != nullptr) {
        m_nativeAd->deleteAllNendHttpHelper();
    }

    CC_SAFE_DELETE(m_client);

    nativeAds.clear();
}

#pragma mark - Internal
void NendNativeAdClient::callbackLoadResult(const std::function<void(NendNativeAd *, NendNativeLoadResultCode, std::string)> &callback, NendNativeAd *ad, int *errorCode, std::string errorMessage)
{
    if (ad) {
        if (this->isGifImageFormat(ad->getAdImageUrl()) || this->isGifImageFormat(ad->getLogoImageUrl())) {
            callback(nullptr, NEND_INVALID_IMAGE_FORMAT, "The format of the image is invalid.");
        } else {
            nativeAds.pushBack(ad);
            callback(ad, NEND_SUCCESS_LOAD_AD, "");
        }
        CC_SAFE_RELEASE(ad);
    } else {
        int errorResult = *errorCode;
        switch (errorResult) {
            case NEND_FAILED_TO_REQUEST:
                callback(nullptr, NEND_FAILED_TO_REQUEST, errorMessage);
                break;
            case NEND_INVALID_RESPONSE_TYPE:
                callback(nullptr, NEND_INVALID_RESPONSE_TYPE, errorMessage);
                break;
            default:
                callback(nullptr, NEND_FAILED_TO_REQUEST, errorMessage);
                break;
        }
    }
}

bool NendNativeAdClient::isGifImageFormat(const std::string &imageUrl)
{
    if (imageUrl.size() == 0) return false;
    std::string lowerUrl = imageUrl;
    std::transform(lowerUrl.begin(), lowerUrl.end(), lowerUrl.begin(), tolower);
    return (lowerUrl.find(".gif") != std::string::npos);
}

#pragma mark - loadNativeAdClient
void NendNativeAdClient::loadAd(const std::function<void(nend_module::NendNativeAd *, NendNativeLoadResultCode, std::string)> &callback)
{
    this->m_client->loadAd([=](NendNativeAd *ad, int *errorCode, std::string errorMessage) {
        if (ad) {
            this->m_nativeAd = ad;
        }
        this->callbackLoadResult(callback, ad, errorCode, errorMessage);
    });
}

void NendNativeAdClient::loadAd(const std::function<void(NendNativeLoadResultCode resultCode, std::string errorMessage)> &callback)
{
    this->loadAd([=](NendNativeAd *nativeAd, NendNativeLoadResultCode resultCode, std::string errorMessage) {
        callback(resultCode, errorMessage);
    });
}

#pragma mark - renderAdViews
void NendNativeAdClient::renderAdViews(Node *container, NendNativeAdBinder *binder)
{
    if (!m_nativeAd) {
        NendNativeAdLog::logWarn("renderAdViews invalid. Because of loadAd is incomplete");
        return;
    }

    m_nativeAd->renderAdViews(container, binder, m_explicitly);

    m_nativeAd->setAdClickCallback([=](NendNativeAd *nativeAd, Node *container) {
        if (this->m_adClickCallback) {
            this->m_adClickCallback(container);
        }
    });

    if (m_renderAdViewSuccessCallback) {
        m_renderAdViewSuccessCallback(container);
    }
}

void NendNativeAdClient::setRenderAdViewSuccessCallback(const std::function<void(Node *)> &callback)
{
    m_renderAdViewSuccessCallback = callback;
}

void NendNativeAdClient::setAdClickCallback(const std::function<void(Node *)> &callback)
{
    m_adClickCallback = callback;
}

#pragma mark - enableAutoReload
void NendNativeAdClient::enableAutoReload(const int interval, const std::function<void(nend_module::NendNativeAd *, NendNativeLoadResultCode, std::string)> &callback)
{
    this->m_client->enableAutoReload(interval, [=](NendNativeAd *ad, int *errorCode, std::string errorMessage) {
        if (ad) {
            this->m_nativeAd = ad;
        }
        auto callback_ = callback;
        this->callbackLoadResult(callback_, ad, errorCode, errorMessage);
    });
}

#pragma mark - disableAutoReload
void NendNativeAdClient::disableAutoReload()
{
    this->m_client->disableAutoReload();
}
