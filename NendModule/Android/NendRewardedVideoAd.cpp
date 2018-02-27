#include "cocos2d.h"
#include "NendRewardedVideoAd.h"

#include "NendVideoAdJni.h"

NS_NEND_BEGIN

class RewardedVideoAd::Impl {
public:
    Impl(RewardedVideoAd* videoAd, const std::string& spotId, const std::string& apiKey, int id);

    virtual ~Impl();

    void postEventCallback(EventType event);

    jobject getRewardedVideoAd() const {
        return rewardedVideoAd_;
    }

    void setErrorCode(int errorCode) {
        errorCode_ = errorCode;
    }

    void setCurrencyName(const std::string& name) {
        currencyName_ = name;
    }

    void setCurrencyAmount(int amount) {
        currencyAmount_ = amount;
    }

private:
    int id_;
    RewardedVideoAd* outer_;
    jobject rewardedVideoAd_;
    int errorCode_;
    std::string currencyName_;
    int currencyAmount_;
};

using Event = typename nend_module::video::VideoAd<nend_module::video::RewardedVideoAd>::EventType;
std::unordered_map<int, nend_module::video::RewardedVideoAd::Impl*> sRewardedVideoAdMap;

RewardedVideoAd::Impl::Impl(RewardedVideoAd* videoAd, const std::string& spotId, const std::string& apiKey, int id)
        : outer_(videoAd), id_(id), rewardedVideoAd_(nullptr) {
    jobject ret = VideoAdJni::createRewardedVideoAd(spotId, apiKey, id);
    if (ret != nullptr) {
        CCLOG("RewardedVideoAd::Impl = %d", id);
        rewardedVideoAd_ = cocos2d::JniHelper::getEnv()->NewGlobalRef(ret);
        JniUtils::insertCallbackObject(id, this, sRewardedVideoAdMap);
    }
}

RewardedVideoAd::Impl::~Impl() {
    CCLOG("Destruct RewardedVideoAdAndroid::Impl %p", this);
    JniUtils::eraseCallbackObject(id_, sRewardedVideoAdMap);
    if (rewardedVideoAd_ != nullptr) {
        cocos2d::JniHelper::getEnv()->DeleteGlobalRef(rewardedVideoAd_);
        rewardedVideoAd_ = nullptr;
    }
}

void RewardedVideoAd::Impl::postEventCallback(Event event) {
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
        case Event::REWARDED:
            if (outer_->onRewarded_ != nullptr) {
                outer_->onRewarded_({currencyName_, currencyAmount_});
            }
            break;
    }
}

#pragma mark -

RewardedVideoAd::RewardedVideoAd(const std::string& spotId, const std::string& apiKey, bool isOutputLog)
        : VideoAd<RewardedVideoAd>(), onRewarded_(nullptr) {
    impl_.reset(new RewardedVideoAd::Impl(this, spotId, apiKey, JniUtils::getNextId()));
}

RewardedVideoAd::~RewardedVideoAd() {
    CCLOG("Destruct RewardedVideoAd %p", this);
    destroyImpl();
}

void RewardedVideoAd::loadAdImpl() {
    VideoAdJni::loadAd(impl_->getRewardedVideoAd());
}

bool RewardedVideoAd::isReadyImpl() {
    return VideoAdJni::isReady(impl_->getRewardedVideoAd());
}

void RewardedVideoAd::showAdImpl() {
    VideoAdJni::showAd(impl_->getRewardedVideoAd());
}

void RewardedVideoAd::setUserIdImpl(const std::string& userId) {
    VideoAdJni::setUserId(impl_->getRewardedVideoAd(), userId);
}

void RewardedVideoAd::setUserFeatureImpl(NendUserFeature *userFeature) {
    VideoAdJni::setUserFeature(impl_->getRewardedVideoAd(), userFeature->buildUserFeature());
}

void RewardedVideoAd::setMediationNameImpl(const std::string& mediationName) {
    VideoAdJni::setMediationName(impl_->getRewardedVideoAd(), mediationName);
}

void RewardedVideoAd::destroyImpl() {
    if (isDestroyNeeded()) {
        VideoAdJni::destroy(impl_->getRewardedVideoAd());
    }
}

extern "C" {

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdLoaded(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::LOADED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdFailedToLoad(JNIEnv* env, jobject thiz, jint id,
                                                                          jint errorCode) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->setErrorCode(errorCode);
        impl->postEventCallback(Event::FAILED_TO_LOAD);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdShown(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::SHOWN);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdStarted(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::STARTED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdStopped(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::STOPPED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdCompleted(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::COMPLETED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdClosed(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::CLOSED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdFailedToPlay(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::FAILED_TO_PLAY);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdClicked(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::AD_CLICKED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewardedVideoAdInformationClicked(JNIEnv* env, jobject thiz, jint id) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        impl->postEventCallback(Event::INFORMATION_CLICKED);
    }
}

JNIEXPORT void JNICALL
Java_net_nend_NendModule_VideoAdUtils_nativeOnRewarded(JNIEnv* env, jobject thiz, jint id, jstring name, jint amount) {
    RewardedVideoAd::Impl* impl = JniUtils::findCallbackObject(id, sRewardedVideoAdMap);
    if (impl != nullptr) {
        const char* nameStr = env->GetStringUTFChars(name, JNI_FALSE);
        impl->setCurrencyName(nameStr);
        impl->setCurrencyAmount(amount);
        impl->postEventCallback(Event::REWARDED);
        env->ReleaseStringUTFChars(name, nameStr);
    }
}

}

NS_NEND_END
