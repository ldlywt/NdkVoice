package com.ldlywt.ffmpegstudy;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;

public class ChangeVoiceActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_change_voice);
    }

    public void startChange(View view) {
//        String path = Environment.getExternalStorageDirectory().getPath()+File.separator+"voice.wav";
        String path = new File(Environment.getExternalStorageDirectory(), "test.mp3").getAbsolutePath();
        Log.i("getPath", path);
        switch (view.getId()) {
            // 普通
            case R.id.btn_normal:
                NativeUtil.changeVoice(path, 0);
                break;
            // 萝莉
            case R.id.btn_luoli:
                NativeUtil.changeVoice(path, 1);
                break;
            // 大叔
            case R.id.btn_dashu:
                NativeUtil.changeVoice(path, 2);
                break;
            // 惊悚
            case R.id.btn_jingsong:
                NativeUtil.changeVoice(path, 3);
                break;
            // 搞怪
            case R.id.btn_gaoguai:
                NativeUtil.changeVoice(path, 4);
                break;
            // 空灵
            case R.id.btn_kongling:
                NativeUtil.changeVoice(path, 5);
                break;

        }

    }

}
