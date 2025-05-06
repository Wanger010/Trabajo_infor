#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caudal.h"

// Menu principal
void mostrarMenu() {
    printf("\n----------- ANALISIS DE CAUDALES EN ESPANA ----------\n");
    printf("\n----- ANIOS 2012-2021. TODOS LOS MESES INCLUIDOS-----\n");
    printf("\n Hay que introducir el nombre ¡TAL CUAL! viene dado :D \n");
    printf("\n              Caudales medidos en hm^3!              \n");
    printf("\n1. Calcular medias anuales por cuenca \n");
    printf("2. Ver evolucion de una cuenca en un rango de anios\n");
    printf("4. Comparar dos embalses en un anio\n");
    printf("5. Ver meses del caudal de un embalse de un anio concreto\n");
    printf("6. Ver relacion caudal-produccion agricola\n");
    printf("7. Salir\n");
    printf("------------------------------------------\n");
    printf("Ingrese su opcion: ");
}

int main() {
	int totalEmbalses = contarLineas("dataset.csv");
    
    // Verificar si se obtuvo un número válido de embalses
    if (totalEmbalses <= 0) {
        printf("No se pudo obtener la cantidad de embalses.\n");
        return 1;
    }
	
	// Leer los datos de los embalses
	//En este trozo se declara el archivo csv que se quiere abrir
	//despues se usa el puntero *nombreArchivo para acortar
    Embalse *embalses = leerDatos("dataset.csv", totalEmbalses);

    if (embalses == NULL) {
        printf("No se pudieron cargar los datos.\n");
        return 1;
    }

    mostrarCuencasYEmbalses(Embalse *embalses, totalEmbalses);

//switch de opciones.
    int opcion;
    do {
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("media anual caudal de cuenca.\n");
                calcularMediaAnualPorCuenca(embalses, totalEmbalses);
                break;
            case 2:
                printf("Funcion en desarrollo: evolucion de cuenca.\n");
                break;
            case 3:
                printf("Comparacion de cuencas.\n");
                compararCuencas(embalses, totalEmbalses);
                break;
            case 4:
                printf("Comparacion de embalses.\n");
                compararEmbalses(embalses, totalEmbalses);
                break;
            case 5:
                printf("Datos concretos de un embalse.\n");
                datosConcretos(embalses, totalEmbalses);
                break;
            case 6:
                printf("Funcion en desarrollo: caudal vs agricultura.\n");
                break;
            case 7:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 5); //estructura do while para que el menú se 
                           //repita hasta que el usuario elija salir
    free(embalses);
    return 0;
} 