package com.ldlywt.ffmpegstudy;

import android.text.TextUtils;

import com.ldlywt.ffmpegstudy.listener.MediaErrorListener;


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

    private MediaErrorListener mErrorListener;

    public void setOnErrorListener(MediaErrorListener mErrorListener) {
        this.mErrorListener = mErrorListener;
    }

    // called from jni
    private void onError(int code, String msg) {
        if (mErrorListener != null) {
            mErrorListener.onError(code, msg);
        }
    }

    public void setDataSource(String url) {
        this.url = url;
    }

    public void play() {
        if (TextUtils.isEmpty(url)) {
            throw new NullPointerException("url is null, please call method setDataSource");
        }

        playMusic(nativeHandle, url);
    }

    public void pause() {

    }

    public void stop() {

    }


    private native long nativeInit();

    private native void prepare(long nativeHandle);

    private native void playMusic(long nativeHandle, String url);

    public native void pauseMusic();

    public static native void changeVoice(String path, int mode);
}
