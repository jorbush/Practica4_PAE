/*
 * hal_dyn_uart.c
 *
 *  Created on: 22 mar. 2020
 *      Author: droma
 */

#include "../hal_dyn_uart/hal_dyn_uart.h"

#include "msp.h"
#define TXD2_READY (UCA2IFG & UCTXIFG)

/* funcions per canviar el sentit de les comunicacions */
void Sentit_Dades_Rx(void) { //Configuració del Half Duplex dels motors: Recepció
	P3OUT &= ~BIT0;   //El pin P3.0 (DIRECTION_PORT) el posem a 0 (Rx)
}

void Sentit_Dades_Tx(void) { //Configuració del Half Duplex dels motors: Transmissió
	P3OUT |= BIT0;    //El pin P3.0 (DIRECTION_PORT) el posem a 1 (Tx)
}

/* funció TxUAC0(byte): envia un byte de dades per la UART 0 */
void TxUAC2(byte bTxdData) {
	while (!TXD2_READY)
		;   // Espera a que estigui preparat el buffer de transmissió
	UCA2TXBUF = bTxdData;
}

void EUSCIA2_IRQHandler(void) {  //interrupcion de recepcion en la UART A2
//UCA2IE &= ~UCRXIE; //Interrupciones desactivadas en RX
	DatoLeido_UART = UCA2RXBUF;
	Byte_Recibido = true;
	//UCA2IE |= UCRXIE; //Interrupciones reactivadas en RX
}

void Activa_Timer_TimeOut() {
	// TODO: Implementar iniciar timer

	TA1CTL |= 0x0200; //MC_2. El timercompta de forma cíclica des de 0 al seu valor màxim. Esta activat el bit 9, el timer selecciona la font SMCLK.
	TA1CTL &= 0xFE09; // Desactivem la resta de bits per si estan activats.
	TA1CCR0 = 0x5DC0; // Establim el limit del comptador.
	TA1CCTL0 |= CCIE; // Habilitem l'interrupció.
}


void Reset_Timeout() {
	// TODO: Implementar reset comptador time out
	 contimer = 0;

}

bool TimeOut(uint16_t cnt) {
	   Reset_Timeout();
	   TA1CTL |= 0x0010; //Activem el bit quatre ("Up mode", el comptador va cap amunt fins arribar a TA0CCR0).
	   TA1CTL &= ~0x0020; // Desactivem la resta de bits.
	   volatile uint32_t i = cnt;
	   do   { //El "delay()" no es mas que un bucle tonto para perder tiempo
	  }
	   while (contimer < i);//Basicamente hacemos un do-while que disminuya el tiempo que hemos
	                  //puesto por parametros hasta 0, despues termina el bucle

	   TA1CTL &= ~0x0030; //Desactivem tots els bits, inclos el 4 que es el que hem estat utilitzant.
	   return true;
}

void rx_uart_byte(struct RxReturn respuesta) {
	Byte_Recibido = false;  //No_se_ha_recibido_Byte();
	Reset_Timeout();
	while (!Byte_Recibido) //Se_ha_recibido_Byte())
	{
		respuesta.time_out = TimeOut(1000); // tiempo en decenas de microsegundos (ara 10ms)
		if (respuesta.time_out)
			break;  //sale del while
	}
	if (!respuesta.time_out) {
		respuesta.StatusPacket[respuesta.idx++] = DatoLeido_UART;
	}

}
