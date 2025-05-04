#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caudal.h"

// Cuenta cuantas lineas tiene el archivo
// sin contar el encabezado
int contarLineas(const char *dataset) {
    FILE *archivo = fopen("dataset.csv", "r"); // Abre el archivo en modo lectura
    //Comprobación de apertura, NULL=puntero vacío
	if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return -1;
    }

    int totalEmbalses = 0;
    char linea[1030];

    // Saltar la primera linea (cabecera)
    fgets(linea, sizeof(linea), archivo);

    // Contar el resto de lineas
    while (fgets(linea, sizeof(linea), archivo)) {
        totalEmbalses++;
    }
    printf("Numero de lineas del csv: %d",totalEmbalses);
    fclose(archivo);
    return totalEmbalses;
}

// Función que lee el archivo CSV y carga los datos de embalses en memoria dinámica
Embalse *leerDatos(const char *dataset, int totalEmbalses) {

    // Primero, verificamos si el número total de embalses es válido
    // Si totalEmbalses es menor o igual a cero, no hay nada que hacer, así que retornamos NULL
    if (totalEmbalses <= 0) {
        printf("No hay datos que leer (totalEmbalses <= 0).\n");
        return NULL;  // Termina la función retornando NULL porque no hay datos
    }

    // Reservamos memoria suficiente para almacenar los embalses en un array dinámico
    Embalse *embalses = (Embalse*) malloc(sizeof(Embalse) * totalEmbalses);
    
    // Comprobamos si la reserva de memoria fue exitosa. Si no, mostramos un mensaje de error
    if (embalses == NULL) {
        printf("No se pudo asignar memoria.\n");
        return NULL;  // Terminamos la función y devolvemos NULL si no se pudo reservar memoria
    }

    // Ahora, intentamos abrir el archivo CSV para leer los datos
    FILE *archivo = fopen("dataset.csv", "r");
    
    // Verificamos si el archivo se abrió correctamente. Si no, liberamos la memoria reservada y retornamos NULL
    if (archivo == NULL) {
        printf("\nError al abrir el archivo.\n");
        free(embalses);  // Liberamos la memoria previamente reservada
        return NULL;  // Terminamos la función porque no pudimos abrir el archivo
    }

    // Declaramos un buffer para leer cada línea del archivo
    char linea[1030];

    // Leemos la primera línea del archivo (la cabecera), pero no la procesamos, solo la saltamos
    // fgets acaba en \n o cuando el buffer se llena -> muy interesante para leer lineas
    fgets(linea, sizeof(linea), archivo);

    // Inicializamos un índice para empezar a llenar el array de embalses
    int i = 0;

    // Ahora, leemos cada línea del archivo hasta llegar al límite de embalses 
    // o hasta que se acaben las líneas, fgets ya es una función lógica en si
    // misma pues devuelve un NULL=False o un valor=True
    while (fgets(linea, sizeof(linea), archivo) && i < totalEmbalses) {
        
        // Usamos strtok para dividir la línea en campos separados por comas
        // El primer campo es el nombre de la cuenca
        char *temp = strtok(linea, ",");

        // Campo 1: Guardamos el nombre de la cuenca en la estructura Embalse
        strcpy(embalses[i].cuenca, temp);

        // Campo 2: El siguiente campo es el nombre del embalse
        // Al poner NULL pasa a la sigueinte parte tras la coma
        // Esto se debe a que esta función cambia la ',' por '\0'=NULL.
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            strcpy(embalses[i].embalseNombre, temp);  // Guardamos el nombre del embalse
        } else {
            embalses[i].embalseNombre[0] = '\0';  // Si no hay nombre, asignamos una cadena vacía
        }

        // Campo 3: El siguiente campo es el mes (leemos si está presente)
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            strcpy(embalses[i].mes, temp);  // Guardamos el mes
        } else {
            embalses[i].mes[0] = '\0';  // Si no hay mes, asignamos una cadena vacía
        }

        // A continuación, leemos los caudales para los años de 2012 a 2021
        // Si algún campo no contiene un valor válido, asignamos el valor 0 por defecto
        for (int j = 0; j < NUM_ANIOS; j++) {
            temp = strtok(NULL, ",");  // Obtenemos el siguiente campo que debería ser el caudal

            if (temp != NULL) {
                embalses[i].datos.caudales[j] = atof(temp);  // Convertimos el caudal (cadena) a un float
            } else {
                embalses[i].datos.caudales[j] = 0;  // Si no hay valor, asignamos 0
            }

            // Asignamos el año correspondiente a este caudal
            embalses[i].datos.anios[j] = 2012 + j;
        }

        // Aumentamos el índice para procesar el siguiente embalse
        i++;
    }

    // Cerramos el archivo después de haber leído todos los datos
    fclose(archivo);

    // Finalmente, volvemos el puntero a la memoria reservada que contiene todos los datos de los embalses
    return embalses;
}


// Menu principal
void mostrarMenu() {
    printf("\n----------- ANALISIS DE EMBALSES EN ESPANA ----------\n");
    printf("\n----- ANIOS 2012-2021. TODOS LOS MESES INCLUIDOS-----\n");
    printf("\n Hay que introducir el nombre tal cual viene dado :D \n");
    printf("\n1. Calcular medias anuales por cuenca y mes\n");
    printf("2. Ver evolucion de una cuenca\n");
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
    float media;  
    do {
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Media anual capacidad de cuenca.\n"); 
                media = calcularMediaAnualPorCuenca(embalses, totalEmbalses); 
                break;
            case 2:
                printf("Evolucion de cuenca.\n");
                break;
            case 3:
                printf("Comparacion de cuencas.\n");
                break;
            case 4:
                printf("Relación capacidad-agricultura.\n");
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




// Función 1: Calcular la media anual de capacidad para una cuenca específica
void calcularMediaAnualPorCuenca(Embalse *embalses, int totalEmbalses) {
    int i, j, k;
    float mediaMensual = 0.0, mediaAnual = 0.0; 
    char nom_cuenca[100];
    int totalEmbalses = contarLineas("dataset.csv");
    printf("Introduzca el nombre de la cuenca: ");
    scanf("%s", nom_cuenca);  
    for (i = 0; i < totalEmbalses; i++) {
        // Comparamos la cuenca introducida por el usuario con la cuenca del embalse actual
        if (strcmp(embalses[i].cuenca, nom_cuenca) == 0) { 
            printf("El embalse elegido ha sido: %s\n", embalses[i].embalseNombre);  
            for(j = 0; j < NUM_MESES; j++) {
                // Calculamos la media anual de capacidad para el embalse actual 
                mediaMensual += embalses[i].datos.caudales[j]; 
            }  
        }
        mediaMensual /= NUM_ANIOS;
        mediaAnual += mediaMensual; 
    }
    mediaAnual /= NUM_MESES;
    printf("Media anual de %s es de: %.2f Hm³\n", embalses[i].embalseNombre, mediaAnual);
}

// Función 2: Mostrar la evolución del caudal de la cuenca entre un rango de años
void evolucionCuenca(Embalse* embalses, int totalEmbalses) {

    }

// Función 3: Comparar el caudal de dos cuencas en un año específico
void compararCuencas(Embalse* embalses, int totalEmbalses) {

}

// Función 4: Comparar el caudal con la producción agrícola (pendiente de implementación)
void compararCaudalAgricola(Embalse* embalses, int totalEmbalses) {

}
