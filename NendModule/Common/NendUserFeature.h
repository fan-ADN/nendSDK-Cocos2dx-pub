//
//  NendUserFeature.h
//  NendModuleProj
//

#ifndef NendUserFeature_h
#define NendUserFeature_h

#include <memory>
#include <string>
#include "NendVideoAdMacros.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

NS_NEND_BEGIN

class NendUserFeature {
public:
    enum class Gender : int {
        MALE = 1,
        FEMALE
    };

    NendUserFeature();

    ~NendUserFeature();
    
    void setGender(Gender gender);

    void setBirthday(int year, int month, int day);

    void setAge(int age);

    template <typename T> void addCustomValue(T value, const std::string& key);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jobject buildUserFeature();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void* getUserFeature();
#endif
private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

NS_NEND_END
#endif /* NendUserFeature_h */
