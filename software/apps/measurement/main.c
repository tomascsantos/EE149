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
#include "mpu9250.h"


enum bike_state {
  REST = 0,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

typedef struct {
  float w;
  float x;
  float y;
  float z;
} quaternion_t;

mpu9250_measurement_t init_measurement;
mpu9250_measurement_t acc_measurement;
mpu9250_measurement_t gyro_measurement;
mpu9250_measurement_t gravity = {
  .x_axis = 0,
  .y_axis = 0,
  .z_axis = 1
};
quaternion_t quaternion;
  
  quaternion_t convert_angle_to_quaternion(mpu9250_measurement_t angle){
    float yaw = angle.z_axis / 180 * 3.1415;
    float pitch = angle.y_axis / 180 * 3.1415;
    float roll = angle.x_axis / 180 * 3.1415;
    float cy = cos(yaw / 2);
    float sy = sin(yaw / 2);
    float cp = cos(pitch / 2);
    float sp = sin(pitch / 2);
    float cr = cos(roll / 2);
    float sr = sin(roll / 2);
    quaternion_t q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * cp * sr - sy * sp * cr;
    q.y = sy * cp * sr + cy * sp * cr;
    q.z = sy * cp * cr - cy * sp * sr;
    return q;
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

  mpu9250_measurement_t rotate_axes(mpu9250_measurement_t acc, quaternion_t q){
    float r00 = 1 - 2 * (q.y * q.y + q.z * q.z);
    float r01 = 2 * (q.x * q.y - q.z * q.w);
    float r02 = 2 * (q.x * q.z + q.y * q.w);
    float r10 = 2 * (q.x * q.y + q.z * q.w);
    float r11 = 1 - 2 * (q.x * q.x + q.z * q.z);
    float r12 = 2 * (q.y * q.z - q.x * q.w);
    float r20 = 2 * (q.x * q.z - q.y * q.w);
    float r21 = 2 * (q.y * q.z + q.x * q.w);
    float r22 = 1 - 2 * (q.x * q.x + q.y * q.y);
    float x = r00 * acc.x_axis + r01 * acc.y_axis + r02 * acc.z_axis;
    float y = r10 * acc.x_axis + r11 * acc.y_axis + r12 * acc.z_axis;
    float z = r20 * acc.x_axis + r21 * acc.y_axis + r22 * acc.z_axis;
    acc.x_axis = x;
    acc.y_axis = y;
    acc.z_axis = z - 1;
    return acc;
  }

  mpu9250_measurement_t read_smoothed(int samples){
    acc_measurement.x_axis = 0;
    acc_measurement.y_axis = 0;
    acc_measurement.z_axis = 0;
    mpu9250_measurement_t measurement;
    for(int i = 0; i<samples; i++){
      measurement = mpu9250_read_accelerometer();
      acc_measurement.x_axis += measurement.x_axis;
      acc_measurement.y_axis += measurement.y_axis;
      acc_measurement.z_axis += measurement.z_axis;
    }
    acc_measurement.x_axis = acc_measurement.x_axis / samples;
    acc_measurement.y_axis = acc_measurement.y_axis / samples;
    acc_measurement.z_axis = acc_measurement.z_axis / samples;
    return acc_measurement;
  }


  int read_bike_state(){
    return REST;
  }

  ret_code_t init_bike_state(int samples){
    init_measurement = read_smoothed(samples);
    mpu9250_stop_gyro_integration();
    ret_code_t error_code = mpu9250_start_gyro_integration();
    return error_code;
  }

 
// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

int main (void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);
  mpu9250_init(&twi_mngr_instance);
  printf("IMU initialized!\n");
  
  //init bike state
  error_code = init_bike_state(10);
  APP_ERROR_CHECK(error_code);

  // initialization complete
  printf("Buckler initialized!\n");

  // loop forever
  while (1) {
    acc_measurement = read_smoothed(5);
    gyro_measurement = mpu9250_read_gyro_integration();
    quaternion = convert_angle_to_quaternion(gyro_measurement);
    acc_measurement = rotate_axes(acc_measurement, quaternion);
    // display results
    printf("%.6f    ", acc_measurement.x_axis);
    printf("%.6f    ", acc_measurement.y_axis);
    printf("%.6f    \n", acc_measurement.z_axis);
    //printf("%.6f    ", gyro_measurement.x_axis);
    //printf("%.6f    ", gyro_measurement.y_axis);
    //printf("%.6f    \n", gyro_measurement.z_axis);
    printf("%.6f    ", quaternion.x);
    printf("%.6f    ", quaternion.y);
    printf("%.6f    ", quaternion.z);
    printf("%.6f    \n", quaternion.w);

    nrf_delay_ms(5);
  }        
}