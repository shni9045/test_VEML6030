/*
 * oscillators.c - Source file containing implementation of CMU functionality
 *
 *
 */


#include "oscillators.h"

// Macro definition for clock prescaling value
#define CLK_PRESCALER (4)

#if(LOWEST_ENERGY_MODE==3)
#define OSC_TYPE (cmuOsc_ULFRCO)
#define OSC_SEL (cmuSelect_ULFRCO)
#else
#define OSC_TYPE (cmuOsc_LFXO)
#define OSC_SEL (cmuSelect_LFXO)
#endif


/* Function that initializes CMU by enabling and selecting appropriate oscillators
 * PARAMETERS : NONE
 * RETURNS    : NONE
 *
 */
void Init_CMU(void){

  // Enable the oscillator
  CMU_OscillatorEnable(OSC_TYPE,true,true);

  // Enable oscillator for I2C
  //CMU_OscillatorEnable(cmuOsc_HFXO,true,true);
  CMU_OscillatorEnable(cmuOsc_HFRCO,true,true);

  // Select the reference oscillator for LFA
  CMU_ClockSelectSet(cmuClock_LFA,OSC_SEL);

  // Select the reference oscillator for HF
  CMU_ClockSelectSet(cmuClock_HFPER,cmuSelect_HFRCO);


}


/* Function that configures CMU by enabling and prescaling appropriate clocks
 * PARAMETERS : NONE
 * RETURNS    : NONE
 */
void Configure_CMU(void){

  // Enable LFA CLock tree
  CMU_ClockEnable(cmuClock_LFA,true);

  // Enable LFA CLock tree
   CMU_ClockEnable(cmuClock_HFPER,true);

  // Prescaling of LETIMER clock
  CMU_ClockDivSet(cmuClock_LETIMER0,CLK_PRESCALER);

  // Prescaling of I2C0 clock
  CMU_ClockDivSet(cmuClock_I2C0,4);

  // Enabling the LETIMER0 peripheral clock
  CMU_ClockEnable(cmuClock_LETIMER0,true);

  // Enabling the I2C0 peripheral clock
   CMU_ClockEnable(cmuClock_I2C0,true);

   CMU_ClockEnable(cmuClock_GPIO,true);


}
