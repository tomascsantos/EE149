/** Generated by YAKINDU Statechart Tools code generator. */

#ifndef ROBOT_TEMPLATE_H_
#define ROBOT_TEMPLATE_H_

#include "../states.h"
#include "../helper_funcs.h"
#include "../src/sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'robot_template'.
*/

/*! Define number of states in the state enum */

#define ROBOT_TEMPLATE_STATE_COUNT 6

/*! Define dimension of the state configuration vector for orthogonal states. */
#define ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES 1


/*! Define indices of states in the StateConfVector */
#define SCVI_ROBOT_TEMPLATE_MAIN_REGION_ACTIVE 0
#define SCVI_ROBOT_TEMPLATE_MAIN_REGION_ACTIVE_R1_DRIVE 0
#define SCVI_ROBOT_TEMPLATE_MAIN_REGION_ACTIVE_R1_TURN 0
#define SCVI_ROBOT_TEMPLATE_MAIN_REGION_ACTIVE_R1_REVERSE 0
#define SCVI_ROBOT_TEMPLATE_MAIN_REGION_OFF 0
#define SCVI_ROBOT_TEMPLATE_MAIN_REGION_MOVE 0

/*! Enumeration of all states */ 
typedef enum
{
	Robot_template_last_state,
	Robot_template_main_region_ACTIVE,
	Robot_template_main_region_ACTIVE_r1_Drive,
	Robot_template_main_region_ACTIVE_r1_Turn,
	Robot_template_main_region_ACTIVE_r1_Reverse,
	Robot_template_main_region_OFF,
	Robot_template_main_region_move
} Robot_templateStates;





/*! Type definition of the data structure for the Robot_templateIface interface scope. */
typedef struct
{
	sc_boolean pushed;
	float distance;
	float angle;
	uint16_t prev_encoder;
	states curr_state;
	uint16_t right_speed;
	uint16_t left_speed;
	uint16_t x_dif;
	uint16_t x_d;
	uint16_t x_curr;
	uint16_t y_dif;
	uint16_t y_d;
	uint16_t y_curr;
	float angle_d;
	float angle_curr;
	sc_boolean uphill;
	uint16_t direction;
} Robot_templateIface;




/*! 
 * Type definition of the data structure for the Robot_template state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	Robot_templateStates stateConfVector[ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	Robot_templateIface iface;
} Robot_template;



/*! Initializes the Robot_template state machine data structures. Must be called before first usage.*/
extern void robot_template_init(Robot_template* handle);

/*! Activates the state machine */
extern void robot_template_enter(Robot_template* handle);

/*! Deactivates the state machine */
extern void robot_template_exit(Robot_template* handle);

/*! Performs a 'run to completion' step. */
extern void robot_template_runCycle(Robot_template* handle);


/*! Gets the value of the variable 'pushed' that is defined in the default interface scope. */ 
extern sc_boolean robot_templateIface_get_pushed(const Robot_template* handle);
/*! Sets the value of the variable 'pushed' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_pushed(Robot_template* handle, sc_boolean value);
/*! Gets the value of the variable 'distance' that is defined in the default interface scope. */ 
extern float robot_templateIface_get_distance(const Robot_template* handle);
/*! Sets the value of the variable 'distance' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_distance(Robot_template* handle, float value);
/*! Gets the value of the variable 'angle' that is defined in the default interface scope. */ 
extern float robot_templateIface_get_angle(const Robot_template* handle);
/*! Sets the value of the variable 'angle' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_angle(Robot_template* handle, float value);
/*! Gets the value of the variable 'prev_encoder' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_prev_encoder(const Robot_template* handle);
/*! Sets the value of the variable 'prev_encoder' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_prev_encoder(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'curr_state' that is defined in the default interface scope. */ 
extern states robot_templateIface_get_curr_state(const Robot_template* handle);
/*! Sets the value of the variable 'curr_state' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_curr_state(Robot_template* handle, states value);
/*! Gets the value of the variable 'right_speed' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_right_speed(const Robot_template* handle);
/*! Sets the value of the variable 'right_speed' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_right_speed(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'left_speed' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_left_speed(const Robot_template* handle);
/*! Sets the value of the variable 'left_speed' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_left_speed(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'x_dif' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_x_dif(const Robot_template* handle);
/*! Sets the value of the variable 'x_dif' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_x_dif(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'x_d' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_x_d(const Robot_template* handle);
/*! Sets the value of the variable 'x_d' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_x_d(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'x_curr' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_x_curr(const Robot_template* handle);
/*! Sets the value of the variable 'x_curr' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_x_curr(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'y_dif' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_y_dif(const Robot_template* handle);
/*! Sets the value of the variable 'y_dif' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_y_dif(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'y_d' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_y_d(const Robot_template* handle);
/*! Sets the value of the variable 'y_d' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_y_d(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'y_curr' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_y_curr(const Robot_template* handle);
/*! Sets the value of the variable 'y_curr' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_y_curr(Robot_template* handle, uint16_t value);
/*! Gets the value of the variable 'angle_d' that is defined in the default interface scope. */ 
extern float robot_templateIface_get_angle_d(const Robot_template* handle);
/*! Sets the value of the variable 'angle_d' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_angle_d(Robot_template* handle, float value);
/*! Gets the value of the variable 'angle_curr' that is defined in the default interface scope. */ 
extern float robot_templateIface_get_angle_curr(const Robot_template* handle);
/*! Sets the value of the variable 'angle_curr' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_angle_curr(Robot_template* handle, float value);
/*! Gets the value of the variable 'uphill' that is defined in the default interface scope. */ 
extern sc_boolean robot_templateIface_get_uphill(const Robot_template* handle);
/*! Sets the value of the variable 'uphill' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_uphill(Robot_template* handle, sc_boolean value);
/*! Gets the value of the variable 'direction' that is defined in the default interface scope. */ 
extern uint16_t robot_templateIface_get_direction(const Robot_template* handle);
/*! Sets the value of the variable 'direction' that is defined in the default interface scope. */ 
extern void robot_templateIface_set_direction(Robot_template* handle, uint16_t value);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean robot_template_isActive(const Robot_template* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean robot_template_isFinal(const Robot_template* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean robot_template_isStateActive(const Robot_template* handle, Robot_templateStates state);


#ifdef __cplusplus
}
#endif 

#endif /* ROBOT_TEMPLATE_H_ */
