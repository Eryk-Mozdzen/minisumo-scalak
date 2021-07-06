package com.example.sumomenagerv4.Bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.fragment.app.Fragment;

import com.example.sumomenagerv4.R;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Set;

import static android.app.Activity.RESULT_OK;

public class BluetoothFragment extends Fragment {

    private static final int REQUEST_ENABLE_BT = 0;

    public ListView list_paired_devices;
    public Button button_on_off, button_list;
    public BluetoothAdapter bluetoothAdapter;

    public ArrayList<String> bluetooth_array_name;
    public ArrayList<String> bluetooth_array_info;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_bluetooth, container, false);

        list_paired_devices = view.findViewById(R.id.bluetooth_list);
        button_on_off = view.findViewById(R.id.bluetooth_button_on_off);
        button_list = view.findViewById(R.id.bluetooth_button_get_list);

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if(bluetoothAdapter.isEnabled()) {
            button_on_off.setText(getString(R.string.bluetooth_off));
        } else {
            button_list.setEnabled(false);
        }

        list_paired_devices.setAdapter(new BluetoothListAdapter(getActivity(), bluetooth_array_name, bluetooth_array_info));

        //((AppCompatActivity)getActivity()).setSupportActionBar((Toolbar)view.findViewById(R.id.toolbar));
        //((AppCompatActivity)getActivity()).getSupportActionBar().setTitle("Bluetooth");
        //view.findViewById(R.id.toolbar).setTitle("Bluetooth");

        button_on_off.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!bluetoothAdapter.isEnabled()) {
                    button_on_off.setText(getString(R.string.bluetooth_turning_on));
                    button_on_off.setEnabled(false);

                    Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(intent, REQUEST_ENABLE_BT);
                } else {
                    button_on_off.setText(getString(R.string.bluetooth_turning_off));
                    button_on_off.setEnabled(false);

                    bluetoothAdapter.disable();

                    button_on_off.setText(getString(R.string.bluetooth_on));
                    button_on_off.setEnabled(true);
                    button_list.setEnabled(false);
                    showToast("Bluetooth disabled successfully");

                    bluetooth_array_name = new ArrayList<>();
                    bluetooth_array_info = new ArrayList<>();
                    list_paired_devices.setAdapter(new BluetoothListAdapter(getActivity(), bluetooth_array_name, bluetooth_array_info));
                }
            }
        });

        button_list.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                refresh_list();
                showToast("List refreshed");
            }
        });

        list_paired_devices.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                //showToast(""+i);

                //TextView textName = view.findViewById(R.id.row_name);
                //TextView textInfo = view.findViewById(R.id.row_info);

                //textName.setBackgroundColor(Color.rgb(0,255,0));
                //textInfo.setBackgroundColor(Color.rgb(0,255,0));

                list_paired_devices.setItemChecked(i, true);
            }
        });

        return view;
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {

        switch(requestCode) {
            case REQUEST_ENABLE_BT:
                if(resultCode==RESULT_OK) {
                    button_on_off.setText(getString(R.string.bluetooth_off));
                    button_on_off.setEnabled(true);
                    button_list.setEnabled(true);
                    showToast("Bluetooth enabled successfully");
                    refresh_list();
                } else {
                    button_on_off.setText(getString(R.string.bluetooth_on));
                    button_on_off.setEnabled(true);
                    showToast("Cannot enable Bluetooth");
                }
                break;
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    private void refresh_list() {
        Set<BluetoothDevice> devices = bluetoothAdapter.getBondedDevices();

        bluetooth_array_name = new ArrayList<>();
        bluetooth_array_info = new ArrayList<>();
        for(BluetoothDevice device : devices) {
            bluetooth_array_name.add(device.getName());
            bluetooth_array_info.add(device+"");
        }

        list_paired_devices.setAdapter(new BluetoothListAdapter(getActivity(), bluetooth_array_name, bluetooth_array_info));
    }

    private void showToast(String msg) {
        Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);

        outState.putStringArray("key_list_name", bluetooth_array_name.toArray(new String[1]));
        outState.putStringArray("key_list_info", bluetooth_array_info.toArray(new String[1]));
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        bluetooth_array_name = new ArrayList<>();
        bluetooth_array_info = new ArrayList<>();

        if(savedInstanceState!=null) {

            bluetooth_array_name = new ArrayList<>(Arrays.asList(savedInstanceState.getStringArray("key_list_name")));
            bluetooth_array_info = new ArrayList<>(Arrays.asList(savedInstanceState.getStringArray("key_list_info")));

        }
    }
}
