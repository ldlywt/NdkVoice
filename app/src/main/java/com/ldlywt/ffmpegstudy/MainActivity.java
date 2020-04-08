package com.ldlywt.ffmpegstudy;

import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import com.ldlywt.ffmpegstudy.listener.MediaErrorListener;
import com.yanzhenjie.permission.AndPermission;
import com.yanzhenjie.permission.runtime.Permission;

import java.io.File;
import java.util.Stack;

public class MainActivity extends AppCompatActivity {
    private NativeUtil mPlayer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        AndPermission.with(this)
                .runtime()
                .permission(Permission.READ_EXTERNAL_STORAGE)
                .onGranted(permissions -> {
                    // Storage permission are allowed.
                })
                .onDenied(permissions -> {
                    // Storage permission are not allowed.
                })
                .start();

        Utils.readRaw(this);

        String path = new File(Environment.getExternalStorageDirectory(), "input.mp3").getAbsolutePath();
        mPlayer = new NativeUtil();
        mPlayer.setDataSource(path);
        mPlayer.setOnErrorListener(new MediaErrorListener() {
            @Override
            public void onError(int code, String msg) {
                Log.e("TAG", "error code: " + code);
                Log.e("TAG", "error msg: " + msg);
            }
        });


        findViewById(R.id.play).setOnClickListener(view -> new Thread(() -> mPlayer.play()).start());
        findViewById(R.id.change_voice).setOnClickListener(view -> startActivity(new Intent(this, ChangeVoiceActivity.class)));


    }


}
