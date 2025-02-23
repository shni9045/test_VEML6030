/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Date:        08-07-2021
 * Author:      Dave Sluiter
 * Description: This code was created by the Silicon Labs application wizard
 *              and started as "Bluetooth - SoC Empty".
 *              It is to be used only for ECEN 5823 "IoT Embedded Firmware".
 *              The MSLA referenced above is in effect.
 *
 ******************************************************************************/

// Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6


#include "app.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG (1)

#include "src/log.h"

uint16_t eco2;
uint16_t tvoc;


/*****************************************************************************
 * Application Power Manager callbacks
 *****************************************************************************/
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)



bool app_is_ok_to_sleep(void)
{

  return APP_IS_OK_TO_SLEEP;

} // app_is_ok_to_sleep()



sl_power_manager_on_isr_exit_t app_sleep_on_isr_exit(void)
{

  return APP_SLEEP_ON_ISR_EXIT;

} // app_sleep_on_isr_exit()



#endif // defined(SL_CATALOG_POWER_MANAGER_PRESENT)




/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{

  //uint16_t temperature = 0;
  // Put your application 1-time init code here
  // This is called once during start-up.
  // Don't call any Bluetooth API functions until after the boot event.

  // Initilaize GPIO by setting strength
  gpioInit();
  
  // Initializes CMU by enabling and selecting appropriate oscillators
  Init_CMU();

  // Configures CMU by enabling and prescaling appropriate clocks
  Configure_CMU();

  // Initialize I2C
  I2C_init();

  // Initialize LETIMER by setting various bit field in LETIMER0_CTRL register
  Timer_init();

  // Load vale into comp0 & comp1 register
  Timer_load();

  // Clear pending LETIMER0 Interrupts in NVIC
  NVIC_ClearPendingIRQ(LETIMER0_IRQn);

  // Enable LETIMER0 Interrupt in NVIC
  NVIC_EnableIRQ(LETIMER0_IRQn);

  // Enable required interrupts of LETIMER0
  Timer_InterruptEnable();

  //Start timer
  Timer_Onoff(true);

  I2C_Write_VEML6030init();

  LOG_INFO("YOO ---1\r");


#if(LOWEST_ENERGY_MODE==1 || LOWEST_ENERGY_MODE==2 )
    sl_power_manager_add_em_requirement(LOWEST_ENERGY_MODE);
#endif


}




/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  // Put your application code here.
  // This is called repeatedly from the main while(1) loop
  // Notice: This function is not passed or has access to Bluetooth stack events.
  //         We will create/use a scheme that is far more energy efficient in
  //         later assignments.


  uint32_t event;

  event=getNextEvent();

  switch(event){

    case evtLETIMER0_UF:{

        read_ALS_VLM6030();
        //gpioLed0SetOn();


        break;
    }

    case evtLETIMER0_COMP1:{

        //gpioLed0SetOff();
        break;
     }

    default: break;

  }



}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *
 * The code here will process events from the Bluetooth stack. This is the only
 * opportunity we will get to act on an event.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  
  // Just a trick to hide a compiler warning about unused input parameter evt.
  // We will add real functionality here later.
  if (evt->header) {
      printf(".\n");
  }

  // Some events require responses from our application code,
  // and don’t necessarily advance our state machines.
  // For assignment 5 uncomment the next 2 function calls
  // handle_ble_event(evt); // put this code in ble.c/.h

  // sequence through states driven by events
  // state_machine(evt);    // put this code in scheduler.c/.h
  
  
   
} // sl_bt_on_event()

