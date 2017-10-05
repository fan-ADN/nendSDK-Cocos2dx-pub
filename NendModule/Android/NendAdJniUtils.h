#ifndef NendAdJniUtils_h
#define NendAdJniUtils_h

#include <atomic>
#include <mutex>
#include <unordered_map>

#include "platform/android/jni/JniHelper.h"
#include "base/ccUTF8.h"

#include "NendVideoAdMacros.h"

NS_NEND_BEGIN

class JniUtils {
public:
    template <typename... Ts>
    static jobject
    callStaticObjectMethod(const char* className, const char* methodName, const char* signature, Ts... args) {
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, className, methodName, signature)) {
            jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID, convertArgument(t, args)...);
            t.env->DeleteLocalRef(t.classID);
            deleteAllocatedObjects(t.env);
            return ret;
        }
        return nullptr;
    }

    template <typename... Ts>
    static bool
    callStaticBooleanMethod(const char* className, const char* methodName, const char* signature, Ts... args) {
        jboolean ret = JNI_FALSE;
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, className, methodName, signature)) {
            ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, convertArgument(t, args)...);
            t.env->DeleteLocalRef(t.classID);
            deleteAllocatedObjects(t.env);
        }
        return ret == JNI_TRUE;
    }

    template <typename... Ts>
    static void callStaticVoidMethod(const char* className, const char* methodName, const char* signature, Ts... args) {
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, className, methodName, signature)) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID, convertArgument(t, args)...);
            t.env->DeleteLocalRef(t.classID);
            deleteAllocatedObjects(t.env);
        }
    }

    static int getNextId() {
        return id.fetch_add(1);
    }

    template <class T>
    static void insertCallbackObject(int id, T* obj, std::unordered_map<int, T*>& map) {
        std::lock_guard<std::mutex> guard(commonMutex);
        map.insert(std::make_pair(id, obj));
    }

    template <class T>
    static void eraseCallbackObject(int id, std::unordered_map<int, T*>& map) {
        std::lock_guard<std::mutex> guard(commonMutex);
        map.erase(id);
    }

    template <class T>
    static T* findCallbackObject(int id, std::unordered_map<int, T*>& map) {
        std::lock_guard<std::mutex> guard(commonMutex);
        auto itr = map.find(id);
        if (map.end() == itr) {
            return nullptr;
        } else {
            return itr->second;
        }
    }

private:
    static std::unordered_map<JNIEnv*, std::vector<jobject>> allocatedObjects;
    static std::mutex commonMutex;
    static std::atomic_int id;
    static jstring utf8ToJstring(JNIEnv* env, const std::string& utf8Str);

    template <typename T>
    static T convertArgument(cocos2d::JniMethodInfo& t, T arg) {
        return arg;
    }

    static jstring convertArgument(cocos2d::JniMethodInfo& t, std::string arg);
    static void deleteAllocatedObjects(JNIEnv* env);
};

NS_NEND_END

#endif //NendAdJniUtils_h
