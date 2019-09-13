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
}

void GPIOTE_IRQHandler(void) {
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    gpio_config(24, 1); //led
    gpio_set(24);
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  NRF_GPIOTE->CONFIG[0] = 28;
  NRF_GPIOTE->INTENSET = 1;

  NVIC_EnableIRQ(GPIOTE_IRQn);

  

  // loop forever
  while (1) {
    printf("Looping\n");
    nrf_delay_ms(1000);
  }
}

