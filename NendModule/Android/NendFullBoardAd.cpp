//
//  NendFullBoardAd.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendFullBoardAd.h"
#include "platform/android/jni/JniHelper.h"
#include "NendNativeAdLog.h"
#define CLASS_NAME "net/nend/NendModule/NendFullBoardAd"
using namespace nend_module;

class NendFullBoardAd::NendFullBoardAdInner
{
private:
    jobject m_nendFullBoardLoader;
    jobject m_nendFullBoard;
public:
    NendFullBoardAdInner(const std::string spotID, const std::string apiKey)
    {
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createFullBoardLoader", "(Ljava/lang/String;Ljava/lang/String;)Lnet/nend/android/NendAdFullBoardLoader;")) {
            jstring stringApiKey = t.env->NewStringUTF(apiKey.c_str());
            jstring stringSpotID = t.env->NewStringUTF(spotID.c_str());
            m_nendFullBoardLoader = t.env->NewGlobalRef(t.env->CallStaticObjectMethod(t.classID, t.methodID, stringSpotID, stringApiKey));
            t.env->DeleteLocalRef(stringApiKey);
            t.env->DeleteLocalRef(stringSpotID);
            t.env->DeleteLocalRef(t.classID);
        }
        m_nendFullBoard = NULL;
    }
    virtual ~NendFullBoardAdInner()
    {
        NendNativeAdLog::logDebug(__FUNCTION__);
        cocos2d::JniHelper::getEnv()->DeleteGlobalRef(m_nendFullBoard);
        cocos2d::JniHelper::getEnv()->DeleteGlobalRef(m_nendFullBoardLoader);
    }
    
    void setNendFullBoard(JNIEnv* env, jobject nadFullBoard)
    {
        m_nendFullBoard = env->NewGlobalRef(nadFullBoard);
    }
    
    jobject getNendFullBoard()
    {
        return m_nendFullBoard;
    }

    jobject getNendFullBoardLoader()
    {
        return m_nendFullBoardLoader;
    }
};

#pragma mark - Constructor / Destructor
NendFullBoardAd::NendFullBoardAd(const std::string& spotID, const std::string& apiKey)
{
    m_Inner = new NendFullBoardAdInner(spotID, apiKey);
}

NendFullBoardAd::~NendFullBoardAd()
{
    NendNativeAdLog::logDebug(__FUNCTION__);
    CC_SAFE_DELETE(m_Inner);
}

void NendFullBoardAd::load()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadAd", "(Lnet/nend/android/NendAdFullBoardLoader;J)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, m_Inner->getNendFullBoardLoader(), (jlong)this);
        t.env->DeleteLocalRef(t.classID);
    }
}

void NendFullBoardAd::show()
{
    if (NULL != m_Inner->getNendFullBoard()) {
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showAd", "(Lnet/nend/android/NendAdFullBoard;J)V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID, m_Inner->getNendFullBoard(), (jlong)this);
            t.env->DeleteLocalRef(t.classID);
        }
    } else {
        NendNativeAdLog::logDebug("Ad has not downloaded yet.");
    }
}

#pragma mark - JNICALL
extern "C"
{
    JNIEXPORT void JNICALL Java_net_nend_NendModule_NendFullBoardAd_onSuccessJNICallback(JNIEnv* env, jobject thiz, jobject nadFullBoard, jlong thisPtr) {
        NendFullBoardAd *nendFullBoardAd = (NendFullBoardAd*)thisPtr;
        nendFullBoardAd->m_Inner->setNendFullBoard(env, nadFullBoard);
        if (nendFullBoardAd->onFullBoardAdLoaded) {
            nendFullBoardAd->onFullBoardAdLoaded(*nendFullBoardAd);
        }
    }
    
    JNIEXPORT void JNICALL Java_net_nend_NendModule_NendFullBoardAd_onFailureJNICallback(JNIEnv* env, jobject thiz, jint fullBoardAdError, jlong thisPtr) {
        NendFullBoardAd *nendFullBoardAd = (NendFullBoardAd*)thisPtr;
        if (nendFullBoardAd->onFullBoardAdFailedToLoad) {
            nendFullBoardAd->onFullBoardAdFailedToLoad(*nendFullBoardAd, static_cast<NendFullBoardAd::ErrorType>(fullBoardAdError));
        }
    }
    
    JNIEXPORT void JNICALL Java_net_nend_NendModule_NendFullBoardAd_onShowAdJNICallback(JNIEnv* env, jobject thiz, jobject nadFullBoard, jlong thisPtr) {
        NendFullBoardAd *nendFullBoardAd = (NendFullBoardAd*)thisPtr;
        if (nendFullBoardAd->onFullBoardAdShown) {
            nendFullBoardAd->onFullBoardAdShown(*nendFullBoardAd);
        }
    }
    JNIEXPORT void JNICALL Java_net_nend_NendModule_NendFullBoardAd_onDismissAdJNICallback(JNIEnv* env, jobject thiz, jobject nadFullBoard, jlong thisPtr) {
        NendFullBoardAd *nendFullBoardAd = (NendFullBoardAd*)thisPtr;
        if (nendFullBoardAd->onFullBoardAdDismissed) {
            nendFullBoardAd->onFullBoardAdDismissed(*nendFullBoardAd);
        }
    }
    JNIEXPORT void JNICALL Java_net_nend_NendModule_NendFullBoardAd_onClickAdJNICallback(JNIEnv* env, jobject thiz, jobject nadFullBoard, jlong thisPtr) {
        NendFullBoardAd *nendFullBoardAd = (NendFullBoardAd*)thisPtr;
        if (nendFullBoardAd->onFullBoardAdClicked) {
            nendFullBoardAd->onFullBoardAdClicked(*nendFullBoardAd);
        }
    }
}
