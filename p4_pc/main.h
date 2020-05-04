/*
 * main.h
 *
 *  Created on: 19 mar. 2020
 *      Author: droma
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <semaphore.h>

sem_t sem_tx_msp;
sem_t sem_tx_cli;

enum dyn_module
{
	MOTOR_LEFT = 0,
	MOTOR_RIGHT = 1,
	SENSOR = 2
};

#endif /* MAIN_H_ */
