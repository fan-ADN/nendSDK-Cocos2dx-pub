#ifndef NendVideoAdJni_h
#define NendVideoAdJni_h

#include "NendAdJniUtils.h"

NS_NEND_BEGIN

#define kNendVideoAdBridgeClass "net/nend/NendModule/VideoAdUtils"

class VideoAdJni {
public:
    static jobject createInterstitialVideoAd(const std::string& spotId, const std::string& apiKey, int id) {
        return JniUtils::callStaticObjectMethod(kNendVideoAdBridgeClass,
                                                "createInterstitialVideoAd",
                                                "(Ljava/lang/String;Ljava/lang/String;I)Lnet/nend/android/NendAdInterstitialVideo;",
                                                spotId, apiKey, (jint) id);
    }

    static jobject createRewardedVideoAd(const std::string& spotId, const std::string& apiKey, int id) {
        return JniUtils::callStaticObjectMethod(kNendVideoAdBridgeClass,
                                                "createRewardedVideoAd",
                                                "(Ljava/lang/String;Ljava/lang/String;I)Lnet/nend/android/NendAdRewardedVideo;",
                                                spotId, apiKey, (jint) id);
    }

    static void loadAd(const jobject& videoAd) {
        if (videoAd != nullptr) {
            JniUtils::callStaticVoidMethod(kNendVideoAdBridgeClass, "loadAd", "(Lnet/nend/android/NendAdVideo;)V", videoAd);
        }
    }

    static bool isReady(const jobject& videoAd) {
        return videoAd != nullptr &&
               JniUtils::callStaticBooleanMethod(kNendVideoAdBridgeClass, "isReady", "(Lnet/nend/android/NendAdVideo;)Z", videoAd);
    }

    static void showAd(const jobject& videoAd) {
        if (videoAd != nullptr) {
            JniUtils::callStaticVoidMethod(kNendVideoAdBridgeClass, "showAd", "(Lnet/nend/android/NendAdVideo;)V", videoAd);
        }
    }

    static void setUserId(const jobject& videoAd, const std::string& userId) {
        if (videoAd != nullptr) {
            JniUtils::callStaticVoidMethod(kNendVideoAdBridgeClass, "setUserId",
                                           "(Lnet/nend/android/NendAdVideo;Ljava/lang/String;)V", videoAd, userId);
        }
    }

    static void setMediationName(const jobject& videoAd, const std::string& mediationName) {
        if (videoAd != nullptr) {
            JniUtils::callStaticVoidMethod(kNendVideoAdBridgeClass, "setMediationName",
                                           "(Lnet/nend/android/NendAdVideo;Ljava/lang/String;)V", videoAd,
                                           mediationName);
        }
    }

    static void destroy(const jobject& videoAd) {
        if (videoAd != nullptr) {
            JniUtils::callStaticVoidMethod(kNendVideoAdBridgeClass, "destroy", "(Lnet/nend/android/NendAdVideo;)V", videoAd);
        }
    }

    static void addFallbackFullBoard(const jobject& videoAd, const std::string& spotId, const std::string& apiKey) {
        if (videoAd != nullptr) {
            JniUtils::callStaticVoidMethod(kNendVideoAdBridgeClass, "addFallbackFullBoard",
                                           "(Lnet/nend/android/NendAdInterstitialVideo;Ljava/lang/String;Ljava/lang/String;)V",
                                           videoAd, spotId, apiKey);
        }
    }
};

NS_NEND_END

#endif //NendVideoAdJni_h
