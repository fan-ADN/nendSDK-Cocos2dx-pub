package net.nend.NendModule;

import android.app.Activity;
import android.graphics.Point;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import net.nend.android.NendAdInformationListener;
import net.nend.android.NendAdView;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.util.HashMap;

public class NendModule {

    private static HashMap<String, NendAdView> mNendAdHashMap = new HashMap<>();
    private static ViewGroup sRootView = null;
    private static NendAdInformationListener sAdListener = new NendAdInformationListener() {
        @Override
        public void onInformationButtonClick(NendAdView nendAdView) {
            EventListener(EventListenerType.onClickInformation, nendAdView);
        }

        @Override
        public void onReceiveAd(NendAdView nendAdView) {
            EventListener(EventListenerType.onReceiveAd, nendAdView);
        }

        @Override
        public void onFailedToReceiveAd(NendAdView nendAdView) {
            EventListener(EventListenerType.onFailedToReceiveAd, nendAdView);
        }

        @Override
        public void onClick(NendAdView nendAdView) {
            EventListener(EventListenerType.onClick, nendAdView);
        }

        @Override
        public void onDismissScreen(NendAdView nendAdView) {
            // 未サポート
        }
    };

    private static void releaseViews(NendAdView holdNendAdView) {
        holdNendAdView.removeAllViews();
        holdNendAdView.removeListener();
        if (null != sRootView) {
            sRootView.removeView(holdNendAdView);
        }
    }

