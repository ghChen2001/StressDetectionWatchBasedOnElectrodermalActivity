/*
 * Copyright 2016 Junk Chen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.junkchen.blelib.sample;

import com.junkchen.blelib.GattAttributes;

/**
 * Created by JunkChen on 2016/3/14 0014.
 */
public class MyGattAttributes extends GattAttributes {
    //GATT Characteristics
    public static final String CHARACTERISTIC_CONFIG_CONTROL = "33221111-5544-7766-9988-AABBCCDDEEFF";
    public static final String CHARACTERISTIC_CONFIG_PASSWORD = "33221112-5544-7766-9988-AABBCCDDEEFF";
    public static final String CHARACTERISTIC_CONFIG_STATUS = "33221113-5544-7766-9988-AABBCCDDEEFF";

    static {
        // Characteristics name.
        attributes.put(CHARACTERISTIC_CONFIG_CONTROL, "Config Control");
        attributes.put(CHARACTERISTIC_CONFIG_PASSWORD, "Config Password");
        attributes.put(CHARACTERISTIC_CONFIG_STATUS, "Config Status");
    }
}
