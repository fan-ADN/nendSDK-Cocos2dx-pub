//
//  NendNativeAdBinder.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef NendNativeAdBinder_h
#define NendNativeAdBinder_h

#include "cocos2d.h"

namespace nend_module
{
    
    class NendNativeAdBinder : public cocos2d::Ref
    {
    private:
        std::string m_prText_Name;
        std::string m_shortTitle_Name;
        std::string m_longText_Name;
        std::string m_promotionName_Name;
        std::string m_promotionUrl_Name;
        std::string m_actionText_Name;
        std::string m_adImage_Name;
        std::string m_logoImage_Name;
        std::string m_clipNode_Name;
    public:
        static NendNativeAdBinder* create();
        NendNativeAdBinder();
        ~NendNativeAdBinder();
        
        void setPrText_Name(std::string name){m_prText_Name = name;}
        std::string getPrText_Name(){return m_prText_Name;}
        
        void setShortTitle_Name(std::string name){m_shortTitle_Name = name;}
        std::string getShortTitle_Name(){return m_shortTitle_Name;}
        
        void setLongText_Name(std::string name){m_longText_Name = name;}
        std::string getLongText_Name(){return m_longText_Name;}
        
        void setPromotionName_Name(std::string name){m_promotionName_Name = name;}
        std::string getPromotionName_Name(){return m_promotionName_Name;}
        
        void setPromotionUrl_Name(std::string name){m_promotionUrl_Name = name;}
        std::string getPromotionUrl_Name(){return m_promotionUrl_Name;}
        
        void setActionText_Name(std::string name){m_actionText_Name = name;}
        std::string getActionText_Name(){return m_actionText_Name;}
        
        void setAdImage_Name(std::string name){m_adImage_Name = name;}
        std::string getAdImage_Name(){return m_adImage_Name;}
        
        void setLogoImage_Name(std::string name){m_logoImage_Name = name;}
        std::string getLogoImage_Name(){return m_logoImage_Name;}
        
        void setClipNode_Name(std::string name) __attribute__((deprecated("not used. will be removed."))){m_clipNode_Name = name;}
        std::string getClipNode_Name() __attribute__((deprecated("not used. will be removed."))){return m_clipNode_Name;}
    };
}

#endif /* NendNativeAdBinder_h */
