//
//  NendNativeAdRender.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAdRender.h"

using namespace nend_module;
using namespace nend_module::internal;

#pragma mark - Internal
Sprite *findNendSpriteFromContainer(Node *container, std::string findName)
{
    auto children = utils::findChildren(*container, findName);
    if (children.empty()) {
        return nullptr;
    }
    auto foundSprite = dynamic_cast<Sprite *>(children.front());
    if (foundSprite) {
        return foundSprite;
    }
    return nullptr;
}
void clearNendNativeLabel(Label *label)
{
    if (label) {
        label->setString("");
    }
}
void clearNendNativeSprite(Sprite *sprite)
{
    if (sprite) {
        sprite->setTexture(nullptr);
    }
}
void setTexture(Sprite *sprite, Texture2D *texture)
{
    sprite->setTexture(texture);
    cocos2d::Rect rect = cocos2d::Rect();
    rect.size = texture->getContentSize();
    sprite->setTextureRect(rect);
}

#pragma mark - findLabelFromContainer
Label *NendNativeAdRender::findLabelFromContainer(Node *container, std::string findName)
{
    auto children = utils::findChildren(*container, findName);
    if (children.empty()) {
        return nullptr;
    }
    auto foundLabel = dynamic_cast<Label *>(children.front());
    if (foundLabel) {
        return foundLabel;
    }
    return nullptr;
}

#pragma mark - renderNativeAdView
void NendNativeAdRender::renderNativeAdView(Node *container, NendNativeAd *nativeAd, NendNativeAdBinder *binder, NendNativeAdvertisingExplicitly explicitly)
{
    CCASSERT(container != nullptr && binder != nullptr, "renderAdViews parameter is incomplete.");

    NendNativeAdRender::clearRenderNativeAdView(container, binder);
    NendNativeAdRender::renderAdElements(container, nativeAd, binder, explicitly);
}

#pragma mark - clearRenderNativeAdView
void NendNativeAdRender::clearRenderNativeAdView(Node *container, NendNativeAdBinder *binder)
{
    clearNendNativeLabel(findLabelFromContainer(container, createFindName(binder->getPrText_Name())));
    clearNendNativeLabel(findLabelFromContainer(container, createFindName(binder->getShortTitle_Name())));
    clearNendNativeLabel(findLabelFromContainer(container, createFindName(binder->getLongText_Name())));
    clearNendNativeLabel(findLabelFromContainer(container, createFindName(binder->getPromotionName_Name())));
    clearNendNativeLabel(findLabelFromContainer(container, createFindName(binder->getPromotionUrl_Name())));
    clearNendNativeLabel(findLabelFromContainer(container, createFindName(binder->getActionText_Name())));
    clearNendNativeSprite(findNendSpriteFromContainer(container, createFindName(binder->getAdImage_Name())));
    clearNendNativeSprite(findNendSpriteFromContainer(container, createFindName(binder->getLogoImage_Name())));
}

#pragma mark - renderAdElements
void NendNativeAdRender::renderAdElements(Node *container, NendNativeAd *nativeAd, NendNativeAdBinder *binder, NendNativeAdvertisingExplicitly explicitly)
{
    Label *prTextLabel = findLabelFromContainer(container, createFindName(binder->getPrText_Name()));
    Label *shortTextLabel = findLabelFromContainer(container, createFindName(binder->getShortTitle_Name()));
    Label *longTextLabel = findLabelFromContainer(container, createFindName(binder->getLongText_Name()));
    Label *promoNameLabel = findLabelFromContainer(container, createFindName(binder->getPromotionName_Name()));
    Label *promoUrlLabel = findLabelFromContainer(container, createFindName(binder->getPromotionUrl_Name()));
    Label *actionTextLabel = findLabelFromContainer(container, createFindName(binder->getActionText_Name()));
    Sprite *adSprite = findNendSpriteFromContainer(container, createFindName(binder->getAdImage_Name()));
    Sprite *logoSprite = findNendSpriteFromContainer(container, createFindName(binder->getLogoImage_Name()));

    CCASSERT(prTextLabel != nullptr, "Cound not found PrText Label in container Node.");

    prTextLabel->setString(nativeAd->prTextForAdvertisingExplicitly(explicitly));

    if (shortTextLabel) {
        shortTextLabel->setString(nativeAd->getShortText());
    }

    if (longTextLabel) {
        longTextLabel->setString(nativeAd->getLongText());
    }

    if (promoNameLabel) {
        promoNameLabel->setString(nativeAd->getPromotionNameText());
    }

    if (!nativeAd->getAdImageUrl().empty()) {
        if (adSprite) {
            nativeAd->downloadAdImage([=](Texture2D *texture, std::string errorMessage) {
                if (nullptr != texture) {
                    setTexture(adSprite, texture);
                }
            });
        }
    }

    if (!nativeAd->getLogoImageUrl().empty()) {
        if (logoSprite) {
            nativeAd->downloadLogoImage([=](Texture2D *texture, std::string errorMessage) {
                if (nullptr != texture) {
                    setTexture(logoSprite, texture);
                }
            });
        }
    }

    if (promoUrlLabel) {
        promoUrlLabel->setString(nativeAd->getPromotionUrlText());
    }

    if (actionTextLabel) {
        actionTextLabel->setString(nativeAd->getActionButtonText());
    }
}
