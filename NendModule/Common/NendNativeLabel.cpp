//
//  NendNativeLabel.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAdLog.h"
#include "NendNativeLabel.h"

USING_NS_CC;
using namespace nend_module;

#pragma mark - Constructor / Destructor
NendNativeLabel::NendNativeLabel()
{
}
NendNativeLabel::~NendNativeLabel()
{
    NendNativeAdLog::logDebug(__FUNCTION__);
}

#pragma mark - create
NendNativeLabel *NendNativeLabel::create()
{
    NendNativeLabel *label = new NendNativeLabel();

    if (label->init()) {
        label->autorelease();
        return label;
    }

    CC_SAFE_DELETE(label);
    return NULL;
}