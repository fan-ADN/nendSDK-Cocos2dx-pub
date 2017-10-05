//
//  NendNativeSprite.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAdLog.h"
#include "NendNativeSprite.h"

USING_NS_CC;
using namespace nend_module;

#pragma mark - Constructor / Destructor
NendNativeSprite::NendNativeSprite()
{
}
NendNativeSprite::~NendNativeSprite()
{
    NendNativeAdLog::logDebug(__FUNCTION__);
}

#pragma mark - createWithImageType
NendNativeSprite *NendNativeSprite::create()
{
    NendNativeSprite *sprite = new NendNativeSprite();

    if (sprite->init()) {
        sprite->autorelease();
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return NULL;
}