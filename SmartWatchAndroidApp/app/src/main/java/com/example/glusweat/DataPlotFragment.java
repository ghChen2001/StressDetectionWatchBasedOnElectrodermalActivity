package com.example.glusweat;

import static com.example.glusweat.MainActivity.mBleService;
import static com.example.glusweat.MainFragment.uiHandler;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.icu.text.DateFormat;
import android.icu.text.SimpleDateFormat;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.FrameLayout;


import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.EntryXComparator;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.sql.Time;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Objects;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link DataPlotFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class DataPlotFragment extends Fragment {

    private static final String TAG = DataPlotFragment.class.getSimpleName();

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private static Handler uiHandler;
    private int EDA_DATA = 0;
    private int BVP_DATA = 1;
    private int ACC_DATA = 2;
    private int TEMP_DATA = 3;
    private String value = "value";
    private String number = "number";

    private LineChart DataChart;
    private LineChart BvpChart;
    private LineChart AccChart;
    private LineChart TempChart;
    private List<Entry> ChartEntry1 = new ArrayList<Entry>();
    private List<Entry> ChartEntryBvp = new ArrayList<Entry>();
    private List<Entry> ChartEntryAccX = new ArrayList<Entry>();
    private List<Entry> ChartEntryAccY = new ArrayList<Entry>();
    private List<Entry> ChartEntryAccZ = new ArrayList<Entry>();
    private List<Entry> ChartEntryAnrX = new ArrayList<Entry>();
    private List<Entry> ChartEntryAnrY = new ArrayList<Entry>();
    private List<Entry> ChartEntryAnrZ = new ArrayList<Entry>();
    private List<Entry> ChartEntryTemp = new ArrayList<Entry>();

    private LineDataSet dataSet1;
    private LineDataSet dataSetBvp;
    private LineDataSet dataSetAccX;
    private LineDataSet dataSetAccY;
    private LineDataSet dataSetAccZ;
    private LineDataSet dataSetAnrX;
    private LineDataSet dataSetAnrY;
    private LineDataSet dataSetAnrZ;
    private LineDataSet dataSetTemp;
    private int pointsCnt = 0;
    private int pointsCntBvp = 0;
    private int pointsCntAcc = 0;
    private int pointsCntTemp = 0;
    private ArrayList<String> EdaData = new ArrayList<String>();
    private ArrayList<String> BvpData = new ArrayList<String>();
    private ArrayList<String> AccXData = new ArrayList<String>();
    private ArrayList<String> AccYData = new ArrayList<String>();
    private ArrayList<String> AccZData = new ArrayList<String>();
    private ArrayList<String> TempData = new ArrayList<String>();


    private Button clearBtn;
    private Button saveBtn;

    public DataPlotFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment DataPlotFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static DataPlotFragment newInstance(String param1, String param2) {
        DataPlotFragment fragment = new DataPlotFragment();
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
                if (msg.what == EDA_DATA) {
                    float[] dataArray = msg.getData().getFloatArray(value);
                    assert dataArray != null;
                    int dataNumber = msg.getData().getInt(number);

                    int ptr;
                    for (ptr = 0; ptr < dataNumber; ptr++) {
                        List<ILineDataSet> dataSets = new ArrayList<ILineDataSet>();
                        ChartEntry1.add(new Entry(pointsCnt, dataArray[ptr]));
//                        ChartEntry1.sort(new EntryXComparator());
                        dataSet1 = new LineDataSet(ChartEntry1, "EDA");
                        dataSet1.setColor(getResources().getColor(R.color.colorDot));
                        dataSet1.setDrawCircleHole(false);
                        dataSet1.setDrawValues(false);
                        dataSet1.setDrawCircles(false);
//                        dataSet1.setValueTextColor(R.color.colorDot);
                        dataSets.add(dataSet1);

                        LineData lineData = new LineData(dataSets);
                        DataChart.setData(lineData);
                        DataChart.invalidate(); // refresh

                        EdaData.add(String.valueOf(dataArray[ptr]));
                        pointsCnt++;
                    }
                } else if (msg.what == BVP_DATA) {
                    int[] dataArray = msg.getData().getIntArray(value);
                    assert dataArray != null;

                    int ptr;
                    for (ptr = 0; ptr < 20; ptr++) {
                        List<ILineDataSet> dataSets = new ArrayList<ILineDataSet>();

                        ChartEntryBvp.add(new Entry(pointsCntBvp, dataArray[ptr]));
//                        ChartEntry1.sort(new EntryXComparator());
                        dataSetBvp = new LineDataSet(ChartEntryBvp, "BVP");
                        dataSetBvp.setColor(getResources().getColor(R.color.colorBvp));
                        dataSetBvp.setLineWidth(1.2f);
                        dataSetBvp.setDrawCircleHole(false);
                        dataSetBvp.setDrawCircles(false);
                        dataSetBvp.setDrawValues(false);
//                        dataSetBvp.setValueTextColor(R.color.colorDot);
                        dataSets.add(dataSetBvp);

                        LineData lineData = new LineData(dataSets);
                        BvpChart.setData(lineData);
                        BvpChart.invalidate(); // refresh

                        BvpData.add(String.valueOf(dataArray[ptr]));
                        pointsCntBvp++;
                    }
                } else if (msg.what == ACC_DATA) {
                    float[] dataArray = msg.getData().getFloatArray(value);
                    assert dataArray != null;

//                    int ptr = 0;
//                    for (ptr = 0; ptr < 2; ptr++){
                    List<ILineDataSet> dataSets = new ArrayList<ILineDataSet>();
                    ChartEntryAccX.add(new Entry(pointsCntAcc, dataArray[0]));
                    AccXData.add(String.valueOf(dataArray[0]));
                    ChartEntryAccY.add(new Entry(pointsCntAcc, dataArray[1]));
                    AccYData.add(String.valueOf(dataArray[1]));
                    ChartEntryAccZ.add(new Entry(pointsCntAcc, dataArray[2]));
                    AccZData.add(String.valueOf(dataArray[2]));
                    ChartEntryAccX.add(new Entry(pointsCntAcc + 1, dataArray[3]));
                    AccXData.add(String.valueOf(dataArray[3]));
                    ChartEntryAccY.add(new Entry(pointsCntAcc + 1, dataArray[4]));
                    AccYData.add(String.valueOf(dataArray[4]));
                    ChartEntryAccZ.add(new Entry(pointsCntAcc + 1, dataArray[5]));
                    AccZData.add(String.valueOf(dataArray[5]));

                    ChartEntryAnrX.add(new Entry(pointsCntAcc, dataArray[6]));
                    ChartEntryAnrY.add(new Entry(pointsCntAcc, dataArray[7]));
                    ChartEntryAnrZ.add(new Entry(pointsCntAcc, dataArray[8]));
                    ChartEntryAnrX.add(new Entry(pointsCntAcc + 1, dataArray[9]));
                    ChartEntryAnrY.add(new Entry(pointsCntAcc + 1, dataArray[10]));
                    ChartEntryAnrZ.add(new Entry(pointsCntAcc + 1, dataArray[11]));

                    dataSetAccX = new LineDataSet(ChartEntryAccX, "Acc-X");
                    dataSetAccX.setColor(getResources().getColor(R.color.colorAccX));
//                    dataSetAccX.setCircleColor(R.color.colorAccX);
                    dataSetAccX.setLineWidth(1.0f);
                    dataSetAccX.setDrawCircleHole(false);
                    dataSetAccX.setDrawCircles(false);
                    dataSetAccX.setDrawValues(false);
//                    dataSetAccX.setValueTextColor(R.color.colorAccX);
                    dataSets.add(dataSetAccX);

                    dataSetAccY = new LineDataSet(ChartEntryAccY, "Acc-Y");
                    dataSetAccY.setColor(getResources().getColor(R.color.colorAccY));
                    dataSetAccY.setLineWidth(1.0f);
                    dataSetAccY.setDrawCircleHole(false);
                    dataSetAccY.setDrawCircles(false);
                    dataSetAccY.setDrawValues(false);
//                    dataSetAccY.setValueTextColor(R.color.colorAccY);
                    dataSets.add(dataSetAccY);

                    dataSetAccZ = new LineDataSet(ChartEntryAccZ, "Acc-Z");
                    dataSetAccZ.setColor(getResources().getColor(R.color.colorAccZ));
                    dataSetAccZ.setLineWidth(1.0f);
                    dataSetAccZ.setDrawCircleHole(false);
                    dataSetAccZ.setDrawCircles(false);
                    dataSetAccZ.setDrawValues(false);
//                    dataSetAccZ.setValueTextColor(R.color.colorAccZ);
                    dataSets.add(dataSetAccZ);

//                        dataSetAnrX = new LineDataSet(ChartEntryAnrX, "X_");
//                        dataSetAnrX.setColor(R.color.colorAnrX);
//                        dataSetAnrX.setLineWidth(1.0f);
//                        dataSetAnrX.setDrawCircleHole(false);
//                        dataSetAnrX.setDrawCircles(false);
//                        dataSetAnrX.setDrawValues(false);
//                        dataSetAnrX.setValueTextColor(R.color.colorAnrX);
//                        dataSets.add(dataSetAnrX);
//
//                        dataSetAnrY = new LineDataSet(ChartEntryAnrY, "Y_");
//                        dataSetAnrY.setColor(R.color.colorAnrY);
//                        dataSetAnrY.setLineWidth(1.0f);
//                        dataSetAnrY.setDrawCircleHole(false);
//                        dataSetAnrY.setDrawCircles(false);
//                        dataSetAnrY.setDrawValues(false);
//                        dataSetAnrY.setValueTextColor(R.color.colorAnrY);
//                        dataSets.add(dataSetAnrY);
//
//                        dataSetAnrZ = new LineDataSet(ChartEntryAnrZ, "Z_");
//                        dataSetAnrZ.setColor(R.color.colorAnrZ);
//                        dataSetAnrZ.setLineWidth(1.0f);
//                        dataSetAnrZ.setDrawCircleHole(false);
//                        dataSetAnrZ.setDrawCircles(false);
//                        dataSetAnrZ.setDrawValues(false);
//                        dataSetAnrZ.setValueTextColor(R.color.colorAnrZ);
//                        dataSets.add(dataSetAnrZ);

                    LineData lineData = new LineData(dataSets);
                    AccChart.setData(lineData);
                    AccChart.invalidate(); // refresh

                    pointsCntAcc += 2;
//                    }
//                    BvpData.add(String.valueOf(dataArray[ptr]));
                } else if (msg.what == TEMP_DATA) {
                    float[] dataArray = msg.getData().getFloatArray(value);
                    assert dataArray != null;

                    int ptr;
                    for (ptr = 0; ptr < 1; ptr++) {
                        List<ILineDataSet> dataSets = new ArrayList<ILineDataSet>();
                        ChartEntryTemp.add(new Entry(pointsCntTemp, dataArray[ptr]));
//                        ChartEntry1.sort(new EntryXComparator());
                        dataSetTemp = new LineDataSet(ChartEntryTemp, "TEMP");
                        dataSetTemp.setColor(getResources().getColor(R.color.colorTemp));
                        dataSetTemp.setLineWidth(1.0f);
                        dataSetTemp.setDrawCircleHole(false);
                        dataSetTemp.setDrawCircles(false);
                        dataSetTemp.setDrawValues(false);
//                        dataSetTemp.setValueTextColor(getResources().getColor(R.color.colorDot));
                        dataSets.add(dataSetTemp);

                        LineData lineData = new LineData(dataSets);
                        TempChart.setData(lineData);
                        TempChart.invalidate(); // refresh

                        TempData.add(String.valueOf(dataArray[ptr]));
                        pointsCntTemp++;
                    }
                }
            }
        };
    }

    @Override
    public void onHiddenChanged(boolean hidden) {
        super.onHiddenChanged(hidden);
    }

    @Override
    public void onStart() {
        super.onStart();

        DataChart = requireView().findViewById(R.id.DataPlotView);
        BvpChart = requireView().findViewById(R.id.BvpPlotView);
        AccChart = requireView().findViewById(R.id.AccPlotView);
        TempChart = requireView().findViewById(R.id.TempPlotView);

        // Receiver for Ble normal information.
        IntentFilter BlePlotEdaFilter = new IntentFilter("com.example.BLE_VALUE_EDAPLOT");
        BroadcastReceiver BlePlotEdaReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                byte[] receivedValue = intent.getByteArrayExtra("value");
                assert receivedValue != null;

                int EdaNumber = ((receivedValue[1] << 8) & 0xFF00) | (receivedValue[0] & 0x00FF);
                Log.i(TAG, "Total Eda Result Number: " + EdaNumber);

                float[] resultArray = new float[EdaNumber];

                int ptr;
                int tempInt;
                // Fill the result array.
                for (ptr = 0; ptr < EdaNumber; ptr++) {
                    tempInt = ((receivedValue[7 + 4 * ptr] << 24) & 0xFF000000) | ((receivedValue[6 + 4 * ptr] << 16) & 0x00FF0000) | ((receivedValue[5 + 4 * ptr] << 8) & 0x0000FF00) | ((receivedValue[4 + 4 * ptr]) & 0x000000FF);
                    resultArray[ptr] = Float.intBitsToFloat(tempInt);
                    Log.i(TAG, String.valueOf(resultArray[ptr]));
                }

                Message uiMessage = null;
                Bundle bundle = new Bundle();

                uiMessage = uiHandler.obtainMessage();
                uiMessage.what = EDA_DATA;
                bundle.putFloatArray(value, resultArray);
                bundle.putInt(number, EdaNumber);
                uiMessage.setData(bundle);
                uiHandler.sendMessage(uiMessage);
            }
        };
        requireActivity().registerReceiver(BlePlotEdaReceiver, BlePlotEdaFilter);

        // Receiver for Ble Bvp information.
        IntentFilter BlePlotBvpFilter = new IntentFilter("com.example.BLE_VALUE_BVPPLOT");
        BroadcastReceiver BlePlotBvpReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                byte[] receivedValue = intent.getByteArrayExtra("value");
                assert receivedValue != null;

