/*
 * dyn_app_motor.h
 *
 *  Created on: 30 mar. 2020
 *      Author: Jordi i Nerea
 *
 *  Declaració de les funcions de moviment.
 *
 */

#ifndef DYN_DYN_APP_MOTOR_H_
#define DYN_DYN_APP_MOTOR_H_

#include "dyn_frames.h"

int write_speed(uint8_t module_id, uint16_t velocitat);
int read_speed(uint8_t module_id, uint16_t* reg_read_val);

enum rotate{TO_LEFT,TO_RIGHT};

int wheel_rotation(uint8_t module_id, enum rotate direccio, unsigned int velocitat); // La velociat ha ser positiva, per això la declarem com "unsigned".

int movement_continue(uint8_t module_id);
int stop_movement(uint8_t wRight, uint8_t wLeft);

int right_movement(uint8_t wRight, uint8_t wLeft, unsigned int velocitat);
int left_movement(uint8_t wRight, uint8_t wLeft, unsigned int velocitat);
int forward_movement(uint8_t wRight, uint8_t wLeft, unsigned int velocitat);
int backward_movement(uint8_t wRight, uint8_t wLeft, unsigned int velocitat);


#endif /* DYN_DYN_APP_MOTOR_H_ */
