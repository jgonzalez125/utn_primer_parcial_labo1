/*
 * menu.c
 *
 *  Created on: Oct 14, 2020
 *      Author: juang
 */

#include <stdio.h>
#include <stdlib.h>
#include "utn.h"
#include "electrodomesticos.h"
#include "marca.h"
#include "reparacion.h"
#include "servicio.h"

#define CANTIDAD_ELECTRODOMESTICOS 100
#define CANTIDAD_MARCAS 5
#define CANTIDAD_SERVICIOS 4
#define CANTIDAD_REPARACIONES 100

#define ID_SERVICIOS_MINIMO 20000
#define ID_SERVICIOS_MAXIMO 20003

#define ID_MARCAS_MINIMO 1000
#define ID_MARCAS_MAXIMO 1004



void menu() {
	setbuf(stdout, NULL);

	//Arrays de entidades
	Electrodomestico arrayElectrodomesticos[CANTIDAD_ELECTRODOMESTICOS];
	Reparacion arrayReparaciones[CANTIDAD_REPARACIONES];
	Servicio arrayServicios[CANTIDAD_SERVICIOS];
	Marca arrayMarcas[CANTIDAD_MARCAS];

	char opcion;

	//Flags para cargar reparacion
	int electrodomesticosCargados = 0;
	int cargoReparacion = 0;

	//Ids autoincrementables
	int idElectrodomesticos = 0;
	int idServicios = 20000;
	int idMarcas = 1000;
	int idReparacion = 0;

	//Variables auxiliares
	int auxiliarSerie;
	int auxiliarIdMarca;
	int auxiliarId;
	int auxiliarIndice;



	if (!elec_initArray(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS)
				&& !rep_initArray(arrayReparaciones, CANTIDAD_REPARACIONES)) {
			printf("\nArray init ok");

			// Marcas hardcodeadas
			mar_altaForzada(arrayMarcas, CANTIDAD_MARCAS, &idMarcas, 0, "Wirpool", "Lavarropas");
			mar_altaForzada(arrayMarcas, CANTIDAD_MARCAS, &idMarcas, 1, "Sony", "Ultima tecnologia");
			mar_altaForzada(arrayMarcas, CANTIDAD_MARCAS, &idMarcas, 2, "Liliana", "Lo mejor para vos");
			mar_altaForzada(arrayMarcas, CANTIDAD_MARCAS, &idMarcas, 3, "Gafa", "Compra ideal");
			mar_altaForzada(arrayMarcas, CANTIDAD_MARCAS, &idMarcas, 4, "Philips", "Siempre junto a vos");

			// Servicios hardcodeados
			serv_altaForzada(arrayServicios, CANTIDAD_SERVICIOS, &idServicios, 0, "Garantia", 250);
			serv_altaForzada(arrayServicios, CANTIDAD_SERVICIOS, &idServicios, 1, "Mantenimiento", 500);
			serv_altaForzada(arrayServicios, CANTIDAD_SERVICIOS, &idServicios, 2, "Repuestos", 400);
			serv_altaForzada(arrayServicios, CANTIDAD_SERVICIOS, &idServicios, 3, "Refaccion", 600);

		do {
			fpurge(stdin);
			if (!utn_getCaracter(&opcion,"\nA)ALTA ELECTRODOMESTICO"
										"\nB)MODIFICAR ELECTRODOMESTICO: Se ingresará el id, "
										"permitiendo en un submenú modificar: \n  1-Serie\n  2-Modelo"
										"\nC)BAJA ELECTRODOMESTICO"
										"\nD)LISTAR ELECTRODOMESTICOS"
										"\nE)LISTAR MARCAS"
										"\nF)LISTAR SERVICIOS"
										"\nG)ALTA REPARACION"
										"\nH)LISTAR REPARACIONES"
										"\nI)SALIR", "\nError", 'A', 'I', 3)) {
				switch (opcion) {
				case 'A':
					auxiliarIndice = elec_getEmptyIndex(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS);
					mar_imprimirArray(arrayMarcas, CANTIDAD_MARCAS);

					if(auxiliarIndice >= 0 &&
					  !utn_getNumero(&auxiliarIdMarca, "\nIngrese id de marca del electrodomestico:", "\nError, reingrese", ID_MARCAS_MINIMO, ID_MARCAS_MAXIMO, 3) &&
					   mar_buscarId(arrayMarcas, CANTIDAD_MARCAS,auxiliarIdMarca) >= 0) {

						if(!elec_cargarArray(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS, auxiliarIndice, &idElectrodomesticos, auxiliarIdMarca)){
							printf("\nElectrodomestico cargado exitosamente");
							elec_imprimir(&arrayElectrodomesticos[auxiliarIndice]);
							electrodomesticosCargados++;
						}else{
							printf("\nNo se pudo realizar el alta");
						}

					}else{
						printf("\nEl id de marca no es valido");
					}
					break;
				case 'B':
					if(!electrodomesticosCargados){
						printf("\nDebe cargar un electrodomestico para poder modificar");
					}else{
						elec_imprimirArray(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS);

						if(!utn_getNumero(&auxiliarId, "\nIngrese id a modificar: ", "\nError, reingrese", 0, CANTIDAD_ELECTRODOMESTICOS, 3)){
							auxiliarIndice = elec_buscarId(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS, auxiliarId);
							if(auxiliarIndice >= 0 &&
							   !elec_modificarArray(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS, arrayReparaciones, CANTIDAD_REPARACIONES, auxiliarIndice)){
								printf("\n\nModificacion realizada exitosamente");
							}else{
								printf("\nEl id %d no es valido", auxiliarId);
							}
						}
					}
					break;
				case 'C':
					if(!electrodomesticosCargados){
							printf("\nDebe cargar un electrodomestico para poder dar de baja");
						}else{
							elec_imprimirArray(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS);
							if(!utn_getNumero(&auxiliarId, "\nIngrese id a dar de baja: ", "\nError, reingrese", 0, CANTIDAD_ELECTRODOMESTICOS, 3)){
								auxiliarIndice = elec_buscarId(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS, auxiliarId);
								if(auxiliarIndice >= 0 &&
									!elec_bajaArray(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS, arrayReparaciones, CANTIDAD_REPARACIONES, auxiliarIndice)){
									printf("\n\nBaja realizada exitosamente");
									electrodomesticosCargados--;
								}else{
									printf("\nEl id %d no es valido", auxiliarId);
								}
							}
						}
					break;
				case 'D':
					if(!electrodomesticosCargados){
						printf("\nDebe cargar un electrodomestico para poder imprimir");
					}else{
						printf("\nElectrodomesticos ordenados por modelo y serie: (menor a mayor)");
						elec_ordenarPorModeloSerie(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS);
						elec_imprimirArray(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS);
					}
					break;
				case 'E':
					printf("\n=========== MARCAS ===========");
					mar_imprimirArray(arrayMarcas, CANTIDAD_MARCAS);
					break;
				case 'F':
					printf("\n=================== SERVICIOS ====================");
					serv_imprimirArray(arrayServicios, CANTIDAD_SERVICIOS);
					break;
				case 'G':
					if(!electrodomesticosCargados){
						printf("\nNo hay electrodomesticos cargados");
					}else{
						auxiliarIndice = rep_getEmptyIndex(arrayReparaciones, CANTIDAD_REPARACIONES);
						if(auxiliarIndice >= 0){
							elec_imprimirArray(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS);
							if(!utn_getNumero(&auxiliarSerie, "\nIngrese numero de serie de electrodomestico a ser reparado:", "\nError, reingrese",
								1, 10000, 3) &&
									elec_buscarSerie(arrayElectrodomesticos, CANTIDAD_ELECTRODOMESTICOS, auxiliarSerie) >= 0){

								printf("\n=================== SERVICIOS ====================");
								serv_imprimirArray(arrayServicios, CANTIDAD_SERVICIOS);
								if(!utn_getNumero(&auxiliarId, "\nIngrese id de servicio a ser realizado:", "\nError, reingrese", ID_SERVICIOS_MINIMO, ID_SERVICIOS_MAXIMO, 3)){
									if(!rep_cargarArray(arrayReparaciones, CANTIDAD_REPARACIONES, auxiliarIndice, auxiliarSerie, auxiliarId, &idReparacion)){
										printf("\nAlta realizada exitosamente");
										cargoReparacion = 1;
									}else{
										//fallo rep_cargarArray()
										printf("\nNo se puedo realizar el alta");
									}
								}else{
									// fallo el input de id de servicio
									printf("\nId de servicio no es valida");
								}
							}else{
								// elec_buscarSerie() == -1
								printf("\nNumero de serie de electrodomestico no es valido");
							}
						}else{
							// rep_getEmptyIndex() == -1
							printf("\nNo hay mas espacios vacios");
						}
					}
					break;
				case 'H':
					if(!cargoReparacion){
						printf("\nDebe cargar una reparacion previo a imprimir");
					}else{
						rep_imprimirArray(arrayReparaciones, CANTIDAD_REPARACIONES);
					}
					break;
				default:
					printf("\nSaliendo...");
					break;
				}
			} else {
				break;
			}
		} while (opcion != 'I');

	}
}
