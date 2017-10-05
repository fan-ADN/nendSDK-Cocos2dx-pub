package net.nend.NendModule;

import net.nend.android.NendAdInterstitialVideo;
import net.nend.android.NendAdRewardItem;
import net.nend.android.NendAdRewardedListener;
import net.nend.android.NendAdRewardedVideo;
import net.nend.android.NendAdVideo;
import net.nend.android.NendAdVideoListener;

import org.cocos2dx.lib.Cocos2dxHelper;

public class VideoAdUtils {

    public static NendAdInterstitialVideo createInterstitialVideoAd(String spotId, String apiKey, int id) {
        NendAdInterstitialVideo videoAd = new NendAdInterstitialVideo(Cocos2dxHelper.getActivity(), Integer.valueOf(spotId), apiKey);
        videoAd.setAdListener(new InterstitialVideoAdListener(id));
        return videoAd;
    }

    public static NendAdRewardedVideo createRewardedVideoAd(String spotId, String apiKey, int id) {
        NendAdRewardedVideo videoAd = new NendAdRewardedVideo(Cocos2dxHelper.getActivity(), Integer.valueOf(spotId), apiKey);
        videoAd.setAdListener(new RewardedVideoAdListener(id));
        return videoAd;
    }

    public static void loadAd(NendAdVideo videoAd) {
        videoAd.loadAd();
    }

    public static boolean isReady(NendAdVideo videoAd) {
        return videoAd.isLoaded();
    }

    public static void showAd(NendAdVideo videoAd) {
        videoAd.showAd(Cocos2dxHelper.getActivity());
    }

    public static void setUserId(NendAdVideo videoAd, String userId) {
        videoAd.setUserId(userId);
    }

    public static void setMediationName(NendAdVideo videoAd, String mediationName) {
        videoAd.setMediationName(mediationName);
    }

    public static void destroy(NendAdVideo videoAd) {
        videoAd.releaseAd();
    }

    public static void addFallbackFullBoard(NendAdInterstitialVideo videoAd, String spotId, String apiKey) {
        videoAd.addFallbackFullboard(Integer.valueOf(spotId), apiKey);
    }

    public static native void nativeOnInterstitialVideoAdLoaded(int id);

    public static native void nativeOnInterstitialVideoAdFailedToLoad(int id, int errorCode);

    public static native void nativeOnInterstitialVideoAdShown(int id);

    public static native void nativeOnInterstitialVideoAdStarted(int id);

    public static native void nativeOnInterstitialVideoAdStopped(int id);

    public static native void nativeOnInterstitialVideoAdCompleted(int id);

    public static native void nativeOnInterstitialVideoAdClosed(int id);

    public static native void nativeOnInterstitialVideoAdFailedToPlay(int id);

    public static native void nativeOnInterstitialVideoAdClicked(int id);

    public static native void nativeOnInterstitialVideoAdInformationClicked(int id);

    public static native void nativeOnRewardedVideoAdLoaded(int id);

    public static native void nativeOnRewardedVideoAdFailedToLoad(int id, int errorCode);

    public static native void nativeOnRewardedVideoAdShown(int id);

    public static native void nativeOnRewardedVideoAdStarted(int id);

    public static native void nativeOnRewardedVideoAdStopped(int id);

    public static native void nativeOnRewardedVideoAdCompleted(int id);

    public static native void nativeOnRewardedVideoAdClosed(int id);

    public static native void nativeOnRewardedVideoAdFailedToPlay(int id);

    public static native void nativeOnRewardedVideoAdClicked(int id);

    public static native void nativeOnRewardedVideoAdInformationClicked(int id);

    public static native void nativeOnRewarded(int id, String name, int amount);

    private static class InterstitialVideoAdListener implements NendAdVideoListener {

        protected final int mId;

        private InterstitialVideoAdListener(int id) {
            mId = id;
        }

        @Override
        public void onLoaded(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnLoaded();
                }
            });
        }

        @Override
        public void onFailedToLoad(NendAdVideo nendAdVideo, final int errorCode) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnFailedToLoad(errorCode);
                }
            });
        }

        @Override
        public void onFailedToPlay(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnFailedToPlay();
                }
            });
        }

        @Override
        public void onShown(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnShown();
                }
            });
        }

        @Override
        public void onClosed(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnClosed();
                }
            });
        }

        @Override
        public void onStarted(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnStarted();
                }
            });
        }

        @Override
        public void onStopped(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnStopped();
                }
            });
        }

        @Override
        public void onCompleted(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnCompleted();
                }
            });
        }

        @Override
        public void onAdClicked(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnAdClicked();
                }
            });
        }

        @Override
        public void onInformationClicked(NendAdVideo nendAdVideo) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    postNativeOnInformationClicked();
                }
            });
        }

        protected void postNativeOnLoaded() {
            nativeOnInterstitialVideoAdLoaded(mId);
        }

        protected void postNativeOnFailedToLoad(int errorCode) {
            nativeOnInterstitialVideoAdFailedToLoad(mId, errorCode);
        }

        protected void postNativeOnFailedToPlay() {
            nativeOnInterstitialVideoAdFailedToPlay(mId);
        }

        protected void postNativeOnShown() {
            nativeOnInterstitialVideoAdShown(mId);
        }

        protected void postNativeOnClosed() {
            nativeOnInterstitialVideoAdClosed(mId);
        }

        protected void postNativeOnStarted() {
            nativeOnInterstitialVideoAdStarted(mId);
        }

        protected void postNativeOnStopped() {
            nativeOnInterstitialVideoAdStopped(mId);
        }

        protected void postNativeOnCompleted() {
            nativeOnInterstitialVideoAdCompleted(mId);
        }

        protected void postNativeOnAdClicked() {
            nativeOnInterstitialVideoAdClicked(mId);
        }

        protected void postNativeOnInformationClicked() {
            nativeOnInterstitialVideoAdInformationClicked(mId);
        }

    }

    private static class RewardedVideoAdListener extends InterstitialVideoAdListener implements NendAdRewardedListener {

        private RewardedVideoAdListener(int id) {
            super(id);
        }

        @Override
        public void onRewarded(NendAdVideo nendAdVideo, final NendAdRewardItem nendAdRewardItem) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    nativeOnRewarded(mId, nendAdRewardItem.getCurrencyName(), nendAdRewardItem.getCurrencyAmount());
                }
            });
        }

        @Override
        protected void postNativeOnLoaded() {
            nativeOnRewardedVideoAdLoaded(mId);
        }

        @Override
        protected void postNativeOnFailedToLoad(int errorCode) {
            nativeOnRewardedVideoAdFailedToLoad(mId, errorCode);
        }

        @Override
        protected void postNativeOnFailedToPlay() {
            nativeOnRewardedVideoAdFailedToPlay(mId);
        }

        @Override
        protected void postNativeOnShown() {
            nativeOnRewardedVideoAdShown(mId);
        }

        @Override
        protected void postNativeOnClosed() {
            nativeOnRewardedVideoAdClosed(mId);
        }

        @Override
        protected void postNativeOnStarted() {
            nativeOnRewardedVideoAdStarted(mId);
        }

        @Override
        protected void postNativeOnStopped() {
            nativeOnRewardedVideoAdStopped(mId);
        }

        @Override
        protected void postNativeOnCompleted() {
            nativeOnRewardedVideoAdCompleted(mId);
        }

        @Override
        protected void postNativeOnAdClicked() {
            nativeOnRewardedVideoAdClicked(mId);
        }

        @Override
        protected void postNativeOnInformationClicked() {
            nativeOnRewardedVideoAdInformationClicked(mId);
        }
    }
}
