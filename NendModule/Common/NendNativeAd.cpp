//
//  NendNativeAd.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAd.h"
#include "NendLogger.h"
#include "NendNativeAdRender.h"
#include "NendNativeImpressionTracker.h"

#include "network/HttpClient.h"

#include <string>
#include <iostream>

using namespace nend_module;
using namespace nend_module::internal;

std::vector<EventListenerTouchOneByOne *> touchListeners;
std::vector<NendHttpHelper *> httpHelpers;

NendNativeAd::~NendNativeAd()
{
    NendLogger::logDebug(__FUNCTION__);

    for (std::vector<EventListenerTouchOneByOne *>::iterator it = touchListeners.begin(); it != touchListeners.end(); it++) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(*it);
    }
    touchListeners.clear();
    httpHelpers.clear();
    NendNativeImpressionTracker::getInstance()->removeTrackingNode(this);
}

#pragma mark - Internal
std::string createCacheImageFileName(const std::string imageUrl) {
    auto positionOfLastPath = imageUrl.find_last_of("/");
    auto startPositionOfFileName = positionOfLastPath + 1;
    auto fileName = imageUrl.substr(startPositionOfFileName);
    auto positionOfExtension = fileName.find_last_of(".");
    std::string keyName = imageUrl.substr(startPositionOfFileName, positionOfExtension);
    return keyName;
}

void NendNativeAd::downloadImageData(const std::string imageUrl, const std::function<void(Texture2D *texture, std::string errorMessage)> &callback)
{
    if (nullptr == callback) {
        return;
    }

    if (0 == imageUrl.size()) {
        callback(nullptr, "there is no image.");
        return;
    }

    CC_SAFE_RETAIN(this);
    NendHttpHelper* helper = new NendHttpHelper();
    httpHelpers.push_back(helper);
    helper->setCallback([=](HttpResponse *response, NendHttpHelper *httpHelper) {
        const char *responseTag = response->getHttpRequest()->getTag();
        std::string responseTagString = std::string(responseTag);

        if (response->isSucceed()) {
            std::vector<char> *buffer = response->getResponseData();
            auto *img = new Image();
            img->initWithImageData(reinterpret_cast<unsigned char *>(&(buffer->front())), buffer->size());

            auto texture = Director::getInstance()->getTextureCache()->addImage(img, createCacheImageFileName(imageUrl));
            
            callback(texture, "");
            CC_SAFE_RELEASE(img);
            NendLogger::logDebug(StringUtils::format("%s image download was successful. response code:%ld", response->getHttpRequest()->getTag(), response->getResponseCode()));

        } else {
            callback(nullptr, response->getErrorBuffer());
            NendLogger::logError(StringUtils::format("Failed to download %s image. response code:%ld, error:%s", response->getHttpRequest()->getTag(), response->getResponseCode(), response->getErrorBuffer()));
        }
        this->deleteNendHttpHelper(httpHelper);
    });
    helper->sendGetRequest(imageUrl, "");
}

bool isTouchLocationInContainerNode(Touch *touch, Node *node)
{
    Point touchLocationInParent = node->getParent()->convertToNodeSpace(touch->getLocation());
    Rect rect = node->getBoundingBox();
    if (rect.containsPoint(touchLocationInParent)) {
        return true;
    }
    return false;
}

#pragma mark - activateAdView
void NendNativeAd::activateAdView(Node *node, Label *prLabel)
{
    CCASSERT(node != nullptr || prLabel != nullptr, "activateAdView parameter is incomplete.");

    addClickEventToAdView(EventListenerTouchOneByOne::create(), node);
    addClickEventToPrLabel(EventListenerTouchOneByOne::create(), prLabel);
    if (!this->getDidImpression()) {
        if (NendNativeImpressionTracker::getInstance()->isImpressionNode(node)) {
            this->setDidImpression(true);
            NendNativeImpressionTracker::getInstance()->removeTrackingNode(this);
            this->onImpression();
            NendLogger::logDebug("AD impression.");
        } else {
            NendNativeImpressionTracker::getInstance()->addTrackingNode(this, node);
        }
    }
}

