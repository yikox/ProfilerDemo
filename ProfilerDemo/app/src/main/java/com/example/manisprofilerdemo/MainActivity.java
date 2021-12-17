package com.example.ProfilerDemo;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
//import android.util.Log;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        button0 = findViewById(R.id.button0);
        button1 = findViewById(R.id.button1);
        button2 = findViewById(R.id.button2);
        builder = new AlertDialog.Builder(MainActivity.this)
                .setTitle("提示")
                .setMessage("运行完成")
                .setPositiveButton("确定", null);

    }
    JNI jni = new JNI();
    Button button0;
    Button button1;
    Button button2;
    AlertDialog.Builder builder;

    public void ColXCol(View view)
    {
        button0.setEnabled(false); // 禁用按钮
        button1.setEnabled(false); // 禁用按钮
        button2.setEnabled(false); // 禁用按钮
        double runTimeMs = jni.test0();
        button0.setEnabled(true); // 启用按钮，默认状态
        button1.setEnabled(true); // 启用按钮，默认状态
        button2.setEnabled(true); // 启用按钮，默认状态
        String OutMsg=null;
        OutMsg = String.format("运行完毕，运行时间：%.2f（ms）",runTimeMs);
        builder.setMessage((CharSequence)OutMsg).show();
    }

    public void ColXRow(View view)
    {
        button0.setEnabled(false); // 禁用按钮
        button1.setEnabled(false); // 禁用按钮
        button2.setEnabled(false); // 禁用按钮
        double runTimeMs = jni.test1();
        button0.setEnabled(true); // 启用按钮，默认状态
        button1.setEnabled(true); // 启用按钮，默认状态
        button2.setEnabled(true); // 启用按钮，默认状态
        String OutMsg=null;
        OutMsg = String.format("运行完毕，运行时间：%.2f（ms）",runTimeMs);
        builder.setMessage((CharSequence)OutMsg).show();
    }
    public void RowXRow(View view)
    {
        button0.setEnabled(false); // 禁用按钮
        button1.setEnabled(false); // 禁用按钮
        button2.setEnabled(false); // 禁用按钮
        double runTimeMs = jni.test2();
        button0.setEnabled(true); // 启用按钮，默认状态
        button1.setEnabled(true); // 启用按钮，默认状态
        button2.setEnabled(true); // 启用按钮，默认状态
        String OutMsg=null;
        OutMsg = String.format("运行完毕，运行时间：%.2f（ms）",runTimeMs);
        builder.setMessage((CharSequence)OutMsg).show();
    }
}