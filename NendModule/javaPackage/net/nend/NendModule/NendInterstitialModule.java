package net.nend.NendModule;

import android.app.Activity;

import net.nend.android.NendAdInterstitial;
import net.nend.android.NendAdInterstitial.NendAdInterstitialClickType;
import net.nend.android.NendAdInterstitial.NendAdInterstitialShowResult;
import net.nend.android.NendAdInterstitial.NendAdInterstitialStatusCode;

import org.cocos2dx.lib.Cocos2dxActivity;

public class NendInterstitialModule {

    // クリックイベントのタイプが返却される
    public static NendAdInterstitial.OnClickListener setOnClickListener() {

        return new NendAdInterstitial.OnClickListener() {

            @Override
            public void onClick(NendAdInterstitialClickType clickType) {
                int resultCode = clickType.ordinal();
                onClickStatus(resultCode);
            }
        };
    }

    // クリックイベントのタイプとspotIDが返却される
    public static NendAdInterstitial.OnClickListener setOnClickListenerWithSpotId() {

        return new NendAdInterstitial.OnClickListenerSpot() {

            @Override
            public void onClick(NendAdInterstitialClickType clickType) {
                // ここの通知は呼び出されない
            }

            @Override
            public void onClick(NendAdInterstitialClickType clickType, int spotId) {
                int resultCode = clickType.ordinal();
                onClickStatusWithSpotId(resultCode, spotId);
            }
        };
    }

    // ロード結果が返却される
    public static void createInterstitialAd(final String apiKey, final String spotID) {
        final Activity activity = (Activity) Cocos2dxActivity.getContext();

        activity.runOnUiThread(new Runnable() {
            public void run() {

                int intSpotID = Integer.parseInt(spotID);

                NendAdInterstitial.loadAd(activity, apiKey, intSpotID);

                // 広告のロード結果とspotIDの通知を受け取るリスナーを設定する
                NendAdInterstitial.OnCompletionListener listenerWithSpotId = new NendAdInterstitial.OnCompletionListenerSpot() {

                    @Override
                    public void onCompletion(NendAdInterstitialStatusCode statusCode) {
                        // ここの通知は呼び出されない
                    }

                    @Override
                    public void onCompletion(NendAdInterstitialStatusCode statusCode, int spotId) {
                        int resultCode = statusCode.ordinal();
                        loadResultStatus(resultCode);
                        loadResultStatusWithSpotId(resultCode, spotId);
                    }
                };
                NendAdInterstitial.setListener(listenerWithSpotId);
            }
        });
    }

    public static void showInterstitial(final String spotID) {
        final Activity activity = (Activity) Cocos2dxActivity.getContext();

        activity.runOnUiThread(new Runnable() {
            public void run() {

                if (spotID == null) {

                    NendAdInterstitial.OnClickListener listener = setOnClickListener();
                    resultOfShowAd(activity, listener);

                } else {

                    NendAdInterstitial.OnClickListener listener = setOnClickListenerWithSpotId();
                    resultOfShowAdWithSpotId(activity, spotID, listener);

                }
            }
        });
    }

    // インタースティシャル広告の生成
    public static void createInterstitial(final String apiKey, final String spotID) {
        createInterstitialAd(apiKey, spotID);
    }

    // インタースティシャル広告の表示
    public static void showInterstitialView() {
        showInterstitial(null);
    }

    // インタースティシャル広告の表示(spotID指定あり)
    public static void showInterstitialView(final String spotID) {
        showInterstitial(spotID);
    }

    // インタースティシャル広告の削除
    public static void dismissNADInterstitialView() {
        final Activity activity = (Activity) Cocos2dxActivity.getContext();
        activity.runOnUiThread(new Runnable() {
            public void run() {
                NendAdInterstitial.dismissAd();
            }
        });
    }

    public static void setIsAutoReloadEnabled(boolean isAutoReloadEnableed){
        NendAdInterstitial.isAutoReloadEnabled = isAutoReloadEnableed;
    }

    public static void resultOfShowAd(Activity activity, NendAdInterstitial.OnClickListener listener) {
        // 表示結果が返却される				
        NendAdInterstitialShowResult result = NendAdInterstitial.showAd(activity, listener);
        int resultCode = result.ordinal();
        showResultStatus(resultCode);
    }

    public static void resultOfShowAdWithSpotId(Activity activity, final String spotID, NendAdInterstitial.OnClickListener listener) {
        // 表示結果が返却される			
        int intSpotId = Integer.parseInt(spotID);
        NendAdInterstitialShowResult result = NendAdInterstitial.showAd(activity, intSpotId, listener);
        int resultCode = result.ordinal();
        showResultStatusWithSpotId(resultCode, intSpotId);
    }

    public static native void loadResultStatus(final int statusCode);

    public static native void loadResultStatusWithSpotId(final int statusCode, final int spotId);

    public static native void showResultStatus(final int statusCode);

    public static native void showResultStatusWithSpotId(final int statusCode, final int spotId);

    public static native void onClickStatus(final int statusCode);

    public static native void onClickStatusWithSpotId(final int statusCode, final int spotId);
}