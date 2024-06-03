package com.example.glusweat;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DeviceAdapter extends RecyclerView.Adapter<DeviceAdapter.MyViewHolder> {
    protected List<Map<String, Object>> Mydata;
    private LayoutInflater inflater;


    public DeviceAdapter(Context context, List<Map<String, Object>> data){
        inflater=LayoutInflater.from(context);
        this.Mydata=data;
    }

    static class MyViewHolder extends RecyclerView.ViewHolder{
        TextView DeviceName;
        Button LinkBtn;
        TextView DeviceAddr;
        TextView DeviceState;
        public static int anInt = 0;

        public MyViewHolder(View itemView) {
            super(itemView);
            DeviceName = (TextView) itemView.findViewById(R.id.item_name);
            LinkBtn = (Button) itemView.findViewById(R.id.btn_connect);
            DeviceAddr = (TextView) itemView.findViewById(R.id.item_address);
            DeviceState = (TextView) itemView.findViewById(R.id.item_connState);
            anInt++;
        }
    }

    //创建每一行的View 用DeviceAdapter.ViewHolder包装
    @NonNull
    @Override
    public DeviceAdapter.MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemView=inflater.inflate(R.layout.item_device,null);
        return new MyViewHolder(itemView);
    }
    //给每一行View填充数据
    @Override
    public void onBindViewHolder(@NonNull DeviceAdapter.MyViewHolder holder, int position) { }
    //数据源的数量
    @Override
    public int getItemCount() {
        return Mydata.size();
    }
}
