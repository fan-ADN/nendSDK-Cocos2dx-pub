//
//  NendUserFeature.mm
//  NendModuleProj-mobile
//
//  Created by FAN Communications, Inc.
//

#include "NendUserFeature.h"

#import <Foundation/Foundation.h>
#import <NendAd/NADUserFeature.h>

NS_NEND_BEGIN

#pragma mark - NendUserFeature::Impl

class NendUserFeature::Impl {
public:
    Impl();

    ~Impl();

    NADUserFeature* userFeature_;
};

NendUserFeature::Impl::Impl() {
    userFeature_ = [NADUserFeature new];
}

NendUserFeature::Impl::~Impl() {
    CCLOG("Destruct NendUserFeature::Impl %p", this);
    [userFeature_ release];
}

#pragma mark - NendUserFeature

NendUserFeature::NendUserFeature() {
    impl_.reset(new NendUserFeature::Impl());
}

NendUserFeature::~NendUserFeature() {
    CCLOG("Destruct NendUserFeature %p", this);
}

void NendUserFeature::setGender(Gender gender) {
    if (gender == Gender::MALE) {
        impl_->userFeature_.gender = NADGenderMale;
    } else if (gender == Gender::FEMALE) {
        impl_->userFeature_.gender = NADGenderFemale;
    }
}

void NendUserFeature::setBirthday(int year, int month, int day) {
    [impl_->userFeature_ setBirthdayWithYear:year month:month day:day];
}

void NendUserFeature::setAge(int age) {
    [impl_->userFeature_ setAge:age];
}

template <>
void NendUserFeature::addCustomValue<int>(int value, const std::string& key) {
    [impl_->userFeature_ addCustomIntegerValue:value forKey:@(key.c_str())];
}

template <>
void NendUserFeature::addCustomValue<double>(double value, const std::string& key) {
    [impl_->userFeature_ addCustomDoubleValue:value forKey:@(key.c_str())];
}

template <>
void NendUserFeature::addCustomValue<const char *>(const char *value, const std::string& key) {
    [impl_->userFeature_ addCustomStringValue:[NSString stringWithCString:value encoding:NSUTF8StringEncoding] forKey:@(key.c_str())];
}

template <>
void NendUserFeature::addCustomValue<bool>(bool value, const std::string& key) {
    [impl_->userFeature_ addCustomBoolValue:value forKey:@(key.c_str())];
}

void* NendUserFeature::getUserFeature() {
    return impl_->userFeature_;
}

NS_NEND_END
