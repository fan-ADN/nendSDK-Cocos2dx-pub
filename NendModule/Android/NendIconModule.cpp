// NendIconModule.cpp

#include "NendIconModule.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#define CLASS_NAME "net/nend/NendModule/NendIconModule"
static const int DEFAULT_ICON_COUNT = 4;
static const int MAX_ICON_COUNT     = 6;

//シングルトンの設定
NendIconModule* NendIconModule::m_mySingleton = NULL;

NendIconModule::NendIconModule()
{
}

NendIconModule* NendIconModule::sharedNendIconModule(){

    if (NULL == m_mySingleton) {
        //クラス未生成の場合は生成する
        m_mySingleton = new NendIconModule();
    }

    return m_mySingleton;
}

//IconLoaderの生成
void NendIconModule::createNADIconLoader(char* apiKey, char* spotID){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createIconLoader", "(Ljava/lang/String;Ljava/lang/String;)V")) {
		jstring stringApiKey = t.env->NewStringUTF(apiKey);
		jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringApiKey, stringSpotID);
		t.env->DeleteLocalRef(stringApiKey);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

// アイコンの表示位置の座標を計算する
cocos2d::Point NendIconModule::getIconViewPosition(cocos2d::Point pos){
    //高さをandroid基準に変換する
    Size size = Director::getInstance()->getWinSize();

    //座標を割合で取得
    cocos2d::Point pointRate;
    pointRate.x = (pos.x / size.width);
    pointRate.y = (pos.y / size.height);

    //端末の画面サイズを取得
    auto director = Director::getInstance();
    auto pEGLView = director->getOpenGLView();

    Size frameSize = pEGLView->getFrameSize();
    cocos2d::Point newPosition;
    newPosition.x = frameSize.width * pointRate.x;
    newPosition.y = frameSize.height - (frameSize.height * pointRate.y);
    return newPosition;
}

//IconViewの生成(位置指定)
void NendIconModule::createNADIconView(cocos2d::Point pos){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createIconView", "(FF)V")) {
		cocos2d::Point newPosition = NendIconModule::getIconViewPosition(pos);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, newPosition.x, newPosition.y);
		t.env->DeleteLocalRef(t.classID);
	}
}

//IconViewの生成(位置指定、spotID指定、タイトル表示指定、余白指定、文字色指定)
void NendIconModule::createNADIconView(cocos2d::Point pos, char* spotID, bool isVisible, bool isSpace, Color3B titleColor){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createIconView", "(FFLjava/lang/String;ZZIII)V")) {
		cocos2d::Point newPosition = NendIconModule::getIconViewPosition(pos);
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, newPosition.x, newPosition.y, stringSpotID, isVisible, isSpace, titleColor.r, titleColor.g, titleColor.b);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//IconViewの生成(画面下部)
void NendIconModule::createNADIconViewBottom(){
	NendIconModule::createNADIconViewBottom(DEFAULT_ICON_COUNT);
}

//IconViewの生成(画面下部、アイコン数指定)
void NendIconModule::createNADIconViewBottom(int iconCount){
	if (iconCount > MAX_ICON_COUNT){
		iconCount = MAX_ICON_COUNT;
	}
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createIconViewBottom", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, iconCount);
		t.env->DeleteLocalRef(t.classID);
	}
}

//IconViewの生成(画面下部、spotID指定、タイトル表示指定、余白指定、文字色指定)
void NendIconModule::createNADIconViewBottom(char* spotID, bool isVisible, bool isSpace, Color3B titleColor){
	NendIconModule::createNADIconViewBottom(DEFAULT_ICON_COUNT, spotID, isVisible, isSpace, titleColor);
}

//IconViewの生成(画面下部、アイコン数指定、spotID指定、タイトル表示指定、余白指定、文字色指定)
void NendIconModule::createNADIconViewBottom(int iconCount, char* spotID, bool isVisible, bool isSpace, Color3B titleColor){
	if (iconCount > MAX_ICON_COUNT){
		iconCount = MAX_ICON_COUNT;
	}
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createIconViewBottom", "(ILjava/lang/String;ZZIII)V")) {
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, iconCount, stringSpotID, isVisible, isSpace, titleColor.r, titleColor.g, titleColor.b);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//IconViewの生成(画面上部)
void NendIconModule::createNADIconViewTop(){
	NendIconModule::createNADIconViewTop(DEFAULT_ICON_COUNT);
}

//IconViewの生成(画面上部、アイコン数指定）
void NendIconModule::createNADIconViewTop(int iconCount){
	if (iconCount > MAX_ICON_COUNT){
		iconCount = MAX_ICON_COUNT;
	}
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createIconViewTop", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, iconCount);
		t.env->DeleteLocalRef(t.classID);
	}
}

