#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>

#include "main.h"
#include "dyn/dyn_app_common.h"
#include "dyn_test/dyn_emu.h"
#include "dyn_test/b_queue.h"

/**
 * main.c
 */
int main(void)
{
	pthread_t tid;
	uint8_t tmp;

	//Init semaphores for TX data handshake
	sem_init(&sem_tx_msp, 0, 0);
	sem_init(&sem_tx_cli, 0, 0);

	//Init queue for RX data
	init_queue(); // Inicialitzem la cua per guardar els diferents "threads"

	//Start thread for dynamixel module emulation
	pthread_create(&tid, NULL, dyn_emu, (void*) &tid); // Creem un nou fil ("thread").

	//Testing some high level function
	printf("Setting LED to 0 \n");
    dyn_led_control(1, 0); // Apaguem el LED del motor amb ID 1.
    printf("Getting LED value \n");
    dyn_led_read(1, &tmp); // Llegim l'estat del LED del motor amb ID 1 i l'assignem el valor a la variable tmp.
    assert(tmp == 0); // Mirem si el valor de la variable "tmp" és 0; és a dir, que el LED està apagat.
    printf("Setting LED to 1 \n");
	dyn_led_control(1, 1); // Encenem el LED del motor amb ID 1.
	printf("Getting LED value \n");
    dyn_led_read(1, &tmp);// Llegim l'estat del LED del motor amb ID 1 i l'assignem el valor a la variable tmp.
    assert(tmp == 1); // Mirem si el valor de la variable "tmp" és 1; és a dir, que el LED està encès.

	//Signal the emulation thread to stop
	pthread_kill(tid, SIGTERM); // Eliminem el thread ja que hem acabat els tests.

	printf("************************\n");
	printf("Test passed successfully\n");
	return 0;
}
