// BLE RX app
//
// Receives BLE advertisements with data

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "simple_ble.h"

// BLE configuration
// This is mostly irrelevant since we are scanning only
static simple_ble_config_t ble_config = {
        // BLE address is c0:98:e5:49:00:06
        .platform_id       = 0x49,    // used as 4th octet in device BLE address
        .device_id         = 0x0006,  // Last two octets of device address
        .adv_name          = "EE149", // irrelevant in this example
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS), // send a packet once per second (minimum is 20 ms)
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS), // irrelevant if advertising only
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS), // irrelevant if advertising only
};
simple_ble_app_t* simple_ble_app;

void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {

  ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
  // TODO: extract the fields we care about (Peer address and data)

  // TODO: filter on Peer address
  uint8_t* peer_id = adv_report->peer_addr.addr;
  //printf("peer id: %d\n", peer_id);
  uint8_t peer[6];
  peer[0] = 0xFD;
  peer[1] = 0xFF;
  peer[2] = 0x49;
  peer[3] = 0xE5;
  peer[4] = 0x98;
  peer[5] = 0xC0;
  bool all_match = true;
  for (int i = 0; i < 6 ; i++){
  	if (peer_id[i] != peer[i]) {
  		all_match = false;
  		break;
  	}
  }
  if (all_match) { // replace with condition on peer address
    // if address matches C0:98:E5:49:FF:FD, loop until we find field 0xFF
    //printf("matches\n");
    uint8_t* data = adv_report->data.p_data;
    uint8_t offset = 0;
    // while (1) {
	   //  printf("data: %d\n", data[0]);
	   //  printf("data location: %p\n", data);
    // }
    while (1) {
    	uint8_t len = data[offset];
    	uint8_t type = data[offset + 1];
    	//printf("offset: %d\n", offset);
    	//printf("len: %d\n", len);
    	//printf("type: %d\n", type);
    	if (type == 0xFF) {
    		printf("%s\n", data[offset + 2]);
    		break; // (?)
    	} else {
    		offset += len + 1;
    	}
	      // TODO: get length of field
	      // TODO: get type of field: if type is 0xFF, we found it!
	      // Print the data as a string. i.e. printf("%s\n", data + offset)
	      // Otherwise, skip ahead by the length of the current field
    }
  }
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // Initialize

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized\n");

  // Setup BLE
  // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
  simple_ble_app = simple_ble_init(&ble_config);
  scanning_start();
  advertising_stop();

  // TODO: Start scanning

  while(1) {
    // Sleep while SoftDevice handles BLE
    power_manage();
  }
}



