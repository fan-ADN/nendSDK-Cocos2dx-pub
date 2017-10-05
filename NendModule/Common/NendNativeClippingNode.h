//
//  NendNativeClippingNode.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef NendNativeClippingNode_h
#define NendNativeClippingNode_h

#include "cocos2d.h"

USING_NS_CC;

namespace nend_module
{
    typedef enum {
        NEND_CLIP_CENTER,
        NEND_CLIP_TOP_RIGHT,
        NEND_CLIP_TOP_LEFT,
        NEND_CLIP_BOTTOM_LEFT,
        NEND_CLIP_BOTTOM_RIGHT
    } NendNativeClipType;
    
    class NendNativeClippingNode : public cocos2d::ClippingNode
    {
    private:
        cocos2d::Size m_clipSize;
        cocos2d::Point m_adSpritePoint;
        float m_cornerRadius;
        
        NendNativeClipType m_clipType;
        
        void setCornerRadius(float cornerRadius);
        float getCornerRadius();
        
        void setAdSpritePorition(cocos2d::Point spritePoint);
        cocos2d::Point getAdSpritePorition();
        
        void initOptions(Sprite* sprite, NendNativeClipType clipType);
        
        void setClipType(NendNativeClipType type);
    public:
        NendNativeClippingNode();
        ~NendNativeClippingNode();
        
        static NendNativeClippingNode* create(const cocos2d::Size clipSize, const float radius, Sprite* sprite, NendNativeClipType clipType);
        
        void setClipSize(cocos2d::Size clipSize);
        cocos2d::Size getClipSize();
        
        NendNativeClipType getClipType();
    };
}
#endif /* NendNativeClippingNode_h */
