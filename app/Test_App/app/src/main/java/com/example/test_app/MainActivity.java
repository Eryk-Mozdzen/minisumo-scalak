package com.example.test_app;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    Button i1;
    TextView t1;
    String address = null, name = null;

    boolean clicked = false;
    TextView[] input = new TextView[2];

    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    Set<BluetoothDevice> pairedDevices;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        try {
            setw();
        } catch(Exception e) {}
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setw() throws IOException {
        t1 = (TextView)findViewById(R.id.textView1);
        bluetooth_connect_device();
        i1 = (Button)findViewById(R.id.button1);

        input[0] = (TextView)findViewById(R.id.textView2);
        input[1] = (TextView)findViewById(R.id.textView3);

        i1.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if(event.getAction()==MotionEvent.ACTION_UP) {
                    clicked = !clicked;

                    if(clicked) {
                        i1.setText("Stop");
                        sendMessage("f");
                    } else {
                        i1.setText("Start");
                        sendMessage("b");
                    }
                }

                getMessage();

                return true;
            }
        });
    }

    private void bluetooth_connect_device() throws IOException {
        myBluetooth = BluetoothAdapter.getDefaultAdapter();
        pairedDevices = myBluetooth.getBondedDevices();
        if(pairedDevices.size()>0) {
            for(BluetoothDevice bt: pairedDevices) {
                address = bt.getAddress().toString();
                name = bt.getName().toString();
            }
        }
        BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);
        btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);
        btSocket.connect();
        try {
            t1.setText("BT Name: " + name + "\nBT Address: " + address);
            Toast.makeText(getApplicationContext(), "Connected", Toast.LENGTH_SHORT).show();
        } catch(Exception e) {}
    }

    private void sendMessage(String i) {
        try {
            if(btSocket!=null)
                btSocket.getOutputStream().write(i.toString().getBytes());
        } catch(Exception e) {
            Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    private void getMessage() {
        try {
            InputStream inputStream = btSocket.getInputStream();
            int byteCount = inputStream.available();
            if(byteCount>0) {
                byte[] rawBytes = new byte[byteCount];
                for(int i=0; i<byteCount; i++)
                    rawBytes[i] = (byte)inputStream.read();

                final String string = new String(rawBytes,"UTF-8");

                if(string.charAt(0)=='1') input[0].setText(string.charAt(2)+"");
                if(string.charAt(0)=='2') input[1].setText(string.charAt(2)+"");
            }
        } catch(IOException e) {}

    }
}
