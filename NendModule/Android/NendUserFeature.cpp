//
//  NendUserFeature.cpp
//  NendModuleProj-mobile
//
//  Created by FAN Communications, Inc.
//

#include "NendUserFeature.h"

#include "cocos2d.h"
#include "NendAdJniUtils.h"

NS_NEND_BEGIN

#define kNendModuleVideoAdUtilsClass "net/nend/NendModule/VideoAdUtils"

class NendUserFeature::Impl {
public:
    Impl();

    ~Impl();

    jobject userFeatureBuilder_;
};

NendUserFeature::Impl::Impl() {
    jobject ret = JniUtils::callStaticObjectMethod(kNendModuleVideoAdUtilsClass,
                                                   "createUserFeatureBuilder",
                                                   "()Lnet/nend/android/NendAdUserFeature$Builder;");
    if (ret != nullptr) {
        userFeatureBuilder_ = cocos2d::JniHelper::getEnv()->NewGlobalRef(ret);
    }
}

NendUserFeature::Impl::~Impl() {
    CCLOG("Destruct NendUserFeature::Impl %p", this);
    if (userFeatureBuilder_ != nullptr) {
        cocos2d::JniHelper::getEnv()->DeleteGlobalRef(userFeatureBuilder_);
        userFeatureBuilder_ = nullptr;
    }
}

NendUserFeature::NendUserFeature() {
    impl_.reset(new NendUserFeature::Impl());
}

NendUserFeature::~NendUserFeature() {
    CCLOG("Destruct NendUserFeature %p", this);
}

void NendUserFeature::setGender(Gender gender) {
    JniUtils::callStaticVoidMethod(kNendModuleVideoAdUtilsClass,
                                   "setGender",
                                   "(ILnet/nend/android/NendAdUserFeature$Builder;)V",
                                   gender,
                                   impl_->userFeatureBuilder_);
}

void NendUserFeature::setBirthday(int year, int month, int day) {
    JniUtils::callStaticVoidMethod(kNendModuleVideoAdUtilsClass,
                                   "setBirthday",
                                   "(IIILnet/nend/android/NendAdUserFeature$Builder;)V",
                                   year,
                                   month,
                                   day,
                                   impl_->userFeatureBuilder_);
}

void NendUserFeature::setAge(int age) {
    JniUtils::callStaticVoidMethod(kNendModuleVideoAdUtilsClass,
                                   "setAge",
                                   "(ILnet/nend/android/NendAdUserFeature$Builder;)V",
                                   age,
                                   impl_->userFeatureBuilder_);
}

template <>
void NendUserFeature::addCustomValue<int>(int value, const std::string& key) {
    JniUtils::callStaticVoidMethod(kNendModuleVideoAdUtilsClass,
                                   "setCustomFeature",
                                   "(Ljava/lang/String;ILnet/nend/android/NendAdUserFeature$Builder;)V",
                                   key,
                                   value,
                                   impl_->userFeatureBuilder_);
}

template <>
void NendUserFeature::addCustomValue<double>(double value, const std::string& key) {
    JniUtils::callStaticVoidMethod(kNendModuleVideoAdUtilsClass,
                                   "setCustomFeature",
                                   "(Ljava/lang/String;DLnet/nend/android/NendAdUserFeature$Builder;)V",
                                   key,
                                   value,
                                   impl_->userFeatureBuilder_);
}

template <>
void NendUserFeature::addCustomValue<const char *>(const char *value, const std::string& key) {
    JniUtils::callStaticVoidMethod(kNendModuleVideoAdUtilsClass,
                                   "setCustomFeature",
                                   "(Ljava/lang/String;Ljava/lang/String;Lnet/nend/android/NendAdUserFeature$Builder;)V",
                                   key,
                                   (std::string)value,
                                   impl_->userFeatureBuilder_);
}

template <>
void NendUserFeature::addCustomValue<bool>(bool value, const std::string& key) {
    JniUtils::callStaticVoidMethod(kNendModuleVideoAdUtilsClass,
                                   "setCustomFeature",
                                   "(Ljava/lang/String;ZLnet/nend/android/NendAdUserFeature$Builder;)V",
                                   key,
                                   value,
                                   impl_->userFeatureBuilder_);
}

jobject NendUserFeature::buildUserFeature() {
    return JniUtils::callStaticObjectMethod(kNendModuleVideoAdUtilsClass,
                                            "buildNendAdUserFeature",
                                            "(Lnet/nend/android/NendAdUserFeature$Builder;)Lnet/nend/android/NendAdUserFeature;",
                                            impl_->userFeatureBuilder_);
}

NS_NEND_END
