//
//  NendNativeSprite.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef NendNativeSprite_h
#define NendNativeSprite_h

#include "cocos2d.h"

namespace nend_module
{
    class __attribute__((deprecated("will be removed."))) NendNativeSprite : public cocos2d::Sprite
    {
    public:
        NendNativeSprite();
        ~NendNativeSprite();
        
        static NendNativeSprite *create();
    };
}
#endif /* NendNativeSprite_h */
