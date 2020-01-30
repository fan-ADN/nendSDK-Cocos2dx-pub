//
//  NendNativeAdBinder.cpp
//  CocosNendSample
//
//  Created by FAN Communications, Inc.
//
//

#include "NendNativeAdBinder.h"
#include "NendLogger.h"

using namespace nend_module;

NendNativeAdBinder::NendNativeAdBinder()
{
}

NendNativeAdBinder::~NendNativeAdBinder()
{
    NendLogger::logDebug(__FUNCTION__);
}

NendNativeAdBinder *NendNativeAdBinder::create()
{
    auto ret = new (std::nothrow) NendNativeAdBinder();

    if (ret) {
        ret->autorelease();
    }

    return ret;
}
