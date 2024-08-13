package com.example.application;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.application.EmbeddedAndroidThings.Gpio;
import com.example.application.EmbeddedAndroidThings.I2c;
import com.example.application.EmbeddedAndroidThings.PeripheralManager;
import com.example.application.EmbeddedAndroidThings.Uart;
import com.example.application.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "Application";
    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        TextView tv = binding.sampleText;
        tv.setText(TAG);

        PeripheralManager pm = PeripheralManager.getInstance();
        Gpio gpio = pm.openGpio(Gpio.CHIP_A, Gpio.LINE_0);
        I2c i2c = pm.openI2c(I2c.INDEX_0, 0x01);
        Uart uart = pm.openUart(Uart.INDEX_0);
    }
}
