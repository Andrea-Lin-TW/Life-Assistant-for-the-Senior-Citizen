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
#include "stdio.h"
#include "hx_drv_tflm.h"
#define accel_scale 10
char string_buf_ac[100] = "test\n";
char reminder_message[20] ;
uint32_t tick_start = 0;
uint32_t tick_end = 0;
uint32_t tick_now = 0 ;
uint32_t tick_200ms =0;
int8_t action = 0;
int8_t tick_action = 0;
bool fallalert;

bool read_AT_and_cmp(char); 
bool send_AT_and_retry(char, int, char);
bool esp8266_is_initialized = false;


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

/****寄出AT指令去連接ESP8266函數宣告****/
bool read_AT_and_cmp(char *cmp_str)
{
	int8_t recdata[10] = {};
	////String recStr;
	char recStr[10];
	memset(recdata, '\0', sizeof(recdata));
	int count = 0;
	////if (mySerial.available())
	if (hx_drv_swuart_single_read(recdata + count) == HX_DRV_LIB_PASS) {
	do{
		*(recdata+count) = hx_drv_swuart_single_read();
		if (*(recdata + count) == 0xa)
		{
			count++;
			break;
		}
		count++;  
	}while (hx_drv_swuart_single_read(recdata + count) == HX_DRV_LIB_PASS >0);
	count++;
	}
	////Serial.print("response: ");
	hx_drv_uart_print("response: ");
	////Serial.write((char*)recdata, strlen(recdata));
	hx_drv_uart_getchar((char*)recdata, strlen(recdata));

	if (strcmp((char*)recdata, cmp_str) != 0)
	{
		return false;
	}
	return true;
}

bool send_AT_and_retry(char *senddata, int len, char *ack)
{
int retry = 3;
bool valid = false;
	for (int i = 0; i < retry; i++)
	{
		////mySerial.write(senddata, len);
		hx_drv_swuart_write((uint8_t *)senddata, len);
		// delay(200);
		////mySerial.flush();
		// hx_drv_swuart_flush();
		// delay(500);
		if (read_AT_and_cmp(ack))
		{
			valid = true;
			return true;
		}
	}
return false;
}

void esp8266_init(){
	if (!esp8266_is_initialized){
		if (send_AT_and_retry((char *)"AT\x0d\x0a", 4, (char *)"OK\x0d\x0a"))
		{
			hx_drv_uart_print("loar ESP8266 fail.");
			esp8266_is_initialized = true;
		}
		else
		{
			esp8266_is_initialized = false;
		}
	}
}
void sendATcommand(const char *toSend){  
hx_drv_uart_print("Sending: ");
////Serial.println(toSend);
hx_drv_uart_print(toSend);
////mySerial.println(toSend);//, sizeof(toSend));//, strlen(toSend));
hx_drv_swuart_print(toSend);
//delay(200);
////mySerial.flush();
// hx_drv_swuart_flush();
do{
	////mySerial.read();
	hx_drv_swuart_read();
		
////}while (mySerial.available() >0);
}while ((hx_drv_swuart_single_read(recdata + count) == HX_DRV_LIB_PASS)>0);
//delay(200);
}

void message(){
	/****如果tick_now > tick_action****/
	if(tick_now > tick_action)
	{
		////Serial.println(action);
		hx_drv_uart_print(action);
		switch(action)
		{
			case 0:
				////wait_ms = 1000;
				tick_action = 1000;
			break;
			case 1:
				sendATcommand("AT+CWJAP=\"hello\",\"fdsa3304\"");
				////wait_ms = 3000;
				tick_action =3000;
			break;
			case 2:
				sendATcommand("AT+CIPSTART=\"TCP\",\"192.168.87.120\",8080");
				////wait_ms = 3000;
				tick_action =3000;
			break;
			case 3:
			if(fallalert){
				sendATcommand("AT+CIPSEND=5");
				////mySerial.println("smart"); 
				hx_drv_swuart_print("smart");
				fallalert= false;
				////wait_ms = 2000;
				tick_action =2000;
				}else  
			if(hx_drv_swuart_single_read(recdata + count) == HX_DRV_LIB_PASS){
				////Serial.print("message received: ");
				hx_drv_uart_print("message received: ");
				////reminder_message = mySerial.readString();
				reminder_message = hx_drv_swuart_read();
				/////Serial.print("message received: ");
				hx_drv_uart_print("message received: ");
				////Serial.println(reminder_message); 
				hx_drv_uart_print(reminder_message);
			}
			////wait_ms = 10000;
			tick_action =10000;          
			break;
			case 4:
				sendATcommand("AT+CWQAP");
				////wait_ms = 1000;
				tick_action =1000;
			break;
			default:
				////wait_ms= 1000;
				tick_action =1000;
			break;				
		}
		tick_action = tick_action+tick_now;
		action ++;
		if (action>=5) action = 0;
	}
}

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

	tick_now = 0 ;
	hx_drv_tick_start();
	action = 0;
	tick_action = 0;
	tick_200ms =tick_now+200;
	fallalert = false;
  }


uint8_t string_buff_s[20]="                  ";
uint8_t string_buff_f[20]="Fall down!";
void loop() {
	hx_drv_tick_get(&tick_now);
	tick_now = tick_now/400000.0;
	if (tick_now >=tick_200ms)
	{
		/*****三軸加速器偵測if (fall_alert)*****/
		int32_t int_buf;
		accel_type accel_x, accel_y, accel_z;
		uint32_t msec_cnt = 0;
		uint32_t sec_cnt = 0;
		hx_drv_uart_initial(UART_BR_115200);
		if (hx_drv_accelerometer_initial() != HX_DRV_LIB_PASS)
			hx_drv_uart_print("Accelerometer Initialize Fail\n");
		else
			hx_drv_uart_print("Accelerometer Initialize Success\n");

		uint32_t available_count = 0;
		float x, y, z;
		available_count = hx_drv_accelerometer_available_count();				
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

		// hal_delay_ms(10);
		// msec_cnt = msec_cnt + 125;
		// sec_cnt = sec_cnt + (msec_cnt / 1000);
		// msec_cnt = msec_cnt % 1000;
	

		if((accel_y.symbol=='-'&&(int) accel_y.frac_part>7)||((int) accel_x.frac_part>7)||((int) accel_z.frac_part)>7)
		{
			fallalert=true;
			/****偵測到跌倒****/
			if(fallalert)
			{ 	
				tick_200ms = tick_now;
				/****使用者螢幕上顯示跌倒****/
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

				/****如果!esp8266_is_initialized****/
				if (!esp8266_is_initialized){
					esp8266_init();
				}
				else{
					message();
				}
			}
			fallalert=false; 
		}
		else
		{
			fallalert=false;
			tick_200ms = tick_now;
		}
	}
}
