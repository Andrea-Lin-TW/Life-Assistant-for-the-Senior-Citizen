/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "main_functions.h"

#include "detection_responder.h"
#include "image_provider.h"
#include "model_settings.h"
//#include "person_detect_model_data.h"
#include "model.h"

#include "hx_drv_tflm.h"
#include "synopsys_wei_delay.h"
#include "synopsys_wei_i2c_oled1306.h"

#include "stdio.h"
// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;

// In order to use optimized tensorflow lite kernels, a signed int8_t quantized
// model is preferred over the legacy unsigned model format. This means that
// throughout this project, input images must be converted from unisgned to
// signed format. The easiest and quickest way to convert from unsigned to
// signed 8-bit integers is to subtract 128 from the unsigned value to get a
// signed value.

// An area of memory to use for input, output, and intermediate arrays.
constexpr int kTensorArenaSize = 136 * 1024;
static uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  hx_drv_share_switch(SHARE_MODE_I2CM);

	OLED_Init();
	OLED_Clear();
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(face_recognition_model_int8_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  //
  // tflite::AllOpsResolver resolver;
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroMutableOpResolver<6> micro_op_resolver;
  micro_op_resolver.AddAveragePool2D();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddDepthwiseConv2D();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddSoftmax();
  micro_op_resolver.AddMaxPool2D();


  // Build an interpreter to run the model with.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Get information about the memory area to use for the model's input.
  input = interpreter->input(0);
}

// The name of this function is important for Arduino compatibility.
// void loop() {
//   // Get image from provider.
//   if (kTfLiteOk != GetImage(error_reporter, kNumCols, kNumRows, kNumChannels,
//                             input->data.int8)) {
//     TF_LITE_REPORT_ERROR(error_reporter, "Image capture failed.");
//   }

//   // Run the model on this input and make sure it succeeds.
//   if (kTfLiteOk != interpreter->Invoke()) {
//     TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
//   }

//   TfLiteTensor* output = interpreter->output(0);

//   // Process the inference results.
//   int8_t person_score = output->data.uint8[kPersonIndex];
//   int8_t no_person_score = output->data.uint8[kNotAPersonIndex];
//   RespondToDetection(error_reporter, person_score, no_person_score);
// }

uint8_t string_buff_A[20]="Person:Amy";
uint8_t string_buff_Ar[20]="granddaughter";
uint8_t string_buff_Aa[20]="21 years old";
uint8_t string_buff_Ai[20]="like drawing";

uint8_t string_buff_s[20]="                  ";
uint8_t string_buff1[20]="No Person!";

uint8_t string_buff_K[20]="Person:Kimy";
uint8_t string_buff_Kr[20]="friend";
uint8_t string_buff_Ka[20]="20 years old";
uint8_t string_buff_Ki[20]="like swimming";

void loop() {
	// Get image from provider.
	if (kTfLiteOk != GetImage(error_reporter, kNumCols, kNumRows, kNumChannels,
								input->data.int8)) {
		TF_LITE_REPORT_ERROR(error_reporter, "Image capture failed.");
	}

	// Run the model on this input and make sure it succeeds.
	if (kTfLiteOk != interpreter->Invoke()) {
		TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
	}

	TfLiteTensor* output = interpreter->output(0);

	// Process the inference results.
	int8_t Andrea_score = output->data.uint8[kAndrea];
	int8_t Kimy_score = output->data.uint8[kKimy];
	RespondToDetection(error_reporter, Andrea_score, Kimy_score);
	if(Andrea_score>80)
  {
    OLED_SetCursor(1, 0);
	  OLED_DisplayString(string_buff_A);
    OLED_SetCursor(3, 0);
    OLED_DisplayString(string_buff_Ar);
    OLED_SetCursor(5, 0);
    OLED_DisplayString(string_buff_Aa);
    OLED_SetCursor(7, 0);
    OLED_DisplayString(string_buff_Ai);
  }
	
  if(Kimy_score>80)
  {
    OLED_SetCursor(1, 0);
	  OLED_DisplayString(string_buff_K);
    OLED_SetCursor(3, 0);
    OLED_DisplayString(string_buff_Kr);
    OLED_SetCursor(5, 0);
    OLED_DisplayString(string_buff_Ka);
    OLED_SetCursor(7, 0);
    OLED_DisplayString(string_buff_Ki);
  }

  if(Andrea_score<80 && Kimy_score<80)
  { 
    OLED_SetCursor(1, 0);
	  OLED_DisplayString(string_buff1);
    OLED_SetCursor(3, 0);
    OLED_DisplayString(string_buff_s);
    OLED_SetCursor(5, 0);
    OLED_DisplayString(string_buff_s);
    OLED_SetCursor(7, 0);
    OLED_DisplayString(string_buff_s);
  }

	}
