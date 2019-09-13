// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_gpio.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "software_interrupt.h"

#include "buckler.h"

void SWI1_EGU1_IRQHandler(void) {
    NRF_EGU1->EVENTS_TRIGGERED[0] = 0;
    while(1) {
      printf("we interupted software\n");
      nrf_delay_ms(500);
    }
}

void GPIOTE_IRQHandler(void) {
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    gpio_set(24);
    nrf_delay_ms(500);
    gpio_clear(24);
    nrf_delay_ms(500);
    printf("we interupted the software interupt with the button press");
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  gpio_config(24, 1); //led
  gpio_config(22, 0); //switch
  gpio_clear(24);
  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  software_interrupt_init();

  NRF_GPIOTE->CONFIG[0] = 1 | 0x1C << 8 | 0x2 << 16;
  NRF_GPIOTE->INTENSET = 1;

  NVIC_EnableIRQ(GPIOTE_IRQn);
  NVIC_SetPriority(GPIOTE_IRQn, 0);
  NVIC_SetPriority(SWI1_EGU1_IRQn, 1);

  
  software_interrupt_generate();

  // loop forever
  while(1){
    if(gpio_read(22)){
      __WFI();
    } else {
      //do nothing
    }
  }
}

