/*
 * hal_dyn.c
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 *
 * This file implements the different dynamixel instructions
 */

#include "dyn/dyn_instr.h"
#include "dyn/dyn_frames.h"

/**
 * Single byte write instruction
 *
 * This function sends a write instruction for a single address position
 * to a given dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the write is performed
 * @param[in] reg_write_val Value written to the previous address
 * @return Error code to be treated at higher levels.
 */
int dyn_write_byte(uint8_t module_id, DYN_REG_t reg_addr, uint8_t reg_write_val) { // Esriu el valor del tercer parametre entrat a l'adre�a entrada, del m�dul amb l'ID introdu�t.
	uint8_t parameters[2] ; // Hem de pasar dos parametres: l'adre�a on volem escriure i el nombre d'adreces on volem escriure (1).
	struct RxReturn reply; // Per guardar la resposta, el Status Packet, de la instrucci� que enviem.
	// Parametres necessaris per cridar a la funci� per obtenir el Status Packet
	parameters[0] = reg_addr; // Adre�a on volem escriure.
	parameters[1] = reg_write_val; // La longitud de valors que volem escriure (nom�s un).
	reply = RxTxPacket(module_id, 2, DYN_INSTR__WRITE, parameters); // Cridem a la funci� per enviar l'instrucci� d'escriure al m�dul amb l'ID corresponent i rebem el Status Packet.

	return (reply.tx_err < 1) | reply.time_out; // Retorna el codi d'error.
}

/**
 * Single byte read instruction
 *
 * This function sends a read instruction for a single address position
 * to a given dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the read is performed
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code to be treated at higher levels.
 */
int dyn_read_byte(uint8_t module_id, DYN_REG_t reg_addr, uint8_t* reg_read_val) { // Llegeix l'adre�a entrada del m�dul amb l'ID introdu�t i se l'assigna al tercer parametre entrat.
	uint8_t parameters[2]; // Hem de pasar dos parametres: l'adre�a on volem llegir i el nombre d'adreces que volem llegir (1).
	struct RxReturn reply; // Per guardar la resposta, el Status Packet, de la instrucci� que enviem.
	// Parametres necessaris per cridar a la funci� per obtenir el Status Packet.
	parameters[0] = reg_addr; // Adre�a que volem llegir.
	parameters[1] = 1; // El nombre d'adreces que volem llegir (nom�s una).
	reply = RxTxPacket(module_id, 2, DYN_INSTR__READ, parameters); // Cridem a la funci� per enviar l'instrucci� de llegir al m�dul amb l'ID corresponent i rebem el Status Packet.
	*reg_read_val = reply.StatusPacket[5]; // Guardem al punter el valor llegit (el valor que ens interesa del Status Packet obtingut).

	return (reply.tx_err < 1) | reply.time_out; // Retorna el codi d'error.
}

/**
 * Multi-byte write instruction
 *
 * This function sends a write instruction starting at a given address position
 * with a given length for a dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the write is performed
 * @param[in] val Pointer to the byte array to be written
 * @param[in] len Number of position to be written
 * @return Error code to be treated at higher levels.
 */
int dyn_write(uint8_t module_id, DYN_REG_t reg_addr, uint8_t *val, uint8_t len) { // Funci� que escriu els bytes entrats a partir de l'adre�a entrada.
	uint8_t parameters[len+1]; 	// Farem servir un array de longitud "len+1", ja que posarem l'adre�a a partir de la que es realitza
								// l'escriptura (ocupar� la primera posici� del array)
								// i els bytes que s'han d'escriure (de longitud "len"), el nombre d'adreces on volem escriure.
	struct RxReturn reply; // Per guardar la resposta, el Status Packet, de la instrucci� que enviem.

	parameters[0] = reg_addr; // Adre�a a partir de la qual volem comen�ar a escriure.
	int i;
	for (i = 0; i < len ; i++)	{ // Recorrem l'array de bytes que s'han d'escriure.
		parameters[i+1] = val[i]; // Fiquem els bytes a escriure dins l'array de parametres que li passarem al m�dul en el Instruction Packet.
	}

	reply = RxTxPacket(module_id, len + 1, DYN_INSTR__WRITE, parameters); 	// Cridem a la funci� per enviar l'instrucci� d'escriure (Instruction Packet)
																			// al m�dul amb l'ID corresponent i rebem el Status Packet. Li passem "len + 1" ja que es la longitud
																			// de l'array de parametres.
	return (reply.tx_err < 1) | reply.time_out; // Retorna el codi d'error.
}

/**
 * Multi-byte read instruction
 *
 * This function sends a read instruction starting at a given address position
 * with a given length for a dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the read is performed
 * @param[in] len Number of position to be read
 * @param[out] reg_read_val Pointer where the read values are stored
 * @return Error code to be treated at higher levels
 */
int dyn_read(uint8_t module_id, DYN_REG_t reg_addr, uint8_t* reg_read_val, uint8_t len){
	uint8_t parameters[2]; // Hem de pasar dos parametres: l'adre�a on volem comen�ar a llegir i el nombre d'adreces que volem llegir (len).
	struct RxReturn reply; // Per guardar la resposta, el Status Packet, de la instrucci� que enviem.
	// Parametres necessaris per cridar a la funci� per obtenir el Status Packet.
	parameters[0] = reg_addr; // Adre�a a partir de la qual volem comen�ar a llegir.
	parameters[1] = len; // Aquesta longitud correspon al nombre d'adreces que volem llegir.
	reply = RxTxPacket(module_id, 0x04, DYN_INSTR__READ, parameters); 	// Cridem a la funci� per enviar l'instrucci� de llegir (Instruction Packet)
																		// al m�dul amb l'ID corresponent i rebem el Status Packet. Li passem 4 com
																		// la longitud de la instrucci� (nombre de parametres+2).
	int i;
	for (i = 0; i < len; i++) { // Recorrem les adreces que volem llegir.
		reg_read_val[i] = reply.StatusPacket[5 + i]; // Guardem als punters els valors llegits (agafant els valors que ens interesen del Status Packet obtingut).
	}

	return (reply.tx_err < 1) | reply.time_out; // Retorna el codi d'error.

}