//                int EdaNumber = ((receivedValue[1] << 8) & 0xFF00) | (receivedValue[0] & 0x00FF);
//                Log.i(TAG, "Total Bvp Result Number: " + EdaNumber);

                int[] resultArray = new int[20];

                int ptr;
                int tempInt;
                // Fill the result array.
                for (ptr = 0; ptr < 20; ptr++) {
                    resultArray[ptr] = ((receivedValue[3 + 4 * ptr] << 24) & 0xFF000000) | ((receivedValue[2 + 4 * ptr] << 16) & 0x00FF0000) | ((receivedValue[1 + 4 * ptr] << 8) & 0x0000FF00) | ((receivedValue[4 * ptr]) & 0x000000FF);
//                    Log.i(TAG, String.valueOf(resultArray[ptr]));
                }

                Message uiMessage = null;
                Bundle bundle = new Bundle();

                uiMessage = uiHandler.obtainMessage();
                uiMessage.what = BVP_DATA;
                bundle.putIntArray(value, resultArray);
                uiMessage.setData(bundle);
                uiHandler.sendMessage(uiMessage);
            }
        };
        requireActivity().registerReceiver(BlePlotBvpReceiver, BlePlotBvpFilter);

        // Receiver for Ble Acc information.
        IntentFilter BlePlotAccFilter = new IntentFilter("com.example.BLE_VALUE_ACCPLOT");
        BroadcastReceiver BlePlotAccReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                byte[] receivedValue = intent.getByteArrayExtra("value");
                assert receivedValue != null;

