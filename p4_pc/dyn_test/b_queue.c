/*
 * b_queue.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Nerea i Jordi
 *
 *  Simple thread safe circular FIFO queue
 *
 *  Utilitzarem una cua FIFO circular per emmagatzemar els "threads" a executar.
 *  Com es de tipus FIFO, el primer "thread" que entra a la cua (First Input)
 *  és el primer en ser executat (First Output).
 *
 *
 */

#include <pthread.h>
#include <stdint.h>
#include "b_queue.h"

// Avoid mallocs. Since we known we only need one queue, we will declared it static
// instead of the usual init returning a queue object

static struct Queue_t q;


/**
 * Initialize the previous queue
 */
void init_queue() { // Aquesta funció inicialitza la cua.
	//Initialization of the queue pop and push indexes
	q.idx_pop = 0; // Posem l'índex de la cua a zero ja que es ficarà el primera posició de la cua.
	q.idx_push = 0;
	//Initialization of the variable counting the elements pending
	// to be read from the queue
	q.data_pending = 0; //Al principi està buida.
	//Initialization of mutex object for the protect code of the queue
	pthread_mutex_init(&q.mutex, NULL);
}

/**
 * Push a new data byte to the queue
 *
 * @param[in] data Byte to be added to the queue
 * @return Return queue status: full or success
 */
QUEUE_RET queue_push(uint8_t data) { // Funció per introduir un nou "thread" a la cua.
	// Check if the queue is full
	if (q.data_pending >= sizeof(q.data)) { // Si es troba ple no es fica el nou "thread".
		return QUEUE_FULL; // Retornem l'estat de que la cua esta plena.
	}
	// Start the mutex protected code
	pthread_mutex_lock(&q.mutex);
	// Add the data to the latest free position
	q.data[q.idx_push] = data; // Guardem el "thread" al final de la cua.
	// Increment the counter of the read pending data
	q.data_pending++; // Incrementem el valor del comptador dels "threads" que queden per executar.
	// Compute the next write position with modulo the FIFO length
	q.idx_push = (q.idx_push + 1) % sizeof(q.data); // Actualitzem el valor del index pel proper "thread" que es fiqui.
	// Release the mutex lock
	pthread_mutex_unlock(&q.mutex);

	return QUEUE_OK; //Retornem l'estat de que s'ha introduit correctament.
}

/**
 * Pop a byte from the queue
 *
 * @param[out] data Byte removed from the queue
 * @return Return queue status: empty or success
 */
QUEUE_RET queue_pop(uint8_t* data) { // Funció per agafar (i eliminar) un "thread" de la cua.
	// Check if the queue is empty
	if (q.data_pending == 0) { // Si esta buida retornem l'estat de que es troba buida i no s'agafa res.
		return QUEUE_EMPTY;
	}
	// Start the mutex protected code
	pthread_mutex_lock(&q.mutex);
	// Copy the read data to the pointer
	*data = q.data[q.idx_pop]; // Agafem la informació amb el punter passat per paràmetre.
	// Decrement the counter of the read pending data
	q.data_pending--; // Decrementem el valor del comptador dels "threads" que queden per executar.
	// Compute the next read position with modulo the FIFO length
	q.idx_pop = (q.idx_pop + 1) % sizeof(q.data); // Actualitzem el valor del index pel proper "thread" que s'elimini.
	// Release the mutex lock
	pthread_mutex_unlock(&q.mutex);

	return QUEUE_OK; //Retornem l'estat de que l'operació s'ha realitzat correctament.
}
