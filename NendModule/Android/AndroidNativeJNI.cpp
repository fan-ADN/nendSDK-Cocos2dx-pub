//
//  AndroidNativeJNI.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "cocos2d.h"
#include "AndroidNativeJNI.h"
#include "platform/android/jni/JniHelper.h"
#include "NendLogger.h"

using namespace nend_module::internal;

#pragma mark - Constructor / Destructor
AndroidNativeJNI::AndroidNativeJNI()
{
}

AndroidNativeJNI::~AndroidNativeJNI()
{
    NendLogger::logDebug(__FUNCTION__);
}

void AndroidNativeJNI::callJNI(const std::function<void (JNIEnv* env)> &callback)
{
    JavaVM* jvm = JniHelper::getJavaVM();
    JNIEnv *env;
    int status;
    bool isAttached = false;
    
    status = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    
    if(status < 0) {
        status = jvm->AttachCurrentThread(&env, NULL);
        if(status < 0) {
            return;
        }
        isAttached = true;
    }
    
    callback(env);
    
    if(isAttached) {
        jvm->DetachCurrentThread();
    }
}
