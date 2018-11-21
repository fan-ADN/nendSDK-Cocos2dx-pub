//
//  AndroidNativeAd.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "AndroidNativeAd.h"
#include "NendLogger.h"

using namespace nend_module::internal;

#define CLASS_NAME "net/nend/android/NendNativeAdConnector"
#define METHOD_ID_SIGNATURE "(Landroid/app/Activity;)V"
#define METHOD_ID_SIGNATURE_GET_AD_EXPLICITLYTEXT "(I)Ljava/lang/String;"
#define METHOD_NAME_AD_CLICK "performAdClick"
#define METHOD_NAME_AD_INFO "performInformationClick"
#define METHOD_NAME_AD_IMP "sendImpression"
#define METHOD_NAME_GET_AD_EXPLICITLYTEXT "getAdvertisingExplicitlyText"

AndroidNativeAd* AndroidNativeAd::create() {
    auto ret = new (std::nothrow) AndroidNativeAd;
    
    if (ret)
    {
        ret->autorelease();
    }
    
    return ret;
}

#pragma mark - Constructor / Destructor
AndroidNativeAd::AndroidNativeAd()
{
}

AndroidNativeAd::~AndroidNativeAd()
{
    NendLogger::logDebug(__FUNCTION__);
    std::function<void (JNIEnv* env)> callback_ = [=](JNIEnv* env) {
        if (m_connector != NULL) {
            env->DeleteGlobalRef(m_connector);
            NendLogger::logDebug("delete NendNativeAdConnector jobject");
        }
        if (m_activity != NULL) {
            env->DeleteGlobalRef(m_activity);
            NendLogger::logDebug("delete Activity jobject");
        }
    };
    this->callJNI(callback_);
}

void AndroidNativeAd::callJniMethod(NativeAdConnectorMethod methodType)
{
    std::function<void (JNIEnv* env)> callback_ = [=](JNIEnv* env) {
        jmethodID mid;
        jclass cls = env->FindClass(CLASS_NAME);
        
        if (cls == NULL) {
            return;
        }
        
        if (m_connector == NULL || m_activity == NULL) {
            return;
        }
        
        switch (methodType) {
            case NEND_AD_CLICK:
                mid = env->GetMethodID(cls, METHOD_NAME_AD_CLICK, METHOD_ID_SIGNATURE);
                if (mid != NULL) {
                    env->CallVoidMethod(m_connector, mid, m_activity);
                }
                break;
            case NEND_INFO_CLICK:
                mid = env->GetMethodID(cls, METHOD_NAME_AD_INFO, METHOD_ID_SIGNATURE);
                if (mid != NULL) {
                    env->CallVoidMethod(m_connector, mid, m_activity);
                }
                break;
            case NEND_IMPRESSION:
                mid = env->GetMethodID(cls, METHOD_NAME_AD_IMP, "()V");
                if (mid != NULL) {
                    env->CallVoidMethod(m_connector, mid);
                }
                break;
            default:
                break;
        }
    };
    this->callJNI(callback_);
}

#pragma mark - override method
void AndroidNativeAd::onAdClick()
{
    this->callJniMethod(NEND_AD_CLICK);
}

void AndroidNativeAd::onInformationClick()
{
    this->callJniMethod(NEND_INFO_CLICK);
}

void AndroidNativeAd::onImpression()
{
    this->callJniMethod(NEND_IMPRESSION);
}
std::string AndroidNativeAd::prTextForAdvertisingExplicitly(nend_module::NendNativeAdvertisingExplicitly explicitly)
{
    std::function<void (JNIEnv* env)> callback_ = [=](JNIEnv* env) {
        jmethodID mid;
        jclass cls = env->FindClass(CLASS_NAME);
        
        if (cls == NULL) {
            return;
        }
        
        if (m_connector == NULL || m_activity == NULL) {
            return;
        }

        mid = env->GetMethodID(cls, METHOD_NAME_GET_AD_EXPLICITLYTEXT, METHOD_ID_SIGNATURE_GET_AD_EXPLICITLYTEXT);
        if (mid != NULL) {
            jstring jstr = (jstring)env->CallObjectMethod(m_connector, mid, explicitly);
            if (jstr != NULL) {
                const char *prText = env->GetStringUTFChars(jstr, NULL);
                m_advertisingExplicitly = std::string(prText);
                env->ReleaseStringUTFChars(jstr, prText);
                return;
            }
        }
        return;
    };
    this->callJNI(callback_);
    
    return m_advertisingExplicitly;
}
