/*
 * hal_dyn_uart_emu.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Jordi i Nerea.
 *
 * El mode UART es comunica amb els motors i sensors del robot.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "dyn_test/b_queue.h"
#include "main.h"
#include "dyn/dyn_frames.h"
#include "fake_msp.h"

//Defines
typedef uint8_t byte;
#define TXD2_READY (UCA2IFG & UCTXIFG)

/* Funció per inicialitzar i configurar l'UART. */
/*
void Init_UART(void){
	UCA2CTLW0 |= UCSWRST; //Fem un reset de la USCI, desactiva la USCI
	UCA2CTLW0 |= UCSSEL__SMCLK; //UCSYNC=0 mode asíncron, UART és asíncron.
								//UCMODEx = 0 seleccionem mode UART, posem a 0 el bit UCSYNC.
								//UCSPB=0 nomes 1 stop bit
								//UC7BIT=0 8 bits de dades
								//UCMSB=0 bit de menys pes primer
								//UCPAR=x ja que no es fa servir bit de paritat
								//UCPEN=0 sense bit de paritat
								//Triem SMCLK (Secondary Master Clock) (24MHz) com a font del clock BRCLK
	UCA2MCTLW = UCOS16; // Necessitem sobre mostreig => bit 0 = UCOS16 = 1
	UCA2BRW = 3; //Prescaler de BRCLK fixat a 3. Com SMCLK va a24MHz,
				//volem un baud rate de 500kb/s i fem sobre mostreig de 16
				//el rellotge de la UART ha de ser de 8MHz (24MHz/
	//Configurem els pins de la UART
	P3SEL0 |= BIT2 | BIT3; //I/O funció: P3.3 = UART2TX, P3.2 = UART2RX
	P3SEL1 &= ~ (BIT2 | BIT3);
	//Configurem pin de selecció del sentit de les dades Transmissió/Recepeció
	P3SEL0 &= ~BIT0; //Port P3.0 com GPIO (I/O digital)
	P3SEL1 &= ~BIT0;
	P3DIR |= BIT0; //Port P3.0 com sortida Data Direction selector Tx Rx
	P3OUT &= ~BIT0; //Inicialitzem Sentit Dades a 0 Rx
	UCA2CTLW0 &= ~UCSWRST; //Reactivem la línia de comunicacions sèrie
	// UCA2IE |= UCRXIE; //Això només s’ha d’activar quan tinguem la rutina de recepció
} */


/* Funcions per canviar el sentit de les comunicacions: */

/* Funció per canviar el sentit de les dades a RX */
void Sentit_Dades_Rx_emu(void) { //Configuració del Half Duplex dels motors: Recepció
	// P3OUT &= ~BIT0; //El pin P3.0 (DIRECTION_PORT) el posem a 0 Rx
	printf("Changed direction to RX\n");
}
/* Funció per canviar el sentit de les dades a TX */
void Sentit_Dades_Tx_emu(void) { //Configuració del Half Duplex dels motors: Transmissió
	// P3OUT |= BIT0;//El pin P3.0 (DIRECTION_PORT) el posem a 1 Tx
	printf("Changed direction to TX\n");
}


/* Funció TxUAC0(byte): envia un byte de dades per la UART 0 */
void TxUAC2_emu(byte bTxdData) {
	// Wait for Tx consumer to get data
	sem_wait(&sem_tx_cli); 	// Esperem al que el semafor es posi en verd, consumeix el
							// recurs semafor, es decrementa i es posa vermell per qualsevol
							// que intenti accedir al semafor.
	// Copy data to global var
	UCA2TXBUF = bTxdData; // Assignem el byte passat per parametre.
	//printf("%02X ", bTxdData);
	// Signal consumer for new data
	sem_post(&sem_tx_msp); // Tornem a posar el semafor en verd.
}


/* Funció que activa un Timer per quan hi ha un TimeOut. */
void Activa_Timer_TimeOut_emu() {
	// TODO: (Opcional) Implementar iniciar timer
}

/* Funció per gestionar la recepció de bytes quan es produeix una interrupció. */
/*
void EUSCIA2_IRQHandler { // interrupcion de recepcion en la UART A2
	// UCA2IE &= ~UCRXIE;
	//Interrupciones desactivadas en RX
	// DatoLeido_UART = UCA2RXBUF
    // Byte_Recibido = Si;
    // UCA2IE |= UCRXIE; //Interrupciones reactivadas en RX
} */

/**
 * Read a byte from the thread-safe queue and place it
 * inside the Statuspacket of RxReturn
 */
void rx_uart_byte_emu(struct RxReturn * respuesta) { // Funció que llegeix un byte de la cua i la inclou al Statuspacket.
	QUEUE_RET ret = QUEUE_ERR;
	while (ret != QUEUE_OK) { // Mentre no s'hagi llegit correctament amb un estat OK:
		ret = queue_pop(&(respuesta->StatusPacket[respuesta->idx])); // Intenta llegir el byte.
	}
	respuesta->idx++;
}
