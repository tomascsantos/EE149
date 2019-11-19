/*
 * helper_funcs.c
 *
 *  Created on: Sep 23, 2018
 *      Author: shromonaghosh, nealjack
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "helper_funcs.h"
#include "states.h"

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
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"
#include "mpu9250.h"

#define pi 3.141592653589793238462
#define alpha 0.05

extern KobukiSensors_t sensors;
static float slow_theta = 0;
static float slow_psi = 0;

static bool new_psi = true;
static bool new_theta = true;

uint16_t read_encoder(){
  return sensors.leftWheelEncoder;
}

int32_t get_direction_fix(bool is_clockwise) {
  return is_clockwise ? -1 : 1;
}

float update_dist(float dist, uint16_t prev_encoder, bool is_forward){
  const float CONVERSION = 0.00008529;
  uint16_t current_encoder = read_encoder();
  float result = 0.0;
  if (!is_forward){
    uint16_t temp = current_encoder;
    current_encoder = prev_encoder;
    prev_encoder = temp;
  }
  if (current_encoder >= prev_encoder) {
    // normal case
    result = (float)current_encoder - (float)prev_encoder;
  } else {
    // account for wrap
    result = (float)current_encoder + (0xFFFF - (float)prev_encoder);
  }
  result = result * CONVERSION;
  if (result> 1.0 || result< -1.0){
    return dist;
  }else{
    return dist +result;
  }
}

void stall() {
  for (uint32_t i = 0; i < 10; i++) kobukiDriveDirect(0, 0);
}

void drive_kobuki(uint16_t left_wheel, uint16_t right_wheel){
  printf("HERE\n");
  printf("DRIVING\n");
  kobukiDriveDirect(left_wheel, right_wheel);
}

void stop_kobuki(){
  // Your code here
  kobukiDriveDirect(0, 0);
}


bool is_left_cliff(){
  // Your code here
  return sensors.cliffLeft;
}

bool is_center_cliff(){
  // Your code here
  return sensors.cliffCenter;
}

bool is_right_cliff(){
  // Your code here
  return sensors.cliffRight;
}

bool is_cliff() {
  return is_left_cliff() || is_center_cliff() || is_right_cliff(); 
}

bool is_left_bumper(){
  return sensors.bumps_wheelDrops.bumpLeft;
}

bool is_right_bumper(){
  return sensors.bumps_wheelDrops.bumpRight;
}

bool is_center_bumper(){
  return sensors.bumps_wheelDrops.bumpCenter;
}

bool is_button_press(){
  return is_button_pressed(&sensors);
}

void start_gyro(){
  mpu9250_start_gyro_integration();
}

void stop_gyro(){
  mpu9250_stop_gyro_integration();
}

float read_gyro(){
  return mpu9250_read_gyro_integration().z_axis;
}

void print_angle(float angle){
  char buf[16];
  snprintf(buf, 16, "ANGLE: %f", angle);
  display_write(buf, DISPLAY_LINE_1);
}

void clear_line(int line_num) {
  if (line_num == 0) {
    char buf[16];
    snprintf(buf, 16, "     ");
    display_write(buf, DISPLAY_LINE_0);
  } else {
    char buf[16];
    snprintf(buf, 16, "     ");
    display_write(buf, DISPLAY_LINE_1);
  }
}

void print_dist(float dist){
  char buf[16];
  snprintf(buf, 16, "DISTANCE: %f", dist);
  display_write(buf, DISPLAY_LINE_1);
}

void print_state(states current_state){
  switch(current_state){
    case OFF:
      display_write("OFF", DISPLAY_LINE_0);
      break;
    case SLOW_DOWN:
      display_write("SLOW DOWN", DISPLAY_LINE_0);
      break;
    case CALIBRATE_TILT:
      display_write("CALIBRATE TILT", DISPLAY_LINE_0);
      break;
    case DRIVE_FORWARD:
      display_write("DRIVE FORWARD", DISPLAY_LINE_0);
      break;
    case TURN_AROUND:
      display_write("TURN_AROUND", DISPLAY_LINE_0);
      break;
    case AVOID_CLIFF:
      display_write("AVOID CLIFF", DISPLAY_LINE_0);
      break;
    case TURNING:
      display_write("TURNING", DISPLAY_LINE_0);
      break;
    case DRIVING:
      display_write("DRIVING", DISPLAY_LINE_0);
      break;
    case BACKUP:
      display_write("BACKUP", DISPLAY_LINE_0);
      break;
    case RIGHT:
      display_write("RIGHT", DISPLAY_LINE_0);
      break;
    case LEFT:
      display_write("LEFT", DISPLAY_LINE_0);
      break;
    case REORIENT:
      display_write("REORIENT", DISPLAY_LINE_0);
      break;
  }
}

void print_turn(turns current_turn){
  switch(current_turn){
    case RIGHT_TURN:
      display_write("RIGHT TURN", DISPLAY_LINE_0);
      break;
    case LEFT_TURN:
      display_write("LEFT TURN", DISPLAY_LINE_0);
      break;
    case STRAIGHT:
      display_write("STRAIGHT", DISPLAY_LINE_0);
      break;
  }
}

float read_tilt_theta(void){
  // Your code here
  mpu9250_measurement_t values = mpu9250_read_accelerometer();  
  float x = values.x_axis, y = values.y_axis, z = values.z_axis;
  float theta = atan(x / sqrt(y * y + z * z));

  if (new_theta) {
    slow_theta = theta;
    new_theta = false;
  } else {
    slow_theta = (1 - alpha) * slow_theta + alpha * theta;
  }

  return slow_theta * 180 / pi;
}

float read_tilt_psi(void){
  mpu9250_measurement_t values = mpu9250_read_accelerometer();  
  float x = values.x_axis, y = values.y_axis, z = values.z_axis;
  float psi  = atan(y / sqrt(x * x + z * z));

  if (new_psi) {
    slow_psi = psi;
    new_psi = false;
  } else {
    slow_psi = (1 - alpha) * slow_psi + alpha * psi;
  }

  return slow_psi * 180 / pi;
}

float get_abs(float var){
  return fabs(var);
}

uint16_t get_speed(float orig_speed, int32_t direction_fix, float rate) {
  float val = orig_speed * (float) direction_fix * rate;
  return (uint16_t) val;
}

uint16_t to_int(float var) {
  return (uint16_t) var;
}

float to_float(uint16_t var) {
  return (float) var;
}
