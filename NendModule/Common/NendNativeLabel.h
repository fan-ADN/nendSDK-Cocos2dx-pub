//
//  NendNativeLabel.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef NendNativeLabel_h
#define NendNativeLabel_h

#include "cocos2d.h"

USING_NS_CC;

namespace nend_module
{    
    class __attribute__((deprecated("will be removed."))) NendNativeLabel : public cocos2d::Label
    {
    public:
        NendNativeLabel();
        ~NendNativeLabel();
        
        static NendNativeLabel* create();        
    };
}
#endif /* NendNativeLabel_h */
