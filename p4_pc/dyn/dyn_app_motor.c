/*
 * dyn_app_motor.c
 *
 *  Created on: 30 mar. 2020
 *      Author: Jordi i Nerea
 *
 *  Implementació de les funcions de moviment.
 *
 */
#include <stdio.h>
#include "dyn/dyn_app_motor.h"
#include "dyn/dyn_instr.h"

/**
 * Write the speed value on the speed address
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] velocitat The wheel's speed (can be between 0 and 1024)
 * @return Error code
 */
int write_speed(uint8_t module_id, uint16_t velocitat){ // Funció per escriure la velocitat al registre de gestió del motor amb ID "module_id"
	uint8_t low_part= (uint8_t)(velocitat & 0xff); 	// Trunquem la velocitat als 8 bits més baixos (correspon del bit 7 al 0).
	uint8_t high_part = (uint8_t)(velocitat >> 8); 	// Com només volem els 2 bits de la part alta (el bit 9 i 8) per la velocitat,
													// desplacem 8 bits/posicions.
	uint8_t parameters[2] = {low_part,high_part}; 	// Passem com a parametres, com els dos valors a escriure, el valor de
													// la velocitat a la part baixa (del bit 7 al 0) i la part alta (el bit 9 i 8).
	return dyn_write(module_id, 0x20, parameters, 2); // Com comencem a escriure al registre 0x20, escriurem a la part baixa (0x20)
														// el "low_part" i a la part alta (0x21) el "high_part".
}

/**
 * Read the speed value on the speed address
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[out] reg_read_val Pointer where the read values (speed) are stored
 * @return Error code to be treated at higher levels
 */
int read_speed(uint8_t module_id, uint16_t* reg_read_val){ // Funció per llegir la velocitat al registre de gestió del motor amb ID "module_id".
	uint8_t reg_motor[2]; // Llegirà dues adreces: la part baixa (0x20) i la part alta (0x21).
	int reply_error = dyn_read(module_id, 0x20, reg_motor, 2); // Ens retornarà el "Error code". Llegirà les adreces 0x20 (part baixa) i 0x21 (part alta).
	uint8_t low_part = reg_motor[0]; //Assignem la part baixa (0x20).
	uint8_t high_part = reg_motor[1] << 8; // Desplacem els 8 bits de la part alta (0x21).
	*reg_read_val = high_part | low_part; // Fem un "or" per unir la part alta amb la baixa.
	return reply_error;
}

/**
 * Set the wheel's speed and direction.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] direccio Can be TO_LEFT or TO_RIGHT
 * @param[in] velocitat The wheel's speed (can be between 0 and 1024)
 *
 * @return Error code
 */
int wheel_rotation(uint8_t module_id, enum rotate direccio, unsigned int velocitat){ // Funció per establir la velocitat i la direcció de la roda.
	uint8_t velociat_part_alta; // Registre per manipular la part alta (0x21, bits 9 i 8).
	if (direccio == TO_RIGHT){ // En el cas que tinguem que girar cap a la dreta:
		velociat_part_alta = 4+(velocitat >> 8); 	// Desplacem 8 bits/posicions ja que ens interesa els dos bits (9 i 8) de la part alta
													// i sumem un 4 per establir la direcció dreta. Mov_speed_H = ((direction << 2) & 0x04) | ((speed >> 8) & 0x03)
	} else{ // En el cas que tinguem que girar cap a l'esquerra:
		velociat_part_alta = velocitat >> 8; // El mateix que en el cas de girar a la dreta però no s'ha de modificar la direcció per girar a l'esquerra.
	}
	uint8_t parameters[2] = {velocitat, velociat_part_alta}; // Passem com a parametres, com els dos valors a escriure, el valor de
															// la velocitat a la part baixa (del bit 7 al 0) i la part alta (el bit 9 i 8).
	return dyn_write(module_id, 0x20, parameters, 2); 	// Com comencem a escriure al registre 0x20, escriurem a la part baixa (0x20)
														// el "low_part" i a la part alta (0x21) el "high_part".
}

/**
 * The wheel pass to be in continuous rotate.
 *
 * @param[in] module_id Id of the dynamixel module
 * @return Error code
 */
int movement_continue(uint8_t module_id){ // Funció perquè la roda gira de forma continuada.
	uint8_t parameters[2] = {0,0,0,0}; // Posem tots els angles limit de la roda a 0.
	return dyn_write(module_id, 0x06, parameters, 4); // Escrivim a les adreces del angles limits (0x06, 0x07, 0x08 i 0x09) un zero
														// per tal de que la roda no pari de girar.
}

/**
 * Stop the movement of the two wheels.
 *
 * @param[in] wRight Id of the right wheel
 * @param[in] wLeft Id of the left wheel
 * @return Error code
 */
int stop_movement(uint8_t wRight, uint8_t wLeft) { // Funció per parar el moviment de les rodes.
	return wheel_rotation(wLeft, TO_RIGHT, 0) | wheel_rotation(wRight, TO_LEFT, 0); // Posem la velocitat d'ambes rodes a zero.
}

/**
 * The robot rotates to the right.
 *
 * @param[in] wRight Id of the right wheel
 * @param[in] wLeft Id of the left wheel
 * @param[in] velocitat The wheel's speed (can be between 0 and 1024)
 * @return Error code
 */
int right_movement(uint8_t wRight, uint8_t wLeft, unsigned int velocitat) { // Funció per girar cap a la dreta.
	return wheel_rotation(wRight, TO_RIGHT, 0) | wheel_rotation(wLeft, TO_LEFT, velocitat); // Parem la roda dreta (velocitat=0) i movem la roda esquerra.
}

/**
 * The robot rotates to the left.
 *
 * @param[in] wRight Id of the right wheel
 * @param[in] wLeft Id of the left wheel
 * @param[in] velocitat The wheel's speed (can be between 0 and 1024)
 * @return Error code
 */
int left_movement(uint8_t wRight, uint8_t wLeft, unsigned int velocitat) { // Funció girar cap a l'esquerra.
	return wheel_rotation(wLeft, TO_LEFT, 0) | wheel_rotation(wRight, TO_RIGHT, velocitat); // Parem la roda esquerra (velocitat=0) i movem la roda dreta.
}


/**
 * The robot go forward.
 *
 * @param[in] wRight Id of the right wheel
 * @param[in] wLeft Id of the left wheel
 * @param[in] velocitat The wheel's speed (can be between 0 and 1024)
 * @return Error code
 */
int forward_movement(uint8_t wRight, uint8_t wLeft, unsigned int velocitat){ // Funció per anar cap endavant.
	return wheel_rotation(wRight, TO_RIGHT, velocitat) | wheel_rotation(wLeft, TO_LEFT, velocitat); // Girem les dues rodes cap a la mateixa direcció per anar cap endavant.
}

/**
 * The robot go backward.
 *
 * @param[in] wRight Id of the right wheel
 * @param[in] wLeft Id of the left wheel
 * @param[in] velocitat The wheel's speed (can be between 0 and 1024)
 * @return Error code
 */
int backward_movement(uint8_t wRight, uint8_t wLeft, unsigned int velocitat) { // Funció per anar cap enrere.
	return wheel_rotation(wRight, TO_LEFT, velocitat) | wheel_rotation(wLeft, TO_RIGHT, velocitat); // Girem les dues rodes cap a la mateixa direcció, en sentit
																									// contrari al "forward_movement", per anar cap enrere.
}




