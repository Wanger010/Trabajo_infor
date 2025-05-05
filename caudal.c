#include <stdio.h>
#include <string.h> 
#include <math.h>
#include "caudal.h"

void mostrarTablaDeDatos(int ano_inicial, int ano_final, int produccion_avena)
{
	printf("+------------------------------+\n");
	printf("| Año | Avena | Media caudales |\n");
	printf("+------------------------------+\n");

	for(int i = anio_inicial; i <= anio_final, i++)
	{
		printf("| %i |  %.2f | %.2f |\n", i, avena[i], embalses[i].datos.cuenca);
	}
	printf("+------------------------------+\n");
}
//funcion para calcular el coeficiente de correlacion entre la capacidad y la produccion agricola
//utilizado solo dentro de este mismo archivo, no se incluye en la libreria
void calculoCoefcorrelacion(float *produccion, char *mediaAnual, float mediaTotal) {
    float media2_capacidad = 0.0, var2_capacidad = 0.0, covar = 0.0; 
    float media_prod = 0.0, media2_prod = 0.0, var2_prod = 0.0;
    float var_prod = 0.0 , var_capacidad = 0.0, coef_correlacion = 0.0; 

    for(int i=0; i<10; i++){ 
        media_prod += produccion[i];  
    }
    media_prod /= 10; //media de la produccion

    for(int i=0; i<10; i++){
        media2_prod += produccion[i] * produccion[i];  
    }
    media2_prod /= 10; //media de la produccion al cuadrado
                
    var2_prod = media2_prod - (media_prod * media_prod); //var de la produccion al cuadrado
    var_prod = sqrt(var2_prod); //var de la produccion

    for(int j=0; j<10; j++){
        media2_capacidad += mediaAnual[j] * mediaAnual[j];  
    }
    media2_capacidad /= 10; //media de la capacidad al cuadrado 

    var2_capacidad = media2_capacidad - (mediaTotal * mediaTotal); //var de la capacidad al cuadrado
    var_capacidad = sqrt(var2_capacidad); //var de la capacidad 

    for (int k=0; k<10; k++){
        covar += produccion[k] * mediaAnual[k];  
    }
    covar /= 10;  
    covar = covar - (media_prod * mediaTotal); //covar de la capacidad y la produccion
    coef_correlacion = covar / (var_prod * var_capacidad); //coef de correlacion 
    if (coef_correlacion > 0 && coef_correlacion < 1){
        printf("La correlacion entre la capacidad y la produccion es positiva, y es:%2.f \n", coef_correlacion);
    } else if (coef_correlacion < 0 && coef_correlacion > -1){
        printf("La correlacion entre la capacidad y la produccion es negativa, y es: %2.f\n", coef_correlacion);
    } else if (coef_correlacion == 1){
        printf("La correlacion entre la capacidad y la produccion es perfecta");
    } else if (coef_correlacion == -1){
        printf("La correlacion entre la capacidad y la produccion es perfecta");
    } else if (coef_correlacion == 0){
        printf("No hay correlacion entre la capacidad y la produccion\n");
    } else {
        printf("No hay correlacion entre la capacidad y la produccion\n");
    }
}


//Función para mostrar cuencas y embalses (principal)
//Actualmente en el main, se podría incluir en libreria,
//no implementado
void mostrarCuencasYEmbalses(Embalse *embalses, int totalEmbalses) {

}

