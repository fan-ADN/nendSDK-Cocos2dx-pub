//
//  NendVideoAd.h
//  NendModuleProj
//

#ifndef NendVideoAd_h
#define NendVideoAd_h

#include <string>
#include <functional>
#include <atomic>

#include "NendVideoAdMacros.h"
#include "NendUserFeature.h"

NS_NEND_BEGIN

template <class T>
class VideoAd {
public:
    using VideoAdCallback = std::function<void()>;
    using VideoAdErrorCallback = std::function<void(int)>;

    enum class EventType : int {
        LOADED = 0,
        FAILED_TO_LOAD,
        SHOWN,
        STARTED,
        STOPPED,
        COMPLETED,
        CLOSED,
        FAILED_TO_PLAY,
        AD_CLICKED,
        INFORMATION_CLICKED,
        REWARDED
    };

    void loadAd();

    bool isReady();

    void showAd();

    void setUserId(const std::string& userId);

    void setUserFeature(NendUserFeature *userFeature);

    void setMediationName(const std::string& mediationName);

    void destroy();

    void setOnLoaded(const VideoAdCallback& callback) {
        onLoaded_ = callback;
    }

    void setOnFailedToLoad(const VideoAdErrorCallback& callback) {
        onFailedToLoad_ = callback;
    }

    void setOnShown(const VideoAdCallback& callback) {
        onShown_ = callback;
    }

    void setOnClosed(const VideoAdCallback& callback) {
        onClosed_ = callback;
    }

    void setOnStarted(const VideoAdCallback& callback) {
        onStarted_ = callback;
    }

    void setOnStopped(const VideoAdCallback& callback) {
        onStopped_ = callback;
    }

    void setOnCompleted(const VideoAdCallback& callback) {
        onCompleted_ = callback;
    }

    void setOnFailedToPlay(const VideoAdCallback& callback) {
        onFailedToPlay_ = callback;
    }

    void setOnAdClicked(const VideoAdCallback& callback) {
        onAdClicked_ = callback;
    }

    void setOnInformationClicked(const VideoAdCallback& callback) {
        onInformationClicked_ = callback;
    }
    
protected:
    VideoAd();

    bool isDestroyNeeded();
    
    VideoAdCallback onLoaded_;
    VideoAdErrorCallback onFailedToLoad_;
    VideoAdCallback onShown_;
    VideoAdCallback onClosed_;
    VideoAdCallback onStarted_;
    VideoAdCallback onStopped_;
    VideoAdCallback onCompleted_;
    VideoAdCallback onFailedToPlay_;
    VideoAdCallback onAdClicked_;
    VideoAdCallback onInformationClicked_;

private:
    std::atomic_bool destroyed_{false};
};

NS_NEND_END

#endif /* NendVideoAd_h */
