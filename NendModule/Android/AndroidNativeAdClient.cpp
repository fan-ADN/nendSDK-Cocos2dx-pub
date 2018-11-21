//
//  AndroidNativeAdClient.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "AndroidNativeAdClient.h"
#include "NendLogger.h"

using namespace nend_module;
using namespace nend_module::internal;

#define CLASS_NAME "net/nend/NendModule/NendNativeAdClient"
#define METHOD_NAME_CREATE "createNendAdNativeClient"
#define METHOD_NAME_LOAD "loadAd"
#define METHOD_NAME_ENABLE_AUTO_LOAD "enableAutoReload"
#define METHOD_NAME_DISABLE_AUTO_LOAD "disableAutoReload"

std::vector <std::function<void (AndroidNativeAd* nativeAd, int* errorCode, std::string errorMessage)>> callbacks;
std::function<void (AndroidNativeAd* nativeAd, int* errorCode, std::string errorMessage)> s_auto_reload_callback;

AndroidNativeAdClient* AndroidNativeAdClient::create(const std::string apiKey, const std::string spotID){
    
    auto ret = new (std::nothrow) AndroidNativeAdClient(apiKey, spotID);
    
    if (ret) {
        ret->autorelease();
    }
    
    return ret;
}

#pragma mark - Constructor / Destructor

AndroidNativeAdClient::AndroidNativeAdClient(const std::string apiKey, const std::string spotID):INativeAdClient(apiKey, spotID)
{
    std::function<void (JNIEnv* env)> callback_ = [=](JNIEnv* env) {
        jclass cls = env->FindClass(CLASS_NAME);
        
        if (cls == NULL) {
            return;
        }
        
        jmethodID cns = env->GetMethodID(cls, "<init>", "()V");
        if (cns == NULL) {
            return;
        }
        
        jobject localObj = env->NewObject(cls, cns);
        if (localObj == NULL) {
            return;
        }
        m_nativeClient = env->NewGlobalRef(localObj);
        if (m_nativeClient == NULL) {
            return;
        }
        
        jmethodID mid = env->GetMethodID(cls, METHOD_NAME_CREATE, "(Ljava/lang/String;Ljava/lang/String;)V");
        if (mid != NULL) {
            jstring stringApiKey = env->NewStringUTF(apiKey.c_str());
            jstring stringSpotID = env->NewStringUTF(spotID.c_str());
            env->CallVoidMethod(m_nativeClient, mid, stringApiKey, stringSpotID);
        }
    };
    this->callJNI(callback_);
}

AndroidNativeAdClient::~AndroidNativeAdClient()
{
    NendLogger::logDebug(__FUNCTION__);
    std::function<void (JNIEnv* env)> callback_ = [=](JNIEnv* env) {
        if (m_nativeClient != NULL) {
            env->DeleteGlobalRef(m_nativeClient);
            NendLogger::logDebug("delete NendNativeAdClient jobject");
        }
    };
    this->callJNI(callback_);
    
    callbacks.clear();
}

#pragma mark - Internal
std::string convertToCString(const char *charText)
{
    if (charText != NULL){
        return std::string(charText);
    }
    return "";
}

