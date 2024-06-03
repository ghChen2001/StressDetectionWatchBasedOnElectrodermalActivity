package com.example.glusweat;

import static android.Manifest.permission.READ_EXTERNAL_STORAGE;
import static com.example.glusweat.MainActivity.mBleService;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentContainerView;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import androidx.navigation.NavController;
import androidx.navigation.fragment.NavHostFragment;
import androidx.navigation.ui.NavigationUI;

import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothProfile;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.EntryXComparator;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.ZoneOffset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getName();

    private TextView Title;
    private TextView BleState;
    //    private Button btnLink;
//    private Button btnTest;
    private Intent intTest;
    private Intent intLink;

    private TextView textHr;
    private TextView textTemp;
    private TextView textStep;
    private TextView textPressure;
    private BottomNavigationView mBottomNavigationView;
    private FragmentContainerView fragmentContainerView;

    private Fragment mBleLinkFragment;
    private Fragment mDataPlotFragment;
    private Fragment mMainFragment;

    private boolean isCharacteristicNotificationPlotEda = false;
    private boolean isCharacteristicNotificationPlotBvp = false;
    private boolean isCharacteristicNotificationPlotAcc = false;
    private boolean isCharacteristicNotificationPlotTemp = false;
    private boolean isCharacteristicNotificationNormal = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Title = findViewById(R.id.textView);