//IconViewの生成(画面上部、spotID指定、タイトル表示指定、文字色指定、余白指定)
void NendIconModule::createNADIconViewTop(char* spotID, bool isVisible, bool isSpace, Color3B titleColor){
	NendIconModule::createNADIconViewTop(DEFAULT_ICON_COUNT, spotID, isVisible, isSpace, titleColor);
}

//IconViewの生成(画面上部、アイコン数指定、spotID指定、タイトル表示指定、文字色指定、余白指定）
void NendIconModule::createNADIconViewTop(int iconCount, char* spotID, bool isVisible, bool isSpace, Color3B titleColor){
	if (iconCount > MAX_ICON_COUNT){
		iconCount = MAX_ICON_COUNT;
	}
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createIconViewTop", "(ILjava/lang/String;ZZIII)V")) {
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, iconCount, stringSpotID, isVisible, isSpace, titleColor.r, titleColor.g, titleColor.b);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//IconViewの非表示
void NendIconModule::hideNADIconView(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "hideIconView", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}
//IconViewの非表示（spotID指定）
void NendIconModule::hideNADIconView(char* spotID){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "hideIconView", "(Ljava/lang/String;)V")) {
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringSpotID);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//IconViewの再表示
void NendIconModule::showNADIconView(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showIconView", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}
//IconViewの再表示（spotID指定）
void NendIconModule::showNADIconView(char* spotID){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showIconView", "(Ljava/lang/String;)V")) {
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringSpotID);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//広告のロード
void NendIconModule::load(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadIconLoader", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}
//広告のロード（spotID指定）
void NendIconModule::load(char* spotID){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadIconLoader", "(Ljava/lang/String;)V")) {
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringSpotID);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//広告の停止
void NendIconModule::pause(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "pauseIconView", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}
//広告の停止（spotID指定）
void NendIconModule::pause(char* spotID){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "pauseIconView", "(Ljava/lang/String;)V")) {
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringSpotID);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//広告の再開
void NendIconModule::resume(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "resumeIconView", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}
//広告の再開（spotID指定）
void NendIconModule::resume(char* spotID){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "resumeIconView", "(Ljava/lang/String;)V")) {
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringSpotID);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//解放
void NendIconModule::release(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "releaseIconView", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}
//解放（spotID指定）
void NendIconModule::release(char* spotID){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "releaseIconView", "(Ljava/lang/String;)V")) {
	    jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringSpotID);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//callback処理
extern "C"
{
	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendIconModule_onReceiveIconAd(JNIEnv* env, jobject thiz) {
	    // 広告受信成功のEventDispatcher通知
		int resultCode = RECEIVE_AD_ICON;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateNotification, &resultCode);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendIconModule_onReceiveIconAdWithSpotId(JNIEnv* env, jobject thiz, jint spotId) {
	    // 広告受信成功のEventDispatcher通知
		int resultCode = RECEIVE_AD_ICON;
		int intArray[] = {resultCode, spotId};
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateWithSpotIdNotification, intArray);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendIconModule_onFailedIconAd(JNIEnv* env, jobject thiz) {
	    // 広告受信失敗のEventDispatcher通知
		int resultCode = FAIL_TO_RECEIVE_AD_ICON;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateNotification, &resultCode);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendIconModule_onFailedIconAdWithSpotId(JNIEnv* env, jobject thiz, jint spotId) {
	    // 広告受信失敗のEventDispatcher通知
		int resultCode = FAIL_TO_RECEIVE_AD_ICON;
		int intArray[] = {resultCode, spotId};
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateWithSpotIdNotification, intArray);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendIconModule_onClickIconAd(JNIEnv* env, jobject thiz) {
	    // 広告クリックのEventDispatcher通知
		int resultCode = CLICK_AD_ICON;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateNotification, &resultCode);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendIconModule_onClickIconAdWithSpotId(JNIEnv* env, jobject thiz, jint spotId) {
	    // 広告クリックのEventDispatcher通知
		int resultCode = CLICK_AD_ICON;
		int intArray[] = {resultCode, spotId};
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateWithSpotIdNotification, intArray);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendIconModule_onInformationClick(JNIEnv* env, jobject thiz) {
	    // インフォメーションボタンクリックのEventDispatcher通知
		int resultCode = CLICK_INFORMATION_ICON;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateNotification, &resultCode);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendIconModule_onInformationClickWithSpotId(JNIEnv* env, jobject thiz, jint spotId) {
	    // インフォメーションボタンクリックのEventDispatcher通知
		int resultCode = CLICK_INFORMATION_ICON;
		int intArray[] = {resultCode, spotId};
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateWithSpotIdNotification, intArray);
	}
}
