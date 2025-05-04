#include <stdio.h>
#include <string.h>
#include "caudal.h"

//Función para mostrar cuencas y embalses (principal)
//Actualmente en el main, se podría incluir en libreria,
//no implementado
void mostrarCuencasYEmbalses(Embalse *embalses, int totalEmbalses) {

}

// Función 1: Calcular la media anual de caudal para una cuenca específica
void calcularMediaAnualPorCuenca(Embalse *embalses, int totalEmbalses) {
 
}

// Función 2: Mostrar la evolución del caudal de la cuenca entre un rango de años
void evolucionCuenca(Embalse *embalses, int totalEmbalses) {

    }

// Función 3: Comparar el caudal de dos cuencas en un año específico
void compararCuencas(Embalse *embalses, int totalEmbalses) {
    char cuencaA[100], cuencaB[100];
    int anio;
    printf("\nIngrese el nombre de la primera cuenca: ");
    scanf(" %[^\n]", cuencaA);
    printf("Ingrese el nombre de la segunda cuenca: ");
    scanf(" %[^\n]", cuencaB);

    printf("Ingrese el anio a comparar (2012-2021): ");
    scanf("%d", &anio);  // Lee el año

    // Verifica si el año está dentro del rango permitido
    if (anio < 2012 || anio > 2021) {
        printf("Anio fuera de rango.\n");
        return;
    }

    // Inicializa las variables para acumular los caudales
    float sumaA = 0, sumaB = 0;

    // Recorre los embalses y acumula los caudales de ambas cuencas en el año solicitado
    for (int i = 0; i < totalEmbalses; i++) {
        if (embalses[i].datos.anios[anio - 2012] == anio) {  // Accede directamente al año
            if (strcmp(embalses[i].cuenca, cuencaA) == 0) {
                sumaA += embalses[i].datos.caudales[anio - 2012];  // Suma caudales de la cuenca A
            } else if (strcmp(embalses[i].cuenca, cuencaB) == 0) {
                sumaB += embalses[i].datos.caudales[anio - 2012];  // Suma caudales de la cuenca B
            }
        }
    }

    // Muestra la comparación de caudales entre las dos cuencas
    printf("\n--> Comparacion de caudal en el anio %d:\n", anio);
    //-20, el - alinea a la izq. el texto y 20 son los espacios reservados
    printf("%-20s: %.2f\n", cuencaA, sumaA);
    printf("%-20s: %.2f\n", cuencaB, sumaB);

    // Compara los caudales y muestra cuál cuenca tuvo el mayor caudal
    if (sumaA > sumaB) {
        printf("La cuenca '%s' tuvo mayor volumen de agua embalsada.\n", cuencaA);
    } else if (sumaB > sumaA) {
        printf("La cuenca '%s' tuvo mayor volumen de agua embalsada.\n", cuencaB);
    } else {
        printf("Ambas cuencas tuvieron el mismo volumen de agua embalsada.\n");
        printf("Posible error en la escritura del caudal!!!\n");
    }
}

// Función 4: Comparar el caudal con la producción agrícola (pendiente de implementación)
void compararCaudalAgricola(Embalse* embalses, int totalEmbalses) {

}
