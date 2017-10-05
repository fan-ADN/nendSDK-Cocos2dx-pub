//
//  NendNativeAdBinder.cpp
//  CocosNendSample
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAdBinder.h"
#include "NendNativeAdLog.h"

using namespace nend_module;

NendNativeAdBinder::NendNativeAdBinder()
{
}

NendNativeAdBinder::~NendNativeAdBinder()
{
    NendNativeAdLog::logDebug(__FUNCTION__);
}

NendNativeAdBinder *NendNativeAdBinder::create()
{
    auto ret = new (std::nothrow) NendNativeAdBinder();

    if (ret) {
        ret->autorelease();
    }

    return ret;
}