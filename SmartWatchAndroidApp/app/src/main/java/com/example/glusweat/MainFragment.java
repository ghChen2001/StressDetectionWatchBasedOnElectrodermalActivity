package com.example.glusweat;

import static com.example.glusweat.MainActivity.mBleService;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.Objects;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link MainFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class MainFragment extends Fragment {

    private static final String TAG = MainFragment.class.getName();

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    private TextView BleState;
    //    private Button btnLink;
//    private Button btnTest;
    private Intent intTest;
    private Intent intLink;

    private TextView textHr;
    private TextView textTemp;
    private TextView textStep;
    private TextView textPressure;

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;


    private final int UPDATE_HR = 0;
    private final String UPDATE_HR_KEY = "HR";
    private final int UPDATE_TEMP = 1;
    private final String UPDATE_TEMP_KEY = "TEMP";
    private final int UPDATE_STEP = 2;
    private final String UPDATE_STEP_KEY = "STEP";
    private final int UPDATE_STRESS = 3;
    private final String UPDATE_STRESS_KEY = "STRESS";

    public static Handler uiHandler;


    public MainFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment MainFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static MainFragment newInstance(String param1, String param2) {
        MainFragment fragment = new MainFragment();
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

        uiHandler = new Handler(Looper.getMainLooper()) {
            @SuppressLint("SetTextI18n")
            @Override
            public void handleMessage(@NonNull Message msg) {
                super.handleMessage(msg);
                if (msg.what == UPDATE_HR) {
                    String strUpdate = msg.getData().getString(UPDATE_HR_KEY);
                    textHr.setText(strUpdate + " bpm");
                }
                if (msg.what == UPDATE_STEP) {
                    String strUpdate = msg.getData().getString(UPDATE_STEP_KEY);
                    textStep.setText(strUpdate + " 步");
                }
                if (msg.what == UPDATE_STRESS) {
                    String strUpdate = msg.getData().getString(UPDATE_STRESS_KEY);
                    textPressure.setText(strUpdate);
                }
                if (msg.what == UPDATE_TEMP) {
                    String strUpdate = msg.getData().getString(UPDATE_TEMP_KEY);
                    textTemp.setText(strUpdate + "℃");
                }
            }
        };
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_main, container, false);

    }

    @Override
    public void onStart() {
        super.onStart();

        BleState = getView().findViewById(R.id.main_BleLinkState);

        textHr = getView().findViewById(R.id.textViewHrValue);
        textTemp = getView().findViewById(R.id.textViewTempValue);
        textStep = getView().findViewById(R.id.textViewPedoValue);
        textPressure = getView().findViewById(R.id.textViewMentalValue);

        // Receiver for Ble State.
        IntentFilter BleStateFilter = new IntentFilter("com.example.BLE_CONNECTED");
        BleStateFilter.addAction("com.example.BLE_DISCONNECTED");
        BroadcastReceiver BleStateReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                if (Objects.equals(intent.getAction(), "com.example.BLE_CONNECTED")) {
                    BleState.setText(getResources().getString(R.string.textBleState_Linked) + mBleService.getConnectDevice().getName());
                } else if (Objects.equals(intent.getAction(), "com.example.BLE_DISCONNECTED")) {
                    BleState.setText(R.string.textBleState_NoLink);
                }
            }
        };
        requireActivity().registerReceiver(BleStateReceiver, BleStateFilter);

        // Receiver for Ble normal information.
        IntentFilter BleNormalInfoFilter = new IntentFilter("com.example.BLE_VALUE_NORMAL");
        BroadcastReceiver BleNormalInfoReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                byte[] receivedValue = intent.getByteArrayExtra("value");
                Message uiMessage = null;
                Bundle bundle = new Bundle();

                uiMessage = uiHandler.obtainMessage();
                assert receivedValue != null;
                int hr = receivedValue[0] & 0xFF;
                uiMessage.what = UPDATE_HR;
                bundle.putString(UPDATE_HR_KEY, String.valueOf(hr));
                uiMessage.setData(bundle);
                uiHandler.sendMessage(uiMessage);

                uiMessage = uiHandler.obtainMessage();
                int step = (receivedValue[2]) | (receivedValue[1] << 8);
                uiMessage.what = UPDATE_STEP;
                bundle.putString(UPDATE_STEP_KEY, String.valueOf(step));
                uiMessage.setData(bundle);
                uiHandler.sendMessage(uiMessage);

                uiMessage = uiHandler.obtainMessage();
                int mentalFlag = receivedValue[3] & 0xFF;
                uiMessage.what = UPDATE_STRESS;
                if (mentalFlag == 0) {
                    bundle.putString(UPDATE_STRESS_KEY, "Neutral");
                } else if (mentalFlag == 1) {
                    bundle.putString(UPDATE_STRESS_KEY, "Stress");
                } else {
                    bundle.putString(UPDATE_STRESS_KEY, "N/A");
                }
                uiMessage.setData(bundle);
                uiHandler.sendMessage(uiMessage);

                uiMessage = uiHandler.obtainMessage();
                double temp0 = (receivedValue[4] - 48) * 10 + (receivedValue[5] - 48) + (receivedValue[6] - 48) * 0.1;
                uiMessage.what = UPDATE_TEMP;
                bundle.putString(UPDATE_TEMP_KEY, String.valueOf(temp0));
                uiMessage.setData(bundle);
                uiHandler.sendMessage(uiMessage);
            }
        };
        requireActivity().registerReceiver(BleNormalInfoReceiver, BleNormalInfoFilter);
    }

    @Override
    public void onHiddenChanged(boolean hidden) {
        super.onHiddenChanged(hidden);

//        if (mBleService != null && mBleService.getConnectDevice() != null) {
////            mBleService.setCharacteristicNotification("000018ff-0000-1000-8000-00805F9B34FB", "00002aff-0000-1000-8000-00805F9B34FB", true);
////            Log.i(TAG, "Notification Enabled");
//
//            mBleService.setOnDataAvailableListener(new BleService.OnDataAvailableListener() {
//                                                       @Override
//                                                       public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
//                                                           //处理特性读取返回的数据
//                                                       }
//
//                                                       @Override
//                                                       public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
//                                                           //处理通知返回的数据
//                                                           Message uiMessage = null;
//                                                           Bundle bundle = new Bundle();
//
//                                                           byte[] receivedValue = characteristic.getValue();
//
//                                                           uiMessage = uiHandler.obtainMessage();
//                                                           int hr = receivedValue[0] & 0xFF;
//                                                           uiMessage.what = UPDATE_HR;
//                                                           bundle.putString(UPDATE_HR_KEY, String.valueOf(hr));
//                                                           uiMessage.setData(bundle);
//                                                           uiHandler.sendMessage(uiMessage);
//
//                                                           uiMessage = uiHandler.obtainMessage();
//                                                           int step = (receivedValue[2]) | (receivedValue[1] << 8);
//                                                           uiMessage.what = UPDATE_STEP;
//                                                           bundle.putString(UPDATE_STEP_KEY, String.valueOf(step));
//                                                           uiMessage.setData(bundle);
//                                                           uiHandler.sendMessage(uiMessage);
//
//                                                           uiMessage = uiHandler.obtainMessage();
//                                                           int mentalFlag = receivedValue[3] & 0xFF;
//                                                           uiMessage.what = UPDATE_STRESS;
//                                                           if (mentalFlag == 0) {
//                                                               bundle.putString(UPDATE_STRESS_KEY, "Neutral");
//                                                           } else if (mentalFlag == 1) {
//                                                               bundle.putString(UPDATE_STRESS_KEY, "Stress");
//                                                           } else {
//                                                               bundle.putString(UPDATE_STRESS_KEY, "N/A");
//                                                           }
//                                                           uiMessage.setData(bundle);
//                                                           uiHandler.sendMessage(uiMessage);
//
//                                                           uiMessage = uiHandler.obtainMessage();
//                                                           double temp0 = (receivedValue[4] - 48) * 10 + (receivedValue[5] - 48) + (receivedValue[6] - 48) * 0.1;
//                                                           uiMessage.what = UPDATE_TEMP;
//                                                           bundle.putString(UPDATE_TEMP_KEY, String.valueOf(temp0));
//                                                           uiMessage.setData(bundle);
//                                                           uiHandler.sendMessage(uiMessage);
//                                                       }
//
//                                                       @Override
//                                                       public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
//                                                       }
//                                                   }
//            );
//        }
    }

    @Override
    public void onResume() {
        super.onResume();

    }
}