package com.ldlywt.ffmpegstudy

import android.Manifest
import android.content.Intent
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.ldlywt.ffmpegstudy.utils.PermissionHelper
import com.ldlywt.ffmpegstudy.utils.PermissionHelper.PermissionListener
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File


class MainActivity : AppCompatActivity() {
    private var mPlayer: NativeUtil? = null
    private var mHelper: PermissionHelper? = null

    var mMusicFile = File(Environment.getExternalStorageDirectory(), "NoSpeak.mp3")

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermission()
        mPlayer = NativeUtil()
        mPlayer!!.setDataSource(mMusicFile.absolutePath)
        mPlayer!!.setOnErrorListener { code, msg ->
            Log.e("TAG", "error code: $code")
            Log.e("TAG", "error msg: $msg")
        }
        play.setOnClickListener {
            mPlayer!!.play()
            Thread(Runnable {
//                mPlayer!!.prepare()
            }).start()
        }

        pause.setOnClickListener {
            mPlayer!!.pauseMusic()
        }

        change_voice.setOnClickListener {
            startActivity(Intent(this, ChangeVoiceActivity::class.java))
        }

    }

    private fun requestPermission() {
        mHelper = PermissionHelper(this)
        mHelper!!.requestPermissions("请授予[读写]权限！",
                object : PermissionListener {
                    override fun doAfterGrand(vararg permission: String) {}
                    override fun doAfterDenied(vararg permission: String) {
                        Toast.makeText(this@MainActivity, "权限获取失败", Toast.LENGTH_SHORT).show()
                    }
                }, Manifest.permission.WRITE_EXTERNAL_STORAGE)
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        mHelper!!.handleRequestPermissionsResult(requestCode, permissions, grantResults)
    }
}