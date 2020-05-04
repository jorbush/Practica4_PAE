/*
 * dyn_sensor.c
 *
 *  Created on: 18 mar. 2020
 *      Author: Nerea i Jordi
 *
 * High-level functions like "distance_wall_front", etc
 * Generate another file for motors, with functions like "move_forward", etc
 *
 * Funcions d'alt nivell dels sensors.
 *
 */
#include "dyn/dyn_app_sensor.h"
/**
 * To write the Obstacle Detected Compare Value direction. It sets the standard value for the object detection
 * that is in the direct line of object sensor parameter.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_write_val Value written to the previous address
 * @return Error code to be treated at higher levels
 */
int write_obstacle_detected_compare(uint8_t module_id, uint8_t reg_write_val){ 	// Funció per escriure a l'adreça del sensor Obstacle Detected Compare
																				// quan es troba un objecte en linia directa.
	return dyn_write_byte(module_id, 0x14, reg_write_val); 	// Com només necessitem escriure un valor, a l'adreça del sensor
															// Obstacle Detected Compare Value (0x14), cridem a la funció "dyn_write_byte".
}

/**
 * To read the Obstacle Detected Compare Value direction.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code
 */
int read_obstacle_detected_compare(uint8_t module_id, uint8_t *reg_read_val){ // Funció per llegir l'adreça del sensor Obstacle Detected Compare
	return dyn_read_byte(module_id, 0x14, reg_read_val); 	// Només s'ha de llegir una adreça (0x14) de 8 bits (1 Byte),
															// així que fem servir la funció "dyn_read_byte".
}

/**
 * To read the infrared sensor directions for measuring distance.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] direccio Can be I_LEFT, I_CENTER or I_RIGHT
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code
 */
int read_infrared(uint8_t module_id, uint8_t *reg_read_val, enum infrared_sensor_data direccio){ // Funció per llegir les adreces del sensor d'infrarojos (per mesurar la distancia).
	return dyn_read_byte(module_id, direccio, reg_read_val); 	// Només s'ha de llegir una adreça (pot ser 0X1A, 0X1B o 0X1C) de 8 bits (1 Byte),
																// així que fem servir la funció "dyn_read_byte".
}

/**
 * To read the luminosity sensor directions.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] direccio Can be L_LEFT, L_CENTER or L_RIGHT
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code
 */
int read_luminosity(uint8_t module_id, uint8_t *reg_read_val, enum luminosity direccio){ // Funció per llegir les adreces del sensor de lluminositat.
	return dyn_read_byte(module_id, direccio, reg_read_val);	// Només es una s'ha de llegir una adreça (pot ser 0X1D, 0X1E o 0X1F) de 8 bits (1 Byte),
																// així que fem servir la funció "dyn_read_byte".
}

/**
 * To read the Obstacle Detection Flag sensor direction. It becomes more larger than the Obstacle Detected Compare Value
 * and sets object detection bit to 1.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code
 */
int read_obstacle_flag(uint8_t module_id, uint8_t *reg_read_val){ // Funció per llegir l'adreça del sensor Obstacle Detection Flag.
	return dyn_read_byte(module_id, 0x20, reg_read_val); 	// Només s'ha de llegir una adreça (0x20) de 8 bits (1 Byte),
															// així que fem servir la funció "dyn_read_byte".
}

/**
 * To read the Luminosity Detection Flag sensor direction.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code to be treated at higher levels
 */
int read_luminosity_flag(uint8_t module_id, uint8_t *reg_read_val){// Funció per llegir l'adreça del sensor Luminosity Detection Flag.
	return dyn_read_byte(module_id, 0x21, reg_read_val); 	// Només s'ha de llegir una adreça (0x21) de 8 bits (1 Byte),
															// així que fem servir la funció "dyn_read_byte".
}

/**
 * To read the sound data sensor direction. The value ranging from 0 to 255 (0xFF)
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code
 */
int read_sound(uint8_t module_id, uint8_t *reg_read_val){ // Funció per llegir l'adreça del sensor de so.
	return dyn_read_byte(module_id, 0x23, reg_read_val);	// Només s'ha de llegir una adreça (0x23) de 8 bits (1 Byte),
															// així que fem servir la funció "dyn_read_byte".
}











