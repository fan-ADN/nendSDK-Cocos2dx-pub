//
//  NendHttpHelper.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef NendHttpHelper_h
#define NendHttpHelper_h

#include "cocos2d.h"
#include "network/HttpClient.h"

USING_NS_CC;

using namespace cocos2d::network;

namespace nend_module
{
    namespace internal
    {
        class NendHttpHelper : public cocos2d::Ref
        {
        private:
            std::function<void (HttpResponse*)> m_callback;
        public:
            static NendHttpHelper* create();
            NendHttpHelper();
            ~NendHttpHelper();
            void sendGetRequest(std::string url, std::string tag);
            void setCallback( const std::function<void (HttpResponse*)> &callback);
        };
    }
}

#endif /* NendHttpHelper_h */
