#include <stdio.h>
#include <string.h>  
#include <stdlib.h>
#include <math.h>
#include "caudal.h"

//función para limpiar pantalla
//la pongo por si se usa

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Unix/Linux/macOS
    #endif
    }

//Función para cambiar "_" por " "
void reemplazarGuionBajoPorEspacio(char *cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (cadena[i] == '_') {
            cadena[i] = ' ';
        }
    }
}

// Cuenta cuantas lineas tiene el archivo
// sin contar el encabezado
int contarLineas(const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
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
Embalse *leerDatos(const char *nombreArchivo, int totalEmbalses) {

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
    FILE *archivo = fopen(nombreArchivo, "r");
    
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

        //cambio de "_" a un " "
        reemplazarGuionBajoPorEspacio(embalses[i].cuenca);

        // Campo 2: El siguiente campo es el nombre del embalse
        // Al poner NULL pasa a la sigueinte parte tras la coma
        // Esto se debe a que esta función cambia la ',' por '\0'=NULL.
        temp = strtok(NULL, ",");
        if (temp != NULL) {
            strcpy(embalses[i].embalseNombre, temp);  // Guardamos el nombre del embalse
            //cambio de "_" a un " "
            reemplazarGuionBajoPorEspacio(embalses[i].embalseNombre);

        } else {
            embalses[i].embalseNombre[0] = '\0';  // Si no hay nombre, asignamos una cadena vacía
        }

        // Campo 3: El siguiente campo es el mes (leemos si está presente)
		temp = strtok(NULL, ",");
		int mes = (temp != NULL) ? atoi(temp) : 0;
		for (int j = 0; j < NUM_MESES; j++) {
    		embalses[i].datos.mes[j] = 0;  // Inicializamos todo a 0
		}
		embalses[i].datos.mes[mes - 1] = mes;  // Guardamos solo el mes correspondiente
        
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

//Función para mostrar cuencas y embalses (principal)
void mostrarCuencasYEmbalses(Embalse *embalses, int totalEmbalses) {
    //borra datos anteriores del .exe
    limpiarPantalla();
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
    }

// Función para mostrar cuencas y embalses en un archivo TXT
void guardarCuencasYEmbalses(Embalse *embalses, int totalEmbalses) {
    //borra datos anteriores del .exe
    limpiarPantalla();
    // Se intenta abrir (o crear) el archivo para escritura
    FILE *archivo = fopen("Caudales_Disponibles.txt", "w");
    
    // Verificamos si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("Error al crear el archivo Caudales_Disponibles.txt\n");
        return;
    }

    // Escribimos el encabezado en el archivo
    fprintf(archivo, "Listado de cuencas y sus embalses:\n");

    // Recorremos todos los embalses para identificar cuencas únicas
    for (int j = 0; j < totalEmbalses; j++) {
        int cuencaYaImpresa = 0; // Bandera: 0 = cuenca nueva, diferente a las anteriores

        // Verificamos si esta cuenca ya fue impresa antes
        for (int k = 0; k < j; k++) {
            cuencaYaImpresa += strcmp(embalses[j].cuenca, embalses[k].cuenca) == 0;
        }

        // Si la cuenca es nueva, la imprimimos junto con sus embalses
        if (cuencaYaImpresa == 0) {
            // Escribimos el nombre de la cuenca
            fprintf(archivo, "\nCuenca: %s\n", embalses[j].cuenca);
            fprintf(archivo, "  Embalses:\n");

            // Recorremos todos los embalses nuevamente para listar los de esta cuenca
            for (int k = 0; k < totalEmbalses; k++) {
                // Solo consideramos embalses de la cuenca actual
                if (strcmp(embalses[k].cuenca, embalses[j].cuenca) == 0) {
                    int embalseYaImpreso = 0; // Bandera para evitar imprimir duplicados

                    // Verificamos si el embalse ya fue listado antes en esta cuenca
                    for (int l = 0; l < k; l++) {
                        embalseYaImpreso += (
                            strcmp(embalses[k].cuenca, embalses[l].cuenca) == 0 && // misma cuenca
                            strcmp(embalses[k].embalseNombre, embalses[l].embalseNombre) == 0 // mismo embalse
                        );
                    }

                    // Si no ha sido impreso antes, lo escribimos en el archivo
                    if (embalseYaImpreso == 0) {
                        fprintf(archivo, "  - %s\n", embalses[k].embalseNombre);
                    }
                }
            }
        }
    }

    // Cerramos el archivo para guardar los cambios
    fclose(archivo);

    // Mensaje en consola para informar que se generó el archivo
    printf("Datos escritos en Caudales_Disponibles.txt correctamente.\n");
}

void mostrarMediaMensualCaudales(Embalse *embalses, int totalEmbalses)
{
	int meses[13]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, mes_usuario;
	printf("Bien, ahora necesito que me digas el numero del mes, ejemplo --> Enero(1), Febrero(2) y asi seguidamente:  \t");
	scanf("%i", &mes_usuario);
	
	
	FILE *archivo = fopen("dataset.csv", "r");
	if(archivo == NULL)
	{
		printf("Error al abrir el archivo\n");
	}
	
	
	int columna = 0, columna_objetivo;
	int mes2 = 0;
	char linea[1030];
	fgets(linea, sizeof(linea), archivo);
	char *tok = strtok(linea, ",");
	while(tok != NULL)
	{
		if(strcmp(tok, "mes") == 0)
		{
			columna_objetivo = columna;
			break;
		}
	tok = strtok(NULL, ",");
	columna++;	
	}
	
	
	float suma_caudales = 0.0, valorcaudal = 0.0;
	int columnas;
	while(fgets(linea, sizeof(linea), archivo))
	{
		columnas = 1;
		tok = strtok(linea, ",");
		while(tok != NULL)
		{
			if(mes_usuario == atoi(tok))
			{
				while(columnas > columna_objetivo && columnas < 14)
				{
					tok = strtok(NULL, ",");
					float valorcaudal = atof(tok);
					suma_caudales += valorcaudal;
					columnas++;
					if(columnas == 13)
					{
						columnas = 0;
						break;
					}
				}
			}
			tok = strtok(NULL, ",");
			columnas++;
		}
	}
	fclose(archivo);
	
	float media_caudales = suma_caudales / (totalEmbalses / 12);
	mostrarTablaDeDatosMes(mes_usuario, media_caudales);	
}


void mostrarTablaDeDatosMes(int anio, float mediacaudales)
{
	printf("+----------------------+\n");
	printf("| Mes | Media caudales |\n");
	printf("+----------------------+\n");
	printf("|  %i  |    %.2f     |\n", anio, mediacaudales);
	printf("+----------------------+\n");
}


//FUNCION AGRICULTURA
void mostrarTablaDeDatosAnio(int anio, float avenaocebada, float mediacaudales)
{ 
	printf("| %i |  %.2f |    %.2f     |\n", anio, avenaocebada, mediacaudales);
	printf("+--------------------------------+\n");
}
	

//funcion para calcular el coeficiente de correlacion entre la capacidad y la produccion agricola
//utilizado solo dentro de este mismo archivo, no se incluye en la libreria
void calculoCoefcorrelacion(float *produccion, float *mediaAnual, float mediaTotal) {
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
        printf("No hay correlacion entre la capacidad y la produccion, y es: %2.f \n", coef_correlacion);
    }
}

