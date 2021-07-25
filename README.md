# Life-Assistant-for-the-Senior-Citizen

This work mainly uses the low power consumption and full-time smart features of WE-I Plus, combined with **AIOT** and **AR-like** functions to design a **smart glasses** device to help the elderly or demented elderly live independently , To reduce some obstacles or dangers that may be encountered, thereby reducing the psychological pressure and burden of caregivers.

* [Introduction](#introduction)
  * [Function](#function)
  * [Creative goal](#creative-goal)
  * [System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction
Life Assistant for the Senior Citizen-Smart Glasses for Assisting Silver Hair People with Barrier-free Life.

### Function

* **APP settings reminder**

	Combined with the mobile APP, let family members and caregivers send reminders to the WE-I Plus device via the cloud **at a set time**, and project the information on the glasses.
	
	![app_settings_reminder][1]

* **AI face recognition**

	Through the AI face recognition provided by WE-I Plus, the elderly or patients with dementia can identify their relatives and friends through the **relevant information** displayed on the glasses.
	
	![ai_face_recognition][2]
	
* **Fall warning**

	Use the built-in three-axis acceleration sensor of WE-I Plus to detect glass wearing Whether the person fell. If it is, it will **automatically send a warning message** to the family and caregivers of the mobile **APP or LINE**.

	![fall_warning][3]


* **Voice photography**

	Using the speech recognition function of WE-I Plus, when it is detected that the user **says Yes**, it will **start taking pictures** and grab photo data.

	![voice_photography][4]

### Creative goal

We hope that this product can **alleviate the problem of social elderly care**, reduce the gap caused by **insufficient manpower**, and enable the silver-haired people to carry out daily social life without having to rely on caregivers or relatives.

![creative_goal_1][5]

Through the innovative combination of artificial intelligence, the silver-haired people **have more autonomy** in social life, or have a **sense of control** over their own lives, and at the same time can **reduce the burden on caregivers**.

![creative_goal_2][6]

### System Architecture

![system_architecture][7]

## Hardware and Software Setup
### Required Hardware
- Himax WE-I Plus

![required_hardware_1][8]

- OLED SSD1306
- ESP8266
- NodeMCU

![required_hardware_2][9]

### Required Software
- Teraterm
- ARC GNU Toolchain 2019.03
- Vscode
- Anaconda3
- Python3.8
- Tensorflow 2.3.0

### Hardware Connection

![hardware_connection][10]

## User Manual
### Before Running This Application
* Download source code of Life Assistant for the Senior Citizen from github.
* Make sure all connection is correct again.

### Run This Application
Here take Himax WE-I Plus with ARC GNU Toolchain 2019.03 for example to show how to run this application.
1. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and 115200bps.
2. Interact using Himax WE-I Plus and serial port terminal.
3. After ARC connect with the laptop via USB,select the function you want to show in Life-Assistant-for-the-Senior-Citizen/src/WE-I_plus/



[1]: ./doc/app_settings_reminder.png
[2]: ./doc/ai_face_recognition.png
[3]: ./doc/fall_warning.png
[4]: ./doc/voice_photography.png
[5]: ./doc/creative_goal_1.png
[6]: ./doc/creative_goal_2.png
[7]: ./doc/system_architecture.png
[8]: ./doc/required_hardware_1.png
[9]: ./doc/required_hardware_2.png
[10]: ./doc/hardware_connection.png
