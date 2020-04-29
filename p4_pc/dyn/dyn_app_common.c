/*
 * dyn_motor.c
 *
 *  Created on: 18 mar. 2020
 *      Author: Jordi i Nerea
 *
 * High-level functions common for both dynamixel motors and sensors modules.
 *
 */

#include <stdint.h>

#include "dyn/dyn_instr.h"

/**
 * Turn on or off the LED of a given dynamixel module
 *
 * @param[in] id Id of the dynamixel moduel
 * @param[in] val Boolean value for the LED objective state
 */
int dyn_led_control(uint8_t id, bool val) { // Apaguem o encenem el LED del motor o sensor.
	return dyn_write_byte(id, DYN_REG__LED, (uint8_t) val); // Cridem a la funció d'escriure en un mòdul passant el registre del LED.
}

/**
 * Read the current LED status
 *
 * @param[in] id Id of the dynamixel moduel
 * @param[out] val Current LED status
 */
int dyn_led_read(uint8_t id, uint8_t *val) { // Llegim l'estat del LED del motor o sensor, encès o apagat.
	return dyn_read_byte(id, DYN_REG__LED, val); // Cridem a la funció de llegir en un mòdul passant el registre del LED.
}
