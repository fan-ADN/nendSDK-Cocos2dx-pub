//
//  NendHttpHelper.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include <stdio.h>
#include "NendHttpHelper.h"
#include "NendLogger.h"

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
    NendLogger::logDebug(__FUNCTION__);
}

void NendHttpHelper::setCallback(const std::function<void (HttpResponse *, NendHttpHelper*)> &callback)
{
    m_callback = callback;
}

void NendHttpHelper::cancelCallback()
{
    if (m_request) {
        m_request->setResponseCallback(nullptr);
    }
}

void NendHttpHelper::sendGetRequest(std::string url, std::string tag) {
    m_request = new HttpRequest();
    m_request->setUrl(url.c_str());
    m_request->setTag(tag.c_str());
    m_request->setRequestType(HttpRequest::Type::GET);
    m_request->setResponseCallback([=](HttpClient* client, HttpResponse* response){
        if (m_callback) {
            m_callback(response, this);
        }
    });
    auto client = HttpClient::getInstance();
    client->setTimeoutForConnect(30);
    client->setTimeoutForRead(60);
    client->send(m_request);
    m_request->release();
}

