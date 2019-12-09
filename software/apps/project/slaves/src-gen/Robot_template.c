/** Generated by YAKINDU Statechart Tools code generator. */

#include "Robot_template.h"
#include "../src/sc_types.h"

/*! \file Implementation of the state machine 'robot_template'
*/

/* prototypes of all internal functions */
static void enact_main_region_ACTIVE(Robot_template* handle);
static void enact_main_region_ACTIVE_r1_Drive(Robot_template* handle);
static void enact_main_region_ACTIVE_r1_Stop(Robot_template* handle);
static void enact_main_region_OFF(Robot_template* handle);
static void enseq_main_region_ACTIVE_default(Robot_template* handle);
static void enseq_main_region_ACTIVE_r1_Drive_default(Robot_template* handle);
static void enseq_main_region_ACTIVE_r1_Stop_default(Robot_template* handle);
static void enseq_main_region_OFF_default(Robot_template* handle);
static void enseq_main_region_default(Robot_template* handle);
static void enseq_main_region_ACTIVE_r1_default(Robot_template* handle);
static void exseq_main_region_ACTIVE(Robot_template* handle);
static void exseq_main_region_ACTIVE_r1_Drive(Robot_template* handle);
static void exseq_main_region_ACTIVE_r1_Stop(Robot_template* handle);
static void exseq_main_region_OFF(Robot_template* handle);
static void exseq_main_region(Robot_template* handle);
static void exseq_main_region_ACTIVE_r1(Robot_template* handle);
static void react_main_region_ACTIVE_r1__entry_Default(Robot_template* handle);
static void react_main_region__entry_Default(Robot_template* handle);
static sc_boolean react(Robot_template* handle);
static sc_boolean main_region_ACTIVE_react(Robot_template* handle, const sc_boolean try_transition);
static sc_boolean main_region_ACTIVE_r1_Drive_react(Robot_template* handle, const sc_boolean try_transition);
static sc_boolean main_region_ACTIVE_r1_Stop_react(Robot_template* handle, const sc_boolean try_transition);
static sc_boolean main_region_OFF_react(Robot_template* handle, const sc_boolean try_transition);
static void clearInEvents(Robot_template* handle);
static void clearOutEvents(Robot_template* handle);


void robot_template_init(Robot_template* handle)
{
	sc_integer i;
	
	for (i = 0; i < ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES; ++i)
	{
		handle->stateConfVector[i] = Robot_template_last_state;
	}
	
	
	handle->stateConfVectorPosition = 0;
	
	clearInEvents(handle);
	clearOutEvents(handle);
	
	/* Default init sequence for statechart robot_template */
	handle->iface.pushed = bool_false;
	handle->iface.distance = 0.0f;
	handle->iface.track = 0.0f;
	handle->iface.angle = 0.0f;
	handle->iface.prev_encoder = 0;
	handle->iface.curr_state = OFF;
	handle->iface.right_speed = 150;
	handle->iface.left_speed = 150;
	handle->iface.cliff_l = bool_false;
	handle->iface.cliff_r = bool_false;
	handle->iface.psi = 0;
	handle->iface.theta = 0;
	handle->iface.angle_d = 0;
	handle->iface.uphill = bool_true;
	handle->iface.direction = 1;
}

void robot_template_enter(Robot_template* handle)
{
	/* Default enter sequence for statechart robot_template */
	enseq_main_region_default(handle);
}

