/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

 /*
	 NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	 The processor MUST be in supervisor mode when vTaskStartScheduler is
	 called.  The demo applications included in the FreeRTOS.org download switch
	 to supervisor mode prior to main being called.  If you are not using one of
	 these demo application projects then ensure Supervisor mode is used.
 */


 /*
  * Creates all the demo application tasks, then starts the scheduler.  The WEB
  * documentation provides more details of the demo application tasks.
  *
  * Main.c also creates a task called "Check".  This only executes every three
  * seconds but has the highest priority so is guaranteed to get processor time.
  * Its main function is to check that all the other tasks are still operational.
  * Each task (other than the "flash" tasks) maintains a unique count that is
  * incremented each time the task successfully completes its function.  Should
  * any error occur within such a task the count is permanently halted.  The
  * check task inspects the count of each task to ensure it has changed since
  * the last time the check task executed.  If all the count variables have
  * changed all the tasks are still executing error free, and the check task
  * toggles the onboard LED.  Should any task contain an error at any time
  * the LED toggle rate will change from 3 seconds to 500ms.
  *
  */

  /* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware(void);
/*-----------------------------------------------------------*/



TaskHandle_t Task1_Handler = NULL;
TaskHandle_t Task2_Handler = NULL;
TaskHandle_t Task3_Handler = NULL;



int task_1_in_time = 0, task_1_out_time = 0, task_1_total_time = 0;
int task_2_in_time = 0, task_2_out_time = 0, task_2_total_time = 0;
int system_time = 0;
int cpu_load = 0;


/* Task to be created. */



void vApplicationIdleHook(void) {



}

void vApplicationTickHook(void) {

}



void Task1(void* pvParameters) {
	int i = 0;
	TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL, (void*)1);
	for (;;) {

		for (i = 0; i < 100000; i++);

		vTaskDelayUntil(&xLastWakeTime, 50);

	}

}

void Task2(void* pvParameters) {
	int i = 0;
	TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL, (void*)2);
	for (;;) {
		for (i = 0; i < 10000; i++);


		vTaskDelayUntil(&xLastWakeTime, 90);


	}
}

void Task3(void* pvParameters) {
	int i = 0;
	TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL, (void*)3);
	for (;;) {
		for (i = 0; i < 100000; i++);


		vTaskDelayUntil(&xLastWakeTime, 180);


	}
}



/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler.
 */


 //void timer1Reset(void){
 //	T1TCR |= 0x2;
 //	T1TCR &= ~0x2;

 //}
 //static void configTimer1(void){
 //	T1PR = 1000;
 //	T1TCR |= 0x1;


 //}
int main(void)
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();



	xTaskCreatePeriodic(
		Task1,       /* Function that implements the task. */
		"Task1,",          /* Text name for the task. */
		100,      /* Stack size in words, not bytes. */
		(void*)1,    /* Parameter passed into the task. */
		1,/* Priority at which the task is created. */
		&Task1_Handler,/* Used to pass out the created task's handle. */
		50);    /*The task periodicty*/


	/* Create Tasks here */
	xTaskCreatePeriodic(
		Task2,       /* Function that implements the task. */
		"Task2",          /* Text name for the task. */
		100,      /* Stack size in words, not bytes. */
		(void*)1,    /* Parameter passed into the task. */
		3,/* Priority at which the task is created. */
		&Task2_Handler,/* Used to pass out the created task's handle. */
		90);    /*The task deadline*/


	xTaskCreatePeriodic(
		Task3,       /* Function that implements the task. */
		"Task3",          /* Text name for the task. */
		100,      /* Stack size in words, not bytes. */
		(void*)1,    /* Parameter passed into the task. */
		3,/* Priority at which the task is created. */
		&Task3_Handler, /* Used to pass out the created task's handle. */
		180);     /*The task periodicity*/







	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for (;; );
}
/*-----------------------------------------------------------*/

static void prvSetupHardware(void)
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	//configTimer1();
	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


