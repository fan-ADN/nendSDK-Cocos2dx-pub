//
//  NendVideoAd.cpp
//  NendModuleProj
//

#include "NendVideoAd.h"

#include "NendInterstitialVideoAd.h"
#include "NendRewardedVideoAd.h"

#define Impl static_cast<T*>(this)

NS_NEND_BEGIN

template <class T>
VideoAd<T>::VideoAd():
        onLoaded_(nullptr),
        onFailedToLoad_(nullptr),
        onShown_(nullptr),
        onStarted_(nullptr),
        onStopped_(nullptr),
        onCompleted_(nullptr),
        onClosed_(nullptr),
        onFailedToPlay_(nullptr),
        onAdClicked_(nullptr),
        onInformationClicked_(nullptr){}

template <class T>
void VideoAd<T>::loadAd() {
    Impl->loadAdImpl();
}

template <class T>
bool VideoAd<T>::isReady() {
    return Impl->isReadyImpl();
}

template <class T>
void VideoAd<T>::showAd() {
    Impl->showAdImpl();
}

template <class T>
void VideoAd<T>::setUserId(const std::string& userId) {
    Impl->setUserIdImpl(userId);
}

template <class T>
void VideoAd<T>::setUserFeature(NendUserFeature *userFeature) {
    Impl->setUserFeatureImpl(userFeature);
}

template <class T>
void VideoAd<T>::setMediationName(const std::string& mediationName) {
    Impl->setMediationNameImpl(mediationName);
}

template <class T>
void VideoAd<T>::destroy() {
    Impl->destroyImpl();
}

template <class T>
bool VideoAd<T>::isDestroyNeeded() {
    bool expected = false;
    return destroyed_.compare_exchange_weak(expected, true);
}

template class VideoAd<InterstitialVideoAd>;
template class VideoAd<RewardedVideoAd>;

NS_NEND_END
