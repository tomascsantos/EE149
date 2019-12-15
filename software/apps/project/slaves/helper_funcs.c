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

extern KobukiSensors_t sensors;


int robot_selector = 0;
float curr_x = 0;
float curr_theta = 0;
float curr_y = 0;
float desired_x = 0;
float desired_y = 0;
extern char buff[64];
extern bool write;

void parse() {
  printf("buffer being parsed: %s", buff);
  if (write) {
    int count = 0;
    char delim[] = ":";
    char temp[64];
    strcpy(temp, buff);
    char *ptr = strtok(temp, delim);
    while (ptr != NULL) {
      printf("start: %s\n", ptr);

      if (count == 0) {
        robot_selector = (int) atof(ptr);
      }
      if (count == 1) {
        curr_x = (float) atof(ptr);
      }
      if (count == 2) {
        curr_y = (float) atof(ptr);
      }
      if (count == 3) {
        curr_theta = (float) atof(ptr);
      }
      if (count == 4) {
        desired_x = (float) atof(ptr);
      }
      if (count == 5) {
        desired_y = (float) atof(ptr);
      }
      count++;
      ptr = strtok(NULL, delim);
    }
    write = 0;
  }
  printf("angle after parsing: %f", curr_theta);
}


// returns true when gyro is greater than half the angle
bool use_heuristic(float gyro, float angle) {
  return (fabs(gyro) > fabs(angle));
}

bool stop_heuristic(float prev_x, float curr_x) {
  return (prev_x != curr_x);
}

// return speeds for left wheel
uint16_t left_heuristic(float dist, float gyro, float angle) {
  float k1 = 100;
  float k2 = .5;
  return (uint16_t) ((k1 * dist) - ((angle - gyro) * k2));
}

//return speeds for right wheel
uint16_t right_heuristic(float dist, float gyro, float angle) {
  float k1 = 100;
  float k2 = .5;  
  return (uint16_t) ((k1 * dist) + ((angle - gyro) * k2));
}


int get_rs() {
  return robot_selector;
}
float get_cx() {
  return curr_x;
}
float get_cy() {
  return curr_y;
}
float get_dx() {
  return desired_x;
}
float get_dy() {
  return desired_y;
}
float get_theta() {
  return curr_theta;
}
float find_rotation(float cx, float cy, float dx, float dy, float or) {
  double ret;
  ret = atan2((double) (dy - cy), (double) (dx - cx));
  ret = (ret - or) * 180 / M_PI;
  if (ret <= -180) {
    return ret + 360;
  }
  if (ret >= 180) {
    return ret - 360;
  }
  return ret;
}

uint16_t left_wheel(float dist, float delta_theta) {
  float k1 = 80;
  float k2 = .6;
  return (uint16_t) ((k1 * dist) - (delta_theta * k2));
}

uint16_t right_wheel(float dist, float delta_theta) {
  float k1 = 80;
  float k2 = .6;  
  return (uint16_t) ((k1 * dist) + (delta_theta * k2));
}

float find_dist(float cx, float cy, float dx, float dy) {
  return sqrt((dx - cx) * (dx - cx) + (dy - cy) * (dy - cy));
}

uint16_t read_encoder(){
  return sensors.leftWheelEncoder;
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

void drive_kobuki(uint16_t left_wheel, uint16_t right_wheel){
	  kobukiDriveDirect(left_wheel, right_wheel);
}

void stop_kobuki(){
	  kobukiDriveDirect(0, 0);
}


bool is_left_cliff(){
  return sensors.cliffLeft;
}

bool is_center_cliff(){
	return sensors.cliffCenter;

}

bool is_right_cliff(){
	 return sensors.cliffRight;

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
  snprintf(buf, 16, "%f", angle);
  display_write(buf, DISPLAY_LINE_1);
}

void print_dist(float dist){
  char buf[16];
  snprintf(buf, 16, "%f", dist);
  display_write(buf, DISPLAY_LINE_1);
}

void print_state(states current_state){
  switch(current_state){
    case OFF:
      display_write("OFF", DISPLAY_LINE_0);
      break;
    case DRIVING:
      display_write("DRIVING", DISPLAY_LINE_0);
      break;
    case TURN_LEFT:
      display_write("TURN_LEFT", DISPLAY_LINE_0);
      break;
    case TURN_RIGHT:
      display_write("TURN_RIGHT", DISPLAY_LINE_0);
      break;
    case FIND:
      display_write("FIND", DISPLAY_LINE_0);
      break;
    case STOP:
      display_write("STOP", DISPLAY_LINE_0);
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
  mpu9250_measurement_t a = mpu9250_read_accelerometer();
  return atan(a.x_axis / sqrt(a.y_axis * a.y_axis + a.z_axis * a.z_axis));
}

float read_tilt_psi(void){
	  mpu9250_measurement_t a = mpu9250_read_accelerometer();
	  return atan(a.y_axis / sqrt(a.x_axis * a.x_axis + a.z_axis * a.z_axis));
}

float get_abs(float var){
  return fabs(var);
}