void createNendNativeAd(JNIEnv* env, jobject nativeAdConnector, jobject activity, std::function<void (AndroidNativeAd* nativeAd)> &callback)
{
    const char *shortText = NULL;
    const char *longText = NULL;
    const char *promotionName = NULL;
    const char *promotionUrl = NULL;
    const char *adImageUrl = NULL;
    const char *logoImageUrl = NULL;
    const char *actionButtonText = NULL;
    
    jclass cls = env->FindClass("net/nend/android/NendNativeAdConnector");
    if (cls == NULL) {
        return;
    }

    if (nativeAdConnector == NULL) {
        return;
    }
    
    jmethodID mid_shortText = env->GetMethodID(cls, "getShortText", "()Ljava/lang/String;");
    jmethodID mid_longText = env->GetMethodID(cls, "getLongText", "()Ljava/lang/String;");
    jmethodID mid_promoName = env->GetMethodID(cls, "getPromotionName", "()Ljava/lang/String;");
    jmethodID mid_promoUrl = env->GetMethodID(cls, "getPromotionUrl", "()Ljava/lang/String;");
    jmethodID mid_adImgUrl = env->GetMethodID(cls, "getAdImageUrl", "()Ljava/lang/String;");
    jmethodID mid_logoImgUrl = env->GetMethodID(cls, "getLogoImageUrl", "()Ljava/lang/String;");
    jmethodID mid_actionText = env->GetMethodID(cls, "getActionButtonText", "()Ljava/lang/String;");

    AndroidNativeAd* nativeAd = AndroidNativeAd::create();
    CC_SAFE_RETAIN(nativeAd);
    nativeAd->setDidImpression(false);

    jobject globalConnector = env->NewGlobalRef(nativeAdConnector);
    nativeAd->m_connector = globalConnector;

    jobject globalActivity = env->NewGlobalRef(activity);
    nativeAd->m_activity = globalActivity;

    if (mid_shortText != NULL) {
        jstring jstr = (jstring)env->CallObjectMethod(globalConnector, mid_shortText);
        if (jstr != NULL) {
            shortText = env->GetStringUTFChars(jstr, NULL);
            nativeAd->setShortText(convertToCString(shortText));
            env->ReleaseStringUTFChars(jstr, shortText);
        }
    }

    if (mid_longText != NULL) {
        jstring jstr = (jstring)env->CallObjectMethod(globalConnector, mid_longText);
        if (jstr != NULL) {
            longText = env->GetStringUTFChars(jstr, NULL);
            nativeAd->setLongText(convertToCString(longText));
            env->ReleaseStringUTFChars(jstr, longText);
        }
    }
    
    if (mid_promoName != NULL) {
        jstring jstr = (jstring)env->CallObjectMethod(globalConnector, mid_promoName);
        if (jstr != NULL) {
            promotionName = env->GetStringUTFChars(jstr, NULL);
            nativeAd->setPromotionNameText(convertToCString(promotionName));
            env->ReleaseStringUTFChars(jstr, promotionName);
        }
    }
    
    if (mid_promoUrl != NULL) {
        jstring jstr = (jstring)env->CallObjectMethod(globalConnector, mid_promoUrl);
        if (jstr != NULL) {
            promotionUrl = env->GetStringUTFChars(jstr, NULL);
            nativeAd->setPromotionUrlText(convertToCString(promotionUrl));
            env->ReleaseStringUTFChars(jstr, promotionUrl);
        }
    }
    
    if (mid_adImgUrl != NULL) {
        jstring jstr = (jstring)env->CallObjectMethod(globalConnector, mid_adImgUrl);
        if (jstr != NULL) {
            adImageUrl = env->GetStringUTFChars(jstr, NULL);
            nativeAd->setAdImageUrl(convertToCString(adImageUrl));
            env->ReleaseStringUTFChars(jstr, adImageUrl);
        }
    }
    
    if (mid_logoImgUrl != NULL) {
        jstring jstr = (jstring)env->CallObjectMethod(globalConnector, mid_logoImgUrl);
        if (jstr != NULL) {
            logoImageUrl = env->GetStringUTFChars(jstr, NULL);
            nativeAd->setLogoImageUrl(convertToCString(logoImageUrl));
            env->ReleaseStringUTFChars(jstr, logoImageUrl);
        }
    }
    
    if (mid_actionText != NULL) {
        jstring jstr = (jstring)env->CallObjectMethod(globalConnector, mid_actionText);
        if (jstr != NULL) {
            actionButtonText = env->GetStringUTFChars(jstr, NULL);
            nativeAd->setActionButtonText(convertToCString(actionButtonText));
            env->ReleaseStringUTFChars(jstr, actionButtonText);
        }
    }
    
    callback(nativeAd);
}

void loadAdFailed(JNIEnv* env, jobject errorObject, std::function<void (int errorInt, std::string message)> &callback)
{
    const char *errorMessage = NULL;
    
    jclass clsj = env->GetObjectClass(errorObject);
    jfieldID fId_errorCode = env->GetFieldID(clsj, "errorCode", "I");
    jfieldID fId_errorMessage = env->GetFieldID(clsj, "errorMessage", "Ljava/lang/String;");
    
    jint errorCode = env->GetIntField(errorObject, fId_errorCode);
    
    jstring messageObj = (jstring)env->GetObjectField(errorObject, fId_errorMessage);
    if (messageObj) {
        errorMessage = env->GetStringUTFChars(messageObj, NULL);
    }
    
    if (callback) {
        callback(errorCode, std::string(errorMessage));
    }
    
    NendLogger::logError(StringUtils::format("loadAd error: %d, description: %s", errorCode, errorMessage));
    
    if (messageObj != NULL && errorMessage != NULL){
        env->ReleaseStringUTFChars(messageObj, errorMessage);
    }
}

