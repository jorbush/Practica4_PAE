/*
 * hal_uart.c
 *
 *  Created on: 18 mar. 2020
 *      Author: Jordi i Nerea.
 *
 * Dynamixel framing layer
 */

#include "dyn_frames.h"

#ifndef __MSP432P401R__
#include "hal_dyn_uart/hal_dyn_uart_emu.h"
#include "dyn_test/fake_msp.h"

#define f_TxUAC2 TxUAC2_emu
#define f_Sentit_Dades_Tx Sentit_Dades_Tx_emu
#define f_Sentit_Dades_Rx Sentit_Dades_Rx_emu
#define f_Activa_Timer_TimeOut Activa_Timer_TimeOut_emu
#define f_rx_uart_byte rx_uart_byte_emu

#else
#include "hal_dyn_uart/hal_dyn_uart.h"
#include "msp.h"

#define f_TxUAC2 TxUAC2u
#define f_Sentit_Dades_Tx Sentit_Dades_Tx
#define f_Sentit_Dades_Rx Sentit_Dades_Rx
#define f_Activa_Timer_TimeOut Activa_Timer_TimeOut
#define f_rx_uart_byte rx_uart_byte

#endif

//TxPacket()  3 paràmetres: ID del Dynamixel, Mida dels paràmetres, Instruction byte. torna la mida del "Return packet"
byte TxPacket(byte bID, byte bParameterLength, byte bInstruction, // Funció on el microcontrolador envia el "Instruction Packet".
		const byte * Parametros) { // ID motors: 1 i 2. ID sensors: 3.
	byte bCount, bCheckSum, bPacketLength; // Variables per comptar el byte que estem tractant, pels errors de comunicació i la longitud de la trama, respectivament.
	byte TxBuffer[32]; // 32 bytes de "Instruction Packet"
	f_Sentit_Dades_Tx();  //El pin P3.0 (DIRECTION_PORT) el posem a 1 (Transmetre)
	TxBuffer[0] = 0xff;    //Primers 2 bytes que indiquen inici de trama FF, FF.
	TxBuffer[1] = 0xff;
	TxBuffer[2] = bID;         //ID del mòdul al que volem enviar el missatge
	TxBuffer[3] = bParameterLength + 2; //Length(Parameter,Instruction,Checksum). El nombre de bytes del "Instruction Packet" = Nombre de paràmetres + 2.
	TxBuffer[4] = bInstruction;    //Instrucció que enviem al Mòdul

	//TODO: La instrucció no ha de poder modificar les primeres 5 instruccions

	for (bCount = 0; bCount < bParameterLength; bCount++) //Comencem a generar la trama que hem d’enviar
			{
		TxBuffer[bCount + 5] = Parametros[bCount];
	}
	bCheckSum = 0; // Inicialitzem el Check Sum (errors de comunicació).
	bPacketLength = bParameterLength + 4 + 2;
	for (bCount = 2; bCount < bPacketLength - 1; bCount++) //Càlcul del checksum
			{
		bCheckSum += TxBuffer[bCount]; // Aquest bucle realitza la següent suma: Check Sum = (ID + LENGTH + INSTRUCTION + PARAMETER 1 + ... + PARAMETER N)
	}
	TxBuffer[bCount] = ~bCheckSum;         //Escriu el Checksum (complement a 1): Check Sum = = ~(ID + LENGTH + INSTRUCTION + PARAMETER 1 + ... + PARAMETER N)
	for (bCount = 0; bCount < bPacketLength; bCount++) //Aquest bucle és el que envia la trama al Mòdul Robot
			{
		f_TxUAC2(TxBuffer[bCount]); // Cridem a la funció del emulador per enviar cada byte del "Instruction Packet" al mòdul.
	}
	while ((UCA2STATW & UCBUSY)) {
	};   //Espera fins que s’ha transmès el últim byte
	f_Sentit_Dades_Rx(); //Posem la línia de dades en Rx perquè el mòdul Dynamixel envia resposta
	return (bPacketLength); //Retornem la longitud del "Instruction Packet".
}

struct RxReturn RxPacket(void) { // Funció per rebre el "Status Packet" del mòdul.
	struct RxReturn respuesta; // Status Packet que retornarem.
	byte bCount; // Comptador per tractar el byte que necessitem.

	respuesta.time_out = false; // Posem el temps per rebre el byte desactivat.
	respuesta.idx = 0; // Posem l'índex del Status Packet a zero.
	f_Sentit_Dades_Rx();   //Ponemos la linea half duplex en Rx.
	f_Activa_Timer_TimeOut(); // Activem el timer que és el Timeout (temps de resposta perquè la UART rebi el byte).
	for (bCount = 0; bCount < 4; bCount++) { // Rep els 4 primers bytes del Status Packet (començament de la trama, ID i longitud d'aquesta).
		f_rx_uart_byte(&respuesta); // La UART rep els bytes mitjançant interrupcions.
	} //fin del for
	if (!respuesta.time_out) { // Si el timer Timeout està desactivat:
		for (bCount = 0; bCount < respuesta.StatusPacket[3]; bCount++) { // De zero fins la longitud del paquet (nombre de bytes).
			f_rx_uart_byte(&respuesta); //Rep byte a byte el "Status Packet" (s'encarrega la UART).
		} //fin del for
	}
	//TODO: Decode packet and verify checksum

	return respuesta; // Retornem el "Status Packet"
}


/**
 * Perform a full read/write transaction
 *
 * This function will send an instruction frame to the dynamixel module
 * and the following status frame
 *
 * @param[in] bID Id of the dynamixel module
 * @param[in] bParameterLength Number of parameters to send
 * @param[in] bInstruction Instruction type of the frame
 * @param[in] Parametros Parameters of the TX frame
 * @return Returns a RxReturn struct with the information of the reply
 */
struct RxReturn RxTxPacket(byte bID, byte bParameterLength, byte bInstruction,
		const byte * Parametros) { // Enviem una instrucció, "Instruction Packet", al mòdul  i rebem el seu Status Packet.
	struct RxReturn respuesta;
	TxPacket(bID, bParameterLength, bInstruction, Parametros); // Enviem la instrucció, "Isntruction Packet", al mòdul.
	respuesta = RxPacket(); // Rebem el Status Packet.
	return respuesta;
}
