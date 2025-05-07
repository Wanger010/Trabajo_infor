#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caudal.h"

// Menu principal
void mostrarMenu() {
    printf("\n            \033[44mANALISIS DE CAUDALES EN ESPANA\033[0m           \n");
    printf("\n\033[34m      ANIOS 2012-2021. TODOS LOS MESES INCLUIDOS     \033[0m\n");
    printf("\n\033[33m Hay que introducir el nombre tal cual viene dado :D \033[0m\n");
    printf("\n\033[33m              Caudales medidos en hm^3!              \033[0m\n");
    printf("\n-1. Se imprime por pantalla cuencas y respectivos embalses \n");
    printf("0. Archivo cuencas y respectivos embalses (.txt) \n");
    printf("1. Calcular medias anuales por cuenca y mes\n");
    printf("2. Ver evolucion de una cuenca\n");
    printf("3. Comparar dos cuencas en un anio\n");
    printf("4. Comparar dos embalses en un anio\n");
    printf("5. Ver meses del caudal de un embalse de un anio concreto\n");
    printf("6. Ver media mensual de todos los caudales 2012-2021\n");	
    printf("7. Ver relacion caudal-produccion agricola\n");
    printf("8. Salir\n");
    printf("9. Limpiar pantalla\n");
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
	// En este trozo se declara el archivo csv que se quiere abrir
    Embalse *embalses = leerDatos("dataset.csv", totalEmbalses);

    if (embalses == NULL) {
        printf("No se pudieron cargar los datos.\n");
        return 1;
    }

    int opcion;
    do{
    mostrarMenu();
    scanf("%d", &opcion);
    //switch de opciones.
   switch (opcion) {
            case -1:
                mostrarCuencasYEmbalses(embalses, totalEmbalses);
                break;
            case 0:
                guardarCuencasYEmbalses(embalses, totalEmbalses);
                break;
            case 1:
                printf("Media anual caudal de cuenca.\n");
                calcularMediaAnualPorCuenca(embalses, totalEmbalses);
                break;
            case 2:
                printf("Evolucion de cuenca.\n");
                evolucionCuenca(embalses, totalEmbalses);
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
                //datosConcretos(embalses, totalEmbalses);
                break;
            case 6:
                printf("Media mensual 2012-2021.\n");
		mostrarMediaMensualCaudales(embalses, totalEmbalses);
                break;
            case 7:
                printf("Relacion caudal-produccion agricola.\n");
                break;
	    case 8:
		printf("Salir\n");
	        break;
            case 9:
		limpiarPantalla();
	        break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 8); //estructura do while para que el menú se 
                           //repita hasta que el usuario elija salir
    free(embalses);
    return 0;
} 
