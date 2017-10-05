package net.nend.NendModule;

import net.nend.android.NendAdFullBoard;
import net.nend.android.NendAdFullBoardLoader;

import org.cocos2dx.lib.Cocos2dxHelper;

public class NendFullBoardAd {

    public static NendAdFullBoardLoader createFullBoardLoader(final String spotId, final String apiKey) {
        return new NendAdFullBoardLoader(Cocos2dxHelper.getActivity(), Integer.parseInt(spotId), apiKey);
    }

    public static void loadAd(final NendAdFullBoardLoader loader, final long nendFullBoardAdNativeAddress) {
        if (null == loader) {
            return;
        }
        loader.loadAd(new NendAdFullBoardLoader.Callback() {
            @Override
            public void onSuccess(NendAdFullBoard nendAdFullBoard) {
                onSuccessJNICallback(nendAdFullBoard, nendFullBoardAdNativeAddress);
            }

            @Override
            public void onFailure(NendAdFullBoardLoader.FullBoardAdError fullBoardAdError) {
                onFailureJNICallback(fullBoardAdError.ordinal() + 1, nendFullBoardAdNativeAddress);
            }
        });
    }

    public static void showAd(final NendAdFullBoard nendAdFullBoard, final long nendFullBoardAdNativeAddress) {
        if (null == nendAdFullBoard) {
            return;
        }
        nendAdFullBoard.setAdListener(new NendAdFullBoard.FullBoardAdListener() {
            @Override
            public void onShowAd(NendAdFullBoard nendAdFullBoard) {
                onShowAdJNICallback(nendAdFullBoard, nendFullBoardAdNativeAddress);
            }

            @Override
            public void onDismissAd(NendAdFullBoard nendAdFullBoard) {
                onDismissAdJNICallback(nendAdFullBoard, nendFullBoardAdNativeAddress);
            }

            @Override
            public void onClickAd(NendAdFullBoard nendAdFullBoard) {
                onClickAdJNICallback(nendAdFullBoard, nendFullBoardAdNativeAddress);
            }
        });

        nendAdFullBoard.show(Cocos2dxHelper.getActivity());

    }

    public static native void onSuccessJNICallback(final NendAdFullBoard nendAdFullBoard, final long nendFullBoardAdNativeAddress);

    public static native void onFailureJNICallback(final int fullBoardAdError, final long nendFullBoardAdNativeAddress);

    public static native void onShowAdJNICallback(final NendAdFullBoard nendAdFullBoard, final long nendFullBoardAdNativeAddress);

    public static native void onDismissAdJNICallback(final NendAdFullBoard nendAdFullBoard, final long nendFullBoardAdNativeAddress);

    public static native void onClickAdJNICallback(final NendAdFullBoard nendAdFullBoard, final long nendFullBoardAdNativeAddress);
}
