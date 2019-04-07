package com.example.hanlon.neontest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.example.neonlibrary.NeonJNI;

import java.util.Arrays;


public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

         findViewById(R.id.bMul).setOnClickListener(new View.OnClickListener(){

             @Override
             public void onClick(View view) {
                 float[] a = new float[]{4.0f,1.0f};
                 float[] b = new float[]{2.0f,3.0f};
                 float[] c = new float[]{0,0};
                 NeonJNI.neonMul(a,b,c);

                 Log.d(TAG,"a = " + Arrays.toString(a));
                 Log.d(TAG,"b = " + Arrays.toString(b));
                 Log.d(TAG,"c = " + Arrays.toString(c));
             }
         });
    }
}