// Función 1: Calcular la media anual de caudal para una cuenca específica
void calcularMediaAnualPorCuenca(Embalse *embalses, int totalEmbalses) {
    int i = 0, j = 0;
    float mediaMensual = 0.0, mediaAnual = 0.0; 
    char nom_cuenca[100]; 
    printf("Introduzca el nombre de la cuenca: ");
    scanf("%s", nom_cuenca); 
    printf("El embalse elegido ha sido: %s\n", nom_cuenca); 
    for (j=0; j<NUM_ANIOS; j++){ 
        for (i = 0; i < totalEmbalses; i++) {
            // Comparamos la cuenca introducida por el usuario con la cuenca del embalse actual
            if (strcmp(embalses[i].cuenca, nom_cuenca) == 0) { 
                mediaMensual += embalses[i].datos.caudales[j];
            }
            mediaMensual /= NUM_MESES; // Calculamos la media mensual para el embalse en enero por ejemplo
            mediaAnual += mediaMensual; 
        }
    } 
    mediaAnual /= NUM_MESES;
    printf("Media anual de %s es de: %.2f Hm³\n", nom_cuenca, mediaAnual); 
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
    float mediaTotal=0;
    char mediaAnual[NUM_ANIOS];
    for(int j=0; j<NUM_ANIOS; j++){
        for(int i=0; i<totalEmbalses; i++){
            mediaAnual[j] += embalses[i].datos.caudales[j]; 
        }
        mediaAnual[j] /= totalEmbalses; //media anual de los caudales
        mediaTotal += mediaAnual[j] ; //media total de los caudales
    } 
    mediaTotal /= NUM_ANIOS; //media total de los caudales
    char opcion_elegida;
    printf("En este apartado se va a estudiar la relacion de la capacidad de los embalses con la produccion agricola del cultivo deseado.\n");
    printf("Para un mejor entendimiento mostraremos tablas grafícas y unos pocos datos \n");
    printf("Los datos de los cultivos vienen dados en toneladas, tenlo en cuenta \n");
	printf("Ahora dime con que tipo de producto agricola deseas estudiar: \n ");
	printf("Con Hortalizas(H), con los Industriales(I) o con los de Grano(G) \n");
	printf("La opcion elegida es: \t");
	scanf("%c", opcion_elegida);
	
	//los datos de los vectores "produccion" son reales
    // han sido obtenidos a traves de tablas de datos presentes en tablas y pdfs  
    // encontrados en la web oficial del ministerio de agricultura, pesca y alimentacion
    // del gobierno de España.
    int anios[] = {2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021};
    switch(opcion_elegida) { 
        
        case 'h':
		case 'H':
			
			
			
			
			
			
			break;
		
		case 'i':
		case 'I':
			
            char indus[2];
            printf("¿Que tipo de cultivo industrial quieres comparar: Girasol(G) o Soja(S)?\n");
            printf("El cultivo industrial elegido es: \n");
            scanf(" %s", indus); 

            switch(indus[2]){ 
                case 'g':
                case 'G':
                float produccion_gira[] = {642.0, 1038.1, 953.0, 769.2, 772.2, 814.7, 950.3, 773.8, 883.1, 771.0};
                calculoCoefcorrelacion(produccion_gira, mediaAnual, mediaTotal); 
                break; 

                case 's':
                case 'S':
                float produccion_soja[] = {1333, 1390, 2650, 4106, 2869, 4599, 4249, 5053, 4515, 4769};  
                calculoCoefcorrelacion(produccion_soja, mediaAnual, mediaTotal);
                break;
            }
			break;
		
		case 'g':
		case 'G':
			
			char grano[0];
			printf("¿Que tipo quieres comparar: avena(A) o cebada(C)?\n");
			printf("El grano elegido es:\t");
			scanf("%c", grano);

			switch(grano[0])
			{
				case 'A':
					char opc[0]; 	
					printf("Ahora tienes que decidirte si quieres saber la avena producida y el caudal de un embalse en un mes entre 2012 y 2021(M) o en un anio(Y)\n");
					printf("Deseas saber: \t");
					scanf("%s", &opc);
	
					switch(opc[0])
					{
						case 'M':
		
						int mes;
						printf("Bien, ahora necesito que me digas el numero del mes, ejemplo --> Enero(1), Febrero(2), etc:  \t");
						scanf("%i", &mes);

						case 'Y':

						int anio_inicial, anio_final;
						printf("Bien, necesito que me digas el anio inicial:\t");
						scanf("%d", &anio_inicial);
						printf("Ahora dime el anio final:\t");
						scanf("%d", &anio_final);

						FILE *archivo= fopen("dataset.csv","r");

						if(archivo == NULL)
						{
							printf("Error al abrir el archivo");
							return 1;
						{
					}
						
				
				case 'C':
					
					char opci[0]; 	
					printf("Ahora tienes que decidirte si quieres saber la cebada producida y el caudal de un embalse en un mes entre 2012 y 2021(M) o en un anio(Y)\n");
					printf("Deseas saber: \t");
					scanf("%s", &opc);
	
					switch(opci[0])
					{
						case 'M':
	
						int meses[12]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, mes;
						printf("Bien, ahora necesito que me digas el numero del mes, ejemplo --> Enero(1), Febrero(2), etc:  \t");
						scanf("%i", &mes);

						


						case 'Y':

						int anio
					}
			}	
			break;
		 
	}


} 
