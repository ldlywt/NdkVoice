package com.ldlywt.ffmpegstudy;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

/**
 * author : wutao
 * e-mail : wutao@himango.cn
 * time   : 2019/11/29
 * desc   :
 * version: 1.0
 */
public class Utils {
    public static void readRaw(Context context) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    InputStream inputStream = context.getResources().openRawResource(R.raw.test);
                    File file = new File(Environment.getExternalStorageDirectory().getPath(), "test.mp3");
                    if (!file.exists()) {
                        file.createNewFile();
                    }
                    FileOutputStream fileOutputStream = new FileOutputStream(file);
                    byte[] buffer = new byte[10];
                    ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
                    int len = 0;
                    while ((len = inputStream.read(buffer)) != -1) {
                        outputStream.write(buffer, 0, len);
                    }
                    byte[] bs = outputStream.toByteArray();
                    fileOutputStream.write(bs);
                    outputStream.close();
                    inputStream.close();
                    fileOutputStream.flush();
                    fileOutputStream.close();
                } catch (Exception e) {
                    e.printStackTrace();
                    Log.i("getPath", e.getMessage());
                }
            }
        }).start();
    }

}
