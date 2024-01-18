package com.intel.screencastfrontclient;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.os.IBinder;
import android.os.ServiceManager;
import android.os.RemoteException;
import android.content.Context;

// import com.intel.screencastfrontclient.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // private ActivityMainBinding binding;
    private boolean state = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // mContext = getApplicationContext();
        // binding = ActivityMainBinding.inflate(getLayoutInflater());
        // setContentView(binding.getRoot());
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        // TextView tv = binding.sampleText;
        TextView tv = (TextView)findViewById(R.id.sample_text);
        tv.setText(CastJNILib.stringFromJNI());

        CastJNILib.init();

        Button control_btn = (Button) findViewById(R.id.btn_1);
        control_btn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                if(state){
                    CastJNILib.startCast();
                    control_btn.setText("STOP");
                }
                else{
                    CastJNILib.stopCast();
                    control_btn.setText("START");
                }
                state = !state;

                Log.d("BUTTONS", "User tapped the Supabutton");
            }
        });

    }

    /**
     * A native method that is implemented by the 'screencastfrontclient' native library,
     * which is packaged with this application.
     */
}