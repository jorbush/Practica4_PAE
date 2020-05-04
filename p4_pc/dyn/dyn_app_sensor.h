/*
 * dyn_sensor.h
 *
 *  Created on: 18 mar. 2020
 *      Author: Nerea i Jordi
 */

#ifndef DYN_SENSOR_H_
#define DYN_SENSOR_H_

#include "dyn_instr.h"

int write_obstacle_detected_compare(uint8_t module_id, uint8_t reg_write_val);
int read_obstacle_detected_compare(uint8_t module_id, uint8_t *reg_read_val);

enum infrared_sensor_data { // Sensor d’infrarojos adreces
	I_LEFT = 0x1A, // left
	I_CENTER = 0x1B, // center
	I_RIGHT = 0x1C // right
};
int read_infrared(uint8_t module_id, uint8_t *reg_read_val, enum infrared_sensor_data direccio);

enum luminosity { // Sensors de lluminositat adreces
	L_LEFT = 0x1D,
	L_CENTER = 0x1E,
	L_RIGHT = 0x1F
};
int read_luminosity(uint8_t module_id, uint8_t *reg_read_val, enum luminosity direccio);

int read_obstacle_flag(uint8_t module_id, uint8_t *reg_read_val);

int read_luminosity_flag(uint8_t module_id, uint8_t *reg_read_val);

int read_sound(uint8_t module_id, uint8_t *reg_read_val);

#endif /* DYN_SENSOR_H_ */
