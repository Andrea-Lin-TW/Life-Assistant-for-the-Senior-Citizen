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

#include "hx_drv_tflm.h"
#include "../inc/synopsys_wei_delay.h"
#include "synopsys_wei_i2c_oled1306.h"


//#include "../inc/main_accelerometer.h"
#include "stdio.h"
char string_buf_ac[100] = "test\n";

#define accel_scale 10

typedef struct
{
	uint8_t symbol;
	uint32_t int_part;
	uint32_t frac_part;
} accel_type;

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
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  //
  // tflite::AllOpsResolver resolver;
  // NOLINTNEXTLINE(runtime-global-variables)
//   static tflite::MicroMutableOpResolver<5> micro_op_resolver;
//   micro_op_resolver.AddAveragePool2D();
//   micro_op_resolver.AddConv2D();
//   micro_op_resolver.AddDepthwiseConv2D();
//   micro_op_resolver.AddReshape();
//   micro_op_resolver.AddSoftmax();

  // Build an interpreter to run the model with.
  // NOLINTNEXTLINE(runtime-global-variables)
//   static tflite::MicroInterpreter static_interpreter(
//       model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
//   interpreter = &static_interpreter;

//   // Allocate memory from the tensor_arena for the model's tensors.
//   TfLiteStatus allocate_status = interpreter->AllocateTensors();
//   if (allocate_status != kTfLiteOk) {
//     TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
//     return;
//   }

  // Get information about the memory area to use for the model's input.
//   input = interpreter->input(0);
// }

uint8_t string_buff_s[20]="                  ";
uint8_t string_buff_f[20]="   Fall down!";
int a;
void loop() {

  	int32_t int_buf;
	accel_type accel_x, accel_y, accel_z;
	uint32_t msec_cnt = 0;
	uint32_t sec_cnt = 0;

	hx_drv_uart_initial(UART_BR_115200);

	//It will initial accelerometer with sampling rate 119 Hz, bandwidth 50 Hz, scale selection 4g at continuous mode.
	//Accelerometer operates in FIFO mode. 
	//FIFO size is 32
	if (hx_drv_accelerometer_initial() != HX_DRV_LIB_PASS)
		hx_drv_uart_print("Accelerometer Initialize Fail\n");
	else
		hx_drv_uart_print("Accelerometer Initialize Success\n");

	//hx_drv_uart_print("Testing: %d.%d sec\n", sec_cnt, msec_cnt / 100);

		uint32_t available_count = 0;
		float x, y, z;
		available_count = hx_drv_accelerometer_available_count();				
		//hx_drv_uart_print("Accel get FIFO: %d\n", available_count);
		for (uint32_t i = 0; i < available_count; i++) 
		{
			hx_drv_accelerometer_receive(&x, &y, &z);
		}

		int_buf = x * accel_scale; //scale value
		if(int_buf < 0)
		{
			int_buf = int_buf * -1;
			accel_x.symbol = '-';
		}
		else 
		{
			accel_x.symbol = '+';
		}
		accel_x.int_part = int_buf / accel_scale;
		accel_x.frac_part = int_buf % accel_scale;


		int_buf = y * accel_scale; //scale value
		if(int_buf < 0)
		{
			int_buf = int_buf * -1;
			accel_y.symbol = '-';
		}
		else 
		{
			accel_y.symbol = '+';
		}
		accel_y.int_part = int_buf / accel_scale;
		accel_y.frac_part = int_buf % accel_scale;


		int_buf = z * accel_scale; //scale value
		if(int_buf < 0)
		{
			int_buf = int_buf * -1;
			accel_z.symbol = '-';
		}
		else 
		{
			accel_z.symbol = '+';
		}
		accel_z.int_part = int_buf / accel_scale;
		accel_z.frac_part = int_buf % accel_scale;



		sprintf(string_buf_ac, "%c%1d.%1d | %c%1d.%1d | %c%1d.%1d G\n", 
				accel_x.symbol, (int) accel_x.int_part, (int) accel_x.frac_part, 
				accel_y.symbol, (int) accel_y.int_part, (int) accel_y.frac_part, 
				accel_z.symbol, (int) accel_z.int_part, (int) accel_z.frac_part);
		hx_drv_uart_print(string_buf_ac);

		hx_drv_uart_print("\n\n");

		hal_delay_ms(10);
		msec_cnt = msec_cnt + 125;
		sec_cnt = sec_cnt + (msec_cnt / 1000);
		msec_cnt = msec_cnt % 1000;
    

    int a;
		if((accel_y.symbol=='-'&&(int) accel_y.frac_part>7)||((int) accel_x.frac_part>7)||((int) accel_z.frac_part)>7)
		{
			a=1;
		  if(a)
			{ 
				OLED_SetCursor(1, 0);
				OLED_DisplayString(string_buff_f);
				OLED_SetCursor(3, 0);
				OLED_DisplayString(string_buff_f);
				OLED_SetCursor(5, 0);
				OLED_DisplayString(string_buff_f);
				OLED_SetCursor(7, 0);
				OLED_DisplayString(string_buff_f);

				OLED_SetCursor(1, 0);
				OLED_DisplayString(string_buff_s);
				OLED_SetCursor(3, 0);
				OLED_DisplayString(string_buff_s);
				OLED_SetCursor(5, 0);
				OLED_DisplayString(string_buff_s);
				OLED_SetCursor(7, 0);
				OLED_DisplayString(string_buff_s);

				OLED_SetCursor(1, 0);
				OLED_DisplayString(string_buff_f);
				OLED_SetCursor(3, 0);
				OLED_DisplayString(string_buff_f);
				OLED_SetCursor(5, 0);
				OLED_DisplayString(string_buff_f);
				OLED_SetCursor(7, 0);
				OLED_DisplayString(string_buff_f);

				OLED_SetCursor(1, 0);
				OLED_DisplayString(string_buff_s);
				OLED_SetCursor(3, 0);
				OLED_DisplayString(string_buff_s);
				OLED_SetCursor(5, 0);
				OLED_DisplayString(string_buff_s);
				OLED_SetCursor(7, 0);
				OLED_DisplayString(string_buff_s);
			}
			a=0; 
		}
  
    
	}