#pragma mark - downloadAdImage
void NendNativeAd::downloadAdImage(const std::function<void(Texture2D *texture, std::string errorMessage)> &callback)
{
    downloadImageData(this->getAdImageUrl(), callback);
}

#pragma mark - downloadLogoImage
void NendNativeAd::downloadLogoImage(const std::function<void(Texture2D *texture, std::string errorMessage)> &callback)
{
    downloadImageData(this->getLogoImageUrl(), callback);
}

#pragma mark - renderAdViews
void NendNativeAd::renderAdViews(Node *node, NendNativeAdBinder *binder, NendNativeAdvertisingExplicitly explicitly)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

        NendNativeAdRender::renderNativeAdView(node, this, binder, explicitly);

        this->activateAdView(node, NendNativeAdRender::findLabelFromContainer(node, NendNativeAdRender::createFindName(binder->getPrText_Name())));

        NendLogger::logInfo(StringUtils::format("Did render ad node of name: %s", node->getName().c_str()));
    });
}

#pragma mark - setAdClickCallback
void NendNativeAd::setAdClickCallback(const std::function<void(NendNativeAd *, Node *)> &callback)
{
    this->m_adClickCallback = callback;
}

#pragma mark - registerEventListener
void NendNativeAd::registerEventListener(EventListenerTouchOneByOne *eventListener, Node *container, bool isAdView)
{
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [=](Touch *touch, Event *event) {
        if (isTouchLocationInContainerNode(touch, container)) {
            return true;
        }
        return false;
    };
    eventListener->onTouchEnded = [=](Touch *touch, Event *event) {
        if (isAdView) {
            this->tryClickAd(touch, container);
        } else {
            this->tryClickPR(touch, (Label *)container);
        }
    };

    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(container, false);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, container);
    touchListeners.push_back(eventListener);
    CC_SAFE_RELEASE(eventListener);
}

#pragma mark - tryClickAd
void NendNativeAd::tryClickAd(Touch *touch, Node *container)
{
    if (isTouchLocationInContainerNode(touch, container)) {
        for (Node *checkNode = container; checkNode != nullptr; checkNode = checkNode->getParent()) {
            if (!checkNode->isVisible()) {
                return;
            }
        }
        if (container->isVisible()) {
            onAdClick();
            if (this->m_adClickCallback) {
                this->m_adClickCallback(this, container);
            }
        }
    }
}

#pragma mark - tryClickPR
void NendNativeAd::tryClickPR(Touch *touch, Label *prLabel)
{
    if (isTouchLocationInContainerNode(touch, prLabel)) {
        for (Node *checkNode = prLabel; checkNode != nullptr; checkNode = checkNode->getParent()) {
            if (!checkNode->isVisible()) {
                return;
            }
        }

        if (prLabel->isVisible()) {
            this->onInformationClick();
            NendLogger::logDebug("Click PR text.");
        }
    }
}

#pragma mark - addClickEventToAdView
void NendNativeAd::addClickEventToAdView(EventListenerTouchOneByOne *listener, Node *container)
{
    CC_SAFE_RETAIN(listener);
    this->registerEventListener(listener, container, true);
}

#pragma mark - addClickEventToPrLabel
void NendNativeAd::addClickEventToPrLabel(EventListenerTouchOneByOne *listener, Label *prLabel)
{
    CC_SAFE_RETAIN(listener);
    this->registerEventListener(listener, prLabel, false);
}

#pragma mark - delete NendHttpHelper
void NendNativeAd::deleteAllNendHttpHelper()
{
    if (httpHelpers.size() == 0) {
        return;
    }
    
    for (auto it = httpHelpers.begin(); it != httpHelpers.end(); it++) {
        NendHttpHelper *helper = *it;
        helper->cancelCallback();
        CC_SAFE_RELEASE(this);
        delete helper;
    }
}

void NendNativeAd::deleteNendHttpHelper(internal::NendHttpHelper *helper)
{
    CC_SAFE_RELEASE(this);
    auto it = httpHelpers.begin();
    while (it != httpHelpers.end()) {
        if(*it == helper) {
            it = httpHelpers.erase(it);
            break;
        } else {
            it++;
        }
    }
    delete helper;
}
