//
//  NendNativeImpressionTracker.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef NendNativeImpressionTracker_h
#define NendNativeImpressionTracker_h

#include <stdio.h>
#include "NendNativeAd.h"

using namespace cocos2d;

namespace nend_module
{
    class NendNativeImpressionTracker : public Ref
    {
    private:
        NendNativeImpressionTracker();
        void checkAdImpression(float dt);
    public:
        static NendNativeImpressionTracker* getInstance();
        void addTrackingNode(NendNativeAd* nativeAd, Node* node);
        void removeTrackingNode(NendNativeAd* nativeAd);
        static bool isImpressionNode(Node* checkNode);
    };
}

#endif /* NendNativeImpressionTracker_h */
