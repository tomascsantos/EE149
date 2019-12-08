// Robot Template app
//
// Framework for creating applications that control the Kobuki robot

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_spi.h"

#include "buckler.h"
#include "simple_ble.h"
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"
#include "mpu9250.h"

#include "states.h"
#include "helper_funcs.h"
#include "Robot_template.h"

// Create a timer
APP_TIMER_DEF(adv_timer);

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

//Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:xx:xx
        .platform_id       = 0x49,    // used as 4th octect in device BLE address
        .device_id         = 0x0006,  // TODO: replace with your lab bench number
        .adv_name          = "slave", // Note that this name is not displayed to save room in the advertisement for data.
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};
simple_ble_app_t* simple_ble_app; //from listener


//_________________NEW LISTNENER___________________
//NOTE: this is re-used for diplay_write characteristic
static simple_ble_service_t led_service = {{
    .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
                0xB5,0x4D,0x22,0x2B,0x89,0x10,0xE6,0x32}
}};

static simple_ble_char_t display_state_char = {.uuid16 = 0x1090};
static char display_buffer[16];

void ble_evt_write(ble_evt_t const* p_ble_evt) {
// This is code to write messages to LCD Display via bluetooth
  if (simple_ble_is_char_event(p_ble_evt, &display_state_char)) {
      printf("Got write to Display characteristic!\n");
      display_write(display_buffer, DISPLAY_LINE_0);
  }
  for(int i = 0; i < 16; i++) {
    display_buffer[i] = '\0';
  }
}
//void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {
//  //p_ble_evt is a struct of type ble_evt_t
//  //this stuct has a field called evt which may be of type ble_gap_evt_t
//  //evt is a struct with field params which may be of type ble_gap_evt_adv_report_t
//  //params is a struct with fields containing advertisement data
//  ble_gap_evt_t event = p_ble_evt->evt.gap_evt;
//  ble_gap_evt_adv_report_t report = event.params.adv_report;
//  // Extract address and compare
//  uint8_t* addr = report.peer_addr.addr;
//  uint8_t master_addr[] = {0x05, 0x00, 0x49, 0xE5, 0x98, 0xC0};
//  // Since memcmp is bad, check each byte in order
//  bool equal = true;
//  for (int j = 0; j < 6; j++){
//    if (addr[j] != gsi_addr[j]) {
//      equal = false;
//    }
//  }
//  if (equal) {
//    // Parse the data field for the hidden message
//    uint8_t* data = report.data.p_data;
//    uint16_t dlen = report.data.len;
//    uint8_t i = 0;
//    uint8_t len = 0;
//    uint8_t type = 0;
//    uint8_t manufact_type = 0xFF;
//    // Loop over bytes
//    while (i < dlen) {
//      // Get length
//      len = data[i];
//      // Compare type to match 0xFF
//      type = data[i+1];
//      // If matches, get data
//      if (type == manufact_type) {
//        // Print data
//        // Skip first two bytes of payload
//        // len-3 to ignore type and company id
//        // i+4 to start at first relevant data byte (skipping company id)
//        printf("Length: %u\n", len);
//        printf("Type: %x\n", type);
//        printf("Data: %s\n", data+i+4);
//        printf("\n");
//        break;
//      }
//      // If no match, skip forward length bytes
//      i = i + len + 1;
//    }
//  }
//}


// global variables
KobukiSensors_t sensors = {0};
// Main application state
// simple_ble_app_t* simple_ble_app; #redundant, see above

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // initialize LEDs
  nrf_gpio_pin_dir_set(23, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(24, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(25, NRF_GPIO_PIN_DIR_OUTPUT);

  // initialize display
  nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
  nrf_drv_spi_config_t spi_config = {
    .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
  };
  error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  APP_ERROR_CHECK(error_code);
  display_init(&spi_instance);
  display_write("Hello, Tomas!", DISPLAY_LINE_0);
  printf("Display initialized!\n");

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);
  mpu9250_init(&twi_mngr_instance);
  printf("IMU initialized!\n");

  //_____NEW - for listener from listener
  // const max44009_config_t config = {
  //   .continuous = 0,
  //   .manual = 0,
  //   .cdr = 0,
  //   .int_time = 3,
  // };
  // max44009_init(&twi_mngr_instance, BUCKLER_LIGHT_INTERRUPT);
  // max44009_config(config);
  // printf("MAX44009 initialized\n");

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);
  simple_ble_add_service(&led_service);

  // for LCD
  simple_ble_add_characteristic(1, 1, 0, 1,
      sizeof(char)*16, (uint8_t*) display_buffer,
      &led_service, &display_state_char);


  //Start advertising
  simple_ble_adv_only_name();

  // initialize Kobuki
  kobukiInit();
  printf("Kobuki initialized!\n");

  // initialize yakindu state machine
  // start statechart
  Robot_template sc_handle;
  robot_template_init(&sc_handle);
  robot_template_enter(&sc_handle);


  // Set a timer to read the light sensor and update advertisement data every second.
  //app_timer_init();
  //app_timer_create(&adv_timer, APP_TIMER_MODE_REPEATED, (app_timer_timeout_handler_t) relative_distances);
  //app_timer_start(adv_timer, APP_TIMER_TICKS(100), NULL); // 100 milliseconds
  
  //LISTENER

  scanning_start();
  // intialize statechart variables
  // if needed

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    kobukiSensorPoll(&sensors);

    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    nrf_delay_ms(1);

    // iterate statechart
    robot_template_runCycle(&sc_handle);
  }
}