void robot_template_runCycle(Robot_template* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Robot_template_main_region_ACTIVE_r1_Drive:
		{
			main_region_ACTIVE_r1_Drive_react(handle, bool_true);
			break;
		}
		case Robot_template_main_region_ACTIVE_r1_Stop:
		{
			main_region_ACTIVE_r1_Stop_react(handle, bool_true);
			break;
		}
		case Robot_template_main_region_OFF:
		{
			main_region_OFF_react(handle, bool_true);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}

void robot_template_exit(Robot_template* handle)
{
	/* Default exit sequence for statechart robot_template */
	exseq_main_region(handle);
}

sc_boolean robot_template_isActive(const Robot_template* handle)
{
	sc_boolean result = bool_false;
	sc_integer i;
	
	for(i = 0; i < ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Robot_template_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean robot_template_isFinal(const Robot_template* handle)
{
(void)(handle);
return bool_false;

}

sc_boolean robot_template_isStateActive(const Robot_template* handle, Robot_templateStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Robot_template_main_region_ACTIVE :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ROBOT_TEMPLATE_MAIN_REGION_ACTIVE] >= Robot_template_main_region_ACTIVE
				&& handle->stateConfVector[SCVI_ROBOT_TEMPLATE_MAIN_REGION_ACTIVE] <= Robot_template_main_region_ACTIVE_r1_Stop);
			break;
		case Robot_template_main_region_ACTIVE_r1_Drive :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ROBOT_TEMPLATE_MAIN_REGION_ACTIVE_R1_DRIVE] == Robot_template_main_region_ACTIVE_r1_Drive
			);
			break;
		case Robot_template_main_region_ACTIVE_r1_Stop :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ROBOT_TEMPLATE_MAIN_REGION_ACTIVE_R1_STOP] == Robot_template_main_region_ACTIVE_r1_Stop
			);
			break;
		case Robot_template_main_region_OFF :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ROBOT_TEMPLATE_MAIN_REGION_OFF] == Robot_template_main_region_OFF
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(Robot_template* handle)
{
}

static void clearOutEvents(Robot_template* handle)
{
}



sc_boolean robot_templateIface_get_pushed(const Robot_template* handle)
{
	return handle->iface.pushed;
}
void robot_templateIface_set_pushed(Robot_template* handle, sc_boolean value)
{
	handle->iface.pushed = value;
}
float robot_templateIface_get_distance(const Robot_template* handle)
{
	return handle->iface.distance;
}
void robot_templateIface_set_distance(Robot_template* handle, float value)
{
	handle->iface.distance = value;
}
float robot_templateIface_get_track(const Robot_template* handle)
{
	return handle->iface.track;
}
void robot_templateIface_set_track(Robot_template* handle, float value)
{
	handle->iface.track = value;
}
float robot_templateIface_get_angle(const Robot_template* handle)
{
	return handle->iface.angle;
}
void robot_templateIface_set_angle(Robot_template* handle, float value)
{
	handle->iface.angle = value;
}
uint16_t robot_templateIface_get_prev_encoder(const Robot_template* handle)
{
	return handle->iface.prev_encoder;
}
void robot_templateIface_set_prev_encoder(Robot_template* handle, uint16_t value)
{
	handle->iface.prev_encoder = value;
}
states robot_templateIface_get_curr_state(const Robot_template* handle)
{
	return handle->iface.curr_state;
}
void robot_templateIface_set_curr_state(Robot_template* handle, states value)
{
	handle->iface.curr_state = value;
}
uint16_t robot_templateIface_get_right_speed(const Robot_template* handle)
{
	return handle->iface.right_speed;
}
void robot_templateIface_set_right_speed(Robot_template* handle, uint16_t value)
{
	handle->iface.right_speed = value;
}
uint16_t robot_templateIface_get_left_speed(const Robot_template* handle)
{
	return handle->iface.left_speed;
}
void robot_templateIface_set_left_speed(Robot_template* handle, uint16_t value)
{
	handle->iface.left_speed = value;
}
sc_boolean robot_templateIface_get_cliff_l(const Robot_template* handle)
{
	return handle->iface.cliff_l;
}
void robot_templateIface_set_cliff_l(Robot_template* handle, sc_boolean value)
{
	handle->iface.cliff_l = value;
}
sc_boolean robot_templateIface_get_cliff_r(const Robot_template* handle)
{
	return handle->iface.cliff_r;
}
void robot_templateIface_set_cliff_r(Robot_template* handle, sc_boolean value)
{
	handle->iface.cliff_r = value;
}
float robot_templateIface_get_psi(const Robot_template* handle)
{
	return handle->iface.psi;
}
void robot_templateIface_set_psi(Robot_template* handle, float value)
{
	handle->iface.psi = value;
}
float robot_templateIface_get_theta(const Robot_template* handle)
{
	return handle->iface.theta;
}
void robot_templateIface_set_theta(Robot_template* handle, float value)
{
	handle->iface.theta = value;
}
float robot_templateIface_get_angle_d(const Robot_template* handle)
{
	return handle->iface.angle_d;
}
void robot_templateIface_set_angle_d(Robot_template* handle, float value)
{
	handle->iface.angle_d = value;
}
sc_boolean robot_templateIface_get_uphill(const Robot_template* handle)
{
	return handle->iface.uphill;
}
void robot_templateIface_set_uphill(Robot_template* handle, sc_boolean value)
{
	handle->iface.uphill = value;
}
uint16_t robot_templateIface_get_direction(const Robot_template* handle)
{
	return handle->iface.direction;
}
void robot_templateIface_set_direction(Robot_template* handle, uint16_t value)
{
	handle->iface.direction = value;
}

