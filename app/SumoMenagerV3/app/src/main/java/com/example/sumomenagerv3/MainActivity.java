package com.example.sumomenagerv3;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    BluetoothAdapter mBluetoothAdapter;
    BluetoothSocket mmSocket;
    BluetoothDevice mmDevice;
    OutputStream mmOutputStream;
    InputStream mmInputStream;
    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    volatile boolean stopWorker;

    boolean clicked = false;
    TextView[] distSensors;
    TextView[] edgeSensors;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        distSensors = new TextView[5];
        distSensors[0] = (TextView)findViewById(R.id.label1);
        distSensors[1] = (TextView)findViewById(R.id.label2);
        distSensors[2] = (TextView)findViewById(R.id.label3);
        distSensors[3] = (TextView)findViewById(R.id.label4);
        distSensors[4] = (TextView)findViewById(R.id.label5);

        edgeSensors = new TextView[2];
        edgeSensors[0] = (TextView)findViewById(R.id.labelColor1);
        edgeSensors[1] = (TextView)findViewById(R.id.labelColor2);

        final Button startButton = (Button)findViewById(R.id.startButton);
        final Button lfButton = (Button)findViewById(R.id.lfButton);
        final Button lbButton = (Button)findViewById(R.id.lbButton);
        final Button rfButton = (Button)findViewById(R.id.rfButton);
        final Button rbButton = (Button)findViewById(R.id.rbButton);

        startButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    clicked = !clicked;

                    if(clicked) {
                        sendData("f");
                        startButton.setText("Stop");
                    } else {
                        sendData("b");
                        startButton.setText("Start");
                    }
                } catch (IOException ex) {}
            }
        });

        lfButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                try {
                    if(event.getAction()==MotionEvent.ACTION_DOWN) sendData("L_MOT 150");
                    if(event.getAction()==MotionEvent.ACTION_UP) sendData("L_MOT 0");
                } catch (IOException ex) {}

                return true;
            }
        });

        lbButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                try {
                    if(event.getAction()==MotionEvent.ACTION_DOWN) sendData("L_MOT -150");
                    if(event.getAction()==MotionEvent.ACTION_UP) sendData("L_MOT 0");
                } catch (IOException ex) {}

                return true;
            }
        });

        rfButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                try {
                    if(event.getAction()==MotionEvent.ACTION_DOWN) sendData("R_MOT 150");
                    if(event.getAction()==MotionEvent.ACTION_UP) sendData("R_MOT 0");
                } catch (IOException ex) {}

                return true;
            }
        });

        rbButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                try {
                    if(event.getAction()==MotionEvent.ACTION_DOWN) sendData("R_MOT -150");
                    if(event.getAction()==MotionEvent.ACTION_UP) sendData("R_MOT 0");
                } catch (IOException ex) {}

                return true;
            }
        });

        try {
            findBT();
            openBT();
        } catch (Exception ex) {
            Toast.makeText(this, "Connection Failed", Toast.LENGTH_SHORT).show();
        }

        final EditText editTextConsole = (EditText)findViewById(R.id.editTextConsole);
        final Button consoleButton = (Button)findViewById(R.id.consoleButton);
        final TextView serialConole = (TextView)findViewById(R.id.serialConsole);
        final ScrollView scrollSerial = (ScrollView)findViewById(R.id.scrollSerial);

        editTextConsole.setVisibility(View.GONE);
        consoleButton.setVisibility(View.GONE);
        serialConole.setVisibility(View.GONE);
        scrollSerial.setVisibility(View.GONE);

        consoleButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                try {
                    if(event.getAction()==MotionEvent.ACTION_DOWN) {
                        sendData(editTextConsole.getText().toString());
                        editTextConsole.setText("");
                    }
                } catch (IOException ex) {}

                return true;
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.top_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        switch(item.getItemId()) {
            case R.id.about_us:
                Toast.makeText(this, "Aplication created by Eryk Możdżeń", Toast.LENGTH_SHORT).show();
                break;
            case R.id.restart_connection:
                try {
                    closeBT();
                    findBT();
                    openBT();
                } catch (IOException ex) {
                    Toast.makeText(this, "Connection Restarted Failed", Toast.LENGTH_SHORT).show();
                }
                break;
            case R.id.show_serial:
                if(item.isChecked()) {
                    item.setChecked(false);
                    findViewById(R.id.consoleButton).setVisibility(View.GONE);
                    findViewById(R.id.serialConsole).setVisibility(View.GONE);
                    findViewById(R.id.editTextConsole).setVisibility(View.GONE);
                    findViewById(R.id.scrollSerial).setVisibility(View.GONE);
                } else {
                    item.setChecked(true);
                    findViewById(R.id.consoleButton).setVisibility(View.VISIBLE);
                    findViewById(R.id.serialConsole).setVisibility(View.VISIBLE);
                    findViewById(R.id.editTextConsole).setVisibility(View.VISIBLE);
                    findViewById(R.id.scrollSerial).setVisibility(View.VISIBLE);
                }
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    void findBT() {
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(mBluetoothAdapter==null) {
            Toast.makeText(getApplicationContext(), "No bluetooth adapter available", Toast.LENGTH_SHORT).show();
        }

        if(!mBluetoothAdapter.isEnabled()) {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, 0);
        }

        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if(pairedDevices.size()>0) {
            for(BluetoothDevice device : pairedDevices) {
                mmDevice = device;
            }
        }
    }

    void openBT() throws IOException {
        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
        mmSocket = mmDevice.createRfcommSocketToServiceRecord(uuid);
        mmSocket.connect();
        mmOutputStream = mmSocket.getOutputStream();
        mmInputStream = mmSocket.getInputStream();

        beginListenForData();

        Toast.makeText(getApplicationContext(), "Connected with: " + mmDevice.getName(), Toast.LENGTH_SHORT).show();
    }

    void beginListenForData() {
        final Handler handler = new Handler();
        final byte delimiter = 10; //This is the ASCII code for a newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        workerThread = new Thread(new Runnable() {
            public void run() {
                while(!Thread.currentThread().isInterrupted() && !stopWorker) {
                    try {
                        int bytesAvailable = mmInputStream.available();
                        if(bytesAvailable>0) {
                            byte[] packetBytes = new byte[bytesAvailable];
                            mmInputStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++) {
                                byte b = packetBytes[i];
                                if(b == delimiter) {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;

                                    handler.post(new Runnable() {
                                        public void run() {
                                            try {
                                                writeOnConsole(data);
                                                writeOnConsole("\n");
                                            } catch (IOException en) {}

                                            try {
                                                String command = data.substring(0, 4);
                                                int index = Character.getNumericValue(data.charAt(4));
                                                int value = Character.getNumericValue(data.charAt(6));

                                                if(command.charAt(0)=='D') {
                                                    if(value==1) distSensors[index].setBackgroundColor(Color.parseColor("#FF0000"));
                                                    if(value==0) distSensors[index].setBackgroundColor(Color.parseColor("#646464"));
                                                }
                                                if(command.charAt(0)=='E') {
                                                    if(value==1) edgeSensors[index].setBackgroundColor(Color.parseColor("#FF0000"));
                                                    if(value==0) edgeSensors[index].setBackgroundColor(Color.parseColor("#646464"));
                                                }
                                            } catch(IndexOutOfBoundsException ex) {
                                                for(int i=0; i<5; i++) distSensors[i].setBackgroundColor(Color.parseColor("#00FFFF"));
                                                for(int i=0; i<2; i++) edgeSensors[i].setBackgroundColor(Color.parseColor("#00FFFF"));
                                            }

                                        }
                                    });
                                } else {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    } catch (IOException ex) {
                        stopWorker = true;
                    }
                }
            }
        });

        workerThread.start();
    }

    void sendData(String msg) throws IOException {
        msg += "\n";
        mmOutputStream.write(msg.getBytes());
        writeOnConsole(msg);
    }

    void closeBT() throws IOException {
        stopWorker = true;
        mmOutputStream.close();
        mmInputStream.close();
        mmSocket.close();
        Toast.makeText(getApplicationContext(), "Bluetooth Closed", Toast.LENGTH_SHORT).show();
    }

    void writeOnConsole(String msg) throws IOException {
        final TextView myTextView = (TextView) findViewById(R.id.serialConsole);
        final ScrollView scrollSerial = (ScrollView)findViewById(R.id.scrollSerial);
        myTextView.setText(myTextView.getText() + msg);
        scrollSerial.post(new Runnable() {
            @Override
            public void run() {
                scrollSerial.fullScroll(View.FOCUS_DOWN);
            }
        });
    }

}
