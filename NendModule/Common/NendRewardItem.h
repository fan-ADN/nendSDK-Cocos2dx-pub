//
//  NendRewardItem.h
//  NendModuleProj
//

#ifndef NendRewardItem_h
#define NendRewardItem_h

#include <string>
#include "NendVideoAdMacros.h"

NS_NEND_BEGIN

struct RewardItem {
    const std::string name_;
    const int amount_;

    RewardItem(const std::string& name, const int amount) : name_(name), amount_(amount) {};

    const std::string& getName() const {
        return name_;
    }

    const int getAmount() const {
        return amount_;
    }
};

NS_NEND_END

#endif /* NendRewardItem_h */
