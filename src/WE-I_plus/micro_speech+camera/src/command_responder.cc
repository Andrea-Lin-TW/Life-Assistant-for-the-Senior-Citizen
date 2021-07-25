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

#include "command_responder.h"
#include "hx_drv_tflm.h"

#include "stdio.h"
#include "string.h"
#include "synopsys_wei_delay.h"
static int32_t last_command_time = 0;
static uint32_t loop = 0;
static bool all_on = 0;

uint8_t string_buf[100] = "test\n";
hx_drv_sensor_image_config_t pimg_config;

void RespondToCommand(tflite::ErrorReporter* error_reporter,
                      int32_t current_time, const char* found_command,
                      uint8_t score, bool is_new_command) {
  loop++;
  if (is_new_command) {
    TF_LITE_REPORT_ERROR(error_reporter, "Heard %s (%d) @%dms", found_command,
                         score, current_time);
    if (found_command[0] == 'y') {
      last_command_time = current_time;
      hx_drv_led_off(HX_DRV_LED_RED);
      hx_drv_led_on(HX_DRV_LED_GREEN);

      hx_drv_uart_initial(UART_BR_115200);
      //sensor start capture and start streaming
 	    if(hx_drv_sensor_initial(&pimg_config) == HX_DRV_LIB_PASS)
	    hx_drv_uart_print("Camera Initialize Successful\n");

      if(hx_drv_sensor_capture(&pimg_config) == HX_DRV_LIB_PASS)
			{
				hx_drv_uart_print("Camera Get Data Success\n");

				uint8_t * img_ptr;
				img_ptr = (uint8_t *) pimg_config.raw_address;
				hx_drv_uart_print("Start to send \n");
				hx_drv_uart_print("Image width: %d\n", pimg_config.img_width);
				hx_drv_uart_print("Image height: %d\n", pimg_config.img_height);
				hx_drv_uart_print("Image size: %d Bytes\n", pimg_config.raw_size);
				hx_drv_uart_print("Image address: 0x%08x\n", img_ptr);

				for(uint32_t heigth_cnt = 0; heigth_cnt < pimg_config.img_height; heigth_cnt ++)
				{
					for(uint32_t width_cnt = 0; width_cnt < pimg_config.img_width; width_cnt ++)
					{
						hx_drv_uart_print("%3d", *img_ptr);
						if(width_cnt != (pimg_config.img_width - 1))
							hx_drv_uart_print(", ");
						else
							hx_drv_uart_print("\n");

						img_ptr = img_ptr + 1;
					}
				}
				hx_drv_uart_print("End of send\n");
			}



    } else if (found_command[0] == 'n') {
      last_command_time = current_time;
      hx_drv_led_off(HX_DRV_LED_GREEN);
      hx_drv_led_on(HX_DRV_LED_RED);
    }
  }

  if (last_command_time != 0) {
    if (last_command_time < (current_time - 3000)) {
      last_command_time = 0;
      hx_drv_led_off(HX_DRV_LED_GREEN);
      hx_drv_led_off(HX_DRV_LED_RED);
    }
  } else {
    if ((loop % 10) == 0) {
      if (all_on) {
        hx_drv_led_on(HX_DRV_LED_RED);
        hx_drv_led_on(HX_DRV_LED_GREEN);
      } else {
        hx_drv_led_off(HX_DRV_LED_RED);
        hx_drv_led_off(HX_DRV_LED_GREEN);
      }
      all_on = !all_on;
    }
  }
}