//        btnLink = findViewById(R.id.btn_Link);
//        btnTest = findViewById(R.id.btn_Test);

        mBottomNavigationView = findViewById(R.id.bottomNav);
        fragmentContainerView = findViewById(R.id.fragmentContainerView);

        mBottomNavigationView.setOnNavigationItemSelectedListener(navListener);
        mMainFragment = getSupportFragmentManager().findFragmentById(R.id.fragmentContainerView);
        doBindService();

    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onPostCreate(@Nullable Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        doUnBindService();
    }

    @SuppressLint("SetTextI18n")
    @Override
    protected void onResume() {
        super.onResume();
    }

    private final BottomNavigationView.OnNavigationItemSelectedListener navListener = item -> {
        // By using switch we can easily get
        // the selected fragment
        // by using there id.
        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        hideAllFragments(transaction);
        int itemId = item.getItemId();

        if (itemId == R.id.menu_link) {
            if (mBleLinkFragment == null) {
                mBleLinkFragment = new BleLinkFragment();
                transaction.add(R.id.fragmentContainerView, mBleLinkFragment);
            }
            transaction.show(mBleLinkFragment).commit();
        } else if (itemId == R.id.menu_plot) {
            if (mDataPlotFragment == null) {
                mDataPlotFragment = new DataPlotFragment();
                transaction.add(R.id.fragmentContainerView, mDataPlotFragment);
            }
            transaction.show(mDataPlotFragment).commit();
        } else if (itemId == R.id.menu_main) {
            Log.i(TAG, String.valueOf(mMainFragment));
            if (mMainFragment == null) {
                mMainFragment = new MainFragment();
                transaction.add(R.id.fragmentContainerView, mMainFragment);
            }
            transaction.show(mMainFragment).commit();
        }

        return true;
    };

    private void hideAllFragments(FragmentTransaction transaction) {
        if (mMainFragment != null) {
            transaction.hide(mMainFragment);
        }
        if (mDataPlotFragment != null) {
            transaction.hide(mDataPlotFragment);
        }
        if (mBleLinkFragment != null) {
            transaction.hide(mBleLinkFragment);
        }
    }

    public static BleService mBleService;
    public static boolean mIsBind;
    private final ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mBleService = ((BleService.LocalBinder) service).getService();
            if (mBleService.initialize()) {
                if (mBleService.enableBluetooth(true)) {
                    //mBleService.scanLeDevice(true);
                    Toast.makeText(MainActivity.this, "Bluetooth was opened", Toast.LENGTH_SHORT).show();
                    Log.i(TAG, "onServiceConnected");

                    mBleService.setOnConnectListener(new BleService.OnConnectionStateChangeListener() {
                        @Override
                        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
                            if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                                //Ble连接已断开
                                Intent BleStateIntent = new Intent("com.example.BLE_DISCONNECTED");
                                sendBroadcast(BleStateIntent);
                            } else if (newState == BluetoothProfile.STATE_CONNECTING) {
                                //Ble正在连接
                            } else if (newState == BluetoothProfile.STATE_CONNECTED) {
                                //Ble已连接
                                Intent BleStateIntent = new Intent("com.example.BLE_CONNECTED");
                                sendBroadcast(BleStateIntent);

                            } else if (newState == BluetoothProfile.STATE_DISCONNECTING) {
                                //Ble正在断开连接
                            }
                        }
                    });

                    mBleService.setOnServicesDiscoveredListener(new BleService.OnServicesDiscoveredListener() {
                        @Override
                        public void onServicesDiscovered(BluetoothGatt gatt, int status) {

                            // Enable Notification for "mysvc", which includes normal information.
                            isCharacteristicNotificationNormal = true;
                            mBleService.setCharacteristicNotification("000018ff-0000-1000-8000-00805f9b34fb", "00002aff-0000-1000-8000-00805f9b34fb", isCharacteristicNotificationNormal);
                            Log.i(TAG, "Notification NORMAL Enabled");
                            // The next set is in "onDescriptorWrite". Must wait DescriptorWrite process to be completed before setting another notification. recursive.
                        }
                    });

                    mBleService.setOnDataAvailableListener(new BleService.OnDataAvailableListener() {
                        @Override
                        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {

                            Log.i(TAG, "onCharacteristicChanged " + characteristic.getUuid().toString());
                            if (characteristic.getUuid().toString().equals("00002aff-0000-1000-8000-00805f9b34fb")) {
                                byte[] receivedValue = characteristic.getValue();
                                Intent BleValueIntent = new Intent("com.example.BLE_VALUE_NORMAL");
                                BleValueIntent.putExtra("value", receivedValue);
                                sendBroadcast(BleValueIntent);
                            } else if (characteristic.getUuid().toString().equals("00002afe-0000-1000-8000-00805f9b34fb")) {
//                                Log.i(TAG, characteristic.getUuid().toString());
                                byte[] receivedValue = characteristic.getValue();
                                Intent BleValueIntent = new Intent("com.example.BLE_VALUE_EDAPLOT");
                                BleValueIntent.putExtra("value", receivedValue);
                                sendBroadcast(BleValueIntent);
                            } else if (characteristic.getUuid().toString().equals("00002afd-0000-1000-8000-00805f9b34fb")) {
//                                Log.i(TAG, characteristic.getUuid().toString());
                                byte[] receivedValue = characteristic.getValue();
                                Intent BleValueIntent = new Intent("com.example.BLE_VALUE_BVPPLOT");
                                BleValueIntent.putExtra("value", receivedValue);
                                sendBroadcast(BleValueIntent);
                            } else if (characteristic.getUuid().toString().equals("00002afc-0000-1000-8000-00805f9b34fb")) {
//                                Log.i(TAG, characteristic.getUuid().toString());
                                byte[] receivedValue = characteristic.getValue();
                                Intent BleValueIntent = new Intent("com.example.BLE_VALUE_ACCPLOT");
                                BleValueIntent.putExtra("value", receivedValue);
                                sendBroadcast(BleValueIntent);
                            } else if (characteristic.getUuid().toString().equals("00002afb-0000-1000-8000-00805f9b34fb")) {
//                                Log.i(TAG, characteristic.getUuid().toString());
                                byte[] receivedValue = characteristic.getValue();
                                Intent BleValueIntent = new Intent("com.example.BLE_VALUE_TEMPPLOT");
                                BleValueIntent.putExtra("value", receivedValue);
                                sendBroadcast(BleValueIntent);
                            }
                        }

                        @Override
                        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
                            if (characteristic.getUuid().toString().equals("00002a2b-0000-1000-8000-00805f9b34fb")){
                                Log.i(TAG, "onCharacteristicRead " + characteristic.getUuid().toString() + "value " + Arrays.toString(characteristic.getValue()));
                                byte[] currentTime = new byte[10];
                                LocalDateTime now = LocalDateTime.now(ZoneOffset.UTC);

                                int year = now.getYear();
                                int month = now.getMonthValue() - 1;
                                int wday = now.getDayOfWeek().getValue();
                                int day = now.getDayOfMonth();
                                int hour = now.getHour();
                                int minute = now.getMinute();
                                int second = now.getSecond();
                                int msec = (int)((long)256 * now.getNano() / 1000000000);

                                currentTime[0] = (byte) (year & 0xFF);
                                currentTime[1] = (byte) ((year >> 8) & 0xFF);
                                currentTime[2] = (byte) (month & 0x0F);
                                currentTime[3] = (byte) (day & 0xFF);
                                currentTime[4] = (byte) (hour & 0xFF);
                                currentTime[5] = (byte) (minute & 0xFF);
                                currentTime[6] = (byte) (second & 0xFF);
                                currentTime[7] = (byte) (wday & 0x0F);
                                currentTime[8] = (byte) (msec & 0xFF);
                                currentTime[9] = 0x00;

                                String currentTimeString = new String(currentTime);

                                System.out.println(year + "-" + month + "-" + day + " " + wday + " " + hour + ":" + minute + ":" + second + ":" + msec);

                                mBleService.writeCharacteristic("00001805-0000-1000-8000-00805f9b34fb", "00002a2b-0000-1000-8000-00805f9b34fb", currentTime);
                            }
                        }

//                        @Override
//                        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
//
//                        }

                        @Override
                        public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {

                        }

                        @Override
                        public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
                            // Continue registering notification
                            Log.i(TAG, "onDescriptorWrite " + descriptor.getUuid().toString());
                            // Avoid recursive set.
                            if (descriptor.getUuid().toString().equals(GattAttributes.DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION.toLowerCase()) && !isCharacteristicNotificationPlotEda) {
                                // Enable Notification for "PlotEDA"
                                isCharacteristicNotificationPlotEda = true;
                                mBleService.setCharacteristicNotification("000018fe-0000-1000-8000-00805f9b34fb", "00002afe-0000-1000-8000-00805f9b34fb", isCharacteristicNotificationPlotEda);
                                Log.i(TAG, "Notification EDA Enabled");
                            } else if (descriptor.getUuid().toString().equals(GattAttributes.DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION.toLowerCase()) && !isCharacteristicNotificationPlotBvp) {
                                // Enable Notification for "PlotBVP"
                                isCharacteristicNotificationPlotBvp = true;
                                mBleService.setCharacteristicNotification("000018fd-0000-1000-8000-00805f9b34fb", "00002afd-0000-1000-8000-00805f9b34fb", isCharacteristicNotificationPlotBvp);
                                Log.i(TAG, "Notification BVP Enabled");
                            } else if (descriptor.getUuid().toString().equals(GattAttributes.DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION.toLowerCase()) && !isCharacteristicNotificationPlotAcc) {
                                // Enable Notification for "PlotACC"
                                isCharacteristicNotificationPlotAcc = true;
                                mBleService.setCharacteristicNotification("000018fc-0000-1000-8000-00805f9b34fb", "00002afc-0000-1000-8000-00805f9b34fb", isCharacteristicNotificationPlotAcc);
                                Log.i(TAG, "Notification ACC Enabled");
                            } else if (descriptor.getUuid().toString().equals(GattAttributes.DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION.toLowerCase()) && !isCharacteristicNotificationPlotTemp) {
                                // Enable Notification for "PlotTEMP"
                                isCharacteristicNotificationPlotTemp = true;
                                mBleService.setCharacteristicNotification("000018fb-0000-1000-8000-00805f9b34fb", "00002afb-0000-1000-8000-00805f9b34fb", isCharacteristicNotificationPlotTemp);
                                Log.i(TAG, "Notification TEMP Enabled");
                            } else if(isCharacteristicNotificationPlotEda && isCharacteristicNotificationPlotBvp && isCharacteristicNotificationPlotAcc && isCharacteristicNotificationPlotTemp){
                                mBleService.readCharacteristic("00001805-0000-1000-8000-00805f9b34fb", "00002a2b-0000-1000-8000-00805f9b34fb");
                            }
                        }
                    });
                }

            } else {
                Toast.makeText(MainActivity.this, "not support Bluetooth", Toast.LENGTH_SHORT).show();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mBleService = null;
            mIsBind = false;
        }
    };

    private void doBindService() {
        Log.i(TAG, "ServiveBind");
        Intent serviceIntent = new Intent(this, BleService.class);
        bindService(serviceIntent, serviceConnection, Context.BIND_AUTO_CREATE);
    }

    private void doUnBindService() {
        if (mIsBind) {
            unbindService(serviceConnection);
            mBleService = null;
            mIsBind = false;
        }
    }


}