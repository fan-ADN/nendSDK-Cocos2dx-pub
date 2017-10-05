#include "cocos2d.h"
#include "NendInterstitialVideoAd.h"

#include "NendVideoAdJni.h"

NS_NEND_BEGIN

class InterstitialVideoAd::Impl {
public:
    Impl(InterstitialVideoAd* videoAd, const std::string& spotId, const std::string& apiKey, int id);

    virtual ~Impl();

    void postEventCallback(EventType event);

    jobject getInterstitialVideoAd() const {
        return interstitialVideoAd_;
    }

    void setErrorCode(int errorCode) {
        errorCode_ = errorCode;
    }

private:
    int id_;
    InterstitialVideoAd* outer_;
    jobject interstitialVideoAd_;
    int errorCode_;
};

using Event = nend_module::video::VideoAd<nend_module::video::InterstitialVideoAd>::EventType;
std::unordered_map<int, nend_module::video::InterstitialVideoAd::Impl*> sInterstitialVideoAdMap;

InterstitialVideoAd::Impl::Impl(InterstitialVideoAd* videoAd, const std::string& spotId, const std::string& apiKey,
                                int id) : outer_(videoAd), id_(id), interstitialVideoAd_(nullptr) {
    jobject ret = VideoAdJni::createInterstitialVideoAd(spotId, apiKey, id);
    if (ret != nullptr) {
        interstitialVideoAd_ = cocos2d::JniHelper::getEnv()->NewGlobalRef(ret);
        CCLOG("InterstitialVideoAd::Impl = %d", id);
        JniUtils::insertCallbackObject(id, this, sInterstitialVideoAdMap);
    }
}

InterstitialVideoAd::Impl::~Impl() {
    CCLOG("Destruct InterstitialVideoAd::Impl %p", this);
    JniUtils::eraseCallbackObject(id_, sInterstitialVideoAdMap);
    if (interstitialVideoAd_ != nullptr) {
        cocos2d::JniHelper::getEnv()->DeleteGlobalRef(interstitialVideoAd_);
        interstitialVideoAd_ = nullptr;
    }
}

void InterstitialVideoAd::Impl::postEventCallback(Event event) {
    switch (event) {
        case Event::LOADED:
            if (outer_->onLoaded_ != nullptr) {
                outer_->onLoaded_();
            }
            break;
        case Event::FAILED_TO_LOAD:
            if (outer_->onFailedToLoad_ != nullptr) {
                outer_->onFailedToLoad_(errorCode_);
            }
            break;
        case Event::SHOWN:
            if (outer_->onShown_ != nullptr) {
                outer_->onShown_();
            }
            break;
        case Event::STARTED:
            if (outer_->onStarted_ != nullptr) {
                outer_->onStarted_();
            }
            break;
        case Event::STOPPED:
            if (outer_->onStopped_ != nullptr) {
                outer_->onStopped_();
            }
            break;
        case Event::COMPLETED:
            if (outer_->onCompleted_ != nullptr) {
                outer_->onCompleted_();
            }
            break;
        case Event::CLOSED:
            if (outer_->onClosed_ != nullptr) {
                outer_->onClosed_();
            }
            break;
        case Event::FAILED_TO_PLAY:
            if (outer_->onFailedToPlay_ != nullptr) {
                outer_->onFailedToPlay_();
            }
            break;
        case Event::AD_CLICKED:
            if (outer_->onAdClicked_ != nullptr) {
                outer_->onAdClicked_();
            }
            break;
        case Event::INFORMATION_CLICKED:
            if (outer_->onInformationClicked_ != nullptr) {
                outer_->onInformationClicked_();
            }
            break;
        default:
            break;
    }
}

#pragma mark -

InterstitialVideoAd::InterstitialVideoAd(const std::string& spotId, const std::string& apiKey, bool isOutputLog)
        : VideoAd<InterstitialVideoAd>() {
    impl_.reset(new InterstitialVideoAd::Impl(this, spotId, apiKey, JniUtils::getNextId()));
}

InterstitialVideoAd::~InterstitialVideoAd() {
    CCLOG("Destruct InterstitialVideoAd %p", this);
    destroyImpl();
}

void InterstitialVideoAd::loadAdImpl() {
    VideoAdJni::loadAd(impl_->getInterstitialVideoAd());
}

bool InterstitialVideoAd::isReadyImpl() {
    return VideoAdJni::isReady(impl_->getInterstitialVideoAd());
}

void InterstitialVideoAd::showAdImpl() {
    VideoAdJni::showAd(impl_->getInterstitialVideoAd());
}

void InterstitialVideoAd::setUserIdImpl(const std::string& userId) {
    VideoAdJni::setUserId(impl_->getInterstitialVideoAd(), userId);
}

void InterstitialVideoAd::setMediationNameImpl(const std::string& mediationName) {
    VideoAdJni::setMediationName(impl_->getInterstitialVideoAd(), mediationName);
}

void InterstitialVideoAd::destroyImpl() {
    if (isDestroyNeeded()) {
        VideoAdJni::destroy(impl_->getInterstitialVideoAd());
    }
}

void InterstitialVideoAd::addFallbackFullBoard(const std::string& spotId, const std::string& apiKey) {
    VideoAdJni::addFallbackFullBoard(impl_->getInterstitialVideoAd(), spotId, apiKey);
}

extern "C" {

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdLoaded(JNIEnv* env, jobject thiz, jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::LOADED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdFailedToLoad(JNIEnv* env, jobject thiz, jint id,
                                                                              jint errorCode) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->setErrorCode(errorCode);
        impl->postEventCallback(Event::FAILED_TO_LOAD);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdShown(JNIEnv* env, jobject thiz, jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::SHOWN);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdStarted(JNIEnv* env, jobject thiz, jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::STARTED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdStopped(JNIEnv* env, jobject thiz, jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::STOPPED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdCompleted(JNIEnv* env, jobject thiz, jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::COMPLETED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdClosed(JNIEnv* env, jobject thiz, jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::CLOSED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdFailedToPlay(JNIEnv* env, jobject thiz, jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::FAILED_TO_PLAY);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdClicked(JNIEnv* env, jobject thiz, jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::AD_CLICKED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnInterstitialVideoAdInformationClicked(JNIEnv* env, jobject thiz,
                                                                                    jint id) {
    InterstitialVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sInterstitialVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::INFORMATION_CLICKED);
    }
}

}
NS_NEND_END
