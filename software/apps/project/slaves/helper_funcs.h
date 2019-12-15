/*
 * helper_funcs.h
 *
 *  Created on: Sep 22, 2018
 *      Author: shromonaghosh
 */

#ifndef HELPER_FUNCS_H_
#define HELPER_FUNCS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "states.h"

extern void print_state(states current_state);
extern void print_turn(turns current_turn);
extern void print_dist(float dist);
extern void print_angle(float angle);

extern bool is_left_cliff();
extern bool is_center_cliff();
extern bool is_right_cliff();
extern bool is_center_bumper();
extern bool is_left_bumper();
extern bool is_right_bumper();
extern bool is_button_press();

extern uint16_t read_encoder();
extern float update_dist(float dist, uint16_t prev_encoder, bool is_forward);

extern int get_rs();
extern float get_cx();
extern float get_cy();
extern float get_dx();
extern float get_dy();
extern float get_msg();
extern float get_theta();

extern void parse();
extern int robot_selector;
extern float curr_x;
extern float curr_theta;
extern float curr_y;
extern float desired_x;
extern float desired_y;

extern bool use_heuristic(float gyro, float angle);
extern bool stop_heuristic(float prev_x, float curr_x);
extern uint16_t left_heuristic(float dist, float gyro, float angle);
extern uint16_t right_heuristic(float dist, float gyro, float angle);

extern float find_rotation(float cx, float cy, float dx, float dy, float or);
extern float find_dist(float cx, float cy, float dx, float dy);

extern uint16_t left_wheel(float dist, float delta_theta);
extern uint16_t right_wheel(float dist, float delta_theta);

extern void drive_kobuki(uint16_t left_wheel, uint16_t right_wheel);
extern void stop_kobuki();

extern void start_gyro();
extern float read_gyro();
extern void stop_gyro();

float read_tilt_theta(void);
float read_tilt_psi(void);

extern float get_abs(float var);

#endif /* HELPER_FUNCS_H_ */
