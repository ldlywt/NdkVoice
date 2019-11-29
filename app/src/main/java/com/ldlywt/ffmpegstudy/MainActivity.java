package com.ldlywt.ffmpegstudy;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.TextView;

import com.ldlywt.ffmpegstudy.listener.MediaErrorListener;
import com.yanzhenjie.permission.AndPermission;
import com.yanzhenjie.permission.runtime.Permission;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    File mMusicFile = new File(Environment.getExternalStorageDirectory(),"input.mp3");
    private Player mPlayer;


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

        mPlayer = new Player();
        mPlayer.setDataSource(mMusicFile.getAbsolutePath());
        mPlayer.setOnErrorListener(new MediaErrorListener() {
            @Override
            public void onError(int code, String msg) {
                Log.e("TAG", "error code: " + code);
                Log.e("TAG", "error msg: " + msg);
            }
        });


        findViewById(R.id.play).setOnClickListener(view -> new Thread(new Runnable() {
            @Override
            public void run() {
                mPlayer.play();
            }
        }).start());
    }
}