//                int EdaNumber = ((receivedValue[1] << 8) & 0xFF00) | (receivedValue[0] & 0x00FF);
//                Log.i(TAG, "Total Bvp Result Number: " + EdaNumber);

                float[] resultArray = new float[12];

                int ptr;
                int tempInt;
                // Fill the result array.
                for (ptr = 0; ptr < 12; ptr++) {
                    tempInt = ((receivedValue[3 + 4 * ptr] << 24) & 0xFF000000) | ((receivedValue[2 + 4 * ptr] << 16) & 0x00FF0000) | ((receivedValue[1 + 4 * ptr] << 8) & 0x0000FF00) | ((receivedValue[4 * ptr]) & 0x000000FF);
                    resultArray[ptr] = Float.intBitsToFloat(tempInt);
                    //                    Log.i(TAG, String.valueOf(resultArray[ptr]));
                }

                Message uiMessage = null;
                Bundle bundle = new Bundle();

                uiMessage = uiHandler.obtainMessage();
                uiMessage.what = ACC_DATA;
                bundle.putFloatArray(value, resultArray);
                uiMessage.setData(bundle);
                uiHandler.sendMessage(uiMessage);
            }
        };
        requireActivity().registerReceiver(BlePlotAccReceiver, BlePlotAccFilter);

        // Receiver for Ble Temp information.
        IntentFilter BlePlotTempFilter = new IntentFilter("com.example.BLE_VALUE_TEMPPLOT");
        BroadcastReceiver BlePlotTempReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                byte[] receivedValue = intent.getByteArrayExtra("value");
                assert receivedValue != null;

