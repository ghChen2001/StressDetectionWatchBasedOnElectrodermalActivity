//  Based on https://github.com/lupyuen/tflite-bl602/blob/main/main_functions.cc
/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "main_functions.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "constants.h"
#include "model.h"
#include "output_handler.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input1 = nullptr;
TfLiteTensor *input2 = nullptr;
TfLiteTensor *output = nullptr;
int inference_count = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
constexpr int kTensorArenaSize = 1000 * 1024;
uint8_t tensor_arena[kTensorArenaSize] __attribute__((section(".psram_noinit"), aligned(32)));
} // namespace

// The name of this function is important for Arduino compatibility.
void setup()
{
    tflite::InitializeTarget();

    // Set up logging. Google style is to avoid globals or statics because of
    // lifetime uncertainty, but since this has a trivial destructor it's okay.
    // NOLINTNEXTLINE(runtime-global-variables)
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    model = tflite::GetModel(model_cnn_without_acctemp_ft_bin_quant_20240521_203209_tflite);
    // model = tflite::GetModel(g_model);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.",
                             model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }
    printf("Model version: %d\r\n", model->version());

    // Pull in only the operation implementations we need.
    // This relies on a complete list of all the ops needed by this graph.
    // An easier approach is to just use the AllOpsResolver, but this will
    // incur some penalty in code space for op implementations that are not
    // needed by this graph.
    // This pulls in all the operation implementations we need.
    // NOLINTNEXTLINE(runtime-global-variables)
    // static tflite::AllOpsResolver resolver;
    static tflite::MicroMutableOpResolver<9> resolver(error_reporter);
    if (resolver.AddConv2D() != kTfLiteOk) {
        return;
    }
    if (resolver.AddFullyConnected() != kTfLiteOk) {
        return;
    }
    if (resolver.AddSoftmax() != kTfLiteOk) {
        return;
    }
    if (resolver.AddReshape() != kTfLiteOk) {
        return;
    }
    if (resolver.AddConcatenation() != kTfLiteOk) {
        return;
    }
    if (resolver.AddExpandDims() != kTfLiteOk) {
        return;
    }
    if (resolver.AddQuantize() != kTfLiteOk) {
        return;
    }
    if (resolver.AddSplitV() != kTfLiteOk) {
        return;
    }
    if (resolver.AddMaxPool2D() != kTfLiteOk) {
        return;
    }

    // Build an interpreter to run the model with.
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        // TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
        printf("AllocateTensors() failed %d\r\n", allocate_status);
        return;
    }

    // Obtain pointers to the model's input and output tensors.
    input1 = interpreter->input(0);
    input2 = interpreter->input(1);
    // if ((input->dims->size != 4) || (input->dims->data[0] != 1) ||
    //     (input->dims->data[1] != 128) ||
    //     (input->dims->data[2] != kChannelNumber) ||
    //     (input->type != kTfLiteInt8)) {
    //     TF_LITE_REPORT_ERROR(error_reporter,
    //                          "Bad input tensor parameters in model");
    //     return;
    // }
    printf("input1 type %d\r\n", input1->type);
    printf("input1 dim size:%d, data[0]:%d, data[1]:%d\r\n", input1->dims->size, input1->dims->data[0], input1->dims->data[1]);
    printf("input2 type %d\r\n", input2->type);
    printf("input2 dim size:%d, data[0]:%d, data[1]:%d\r\n", input2->dims->size, input2->dims->data[0], input2->dims->data[1]);

    output = interpreter->output(0);
    printf("output type %d\r\n", output->type);
    printf("output dim size:%d, data[0]:%d, data[1]:%d\r\n", output->dims->size, output->dims->data[0], output->dims->data[1]);
    printf("input1->params.zero_point %d\r\n", input1->params.zero_point);
    printf("input2->params.zero_point %d\r\n", input2->params.zero_point);
    printf("input1->params.scale %f\r\n", input1->params.scale);
    printf("input2->params.scale %f\r\n", input2->params.scale);


    // Keep track of how many inferences we have performed.
    inference_count = 0;
}

// // The name of this function is important for Arduino compatibility.
// void loop()
// {
//     // Calculate an x value to feed into the model. We compare the current
//     // inference_count to the number of inferences per cycle to determine
//     // our position within the range of possible x values the model was
//     // trained on, and use this to calculate a value.
//     float position = static_cast<float>(inference_count) /
//                      static_cast<float>(kInferencesPerCycle);
//     float x = position * kXrange;

//     // Quantize the input from floating-point to integer
//     int8_t x_quantized = x / input->params.scale + input->params.zero_point;
//     // Place the quantized input in the model's input tensor
//     input->data.int8[0] = x_quantized;

//     // Run inference, and report any error
//     TfLiteStatus invoke_status = interpreter->Invoke();
//     if (invoke_status != kTfLiteOk) {
//         TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on x: %f\n",
//                              static_cast<double>(x));
//         return;
//     }

//     // Obtain the quantized output from model's output tensor
//     int8_t y_quantized = output->data.int8[0];
//     // Dequantize the output from integer to floating-point
//     float y = (y_quantized - output->params.zero_point) * output->params.scale;

//     // Output the results. A custom HandleOutput function can be implemented
//     // for each supported hardware target.
//     printf("x=%f, y=%f\r\n", x, y); ////
//     //// TODO: HandleOutput(error_reporter, x, y);

//     // Increment the inference_counter, and reset it if we have reached
//     // the total number per cycle
//     inference_count += 1;
//     if (inference_count >= kInferencesPerCycle)
//         inference_count = 0;
// }

// int Calculate(float *data_input, int len)
// {
//     for (uint32_t i = 0; i < len; i++) {
//         input->data.int8[i] = (int8_t)(data_input[i] / input->params.scale + input->params.zero_point);
//     }

//     // Run inference, and report any error
//     TfLiteStatus invoke_status = interpreter->Invoke();
//     if (invoke_status != kTfLiteOk) {
//         TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.\n");
//         return -1;
//     }

//     float output0 = (output->data.int8[0] - output->params.zero_point) * output->params.scale;
//     float output1 = (output->data.int8[1] - output->params.zero_point) * output->params.scale;
//     // printf("output0 %.4f, output1 %.4f\r\n", output0, output1);

//     // Obtain the quantized output from model's output tensor
//     return (output0  > output1) ? 0 : 1;
// }

int Calculate2(float *data_input1, float *data_input2, int len1, int len2)
{
    for (int i = 0; i < len1; i++) {
        input1->data.int8[i] = (int8_t)((double)data_input1[i] / (double)input1->params.scale + (double)input1->params.zero_point);
    }

    for (int i = 0; i < len2; i++) {
        input2->data.int8[i] = (int8_t)((double)data_input2[i] / (double)input2->params.scale + (double)input2->params.zero_point);
    }

    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.\n");
        return -1;
    }

    float output0 = (output->data.int8[0] - output->params.zero_point) * output->params.scale;
    float output1 = (output->data.int8[1] - output->params.zero_point) * output->params.scale;
    // printf("output0 %.4f, output1 %.4f\r\n", output0, output1);

    // Obtain the quantized output from model's output tensor
    return (output0 > output1) ? 0 : 1;
}