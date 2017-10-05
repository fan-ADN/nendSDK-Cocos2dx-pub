#include "NendAdJniUtils.h"

NS_NEND_BEGIN

std::unordered_map<JNIEnv*, std::vector<jobject>> JniUtils::allocatedObjects;
std::atomic_int JniUtils::id{0};
std::mutex JniUtils::commonMutex;

jstring JniUtils::utf8ToJstring(JNIEnv* env, const std::string& utf8Str) {
    std::u16string utf16Str;
    bool flag = cocos2d::StringUtils::UTF8ToUTF16(utf8Str, utf16Str);
    
    if(!flag)
    {
        utf16Str.clear();
    }
    jstring stringText = env->NewString((const jchar*)utf16Str.data(), utf16Str.length());
    return stringText;
}

jstring JniUtils::convertArgument(cocos2d::JniMethodInfo& t, std::string arg) {
    jstring ret = utf8ToJstring(t.env, arg.c_str());
    allocatedObjects[t.env].push_back(ret);
    return ret;
}

void JniUtils::deleteAllocatedObjects(JNIEnv* env) {
    if (env) {
        for (const auto& obj : allocatedObjects[env]) {
            env->DeleteLocalRef(obj);
        }
        allocatedObjects[env].clear();
    }
}

NS_NEND_END
