#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>

#include "main.h"
#include "dyn/dyn_app_common.h"
#include "dyn_test/dyn_emu.h"
#include "dyn_test/b_queue.h"
#include "dyn/dyn_app_sensor.h"
#include "dyn/dyn_app_motor.h"
#include "joystick_emu/joystick.h"

uint8_t estado = Ninguno, estado_anterior = Ninguno, finalizar = 0;
/**
 * main.c
 */
int main(void)
{
	pthread_t tid, jid;
	uint16_t sec_tmp;
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

    //TEST VELOCIDAD DE RUEDAS

    //SET
    printf("\n---------- SET VELOCIDAD 1000\n");
    write_speed(MOTOR_LEFT, 0x3e8);

    //GET
    printf("\n---------- LEYENDO OBSTACULO\n");
    read_speed(MOTOR_LEFT, &sec_tmp);


    assert(sec_tmp == 0x3e8);

    printf("\n---------- TEST SUCCESFULLY\n");

    /*******************************/

  //TEST SET OBSTACULOS
  printf("\n---------- TEST OBSTACULOS \n");
  printf("********\n");

  //SET OBSTACULO
  printf("\n---------- SET OBSTACULO detected compare value to 111\n");
  write_obstacle_detected_compare(SENSOR, 0x6F);

  //READ OBSTACULO
  printf("\n---------- LEYENDO OBSTACULO\n");
  read_obstacle_detected_compare(SENSOR, &tmp);


  assert(tmp == 0x6F);
  printf("\n---------- TEST SUCCESFULLY\n");
  /*******************************/


  //TEST DISPOSITIVO A WHEEL MODE


  //SET
  printf("\n---------- SET RUEDA ID: 0\n");
  movement_continue(MOTOR_LEFT);


  printf("\n---------- TEST SUCCESFULLY, WHEEL MODE\n");
  /*******************************/

  // TEST RUEDAS ROTACIÓN IZQUIERDA

  //SET
  printf("\n---------- SET ROTACIÓN RUEDAS DERECHA\n");
  left_movement(MOTOR_LEFT, MOTOR_RIGHT, 120);

  //READ
  printf("\n---------- READ VELOCIDAD RUEDAS\n");
  read_speed(MOTOR_RIGHT, &sec_tmp);

  assert(sec_tmp == 1144);

  read_speed(MOTOR_LEFT, &sec_tmp);

  assert(sec_tmp == 1024);
  printf("\n---------- TEST SUCCESFULLY\n");

  /*******************************/
  //TEST RUEDAS ROTACIÓN DERECHA

  //SET

  printf("\n---------- SET ROTACIÓN RUEDAS IZQUIERDA\n");
  right_movement(MOTOR_LEFT, MOTOR_RIGHT, 120);

  //READ
  printf("\n---------- READ VELOCIDAD RUEDAS\n");
  read_speed(MOTOR_RIGHT, &sec_tmp);

  assert(sec_tmp == 0);

  read_speed(MOTOR_LEFT, &sec_tmp);

  assert(sec_tmp == 120);

  printf("\n---------- TEST SUCCESFULLY\n");

 /*******************************/

  // TEST RETROCESO DE RUEDAS

  //SET
  printf("\n---------- SET VELOCIDAD DE RETROCESO A 120\n");
  backward_movement(MOTOR_LEFT, MOTOR_RIGHT, 120);

  //READ
  printf("\n---------- READ VELOCIDAD DE LAS RUEDAS\n");
  read_speed(MOTOR_RIGHT, &sec_tmp);

  assert(sec_tmp == 120);

  read_speed(MOTOR_LEFT, &sec_tmp);

  assert(sec_tmp == 1144);
  printf("\n---------- TEST SUCCESFULLY\n");
  /*******************************/


  /*******************************/
  //TEST DE MOVIMIENTO HACIA ADELANTE DE LAS RUEDAS

      //SET
      printf("\n---------- SET AVANZAR RUEDAS CON VELOCIDAD 120\n");
      forward_movement(MOTOR_LEFT, MOTOR_RIGHT, 120);

      //READ
      printf("\n---------- READ VELOCIDAD DE LAS RUEDAS\n");
      read_speed(MOTOR_LEFT, &sec_tmp);

      assert(sec_tmp == 120);

      read_speed(MOTOR_RIGHT, &sec_tmp);

      assert(sec_tmp == 1144);

      printf("\n---------- TEST SUCCESFULLY\n");
  /*******************************/


      //LUMINOSITY SENSOR TESTS

      printf("\n---------- LUMINOSITY TESTS\n");
      printf("********\n");

      //SENSOR IZQUIERDO
      printf("\n---------- LUMINOSITY LEFT\n");
      read_luminosity(SENSOR, &tmp, L_LEFT);
      assert(tmp == 0x7E);
      printf("\n---------- TEST SUCCESFULLY\n");

      //SENSOR CENTRAL
      printf("\n---------- CENTER LUMINOSITY\n");
      read_luminosity(SENSOR, &tmp, L_CENTER);
      assert(tmp == 0x55);
      printf("\n---------- TEST SUCCESFULLY\n");

      //SENSOR DERECHO
      printf("\n---------- RIGHT LUMINOSITY\n");
      read_luminosity(SENSOR, &tmp, L_RIGHT);
      assert(tmp == 0x29);
      printf("\n---------- TEST SUCCESFULLY\n");


      printf("\n---------- LUMINOSITY TESTS FINALIZADOS\n");

      /*******************************/

      //IR TESTS

      printf("\n---------- IR TESTS\n");
      printf("********\n");

      //SENSOR IZQUIERDO
      printf("\n---------- LEFT IR\n");
      read_infrared(SENSOR, &tmp, I_LEFT);
      assert(tmp == 0xC8);
      printf("\n---------- TEST SUCCESFULLY\n");

      //SENSOR CENTRAL
      printf("\n---------- CENTER IR\n");
      read_infrared(SENSOR, &tmp, I_CENTER);
      assert(tmp == 0x1C);
      printf("\n---------- TEST SUCCESFULLY\n");

      //SENSOR DERECHO
      printf("\n---------- RIGHT IR\n");
      read_infrared(SENSOR, &tmp, I_RIGHT);
      assert(tmp == 0x2A);
      printf("\n---------- TEST SUCCESFULLY\n");

      printf("\n---------- IR TESTS FINALIZADOS\n");

      /*******************************/

      //SOUND SENSOR TEST

        printf("\n---------- SOUND TEST \n");
        printf("********\n");

        read_sound(SENSOR, &tmp);
        assert(tmp == 0x24);

        printf("\n---------- TEST SUCCESFULLY\n");
       /*******************************/

	printf("********\n");
	printf("Test passed successfully\n");
	printf("Pulsar 'q' para terminar, qualquier tecla para seguir\n");
	putchar('>');
	putchar(' ');
	fflush(stdout);//	return 0;

	while(estado != Quit)
	{
		Get_estado(&estado, &estado_anterior);
		if(estado != estado_anterior){
			Set_estado_anterior(estado);
			printf("estado = %d\n", estado);
			fflush(stdout);
			switch(estado){
			case Sw1:
				printf("HAS APRETADO EL BOTON Sw1 ('a')\n");
				break;
			case Sw2:
				printf("HAS APRETADO EL BOTON Sw2 ('s')\n");
				break;
			case Left:
				printf("HAS APRETADO EL BOTON IZQUIERDO ('j')\n");
				break;
			case Right:
				printf("HAS APRETADO EL BOTON DERECHO ('l')\n");
				break;
			case Center:
				printf("HAS APRETADO EL BOTON CENTRAL ('k')\n");
				break;
			case Up:
				printf("HAS MOVIDO EL JOYSTICK HACIA ARRIBA('i')\n");
				break;
			case Down:
				printf("HAS MOVIDO EL JOYSTICK HACIA ABAJO('m')\n");
				break;
			case Quit:
				printf("Adios!\n");
				break;
			}
			fflush(stdout);
			putchar('>');
			putchar(' ');
		}
	}
	printf("Programa terminado\n");
	//Signal the emulation thread to stop
	pthread_kill(tid, SIGTERM);
	pthread_kill(jid, SIGTERM);
	exit(0);
}
