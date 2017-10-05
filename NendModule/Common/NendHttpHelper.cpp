//
//  NendHttpHelper.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include <stdio.h>
#include "NendHttpHelper.h"
#include "network/HttpClient.h"
#include "NendNativeAdLog.h"

USING_NS_CC;
using namespace cocos2d::network;
using namespace nend_module::internal;

NendHttpHelper* NendHttpHelper::create(){
    auto ret = new (std::nothrow) NendHttpHelper;
    
    if (ret) {
        ret->autorelease();
    }
    
    return ret;
}

#pragma mark - Constructor / Destructor
NendHttpHelper::NendHttpHelper()
{
}

NendHttpHelper::~NendHttpHelper()
{
    NendNativeAdLog::logDebug(__FUNCTION__);
}

void NendHttpHelper::setCallback(const std::function<void (HttpResponse *)> &callback)
{
    m_callback = callback;
}

void NendHttpHelper::sendGetRequest(std::string url, std::string tag) {
    auto request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setTag(tag.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback([=](HttpClient* client, HttpResponse* response){
        if (m_callback) {
            m_callback(response);
        }
    });
    auto client = HttpClient::getInstance();
    client->setTimeoutForConnect(30);
    client->setTimeoutForRead(60);
    client->send(request);
    request->release();
}

