// Analog accelerometer app
//
// Reads data from the ADXL327 analog accelerometer

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>


#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrfx_saadc.h"

#include "buckler.h"
// #include "states.h"

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2

char* fileName = "acceleration.txt";

// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
  // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
nrf_saadc_value_t sample_value (uint8_t channel) {
  nrf_saadc_value_t val;
  ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
  APP_ERROR_CHECK(error_code);
  return val;
}

float supply_volt_ratio = 2.87 / 3;

float convert_adc_to_g(nrf_saadc_value_t adc) {
    float result = 0;
    float lsb = (0.6 / (1.0 / 6)) / (2 << 11); 
    float volts = (float)adc * lsb;
    result = (volts - 1.5 * supply_volt_ratio) / (0.42 * supply_volt_ratio);
    return result;
  }

  float convert_g_to_tilt(char which, float g_x, float g_y, float g_z) {
    float tilt = 0;
    if (which == 'x') {
      tilt = atan(g_x/ sqrt(pow(g_y, 2) + pow(g_z, 2)));
    } else if (which == 'y') {
      tilt = atan(g_y/ sqrt(pow(g_x, 2) + pow(g_z, 2)));
    } else {
      tilt = atan(sqrt(pow(g_x, 2) + pow(g_y, 2)) / g_x);
    }
    return tilt * (180 / 3.14);
  }

  void write_csv_to_sd(char *fileName, float data, int length) {
    // Initialize SD card
    // If no header, add it
    simple_logger_log_header("HEADER for file %s\n", fileName);
    printf("Wrote header to SD card\n");
    // Write sample lines
    simple_logger_log("%f\n", data);
    nrf_gpio_cfg_output(BUCKLER_LED0);
    nrf_gpio_pin_clear(BUCKLER_LED0);
    // for (int i = 0; i < length; i++) {
    //   simple_logger_log("%f \n", data[i]);
    // }
  } 

int main (void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  // initialize analog to digital converter
  nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
  saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
  error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
  APP_ERROR_CHECK(error_code);

  // initialize analog inputs
  // configure with 0 as input pin for now
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
  channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
  channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts
 
  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_X;
  error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Y;
  error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Z;
  error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  nrf_gpio_pin_set(BUCKLER_SD_ENABLE);
  nrf_gpio_pin_set(BUCKLER_SD_CS);
  const char permissions[] = "a"; // w = write, a = append    // Start file
  simple_logger_init(fileName, permissions);

    // If no header, add it
  

  // initialization complete
  printf("Buckler initialized!\n");
  // states state = OFF;

  // loop forever
  while (1) {
    // sample analog inputs
    nrf_saadc_value_t x_val = sample_value(X_CHANNEL);
    nrf_saadc_value_t y_val = sample_value(Y_CHANNEL);
    nrf_saadc_value_t z_val = sample_value(Z_CHANNEL);

    float x_accel =  convert_adc_to_g(x_val);
    // display results
    printf("x: %d\ty: %d\tz:%d\n", x_val, y_val, z_val);
    printf("g_x: %f\n", x_accel);
    // write_csv_to_sd(fileName, x_accel, 1);
    // printf("Angle of x is : %f\n", convert_g_to_tilt('x', convert_adc_to_g(x_val), convert_adc_to_g(y_val), convert_adc_to_g(z_val)));
    nrf_delay_ms(100);

    // TODO: complete state machine
    // switch(state) {
    //   case OFF: {
    //     print_state(state);
    //     // transition logic
    //     if (is_button_pressed(&sensors)) {
    //       state = OFF;
    //     } else if (forwards_state) {
    //         state = FORWARDS;
    //         break;
    //     } else if (backwards_state) {
    //         state = BACKWARDS;
    //         break;
    //     } else if (right_state) {
    //         state = RIGHT;
    //         break;
    //     } else if (left_state) {
    //         state = LEFT;
    //         break;
    //     } else {
    //       state = OFF;
    //       // perform state-specific actions here
    //       kobukiDriveDirect(0, 0);
    //     }
    //     break; // each case needs to end with break!
    //   }
    //   case FORWARDS: {
    //     if (is_button_pressed(&sensors)) {
    //       state = OFF;
    //     }
    //   }
    // }
  }        
}