#pragma mark - loadAd
void AndroidNativeAdClient::loadAd(const std::function<void (NendNativeAd*, int*, std::string)> &callback)
{
    callbacks.push_back(callback);

    std::function<void (JNIEnv* env)> callback_ = [=](JNIEnv* env) {
        jclass cls = env->FindClass(CLASS_NAME);
        
        if (cls == NULL) {
            return;
        }

        if (m_nativeClient == NULL) {
            return;
        }
        
        jmethodID mid = env->GetMethodID(cls, METHOD_NAME_LOAD, "()V");
        if (mid != NULL) {
            env->CallVoidMethod(m_nativeClient, mid);
        }
    };
    this->callJNI(callback_);
}

void AndroidNativeAdClient::enableAutoReload(const int interval, const std::function<void (NendNativeAd*, int*, std::string)> &callback){
    s_auto_reload_callback = callback;
    
    std::function<void (JNIEnv* env)> callback_ = [=](JNIEnv* env) {
        jclass cls = env->FindClass(CLASS_NAME);
        
        if (cls == NULL) {
            return;
        }
        
        if (m_nativeClient == NULL) {
            return;
        }
        
        jmethodID mid = env->GetMethodID(cls, METHOD_NAME_ENABLE_AUTO_LOAD, "(I)V");
        if (mid != NULL) {
            env->CallVoidMethod(m_nativeClient, mid, interval * 1000);
        }
    };
    this->callJNI(callback_);
}

void AndroidNativeAdClient::disableAutoReload()
{
    std::function<void (JNIEnv* env)> callback_ = [=](JNIEnv* env) {
        jclass cls = env->FindClass(CLASS_NAME);
        
        if (cls == NULL) {
            return;
        }
        
        if (m_nativeClient == NULL) {
            return;
        }
        
        jmethodID mid = env->GetMethodID(cls, METHOD_NAME_DISABLE_AUTO_LOAD, "()V");
        if (mid != NULL) {
            env->CallVoidMethod(m_nativeClient, mid);
        }
    };
    this->callJNI(callback_);
}

#pragma mark - JNICALL
extern "C"
{
    JNIEXPORT void JNICALL Java_net_nend_NendModule_NendNativeAdClient_getNativeAdConnectorJNICallBack(JNIEnv* env, jobject thiz, jobject nativeAdConnector, jobject activity) {
        std::function<void (AndroidNativeAd* nativeAd)> callback_ = [=](AndroidNativeAd* nativeAd) {
            auto callback = callbacks.front();
            callback(nativeAd, nullptr, "");
            callbacks.erase(callbacks.begin());
        };
        if (callbacks.empty()) {
            return;
        }
        createNendNativeAd(env, nativeAdConnector, activity, callback_);
    }
    
	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendNativeAdClient_loadAdFailedJNICallBack(JNIEnv* env, jobject thiz, jobject errorObject){
        std::function<void (int errorInt, std::string message)> callback_ = [=](int errorInt, std::string message) {
            auto callback = callbacks.front();
            callback(nullptr, &errorInt, message);
            callbacks.erase(callbacks.begin());

        };
        if (callbacks.empty()) {
            return;
        }
        loadAdFailed(env, errorObject, callback_);
	}

    JNIEXPORT void JNICALL Java_net_nend_NendModule_NendNativeAdClient_getNativeAdConnectorForAutoreloadJNICallBack(JNIEnv* env, jobject thiz, jobject nativeAdConnector, jobject activity) {
        std::function<void (AndroidNativeAd* nativeAd)> callback_ = [=](AndroidNativeAd* nativeAd) {
            s_auto_reload_callback(nativeAd, nullptr, "");
        };
        if (!s_auto_reload_callback) {
            return;
        }
        createNendNativeAd(env, nativeAdConnector, activity, callback_);
    }
    
    JNIEXPORT void JNICALL Java_net_nend_NendModule_NendNativeAdClient_loadAdFailedForAutoreloadJNICallBack(JNIEnv* env, jobject thiz, jobject errorObject){
        std::function<void (int errorInt, std::string message)> callback_ = [=](int errorInt, std::string message) {
            s_auto_reload_callback(nullptr, &errorInt, message);
        };
        if (!s_auto_reload_callback) {
            return;
        }
        loadAdFailed(env, errorObject, callback_);
    }
}
