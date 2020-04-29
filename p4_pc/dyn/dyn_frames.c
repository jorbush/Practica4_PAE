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

//TxPacket()  3 par�metres: ID del Dynamixel, Mida dels par�metres, Instruction byte. torna la mida del "Return packet"
byte TxPacket(byte bID, byte bParameterLength, byte bInstruction, // Funci� on el microcontrolador envia el "Instruction Packet".
		const byte * Parametros) { // ID motors: 1 i 2. ID sensors: 3.
	byte bCount, bCheckSum, bPacketLength; // Variables per comptar el byte que estem tractant, pels errors de comunicaci� i la longitud de la trama, respectivament.
	byte TxBuffer[32]; // 32 bytes de "Instruction Packet"
	f_Sentit_Dades_Tx();  //El pin P3.0 (DIRECTION_PORT) el posem a 1 (Transmetre)
	TxBuffer[0] = 0xff;    //Primers 2 bytes que indiquen inici de trama FF, FF.
	TxBuffer[1] = 0xff;
	TxBuffer[2] = bID;         //ID del m�dul al que volem enviar el missatge
	TxBuffer[3] = bParameterLength + 2; //Length(Parameter,Instruction,Checksum). El nombre de bytes del "Instruction Packet" = Nombre de par�metres + 2.
	TxBuffer[4] = bInstruction;    //Instrucci� que enviem al M�dul

	//TODO: La instrucci� no ha de poder modificar les primeres 5 instruccions

	for (bCount = 0; bCount < bParameterLength; bCount++) //Comencem a generar la trama que hem d�enviar
			{
		TxBuffer[bCount + 5] = Parametros[bCount];
	}
	bCheckSum = 0; // Inicialitzem el Check Sum (errors de comunicaci�).
	bPacketLength = bParameterLength + 4 + 2;
	for (bCount = 2; bCount < bPacketLength - 1; bCount++) //C�lcul del checksum
			{
		bCheckSum += TxBuffer[bCount]; // Aquest bucle realitza la seg�ent suma: Check Sum = (ID + LENGTH + INSTRUCTION + PARAMETER 1 + ... + PARAMETER N)
	}
	TxBuffer[bCount] = ~bCheckSum;         //Escriu el Checksum (complement a 1): Check Sum = = ~(ID + LENGTH + INSTRUCTION + PARAMETER 1 + ... + PARAMETER N)
	for (bCount = 0; bCount < bPacketLength; bCount++) //Aquest bucle �s el que envia la trama al M�dul Robot
			{
		f_TxUAC2(TxBuffer[bCount]); // Cridem a la funci� del emulador per enviar cada byte del "Instruction Packet" al m�dul.
	}
	while ((UCA2STATW & UCBUSY)) {
	};   //Espera fins que s�ha transm�s el �ltim byte
	f_Sentit_Dades_Rx(); //Posem la l�nia de dades en Rx perqu� el m�dul Dynamixel envia resposta
	return (bPacketLength); //Retornem la longitud del "Instruction Packet".
}

struct RxReturn RxPacket(void) { // Funci� per rebre el "Status Packet" del m�dul.
	struct RxReturn respuesta; // Status Packet que retornarem.
	byte bCount; // Comptador per tractar el byte que necessitem.

	respuesta.time_out = false; // Posem el temps per rebre el byte desactivat.
	respuesta.idx = 0; // Posem l'�ndex del Status Packet a zero.
	f_Sentit_Dades_Rx();   //Ponemos la linea half duplex en Rx.
	f_Activa_Timer_TimeOut(); // Activem el timer que �s el Timeout (temps de resposta perqu� la UART rebi el byte).
	for (bCount = 0; bCount < 4; bCount++) { // Rep els 4 primers bytes del Status Packet (comen�ament de la trama, ID i longitud d'aquesta).
		f_rx_uart_byte(&respuesta); // La UART rep els bytes mitjan�ant interrupcions.
	} //fin del for
	if (!respuesta.time_out) { // Si el timer Timeout est� desactivat:
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
		const byte * Parametros) { // Enviem una instrucci�, "Instruction Packet", al m�dul  i rebem el seu Status Packet.
	struct RxReturn respuesta;
	TxPacket(bID, bParameterLength, bInstruction, Parametros); // Enviem la instrucci�, "Isntruction Packet", al m�dul.
	respuesta = RxPacket(); // Rebem el Status Packet.
	return respuesta;
}
