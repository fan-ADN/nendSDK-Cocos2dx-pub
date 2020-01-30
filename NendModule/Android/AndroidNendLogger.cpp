//
//  AndroidNendLogger.cpp
//
//  Created by FAN Communications, Inc.
//
//

#include "AndroidNendLogger.h"
#include "NendLogger.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#define CLASS_NAME "net/nend/android/NendAdLogger"
#define LOGLEVEL_FIELD_NAME "net/nend/android/NendAdLogger$LogLevel"
#define LOGLEVEL_SIGNATURE "Lnet/nend/android/NendAdLogger$LogLevel;"

void AndroidNendLogger::setLogLevel (NendLoggerLogLevel logLevel) {
    JNIEnv* env = cocos2d::JniHelper::getEnv();
    jclass nendAdLogLevelField = env->FindClass(LOGLEVEL_FIELD_NAME);

    jfieldID logLevelValue;
    switch (logLevel) {
        case NendLoggerLogLevel::DEBUG:
            logLevelValue = env->GetStaticFieldID(nendAdLogLevelField, "DEBUG", LOGLEVEL_SIGNATURE);
            break;
        case NendLoggerLogLevel::INFO:
            logLevelValue = env->GetStaticFieldID(nendAdLogLevelField, "INFO", LOGLEVEL_SIGNATURE);
            break;
        case NendLoggerLogLevel::WARN:
            logLevelValue = env->GetStaticFieldID(nendAdLogLevelField, "WARN", LOGLEVEL_SIGNATURE);
            break;
        case NendLoggerLogLevel::ERROR:
            logLevelValue = env->GetStaticFieldID(nendAdLogLevelField, "ERROR", LOGLEVEL_SIGNATURE);
            break;
        default:
            logLevelValue = env->GetStaticFieldID(nendAdLogLevelField, "OFF", LOGLEVEL_SIGNATURE);
            break;
    }

    jobject setLogLevelValue = env->GetStaticObjectField(nendAdLogLevelField, logLevelValue);
    jclass nendAdLoggerClass = env->FindClass(CLASS_NAME);
    jmethodID setLogLevelMethod = env->GetStaticMethodID(nendAdLoggerClass, "setLogLevel", "(Lnet/nend/android/NendAdLogger$LogLevel;)V");
    env->CallStaticVoidMethod(nendAdLoggerClass, setLogLevelMethod, setLogLevelValue);
}
