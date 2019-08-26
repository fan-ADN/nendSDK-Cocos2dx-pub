package net.nend.NendModule;

import android.app.Activity;

import net.nend.android.NendAdNative;
import net.nend.android.NendAdNativeClient;
import net.nend.android.internal.connectors.NendNativeAdConnector;
import net.nend.android.internal.connectors.NendNativeAdConnectorFactory;

import org.cocos2dx.lib.Cocos2dxActivity;

public class NendNativeAdClient {

    private NendAdNativeClient mClient;
    private String mSpotId;

    public static native void getNativeAdConnectorJNICallBack(final NendNativeAdConnector connector, final Activity activity);
    public static native void loadAdFailedJNICallBack(final NendAdClientErrorData errorData);
    public static native void getNativeAdConnectorForAutoreloadJNICallBack(final NendNativeAdConnector connector, final Activity activity);
    public static native void loadAdFailedForAutoreloadJNICallBack(final NendAdClientErrorData errorData);

    public void createNendAdNativeClient(final String apiKey, final String spotID) {
        final Activity activity = (Activity) Cocos2dxActivity.getContext();
        int intSpotID = Integer.parseInt(spotID);
        mSpotId = spotID;
        mClient = new NendAdNativeClient(activity, intSpotID, apiKey);
    }

    public void loadAd() {

        if (mClient == null) {
            return;
        }

        mClient.loadAd(new NendAdNativeClient.Callback() {
            @Override
            public void onSuccess(final NendAdNative nendAdNative) {
                NendNativeAdConnector connector = NendNativeAdConnectorFactory.createNativeAdConnector(nendAdNative);
                connector.setSpotId(mSpotId);
                final Activity activity = (Activity) Cocos2dxActivity.getContext();
                NendNativeAdClient.getNativeAdConnectorJNICallBack(connector, activity);
            }

            @Override
            public void onFailure(NendAdNativeClient.NendError nendError) {
                NendAdClientErrorData errorData = new NendAdClientErrorData();
                errorData.errorCode = nendError.getCode();
                errorData.errorMessage = nendError.getMessage();
                NendNativeAdClient.loadAdFailedJNICallBack(errorData);
            }
        });

    }

    public void enableAutoReload(final int intervalMillis) {

        if (mClient == null) {
            return;
        }

        mClient.enableAutoReload(intervalMillis, new NendAdNativeClient.Callback() {
            @Override
            public void onSuccess(final NendAdNative nendAdNative) {
                NendNativeAdConnector connector = NendNativeAdConnectorFactory.createNativeAdConnector(nendAdNative);
                connector.setSpotId(mSpotId);
                final Activity activity = (Activity) Cocos2dxActivity.getContext();
                NendNativeAdClient.getNativeAdConnectorForAutoreloadJNICallBack(connector, activity);
            }

            @Override
            public void onFailure(NendAdNativeClient.NendError nendError) {
                NendAdClientErrorData errorData = new NendAdClientErrorData();
                errorData.errorCode = nendError.getCode();
                errorData.errorMessage = nendError.getMessage();
                NendNativeAdClient.loadAdFailedForAutoreloadJNICallBack(errorData);
            }
        });
    }

    public void disableAutoReload() {
        if (mClient == null) {
            return;
        }
        mClient.disableAutoReload();
    }

    class NendAdClientErrorData {
        int errorCode;
        String errorMessage;
    }
}
