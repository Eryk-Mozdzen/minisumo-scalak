package com.example.sumomenagerv4.Bluetooth;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.example.sumomenagerv4.R;

import java.util.ArrayList;

public class BluetoothListAdapter extends ArrayAdapter {

    private final Activity context;

    private final String[] nameArray;
    private final String[] infoArray;

    public BluetoothListAdapter(Activity context, String[] nameArrayParam, String[] infoArrayParam){

        super(context, R.layout.bluetooth_list_row , nameArrayParam);

        this.context = context;
        this.nameArray = nameArrayParam;
        this.infoArray = infoArrayParam;
    }

    public BluetoothListAdapter(Activity context, ArrayList<String> nameArrayParam, ArrayList<String> infoArrayParam){

        super(context,R.layout.bluetooth_list_row , nameArrayParam);

        this.context = context;
        this.nameArray = nameArrayParam.toArray(new String[0]);
        this.infoArray = infoArrayParam.toArray(new String[0]);
    }

    public View getView(int position, View view, ViewGroup parent) {
        LayoutInflater inflater = context.getLayoutInflater();
        View rowView = inflater.inflate(R.layout.bluetooth_list_row, null,true);

        TextView nameTextField = rowView.findViewById(R.id.row_name);
        TextView infoTextField = rowView.findViewById(R.id.row_info);

        nameTextField.setText(nameArray[position]);
        infoTextField.setText(infoArray[position]);

        return rowView;
    };

}