/* implementations of all internal functions */

/* Entry action for state 'ACTIVE'. */
static void enact_main_region_ACTIVE(Robot_template* handle)
{
	/* Entry action for state 'ACTIVE'. */
	handle->iface.pushed = bool_false;
}

/* Entry action for state 'Drive'. */
static void enact_main_region_ACTIVE_r1_Drive(Robot_template* handle)
{
	/* Entry action for state 'Drive'. */
	handle->iface.curr_state = DRIVING;
	handle->iface.cliff_l = bool_false;
	handle->iface.cliff_r = bool_false;
}

/* Entry action for state 'Stop'. */
static void enact_main_region_ACTIVE_r1_Stop(Robot_template* handle)
{
	/* Entry action for state 'Stop'. */
	handle->iface.curr_state = FUYCKYAH;
	handle->iface.cliff_l = bool_false;
	handle->iface.cliff_r = bool_false;
}

/* Entry action for state 'OFF'. */
static void enact_main_region_OFF(Robot_template* handle)
{
	/* Entry action for state 'OFF'. */
	handle->iface.pushed = bool_false;
	handle->iface.cliff_l = bool_false;
	handle->iface.cliff_r = bool_false;
}

/* 'default' enter sequence for state ACTIVE */
static void enseq_main_region_ACTIVE_default(Robot_template* handle)
{
	/* 'default' enter sequence for state ACTIVE */
	enact_main_region_ACTIVE(handle);
	enseq_main_region_ACTIVE_r1_default(handle);
}

