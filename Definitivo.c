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
    printf("3. Comparar dos cuencas en un anio\n"); 
    printf("4. Ver relacion capacidad-produccion agricola\n");
    printf("5. Salir\n");
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

// Imprimir en pantalla la lista de cuencas
// y sus embalses (sin repetir)
printf("\nListado de cuencas y sus embalses:\n");

// Bucle principal que recorre todos los embalses leídos
for (int j = 0; j < totalEmbalses; j++) {

    int cuencaYaImpresa = 0; // Contador usado como bandera: 0 = cuenca nueva, !0 = ya fue impresa

    // Verifica si la cuenca actual (embalses[j].cuenca) ya fue impresa antes
    for (int k = 0; k < j; k++) {
        // Comparamos la cuenca actual con las anteriores
        cuencaYaImpresa += strcmp(embalses[j].cuenca, embalses[k].cuenca) == 0;
    }

    // Si la cuenca no ha sido impresa aún (cuencaYaImpresa == 0), procedemos a mostrarla
    if (cuencaYaImpresa == 0) {
        printf("\nCuenca: %s\n", embalses[j].cuenca);
        printf("  Embalses:\n");

        // Segundo bucle: busca e imprime todos los embalses únicos dentro de esta cuenca
        for (int k = 0; k < totalEmbalses; k++) {
            // Solo consideramos los embalses que pertenecen a la cuenca actual
            if (strcmp(embalses[k].cuenca, embalses[j].cuenca) == 0) {

                int embalseYaImpreso = 0; // Contador/bandera para saber si este embalse ya fue mostrado

                // Verifica si este embalse ya fue impreso antes dentro de la misma cuenca
                for (int l = 0; l < k; l++) {
                    embalseYaImpreso += (
                        strcmp(embalses[k].cuenca, embalses[l].cuenca) == 0 && // misma cuenca
                        strcmp(embalses[k].embalseNombre, embalses[l].embalseNombre) == 0 // mismo embalse
                    );
                }

                // Si no se encontró antes (embalseYaImpreso == 0), lo imprimimos
                if (embalseYaImpreso == 0) {
                    printf("  - %s\n", embalses[k].embalseNombre); // Mostramos el nombre del embalse
                }
            }
        }
    }
}

//switch de opciones.
    int opcion;
    do {
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Media anual de la capacidad de la cuenca.\n");
                calcularMediaAnualPorCuenca(embalses, totalEmbalses);
                break;
            case 2:
                printf("Evolucion de la cuenca.\n");
                break;
            case 3:
                printf("Comparacion de cuencas.\n");
                compararCuencas(embalses, totalEmbalses);
                break; 
            case 4:
                printf("Comparacion capacidad vs agricultura.\n");
                break;
            case 5:
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