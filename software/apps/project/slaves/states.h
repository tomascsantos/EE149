/*
 * states.h
 *
 *  Created on: Sep 22, 2018
 *      Author: shromonaghosh
 */

#ifndef STATES_H_
#define STATES_H_

#include <stdio.h>

typedef enum {
    OFF=0,
    DRIVING,
	TURN_LEFT,
	TURN_RIGHT,
	STOP,
	FIND,
} states;

typedef enum {
    LEFT_TURN,
	RIGHT_TURN,
	STRAIGHT,
} turns;

#endif /* STATES_H_ */
