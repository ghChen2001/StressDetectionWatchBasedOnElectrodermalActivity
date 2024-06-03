package com.example.glusweat;

import static com.example.glusweat.MainActivity.mBleService;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link BleLinkFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class BleLinkFragment extends Fragment {

    private static final String TAG = BleLinkFragment.class.getSimpleName();

    //Constant
    public static final int SERVICE_BIND = 1;
    public static final int SERVICE_SHOW = 2;
    public static final int REQUEST_CODE_ACCESS_COARSE_LOCATION = 1;
    private final long SCAN_PERIOD = 5 * 1000;

    //Member fields
    private DeviceAdapter deviceAdapter;
    private ArrayAdapter<String> serviceAdapter; //
    private List<Map<String, Object>> deviceList; // Devices that has been scanned
    private String connDeviceName;
    private String connDeviceAddress;

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    public BleLinkFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment BleLinkFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static BleLinkFragment newInstance(String param1, String param2) {
        BleLinkFragment fragment = new BleLinkFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_ble_link, container, false);
    }

    @Override
    public void onStart() {
        super.onStart();
        initView();
        initAdapter();
        requireActivity().registerReceiver(bleReceiver, makeIntentFilter());
    }

    @Override
    public void onHiddenChanged(boolean hidden) {
        super.onHiddenChanged(hidden);

        requireActivity().registerReceiver(bleReceiver, makeIntentFilter());
    }

    private void verifyIfRequestPermission() {
        if (Build.VERSION.SDK_INT >= 23) {
            Log.i(TAG, "onCreate: checkSelfPermission");
            if (ContextCompat.checkSelfPermission(requireActivity(),
                    Manifest.permission.ACCESS_COARSE_LOCATION)
                    != PackageManager.PERMISSION_GRANTED) {
                Log.i(TAG, "onCreate: Android 6.0 动态申请权限");

                if (ActivityCompat.shouldShowRequestPermissionRationale(requireActivity(),
                        Manifest.permission.READ_CONTACTS)) {
                    Log.i(TAG, "*********onCreate: shouldShowRequestPermissionRationale**********");
                    Toast.makeText(requireActivity(), "只有允许访问位置才能搜索到蓝牙设备", Toast.LENGTH_SHORT).show();
                } else {
                    ActivityCompat.requestPermissions(requireActivity(),
                            new String[]{Manifest.permission.ACCESS_COARSE_LOCATION,
                                    Manifest.permission.ACCESS_FINE_LOCATION},
                            REQUEST_CODE_ACCESS_COARSE_LOCATION);
                }
            } else {
                //showDialog(getResources().getString(R.string.scanning));
                //mBleService.scanLeDevice(true);
            }
        } else {
            //showDialog(getResources().getString(R.string.scanning));
            //mBleService.scanLeDevice(true);
        }
    }

    /**
     * Show dialog
     */
    private ProgressDialog progressDialog;

    private void showDialog(String message) {
        progressDialog = new ProgressDialog(requireActivity());
        progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        progressDialog.setCanceledOnTouchOutside(false);
        progressDialog.setMessage(message);
        progressDialog.show();
    }

    private void dismissDialog() {
        if (progressDialog == null) return;
        progressDialog.dismiss();
        progressDialog = null;
    }

    //Layout View
    private Button btn_scanBle;
    private RecyclerView recv_devList;
    private ProgressBar progbar_scanning;
    private TextView tv_scanning;
    private ListView lstv_showService;

    @SuppressLint("SetTextI18n")
    private void initView() {
        btn_scanBle = getView().findViewById(R.id.btn_scan);//扫描BLE按钮
        recv_devList = getView().findViewById(R.id.recv_device);//设备列表
        progbar_scanning = getView().findViewById(R.id.progressBar_scanning);//进度圈
        TextView txtv = new TextView(requireActivity());
        txtv.setText("Services");
        btn_scanBle.setVisibility(View.VISIBLE);
        progbar_scanning.setVisibility(View.INVISIBLE);
        btn_scanBle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!mBleService.isScanning()) {
                    verifyIfRequestPermission();
//                    mBleService.close();
                    deviceList.clear();
                    deviceAdapter.notifyDataSetChanged();
                    mBleService.scanLeDevice(true, SCAN_PERIOD);
//                    btn_scanBle.setVisibility(View.INVISIBLE);
                    btn_scanBle.setClickable(false);
                    progbar_scanning.setVisibility(View.VISIBLE);
                    progbar_scanning.setIndeterminate(true);
                    new Handler().postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            btn_scanBle.setVisibility(View.VISIBLE);
                            btn_scanBle.setClickable(true);
                            progbar_scanning.setVisibility(View.INVISIBLE);
                            progbar_scanning.setIndeterminate(false);
                        }
                    },SCAN_PERIOD);
                }
            }
        });
        //lstv_showService.setOnItemClickListener(new AdapterView.OnItemClickListener() {
        //@Override
        //public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        //Log.i(TAG, "position = " + position + ", id = " + id);
        //String s = serviceList.get((int) id);
        //Intent intent = new Intent(BleLinkActivity.this, CharacteristicActivity.class);
        //intent.putExtra("characteristic", characteristicList.get((int) id));
        //startActivity(intent);
        // }
        //});
    }



    private List<BluetoothGattService> gattServiceList;
    private List<String> serviceList;
    private List<String[]> characteristicList;

    private void setBleServiceListener() {
        mBleService.setOnServicesDiscoveredListener(new BleService.OnServicesDiscoveredListener() {
            @Override
            public void onServicesDiscovered(BluetoothGatt gatt, int status) {
                if (status == BluetoothGatt.GATT_SUCCESS) {
                    gattServiceList = gatt.getServices();
                    serviceList.clear();
                    for (BluetoothGattService service :
                            gattServiceList) {
                        String serviceUuid = service.getUuid().toString();
                        serviceList.add(MyGattAttributes.lookup(serviceUuid, "Unknown") + "\n" + serviceUuid);
                        Log.i(TAG, MyGattAttributes.lookup(serviceUuid, "Unknown") + "\n" + serviceUuid+ "lllllllllll");

                        List<BluetoothGattCharacteristic> characteristics = service.getCharacteristics();
                        String[] charArra = new String[characteristics.size()];
                        for (int i = 0; i < characteristics.size(); i++) {
                            String charUuid = characteristics.get(i).getUuid().toString();
                            charArra[i] = MyGattAttributes.lookup(charUuid, "Unknown") + "\n" + charUuid;
                        }
                        characteristicList.add(charArra);
                    }
                    mHandler.sendEmptyMessage(SERVICE_SHOW);
                }
            }
        });


    }

    private void initAdapter() {
        deviceList = new ArrayList<>();
        deviceAdapter = new DeviceAdapter(requireActivity(), deviceList) {
            @Override
            public void onBindViewHolder(@NonNull DeviceAdapter.MyViewHolder holder, int position) {
                final Map<String, Object> ScannedDevice = Mydata.get(position);

                holder.DeviceName.setText(String.valueOf(ScannedDevice.get("name")));
                holder.DeviceAddr.setText(String.valueOf(ScannedDevice.get("address")));
                holder.DeviceState.setText(((boolean) ScannedDevice.get("isConnect")) ?
                        R.string.state_connected :
                        R.string.state_disconnected);
                holder.LinkBtn.setText(((boolean) ScannedDevice.get("isConnect")) ?
                        R.string.disconnect :
                        R.string.connect);
                Log.i(TAG, "onBindViewHolder position: "+ position + "address: " + ScannedDevice.get("address"));
                Log.i(TAG, String.valueOf(Mydata));
                holder.LinkBtn.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if ((boolean) ScannedDevice.get("isConnect")) {
                            mBleService.disconnect();
                            showDialog(getString(R.string.disconnecting));
//                            holder.DeviceState.setText(((boolean) ScannedDevice.get("isConnect")) ?
//                                    R.string.state_connected :
//                                    R.string.state_disconnected);
//                            deviceAdapter.notifyDataSetChanged();
                        } else {
                            connDeviceAddress = (String) ScannedDevice.get("address");
                            connDeviceName = (String) ScannedDevice.get("name");
//                            HashMap<String, Object> connDevMap = new HashMap<String, Object>();
//                            connDevMap.put("name", connDeviceName);
//                            connDevMap.put("address", connDeviceAddress);
//                            connDevMap.put("isConnect", false);
//                            deviceList.clear();
//                            deviceList.add(connDevMap);
//                            deviceAdapter.notifyDataSetChanged();
                            mBleService.connect(connDeviceAddress);
                            showDialog(getString(R.string.connecting));
                        }
                    }
                });
            }
        };

        LinearLayoutManager layoutManager = new LinearLayoutManager(requireActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        recv_devList.setLayoutManager(layoutManager);
        recv_devList.setAdapter(deviceAdapter);
        serviceList = new ArrayList<>();
        characteristicList = new ArrayList<>();
        serviceAdapter = new ArrayAdapter<>(requireActivity(), android.R.layout.simple_list_item_1, serviceList);
        //lstv_showService.setAdapter(serviceAdapter);
    }

    private BroadcastReceiver bleReceiver = new BroadcastReceiver() {
        @SuppressLint("NotifyDataSetChanged")
        @Override
        public void onReceive(Context context, Intent intent) {
            if (Objects.equals(intent.getAction(), BleService.ACTION_BLUETOOTH_DEVICE) && intent.getStringExtra("name")!=null) {
                String tmpDevName = intent.getStringExtra("name");
                String tmpDevAddress = intent.getStringExtra("address");
                Log.i(TAG, "name: " + tmpDevName + ", address: " + tmpDevAddress);
                HashMap<String, Object> deviceMap = new HashMap<>();
                deviceMap.put("name", tmpDevName);
                deviceMap.put("address", tmpDevAddress);
                deviceMap.put("isConnect", false);
                deviceList.add(deviceMap);
                Log.i(TAG, String.valueOf(deviceList));
                deviceAdapter.notifyDataSetChanged();
            } else if (Objects.equals(intent.getAction(), BleService.ACTION_GATT_CONNECTED)) {
                deviceList.get(findDeviceFromList(deviceList, connDeviceAddress)).replace("isConnect", true);
                Log.i(TAG, "Connected" + deviceList.get(findDeviceFromList(deviceList, connDeviceAddress)).get("name"));
//                deviceAdapter.notifyDataSetChanged();
                deviceAdapter.notifyItemChanged(findDeviceFromList(deviceList, connDeviceAddress)); // inform device adaptor to update device information.
                serviceAdapter.notifyDataSetChanged();
                dismissDialog();
            } else if (intent.getAction().equals(BleService.ACTION_GATT_DISCONNECTED)) {
                deviceList.get(findDeviceFromList(deviceList, connDeviceAddress)).replace("isConnect", false);
                Log.i(TAG, "Disconnected" + deviceList.get(findDeviceFromList(deviceList, connDeviceAddress)).get("name"));
                serviceList.clear();
                characteristicList.clear();
                deviceAdapter.notifyItemChanged(findDeviceFromList(deviceList, connDeviceAddress)); // inform device adaptor to update device information.
                serviceAdapter.notifyDataSetChanged();
                connDeviceAddress = null;
                connDeviceName = null;
                dismissDialog();
            } else if (intent.getAction().equals(BleService.ACTION_SCAN_FINISHED)) {
                btn_scanBle.setEnabled(true);
                dismissDialog();
            }
        }
    };

    private int findDeviceFromList(List<Map<String, Object>> devList, String connDeviceAddress){
        int index = 0;

        for(;index < devList.size();index++){
            if(devList.get(index).get("address") == connDeviceAddress){
                return index;
            }
        }
        return -1;
    };

    private static IntentFilter makeIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BleService.ACTION_BLUETOOTH_DEVICE);
        intentFilter.addAction(BleService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BleService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BleService.ACTION_SCAN_FINISHED);
        return intentFilter;
    }

    @SuppressLint("HandlerLeak")
    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case SERVICE_BIND:
                    setBleServiceListener();
                    break;
                case SERVICE_SHOW:
                    serviceAdapter.notifyDataSetChanged();
                    break;
            }
        }
    };
}