// Función 1: Calcular la media anual de caudal para una cuenca específica
void calcularMediaAnualPorCuenca(Embalse *embalses, int totalEmbalses) {
    int i = 0, j = 0;
    float mediaMensual = 0.0, mediaAnual = 0.0; 
    char nom_cuenca[100]; 
    printf("Introduzca el nombre de la cuenca: \n");
    scanf(" %[^\n]", nom_cuenca); 
    printf("La cuenca elegida ha sido: %s\n", nom_cuenca); 
    for (int i = 0; i < totalEmbalses; i++){ 
        // Comparamos la cuenca introducida por el usuario con la cuenca del embalse actual
        if (strcmp(embalses[i].cuenca, nom_cuenca) == 0) {
            for (j=0; j<NUM_ANIOS; j++){ 
                
                mediaMensual += embalses[i].datos.caudales[j];
                }
            mediaMensual /= NUM_ANIOS; // Calculamos la media mensual para el embalse en enero por ejemplo
            mediaAnual += mediaMensual; 
        }
    } 
    mediaAnual /= NUM_MESES;
    printf("Media anual de %s es de: %.2f Hm^3\n", nom_cuenca, mediaAnual); 
}

// Función 2: Mostrar la evolución del caudal de la cuenca entre un rango de años
void evolucionEmbalse(Embalse *embalses, int totalEmbalses) {
    int anioInicio, anioFin;
    char embalse[100];
    printf("\nIngrese el nombre del embalse: ");   
    scanf(" %[^\n]", embalse);  // Lee el nombre del embalse
    printf("Ingrese el anio de inicio (2012-2021): ");
    scanf("%i", &anioInicio);  // Lee el año de inicio
    printf("Ingrese el anio de fin (2012-2021): ");
    scanf("%i", &anioFin);  // Lee el año de fin
    
    // Verifica si los años están dentro del rango permitido
    if(anioInicio < 2012 || anioFin > 2021 || anioInicio > anioFin) {
        printf("Introduzca una opcion valida.\n");
        return; 
    } 

    //calculo de media anual de caudal en un embalse
    float mediaAnual [ NUM_ANIOS ] = {0}; // Inicializamos el array de medias anuales
    for (int i = 0; i <totalEmbalses; i++) {
        if (strcmp(embalses[i].embalseNombre, embalse) == 0) { 
            for (int j = anioInicio - 2012; j <= anioFin - 2012; j++) {
                mediaAnual[j] += embalses[i].datos.caudales[j]; 
            }
                
        }
    }  
    for (int i = anioInicio - 2012; i <= anioFin - 2012; i++) {
        mediaAnual[i] /= NUM_MESES; // Calculamos la media anual
    }

    for(int i = anioInicio - 2012; i <= anioFin - 2012; i++) {
        printf("El caudal medio anual del embalse %s en el anio %d es: %.2f Hm^3\n", embalse, embalses[i].datos.anios[i], mediaAnual[i]);
    }

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
        if (strcmp(embalses[i].cuenca, cuencaA) == 0) {
            sumaA += embalses[i].datos.caudales[anio - 2012];  // Suma caudales de la cuenca A
        } else if (strcmp(embalses[i].cuenca, cuencaB) == 0) {
            sumaB += embalses[i].datos.caudales[anio - 2012];  // Suma caudales de la cuenca B
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

// Función 4: Comparar el caudal de dos embalses en un año específico
void compararEmbalses(Embalse *embalses, int totalEmbalses) {
    char embalseA[100], embalseB[100];
    int anio;
    printf("\nIngrese el nombre del primer embalse: ");
    scanf(" %[^\n]", embalseA);
    printf("Ingrese el nombre del segundo embalse: ");
    scanf(" %[^\n]", embalseB);

    printf("Ingrese el anio a comparar (2012-2021): ");
    scanf("%d", &anio);  // Lee el año

    // Verifica si el año está dentro del rango permitido
    if (anio < 2012 || anio > 2021) {
        printf("Anio fuera de rango.\n");
        return;
    }

    // Inicializa las variables para acumular los caudales
    float sumaA = 0, sumaB = 0;

    // Recorre los embalses y acumula los caudales de ambos embalses en el año solicitado
    for (int i = 0; i < totalEmbalses; i++) {
        if (strcmp(embalses[i].embalseNombre, embalseA) == 0) {
            sumaA += embalses[i].datos.caudales[anio - 2012];  // Suma caudales de la embalse A
        } else if (strcmp(embalses[i].embalseNombre, embalseB) == 0) {
            sumaB += embalses[i].datos.caudales[anio - 2012];  // Suma caudales del embalse B
        }
    }

    // Muestra la comparación de caudales entre las dos cuencas
    printf("\n--> Comparacion de caudal en el anio %d:\n", anio);
    //-20, el - alinea a la izq. el texto y 20 son los espacios reservados
    printf("%-20s: %.2f\n", embalseA, sumaA);
    printf("%-20s: %.2f\n", embalseB, sumaB);

    // Compara los caudales y muestra cuál embalse tuvo el mayor caudal
    if(sumaA == 0 && sumaB == 0){
		printf("Da 0 la suma de los caudales, posible problema de escritura");
	} else if (sumaA > sumaB) {
        printf("El embalse '%s' tuvo mayor volumen de agua embalsada.\n", embalseA);
    } else if (sumaB > sumaA) {
        printf("El embalse '%s' tuvo mayor volumen de agua embalsada.\n", embalseB);
    } else if (sumaA == sumaB){		
		printf("Ambos embalses tuvieron el mismo volumen de agua embalsada.\n");
        printf("Posible error en la escritura de las cuencas!!!\n");
	} else {
        printf("Posible error en la escritura de algun embalse!!!\n");
    }
}
 
//FUNCION AGRICULTURA
// Función 6: Comparar el caudal con la producción agrícola 
void compararCaudalAgricola(Embalse* embalses, int totalEmbalses) 
{
    float mediaTotal=0;
    float mediaAnual[NUM_ANIOS];
    for(int j=0; j<NUM_ANIOS; j++){
        for(int i=0; i<totalEmbalses; i++){
            mediaAnual[j] += embalses[i].datos.caudales[j]; 
        }
        mediaAnual[j] /= totalEmbalses; //media anual de los caudales
        mediaTotal += mediaAnual[j] ; //media total de los caudales
    } 
    mediaTotal /= NUM_ANIOS; //media total de los caudales

    char opcion_elegida;
    int anios[] = {2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021};
    int mes_elegido;
    printf("En este apartado se va a estudiar la relacion de la capacidad de los embalses con la produccion agricola del cultivo deseado.\n");
    printf("Para un mejor entendimiento mostraremos tablas grafícas y unos pocos datos \n");
    printf("Los datos de los cultivos vienen dados en toneladas, tenlo en cuenta \n");
	printf("Ahora dime con que tipo de producto agricola deseas estudiar: \n ");
	printf("Con Hortalizas(H), con los Industriales(I) o con los de Grano(G) \n");
	printf("La opcion elegida es: \t");
	scanf(" %c", &opcion_elegida);
	
	//los datos de los vectores "produccion" son reales
    // han sido obtenidos a traves de tablas de datos presentes en tablas y pdfs  
    // encontrados en la web oficial del ministerio de agricultura, pesca y alimentacion
    // del gobierno de España. 
    switch(opcion_elegida) 
    { 
        
        case 'h':
		case 'H':
        {
            int i; 
            
            char nombre_cuenca[500]; 
            printf("Introduzca el nombre de la cuenca que desea comparar: \n");
            scanf("%s", nombre_cuenca);  
            for (int i = 0; i < totalEmbalses; i++)
            {
                if (strcmp(embalses[i].cuenca, nombre_cuenca) == 0)
                {
                    printf("El embalse elegido para comparar ha sido: %s\n", embalses[i].embalseNombre);
                }
            } 
            printf("Del embalse %s que mes quieres comparar a lo largo de los anios?\n",embalses[i].embalseNombre);//preguntamos el mes para asi comparar por ejemplo el mes de enero del 2012 al 2021 en ese embalse y asociarlo a la produccion 
            scanf("%i", &mes_elegido);
            if (mes_elegido < 1 || mes_elegido > 12)
            {
                printf("Mes no valido. Debe ser entre 1 y 12.\n");
                return;
            } else {
                printf("El mes elegido para comparar ha sido: %i\n", embalses[i].datos.mes);
            } 
            
            // Datos
    
            float produccion_h[] = {13148, 12973, 13283, 14626, 14772, 15367, 15545, 14992, 15880, 15180};//datos de produccion de hortalizas en el mes elegido durantes los años establecidos
            calculoCoefcorrelacion( produccion_h, mediaAnual, mediaTotal);
            printf("+-----------------------------------+\n");
            printf("| Año | Hortalizas | Media caudales |\n");
            printf("+-----------------------------------+\n");
            mostrarTablaDeDatosAnio(anios[i], produccion_h[i], mediaAnual[i]);
            // Calcular valor máximo
            float max_valor = produccion_h[0];
            for (int i = 1; i < 10; i++) 
            {
                if (produccion_h[i] > max_valor)
                {
                    max_valor = produccion_h[i];//si hay un valor mayor que el maximo, lo actualiza
                }
            
            }

            // Encabezado
            printf(" Grafica de Barras :\n");
            for (int i = 0; i < 10; i++) 
            {
                // Escalar a 90 caracteres
                float longitud_barra = ((produccion_h[i] / max_valor) * 100);//hacemos la longitud de la barra en funcion del maximo valor de produccion de hortalizas

                // Mostrar años
                printf("%d | ", anios[i]);

                // Dibujar la barra con '#'
                for (int j = 0; j < longitud_barra; j++)
                {
                    putchar('#');
                }

                // Mostrar el valor exacto
                printf(" %.3f\n", produccion_h[i]);
            }   
         
			
		
			break;
        }
		case 'i':
		case 'I':
		{
            char indus[0];
            printf("Que tipo de cultivo industrial quieres comparar: Girasol(G) o Soja(S)?\n");
            printf("El cultivo industrial elegido es: \n");
            scanf(" %s", indus);  

                switch(indus[0])
                { 

                    case 'g':
                    case 'G':
                    {   float produccion_gira[] = {642.0, 1038.1, 953.0, 769.2, 772.2, 814.7, 950.3, 773.8, 883.1, 771.0};
                        int anielegido;
                        printf("Entre que anios deseas estudiar la produccion de girasol?\n");
                        scanf("%i", &anielegido); //se le pide al usuario el año que quiere estudiar
                        if (anielegido < 2012 || anielegido > 2021)
                        {
                            printf("El anio que introduciste es invalido. Debes introducir un anio entre 2012 y 2021. \n");
                            return;
                        }
                        calculoCoefcorrelacion(produccion_gira, mediaAnual, mediaTotal);
                        printf("+--------------------------------+\n");
                        printf("| Año | Girasol | Media caudales |\n");
                        printf("+--------------------------------+\n");
                        mostrarTablaDeDatosAnio(anios[anielegido - 2012], produccion_gira[anielegido - 2012], mediaAnual[anielegido - 2012]);
                        //se procede a hacer la grafica de barras con los datos de produccion de girasol
                        // Calcular valor máximo
                        float max_valor = produccion_gira[0];
                        for (int i = 1; i < 10; i++) 
                        {
                            if (produccion_gira[i] > max_valor)
                            {
                                max_valor = produccion_gira[i];
                            }   
                        }

                        // Encabezado
                        printf("\nGrafica de Barras :\n\n");

                        for (int i = 0; i < 10; i++) 
	                    {
                            // Escalar a 90 caracteres
                            float longitud_barra = ((produccion_gira[i] / max_valor) * 100);//hacemos la longitud de la barra en funcion del maximo valor de produccion de girasol

                            // Mostrar años
                            printf("%d | ", anios[i]);

                            // Dibujar la barra con '#'
                            for (int j = 0; j < longitud_barra; j++)
		                    {
                                putchar('#');
                            }

                            // Mostrar el valor exacto
                            printf(" %.3f\n", produccion_gira[i]);
                        }

       
                        break; 
                    }
                    case 's':
                    case 'S':
                    {
                        float produccion_soja[] = {1333, 1390, 2650, 4106, 2869, 4599, 4249, 5053, 4515, 4769}; 
                        int anioelegido;
                        printf("Entre que años deseas estudiar la produccion de girasol?\n");
                        scanf("%i", &anioelegido); //se le pide al usuario el año que quiere estudiar
                        if (anioelegido < 2012 || anioelegido > 2021)
                        {
                            printf("El anio que introduciste es invalido. Debes introducir un anio entre 2012 y 2021. \n");
                            return;
                        } 
                        calculoCoefcorrelacion(produccion_soja, mediaAnual, mediaTotal);
                        printf("+-----------------------------+\n");
                        printf("| Año | Soja | Media caudales |\n");
                        printf("+-----------------------------+\n");
                        mostrarTablaDeDatosAnio(anios[anioelegido - 2012], produccion_soja[anioelegido - 2012], mediaAnual[anioelegido - 2012]);
                        //se procede a hacer la grafica de barras con los datos de produccion de soja
                        // Calcular valor máximo
                        float max_valor = produccion_soja[0];
                        for (int i = 1; i < 10; i++)
                        {
                            if (produccion_soja[i] > max_valor)
                            {
                                max_valor = produccion_soja[i];
                            }   
                        }
                        // Encabezado
                        printf("\nGrafica de Barras :\n\n");
                        for (int i = 0; i < 10; i++) 
	                    {
                            // Escalar a 90 caracteres
                            float longitud_barra = ((produccion_soja[i] / max_valor) * 100);//hacemos la longitud de la barra en funcion del maximo valor de produccion de soja

                            // Mostrar años
                            printf("%d | ", anios[i]);

                            // Dibujar la barra con '#'
                            for (int j = 0; j < longitud_barra; j++)
		                    {
                                putchar('#');
                            }
                            // Mostrar el valor exacto
                            printf(" %.3f\n", produccion_soja[i]);
                        }
                     
                       break;
                    }
                }

            
		}
		case 'g':	
		case 'G':
		{	
			char grano[0];
			printf("Que tipo quieres comparar: avena(A) o cebada(C)?\n");
			printf("El grano elegido es:\t");
			scanf(" %c", grano);
			scanf("%c", &grano);
			switch(grano[0])
			{
				case 'A':
				{	

					float avena_producida[10]={933.2, 957.7, 649.2, 872.2, 1161.0, 1131.0, 1486.9, 808.3, 1323.8, 1048.7};
					int anio;
					printf("Bien, necesito que me digas el anio que quieres saber:\t");
					scanf("%d", &anio);

					if (anio < 2012 || anio > 2021)
					{
						printf("El anio que introduciste es invalido. Debes introducir un anio entre 2012 y 2021. \n");
						return;
					}	

					FILE *archivo= fopen("dataset.csv","r");
					if(archivo == NULL)
					{
						printf("Error al abrir el archivo");
						return;
					}

					 char linea[1030];
					 int columna = 0, columna_objetivo;

					 fgets(linea, sizeof(linea), archivo);
					 char *tok = strtok(linea, ",");	
					 while(tok != NULL)
					{
						if(atoi(tok) == anio) 
						{
							columna_objetivo = columna;
							break;
						}
						tok = strtok(NULL, ",");
						columna++;
					}

					float sumacaudales = 0;
					 while(fgets(linea,sizeof(linea),archivo))
					 {	
						 int columna2 = 0;
						 float valorcaudal;		
						 tok = strtok(linea, ",");
						 while(tok != NULL)
						{
							if(columna2 == columna_objetivo)
							{
								valorcaudal = atof(tok);
								sumacaudales += valorcaudal;
								break;
							}
							tok = strtok(NULL, ",");
							columna2++;
						}
					 }
					fclose(archivo);

					float media_caudales = sumacaudales / totalEmbalses;
					int i = anio - 2012;
                    printf("+------------------------------+\n");
                    printf("| Año | Avena | Media caudales |\n");
                    printf("+------------------------------+\n");
		    mostrarTablaDeDatosAnio( anio, avena_producida[i], media_caudales); 
                    // Calcular valor máximo
                    float max_valor = avena_producida[0];
                    for (int i = 1; i < 10; i++) 
                    {
                        if (avena_producida[i] > max_valor)
                        {
                            max_valor = avena_producida[i];
                        }   
                    }

                    // Encabezado
                    printf("\nGrafica de Barras :\n\n");

                    for (int i = 0; i < 10; i++) 
                    {
                        // Escalar a 90 caracteres
                        float longitud_barra = ((avena_producida[i] / max_valor) * 100);//hacemos la longitud de la barra en funcion del maximo valor de produccion de avena

                        // Mostrar años
                        printf("%d | ", anios[i]);

                        // Dibujar la barra con '#'
                        for (int j = 0; j < longitud_barra; j++)
                        {
                            putchar('#');
                        }

                        // Mostrar el valor exacto
                        printf(" %.3f\n", avena_producida[i]);
                    }

   
					break;
				}


				case 'C':		
				{
					float cebada_producida[10]={5956.3, 7515.4, 6983.3, 6176.6, 6312.2, 5785.9, 6974.6, 7400.0, 10955.8, 10477.8};	
					int anio;
					printf("Bien, necesito que me digas el anio que quieres saber:\t");
					scanf("%d", &anio);

					if (anio < 2012 || anio > 2021)
					{
						printf("El anio que introduciste es invalido. Debes introducir un anio entre 2012 y 2021. \n");
						return;
					}	

					FILE *archivo= fopen("dataset.csv","r");
					if(archivo == NULL)
					{
						printf("Error al abrir el archivo");
						return;
					}

					char linea[1030];
					int columna = 0, columna_objetivo;

					fgets(linea, sizeof(linea), archivo);
					char *tok = strtok(linea, ",");	
					while(tok != NULL)
					{
						if(atoi(tok) == anio) 
						{
							columna_objetivo = columna;
							break;
						}
						tok = strtok(NULL, ",");
						columna++;
					}

					float sumacaudales = 0;
					while(fgets(linea,sizeof(linea),archivo))
					{	
						int columna2 = 0;
						float valorcaudal;		
						tok = strtok(linea, ",");
						while(tok != NULL)
						{
							if(columna2 == columna_objetivo)
							{
								valorcaudal = atof(tok);
								sumacaudales += valorcaudal;
								break;
							}
							tok = strtok(NULL, ",");
							columna2++;	
						}
					}
					fclose(archivo);

					float media_caudales = sumacaudales / totalEmbalses;
					int i = anio - 2012;
                      			printf("+-------------------------------+\n");
                    			printf("| Año | Cebada | Media caudales |\n");
                    			printf("+-------------------------------+\n");
					mostrarTablaDeDatosAnio( anio, cebada_producida[i], media_caudales);
			                // Calcular valor máximo
			                float max_valor = cebada_producida[0];
			                for (int i = 1; i < 10; i++) 
			                {
		                            if (cebada_producida[i] > max_valor)
		                            {
			                            max_valor = cebada_producida[i];
		                            }   
		                        }
			
			                // Encabezado
			                printf("\nGrafica de Barras :\n\n");
		 
		                        for (int i = 0; i < 10; i++) 
		                        {
			                        // Escalar a 90 caracteres
			                        float longitud_barra = ((cebada_producida[i] / max_valor) * 100);//hacemos la longitud de la barra en funcion del maximo valor de produccion de cebada
			
			                        // Mostrar años
			                        printf("%d | ", anios[i]);
			
			                        // Dibujar la barra con '#'
			                        for (int j = 0; j < longitud_barra; j++)
			                        {
			                            putchar('#');
			                        }
			
			                        // Mostrar el valor exacto
			                        printf(" %.3f\n", cebada_producida[i]);
			                }

					break;		
				}
			} 
		}
    }
}		
