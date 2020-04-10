package com.ldlywt.ffmpegstudy;

import android.text.TextUtils;


/**
 * Created by hcDarren on 2019/6/15.
 * 音频播放器的逻辑处理类
 */
public class NativeUtil {
    static {
        System.loadLibrary("voiceStudy");
    }

    /**
     * url 可以是本地文件路径，也可以是 http 链接
     */
    private String url;

    public NativeUtil() {
        this.nativeHandle = nativeInit();
    }

    private final long nativeHandle;

    private OnErrorListener mErrorListener;

    public void setOnErrorListener(OnErrorListener mErrorListener) {
        this.mErrorListener = mErrorListener;
    }

    public void setDataSource(String url) {
        this.url = url;
    }

    public void play() {
        if (TextUtils.isEmpty(url)) {
            throw new NullPointerException("url is null, please call method setDataSource");
        }

        playMusic(url,nativeHandle);
    }

    public void pause() {

    }

    public void stop() {

    }


    private native long nativeInit();

    private native void prepare(long nativeHandle);

    private native void playMusic(String url,long nativeHandle);

    public native void pauseMusic();

    public static native void changeVoice(String path, int mode);


    //--------------------called from jni---------------------
    private void onError(int code, String msg) {
        if (mErrorListener != null) {
            mErrorListener.onError(code, msg);
        }
    }


    private void onPrepare() {
        if (null != onPrepareListener) {
            onPrepareListener.onPrepared();
        }
    }

    private void onProgress(int progress) {
        if (null != onProgressListener) {
            onProgressListener.onProgress(progress);
        }
    }

    private OnErrorListener onErrorListener;
    private OnProgressListener onProgressListener;
    private OnPrepareListener onPrepareListener;


    public interface OnErrorListener {
        void onError(int code, String msg);
    }

    public interface OnPrepareListener {
        void onPrepared();
    }

    public interface OnProgressListener {
        void onProgress(int progress);
    }
}