//                int EdaNumber = ((receivedValue[1] << 8) & 0xFF00) | (receivedValue[0] & 0x00FF);
//                Log.i(TAG, "Total Bvp Result Number: " + EdaNumber);

                float[] resultArray = new float[1];

                int ptr;
                int tempInt;
                // Fill the result array.
                for (ptr = 0; ptr < 1; ptr++) {
                    tempInt = ((receivedValue[3 + 4 * ptr] << 24) & 0xFF000000) | ((receivedValue[2 + 4 * ptr] << 16) & 0x00FF0000) | ((receivedValue[1 + 4 * ptr] << 8) & 0x0000FF00) | ((receivedValue[4 * ptr]) & 0x000000FF);
                    resultArray[ptr] = Float.intBitsToFloat(tempInt);
                    // Log.i(TAG, String.valueOf(resultArray[ptr]));
                }

                Message uiMessage = null;
                Bundle bundle = new Bundle();

                uiMessage = uiHandler.obtainMessage();
                uiMessage.what = TEMP_DATA;
                bundle.putFloatArray(value, resultArray);
                uiMessage.setData(bundle);
                uiHandler.sendMessage(uiMessage);
            }
        };
        requireActivity().registerReceiver(BlePlotTempReceiver, BlePlotTempFilter);