/* 'default' enter sequence for state Drive */
static void enseq_main_region_ACTIVE_r1_Drive_default(Robot_template* handle)
{
	/* 'default' enter sequence for state Drive */
	enact_main_region_ACTIVE_r1_Drive(handle);
	handle->stateConfVector[0] = Robot_template_main_region_ACTIVE_r1_Drive;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Stop */
static void enseq_main_region_ACTIVE_r1_Stop_default(Robot_template* handle)
{
	/* 'default' enter sequence for state Stop */
	enact_main_region_ACTIVE_r1_Stop(handle);
	handle->stateConfVector[0] = Robot_template_main_region_ACTIVE_r1_Stop;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state OFF */
static void enseq_main_region_OFF_default(Robot_template* handle)
{
	/* 'default' enter sequence for state OFF */
	enact_main_region_OFF(handle);
	handle->stateConfVector[0] = Robot_template_main_region_OFF;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(Robot_template* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* 'default' enter sequence for region r1 */
static void enseq_main_region_ACTIVE_r1_default(Robot_template* handle)
{
	/* 'default' enter sequence for region r1 */
	react_main_region_ACTIVE_r1__entry_Default(handle);
}

/* Default exit sequence for state ACTIVE */
static void exseq_main_region_ACTIVE(Robot_template* handle)
{
	/* Default exit sequence for state ACTIVE */
	exseq_main_region_ACTIVE_r1(handle);
}

/* Default exit sequence for state Drive */
static void exseq_main_region_ACTIVE_r1_Drive(Robot_template* handle)
{
	/* Default exit sequence for state Drive */
	handle->stateConfVector[0] = Robot_template_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Stop */
static void exseq_main_region_ACTIVE_r1_Stop(Robot_template* handle)
{
	/* Default exit sequence for state Stop */
	handle->stateConfVector[0] = Robot_template_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state OFF */
static void exseq_main_region_OFF(Robot_template* handle)
{
	/* Default exit sequence for state OFF */
	handle->stateConfVector[0] = Robot_template_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void exseq_main_region(Robot_template* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of robot_template.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Robot_template_main_region_ACTIVE_r1_Drive :
		{
			exseq_main_region_ACTIVE_r1_Drive(handle);
			break;
		}
		case Robot_template_main_region_ACTIVE_r1_Stop :
		{
			exseq_main_region_ACTIVE_r1_Stop(handle);
			break;
		}
		case Robot_template_main_region_OFF :
		{
			exseq_main_region_OFF(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region r1 */
static void exseq_main_region_ACTIVE_r1(Robot_template* handle)
{
	/* Default exit sequence for region r1 */
	/* Handle exit of all possible states (of robot_template.main_region.ACTIVE.r1) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Robot_template_main_region_ACTIVE_r1_Drive :
		{
			exseq_main_region_ACTIVE_r1_Drive(handle);
			break;
		}
		case Robot_template_main_region_ACTIVE_r1_Stop :
		{
			exseq_main_region_ACTIVE_r1_Stop(handle);
			break;
		}
		default: break;
	}
}

/* Default react sequence for initial entry  */
static void react_main_region_ACTIVE_r1__entry_Default(Robot_template* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_ACTIVE_r1_Drive_default(handle);
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(Robot_template* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_OFF_default(handle);
}

static sc_boolean react(Robot_template* handle) {
	/* State machine reactions. */
	return bool_false;
}

static sc_boolean main_region_ACTIVE_react(Robot_template* handle, const sc_boolean try_transition) {
	/* The reactions of state ACTIVE. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if ((react(handle)) == (bool_false))
		{ 
			if (handle->iface.pushed == bool_true)
			{ 
				exseq_main_region_ACTIVE(handle);
				enseq_main_region_OFF_default(handle);
			}  else
			{
				did_transition = bool_false;
			}
		} 
	} 
	if ((did_transition) == (bool_false))
	{ 
		handle->iface.pushed = is_button_press();
		print_state(handle->iface.curr_state);
	} 
	return did_transition;
}

static sc_boolean main_region_ACTIVE_r1_Drive_react(Robot_template* handle, const sc_boolean try_transition) {
	/* The reactions of state Drive. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if ((main_region_ACTIVE_react(handle, try_transition)) == (bool_false))
		{ 
			if ((handle->iface.distance) >= ((handle->iface.track * 2)))
			{ 
				exseq_main_region_ACTIVE_r1_Drive(handle);
				enseq_main_region_ACTIVE_r1_Stop_default(handle);
			}  else
			{
				did_transition = bool_false;
			}
		} 
	} 
	if ((did_transition) == (bool_false))
	{ 
		drive_kobuki(handle->iface.left_speed, handle->iface.right_speed);
		handle->iface.distance = update_dist(handle->iface.distance, handle->iface.prev_encoder, bool_true);
		handle->iface.prev_encoder = read_encoder();
		print_state(handle->iface.curr_state);
	} 
	return did_transition;
}

static sc_boolean main_region_ACTIVE_r1_Stop_react(Robot_template* handle, const sc_boolean try_transition) {
	/* The reactions of state Stop. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if ((main_region_ACTIVE_react(handle, try_transition)) == (bool_false))
		{ 
			if ((handle->iface.distance) >= (0))
			{ 
				exseq_main_region_ACTIVE_r1_Stop(handle);
				enseq_main_region_ACTIVE_r1_Drive_default(handle);
			}  else
			{
				did_transition = bool_false;
			}
		} 
	} 
	if ((did_transition) == (bool_false))
	{ 
		drive_kobuki(0, 0);
		print_state(handle->iface.curr_state);
		print_dist(handle->iface.distance);
	} 
	return did_transition;
}

static sc_boolean main_region_OFF_react(Robot_template* handle, const sc_boolean try_transition) {
	/* The reactions of state OFF. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if ((react(handle)) == (bool_false))
		{ 
			if (handle->iface.pushed == bool_true)
			{ 
				exseq_main_region_OFF(handle);
				enseq_main_region_ACTIVE_default(handle);
			}  else
			{
				did_transition = bool_false;
			}
		} 
	} 
	if ((did_transition) == (bool_false))
	{ 
		stop_kobuki();
		handle->iface.psi = read_tilt_psi();
		handle->iface.theta = read_tilt_theta();
		handle->iface.distance = get_msg();
		handle->iface.pushed = is_button_press();
		print_state(OFF);
		print_dist(handle->iface.distance);
	} 
	return did_transition;
}