    private static void createNADView(final String apiKey, final String spotID, final boolean isAdjust, final LayoutType type, final Point pos) {

        final Activity activity = (Activity) Cocos2dxActivity.getContext();
        activity.runOnUiThread(new Runnable() {
            public void run() {

                if (null == sRootView) {
                    sRootView = new FrameLayout(activity);
                }
                if (null == sRootView.getParent()) {
                    activity.addContentView(sRootView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
                }

                if (!mNendAdHashMap.containsKey(spotID)) {
                    int intSpotID = Integer.parseInt(spotID);
                    NendAdView nendAdView = new NendAdView(activity, intSpotID, apiKey, isAdjust);
                    nendAdView.setListener(sAdListener);
                    nendAdView.setTag(spotID);
                    nendAdView.loadAd();

                    mNendAdHashMap.put(spotID, nendAdView);

                    FrameLayout.LayoutParams lp = null;

                    switch (type) {
                        case TOP:
                            lp = new FrameLayout.LayoutParams(
                                    ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, Gravity.CENTER_HORIZONTAL | Gravity.TOP);
                            break;
                        case BOTTOM:
                            lp = new FrameLayout.LayoutParams(
                                    ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, Gravity.CENTER_HORIZONTAL | Gravity.BOTTOM);
                            break;
                        case USER:
                            lp = new FrameLayout.LayoutParams(
                                    ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, Gravity.LEFT | Gravity.TOP);
                            lp.leftMargin = pos.x;
                            lp.topMargin = pos.y;
                            break;
                    }
                    nendAdView.setLayoutParams(lp);
                    sRootView.addView(nendAdView);
                }
            }
        });
    }

    private static void selectAdState(NendAdView nendAdView, final NendAdState state) {
        if (nendAdView != null) {
            switch (state) {
                case SHOW:
                    nendAdView.setVisibility(View.VISIBLE);
                    break;
                case HIDE:
                    nendAdView.setVisibility(View.GONE);
                    break;
                case PAUSE:
                    nendAdView.pause();
                    break;
                case RESUME:
                    nendAdView.resume();
                    break;
                case RELEASE:
                    releaseViews(nendAdView);
                    break;
                default:
                    break;
            }
        }
    }

    private static void controlAdState(final String spotID, final NendAdState state) {
        final Activity activity = (Activity) Cocos2dxActivity.getContext();
        activity.runOnUiThread(new Runnable() {
            public void run() {
                if (spotID != null) {
                    NendAdView holdNendAdView = mNendAdHashMap.get(spotID);
                    selectAdState(holdNendAdView, state);
                    if (state == NendAdState.RELEASE) {
                        mNendAdHashMap.remove(spotID);
                    }
                } else {
                    for (NendAdView holdNendAdView : mNendAdHashMap.values()) {
                        selectAdState(holdNendAdView, state);
                    }
                    if (state == NendAdState.RELEASE) {
                        mNendAdHashMap.clear();
                    }
                }
            }
        });
    }

    // 広告の初期化(位置指定)
    public static void createAdView(final String apiKey, final String spotID, final float x, final float y, final boolean isAdjust) {
        createNADView(apiKey, spotID, isAdjust, LayoutType.USER, new Point((int) x, (int) y));
    }

    // 広告の初期化(画面下部中央)
    public static void createAdViewBottom(final String apiKey, final String spotID, final boolean isAdjust) {
        createNADView(apiKey, spotID, isAdjust, LayoutType.BOTTOM, new Point(0, 0));
    }

    // 広告の初期化(画面上部中央)
    public static void createAdViewTop(final String apiKey, final String spotID, final boolean isAdjust) {
        createNADView(apiKey, spotID, isAdjust, LayoutType.TOP, new Point(0, 0));
    }

    // 広告の非表示
    public static void hideAdView() {
        hideAdView(null);
    }

    // 広告の非表示(spotID指定)
    public static void hideAdView(final String spotID) {
        controlAdState(spotID, NendAdState.HIDE);
    }

    // 広告の再表示
    public static void showAdView() {
        showAdView(null);
    }

    // 広告の再表示(spotID指定)
    public static void showAdView(final String spotID) {
        controlAdState(spotID, NendAdState.SHOW);
    }

    // 広告の一時停止
    public static void pauseAdView() {
        pauseAdView(null);
    }

    // 広告の一時停止(spotID)
    public static void pauseAdView(final String spotID) {
        controlAdState(spotID, NendAdState.PAUSE);
    }

    // 広告の再開
    public static void resumeAdView() {
        resumeAdView(null);
    }

    // 広告の再開(spotID指定)
    public static void resumeAdView(final String spotID) {
        controlAdState(spotID, NendAdState.RESUME);
    }

    // 解放
    public static void releaseAdView() {
        releaseAdView(null);
    }

    // 解放(spotID指定)
    public static void releaseAdView(final String spotID) {
        controlAdState(spotID, NendAdState.RELEASE);
    }

    private static void EventListener(EventListenerType type, NendAdView adView) {

        String keySpotID = (String) adView.getTag();
        NendAdView holdNendAdView = mNendAdHashMap.get(keySpotID);
        if (holdNendAdView != null) {
            int intSpotID = Integer.parseInt(keySpotID);

            switch (type) {
                case onReceiveAd:
                    NendModule.onReceiveAd();
                    NendModule.onReceiveAdWithSpotId(intSpotID);
                    break;
                case onFailedToReceiveAd:
                    NendModule.onFailedToReceiveAd();
                    NendModule.onFailedToReceiveAdWithSpotId(intSpotID);
                    break;
                case onClick:
                    NendModule.onClick();
                    NendModule.onClickWithSpotId(intSpotID);
                    break;
                case onClickInformation:
                    NendModule.onClickInformation();
                    NendModule.onClickInformationWithSpotId(intSpotID);
                    break;
                default:
                    break;
            }
        }
    }

    public static native void onReceiveAd();

    public static native void onReceiveAdWithSpotId(final int spotId);

    public static native void onFailedToReceiveAd();

    public static native void onFailedToReceiveAdWithSpotId(final int spotId);

    public static native void onClick();

    public static native void onClickWithSpotId(final int spotId);

    public static native void onClickInformation();

    public static native void onClickInformationWithSpotId(final int spotId);

    enum LayoutType {
        TOP,
        BOTTOM,
        USER
    }

    enum NendAdState {
        HIDE,
        SHOW,
        PAUSE,
        RESUME,
        RELEASE
    }

    enum EventListenerType {
        onReceiveAd,
        onFailedToReceiveAd,
        onClick,
        onClickInformation
    }
}