//        DataChart.getViewTreeObserver().addOnGlobalLayoutListener(
//                new ViewTreeObserver.OnGlobalLayoutListener() {
//                    @Override
//                    public void onGlobalLayout() {
//
////                        DataChart.getViewTreeObserver().removeOnGlobalLayoutListener(this);
////                        int offset = (DataChart.getHeight() - DataChart.getWidth()) / 2;
////
////                        ConstraintLayout.LayoutParams layoutParams = (ConstraintLayout.LayoutParams) DataChart.getLayoutParams();
////                        layoutParams.width = DataChart.getHeight();
////                        layoutParams.height = DataChart.getWidth();
////                        DataChart.setLayoutParams(layoutParams);
////
////                        DataChart.setTranslationX(0);
////                        DataChart.setTranslationY(0);
//                    }
//                });

        clearBtn = requireView().findViewById(R.id.dataPlotClrBtn);
        clearBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ChartEntry1.clear();
                ChartEntryBvp.clear();
                ChartEntryAccX.clear();
                ChartEntryAccY.clear();
                ChartEntryAccZ.clear();
                ChartEntryAnrX.clear();
                ChartEntryAnrY.clear();
                ChartEntryAnrZ.clear();
                ChartEntryTemp.clear();
                pointsCnt = 0;
                pointsCntBvp = 0;
                pointsCntAcc = 0;
                pointsCntTemp = 0;
                DataChart.invalidate(); // refresh
                BvpChart.invalidate();
                AccChart.invalidate();
                TempChart.invalidate();
            }
        });

        saveBtn = requireView().findViewById(R.id.dataPlotSaveBtn);
        saveBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        // write EDA DATA.
                        DateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss", Locale.CHINA);
                        Date date = new Date();

                        File file = new File(requireActivity().getExternalFilesDir(null), "EdaData_" + df.format(date) + ".txt");
                        File file_bvp = new File(requireActivity().getExternalFilesDir(null), "BvpData_" + df.format(date) + ".txt");
                        File file_acc = new File(requireActivity().getExternalFilesDir(null), "AccData_" + df.format(date) + ".txt");
                        File file_temp = new File(requireActivity().getExternalFilesDir(null), "TempData_" + df.format(date) + ".txt");

                        Log.i(TAG, String.valueOf(requireActivity().getExternalFilesDir(null)));

                        // Write Eda.
                        FileOutputStream fos = null;
                        try {
                            fos = new FileOutputStream(file, true);
                        } catch (FileNotFoundException e) {
                            throw new RuntimeException(e);
                        }

                        try {
                            for (int i = 0; i < pointsCnt; i++) {
                                fos.write(String.valueOf(i).getBytes(StandardCharsets.UTF_8));
                                fos.write(", ".getBytes(StandardCharsets.UTF_8));
                                fos.write(String.valueOf(EdaData.get(i)).getBytes(StandardCharsets.UTF_8));
                                fos.write("\r\n".getBytes(StandardCharsets.UTF_8));
                            }
//                            fos.write("success".getBytes(StandardCharsets.UTF_8));
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }

                        // Write Bvp.
                        try {
                            fos = new FileOutputStream(file_bvp, true);
                        } catch (FileNotFoundException e) {
                            throw new RuntimeException(e);
                        }

                        try {
                            for (int i = 0; i < pointsCntBvp; i++) {
                                fos.write(String.valueOf(i).getBytes(StandardCharsets.UTF_8));
                                fos.write(", ".getBytes(StandardCharsets.UTF_8));
                                fos.write(String.valueOf(BvpData.get(i)).getBytes(StandardCharsets.UTF_8));
                                fos.write("\r\n".getBytes(StandardCharsets.UTF_8));
                            }
//                            fos.write("success".getBytes(StandardCharsets.UTF_8));
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }

                        // Write Acc.
                        try {
                            fos = new FileOutputStream(file_acc, true);
                        } catch (FileNotFoundException e) {
                            throw new RuntimeException(e);
                        }

                        try {
                            for (int i = 0; i < pointsCntAcc; i++) {
                                fos.write(String.valueOf(i).getBytes(StandardCharsets.UTF_8));
                                fos.write(", ".getBytes(StandardCharsets.UTF_8));
                                fos.write(String.valueOf(AccXData.get(i)).getBytes(StandardCharsets.UTF_8));
                                fos.write(", ".getBytes(StandardCharsets.UTF_8));
                                fos.write(String.valueOf(AccYData.get(i)).getBytes(StandardCharsets.UTF_8));
                                fos.write(", ".getBytes(StandardCharsets.UTF_8));
                                fos.write(String.valueOf(AccZData.get(i)).getBytes(StandardCharsets.UTF_8));
                                fos.write("\r\n".getBytes(StandardCharsets.UTF_8));
                            }
//                            fos.write("success".getBytes(StandardCharsets.UTF_8));
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }

                        // Write Temp.
                        try {
                            fos = new FileOutputStream(file_temp, true);
                        } catch (FileNotFoundException e) {
                            throw new RuntimeException(e);
                        }

                        try {
                            for (int i = 0; i < pointsCntTemp; i++) {
                                fos.write(String.valueOf(i).getBytes(StandardCharsets.UTF_8));
                                fos.write(", ".getBytes(StandardCharsets.UTF_8));
                                fos.write(String.valueOf(TempData.get(i)).getBytes(StandardCharsets.UTF_8));
                                fos.write("\r\n".getBytes(StandardCharsets.UTF_8));
                            }
//                            fos.write("success".getBytes(StandardCharsets.UTF_8));
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }

                        try {
                            fos.close();
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }
                    }
                }).start();
            }
        });
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_data_plot, container, false);
    }
}