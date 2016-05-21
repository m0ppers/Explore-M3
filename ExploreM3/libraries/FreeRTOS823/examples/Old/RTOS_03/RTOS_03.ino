/***************************************************************************************************
									ExploreEmbedded
****************************************************************************************************
 * File: RTOS_03.ino
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Program to demonstrate the task switching between the three tasks depending upon priorities

The libraries have been tested on ExploreEmbedded development boards. We strongly believe that the
library works on any of development boards for respective controllers. However, ExploreEmbedded
disclaims any kind of hardware failure resulting out of usage of libraries, directly or indirectly.
Files may be subject to change without prior notice. The revision history contains the information
related to updates.


GNU GENERAL PUBLIC LICENSE:
	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program. If not, see <http://www.gnu.org/licenses/>.

Errors and omissions should be reported to codelibraries@exploreembedded.com
 **************************************************************************************************/

/* Scheduler include files. */
#include <FreeRtOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include <croutine.h>


#define LED_IdleTask P2_0
#define LED_Task1 P2_1
#define LED_Task2 P2_2
#define LED_Task3 P2_3
#define LED_Task4 P2_4
#define LED_Task5 P2_5

xTaskHandle TaskHandle_1;
xTaskHandle TaskHandle_2;
xTaskHandle TaskHandle_3;
xTaskHandle TaskHandle_4;
xTaskHandle TaskHandle_5;

void setup()
{
  // put your setup code here, to run once:

  pinMode(LED_IdleTask, OUTPUT);
  pinMode(LED_Task1, OUTPUT);
  pinMode(LED_Task2, OUTPUT);
  pinMode(LED_Task3, OUTPUT);
  pinMode(LED_Task4, OUTPUT);
  pinMode(LED_Task5, OUTPUT);

  /* Initialize the Serial communication module */
  Serial0.begin(38400);
  Serial0.printf("\n\r In Main\n\r");

  /* Create the three tasks with priorities 1,2,3. Only tasks will be created.
   * Tasks will be excecuted once the scheduler is started.
   * An idle task is also created, which will be run when there are no tasks in RUN state */
  xTaskCreate(MyTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, &TaskHandle_1);
  xTaskCreate(MyTask3, "Task3", configMINIMAL_STACK_SIZE, NULL, 3, &TaskHandle_3);

  xTaskCreate(MyIdleTask, "IdleTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

  Serial0.printf("\n\rIn main function, invoking scheduler");

  vTaskStartScheduler();		/* Start the schedular */

}

void loop()
{
  // put your main code here, to run repeatedly:
}

static void MyTask1(void* pvParameters)
{
  LED_Glow(LED_Task1);	 /* Led to indicate the execution of Task1*/
  Serial0.printf("\n\rIn Task1");
  vTaskDelete(TaskHandle_1);
}


static void MyTask2(void* pvParameters)
{
  LED_Glow(LED_Task2);	 /* Led to indicate the execution of Task2*/
  Serial0.printf("\n\rIn Task2 ");
  vTaskDelete(TaskHandle_2);
}


static void MyTask3(void* pvParameters)
{

  LED_Glow(LED_Task3);	 /* Led to indicate the execution of Task3*/
  Serial0.printf("\n\rTask3, creating new tasks 2");

  /* Create two new tasks 2, 4 */
  xTaskCreate(MyTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, &TaskHandle_2);
  Serial0.printf("\n\rTask3, creating new tasks 4");

  xTaskCreate(MyTask4, "Task4", configMINIMAL_STACK_SIZE, NULL, 4, &TaskHandle_4);

  LED_Glow(LED_Task3);	 /* Led to indicate the execution of Task3*/
  Serial0.printf("\n\rBack in Task3, Creating Task5");

  xTaskCreate(MyTask5, "Task5", configMINIMAL_STACK_SIZE, NULL, 5, &TaskHandle_5);

  LED_Glow(LED_Task3);	 /* Led to indicate the execution of Task3*/
  Serial0.printf("\n\rBack in Task3, Exiting task3");

  vTaskDelete(TaskHandle_3);

}



static void MyTask4(void* pvParameters)
{

  LED_Glow(LED_Task4);	 /* Led to indicate the execution of Task4*/
  Serial0.printf("\n\rIn Task4");
  vTaskDelete(TaskHandle_4);
}


static void MyTask5(void* pvParameters)
{

  LED_Glow(LED_Task5);	 /* Led to indicate the execution of Task4*/
  Serial0.printf("\n\rIn Task5");
  vTaskDelete(TaskHandle_5);
}


static void MyIdleTask(void* pvParameters)
{
  while(1)
  {
    LED_Glow(LED_IdleTask);		 /* Led to indicate the execution of Idle Task*/
    Serial0.printf("\n\rIn idle state");
  }
}

/* function to glow only single LED at a time */
static void LED_Glow(int LED)
{
  /* turn-off all LEDS */
  digitalWrite(LED_IdleTask, LOW);
  digitalWrite(LED_Task1, LOW);
  digitalWrite(LED_Task2, LOW);
  digitalWrite(LED_Task3, LOW);
  digitalWrite(LED_Task4, LOW);
  digitalWrite(LED_Task5, LOW);

  /* turn-on only required LED */
  switch(LED)
  {
    case LED_Task1:
      digitalWrite(LED_Task1, HIGH);
      break;
    case LED_Task2:
      digitalWrite(LED_Task2, HIGH);
      break;
    case LED_Task3:
      digitalWrite(LED_Task3, HIGH);
      break;
    case LED_Task4:
      digitalWrite(LED_Task4, HIGH);
      break;
    case LED_Task5:
      digitalWrite(LED_Task5, HIGH);
      break;
    case LED_IdleTask:
      digitalWrite(LED_IdleTask, HIGH);
      break;
    default :
      break;
  }
}