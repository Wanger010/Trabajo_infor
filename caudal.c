#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <stdlib.h>
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

//Función para mostrar cuencas y embalses (principal)
void mostrarCuencasYEmbalses(Embalse *embalses, int totalEmbalses) {
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


//FUNCION AGRICULTURA
void mostrarTablaDeDatos(int anio_inicial, int anio_final, int *produccion, Embalse *embalses)
{
	printf("+------------------------------+\n");
	printf("| Año | Avena | Media caudales |\n");
	printf("+------------------------------+\n");

	for(int i = anio_inicial; i <= anio_final; i++)
	{
        printf("| %i |  %.2f | %.2f |\n", i, produccion[i], embalses[i].datos.caudales[0]);
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
void mostrarCuencasYEmbalses(Embalse *embalses, int totalEmbalses) {
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
void evolucionEmbalse(Embalse *embalses, int totalEmbalses) {
    int anioInicio, anioFin;
    char embalse[100];
    printf("\nIngrese el nombre del embalse: ");   
    scanf(" %[^\n]", embalse);  // Lee el nombre de la cuenca
    printf("Ingrese el año de inicio (2012-2021): ");
    scanf("%i", &anioInicio);  // Lee el año de inicio
    printf("Ingrese el año de fin (2012-2021): ");
    scanf("%i", &anioFin);  // Lee el año de fin
    
    // Verifica si los años están dentro del rango permitido
    if(anioInicio < 2012 || anioFin > 2021 || anioInicio > anioFin) {
        printf("Introduzca una opcion valida.\n");
        return; 
    } 

    //calculo de media anual de caudal en un embalse
    float mediaAnual [ NUM_ANIOS ] = {0}; // Inicializamos el array de medias anuales
    for (int i = anioInicio - 2012; i <= anioFin - 2012; i++) {
        if (strcmp(embalses[i].embalseNombre, embalse) == 0) { 
            for (int j = 0; j <= totalEmbalses; j++) {
                mediaAnual[i] += embalses[i].datos.caudales[j];
            }
            mediaAnual[i] /= NUM_MESES; // Medias anuales   
        }
    } 

    for(int i = anioInicio - 2012; i <= anioFin - 2012; i++) {
        printf("El caudal medio anual del embalse %s en el año %d es: %.2f Hm³\n", embalse, embalses[i].datos.anios[i], mediaAnual[i]);
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


 
//FUNCION AGRICULTURA
// Función 6: Comparar el caudal con la producción agrícola (pendiente de implementación)
void compararCaudalAgricola(Embalse* embalses, int totalEmbalses) 
{  

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

    char nombre_cuenca[500];
    totalEmbalses = contarLineas("dataset.csv");
    printf("Introduzca el nombre de la cuenca que desea comparar: ");
    scanf("%s", nombre_cuenca);  
    for (int i = 0; i < totalEmbalses; i++)
    {
    	if (strcmp(embalses[i].cuenca, nombre_cuenca) == 0)
    	{
    		printf("El embalse elegido para comparar ha sido: %s\n", embalses[i].embalseNombre);
		}
	}
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
    int mes_elegido;
    switch(opcion_elegida) 
    { 
        
        case 'h':
		case 'H':
        {
            printf("¿Del embalse %s que mes quieres comparar a lo largo de los años?\n",embalses[i].embalseNombre);//preguntamos el mes para asi comparar por ejemplo el mes de enero del 2012 al 2021 en ese embalse y asociarlo a la produccion
            scanf("%i",&mes_elegido);
            if (mes_elegido < 1 || mes_elegido > 12)
            {
                printf("Mes no valido. Debe ser entre 1 y 12.\n");
                return;
            }
        
            for (int i=0; i < totalEmbalses; i++)
            {
                if( strcmp(embalses[i].mes, mes_elegido) == 0)
                {
                    printf("El mes elegido para comparar ha sido: %s\n", embalses[i].mes);
                }
            }
            // Datos
    
            float valores[] = {13.148, 12.973, 13.283, 14.626, 14.772,
                    15.367, 15.545, 14.992, 15.880, 15.180};//datos de produccion de hortalizas en el mes elegido durantes los años establecidos

            // Calcular valor máximo
            float max_valor = valores[0];
            for (int i = 1; i < 10; i++) 
            {
                if (valores[i] > max_valor)
                {
                    max_valor = valores[i];
                }
            
            }

            // Encabezado
            printf(" Grafica de Barras :\n");
            for (int i = 0; i < 10; i++) 
            {
                // Escalar a 90 caracteres
                float longitud_barra = ((valores[i] / max_valor) * 90);

                // Mostrar años
                printf("%d | ", anios[i]);

                // Dibujar la barra con '#'
                for (int j = 0; j < longitud_barra; j++)
                {
                    putchar('#');
                }

                // Mostrar el valor exacto
                printf(" %.3f\n", valores[i]);
            }   
        
            
            printf("El caudal medio de la cuenca %s en el mes %s es: %.2f\n", embalses[i].cuenca, embalses[i].mes, embalses[i].datos.caudales[mes_elegido-1]);
			
		
			break;
        }
		case 'i':
		case 'I':
		{
            char indus[0];
            printf("¿Que tipo de cultivo industrial quieres comparar: Girasol(G) o Soja(S)?\n");
            printf("El cultivo industrial elegido es: \n");
            scanf(" %s", indus); 


            switch(indus[2])
            { 

                switch(indus[0])
                { 

                    case 'g':
                    case 'G':
                    {   float produccion_gira[] = {642.0, 1038.1, 953.0, 769.2, 772.2, 814.7, 950.3, 773.8, 883.1, 771.0};
                        calculoCoefcorrelacion(produccion_gira, mediaAnual, mediaTotal);
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
                            float longitud_barra = ((produccion_gira[i] / max_valor) * 100);

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
                        calculoCoefcorrelacion(produccion_soja, mediaAnual, mediaTotal);
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
                            float longitud_barra = ((produccion_soja[i] / max_valor) * 100);

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
			

        }
		case 'g':
		case 'G':
		{	
			char grano[0];
			printf("¿Que tipo quieres comparar: avena(A) o cebada(C)?\n");
			printf("El grano elegido es:\t");
			scanf("%c", grano);

			switch(grano[0])
			{
				case 'A':
            	{	char opc[0]; 	
					printf("Ahora tienes que decidirte si quieres saber la avena producida y el caudal de un embalse en un mes entre 2012 y 2021(M) o en un anio(Y)\n");
					printf("Deseas saber: \t");
					scanf("%s", &opc);
	
					switch(opc[0])
					{
						case 'M':
                        {
						    int mes;
						    printf("Bien, ahora necesito que me digas el numero del mes, ejemplo --> Enero(1), Febrero(2), etc:  \t");
						    scanf("%i", &mes);
                        }
						case 'Y':
                        {

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
                            }
                        }
                    }	
                }
				case 'C':
				{	
					char opci[0]; 	
					printf("Ahora tienes que decidirte si quieres saber la cebada producida y el caudal de un embalse en un mes entre 2012 y 2021(M) o en un anio(Y)\n");
					printf("Deseas saber: \t");
					scanf("%s", &opc);
	
					switch(opci[0])
					{
						case 'M':
                        {
						    int meses[12]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, mes;
						    printf("Bien, ahora necesito que me digas el numero del mes, ejemplo --> Enero(1), Febrero(2), etc:  \t");
						    scanf("%i", &mes);
                            int anio;
                            break;
                        }

						
                    }
                    break;
                }
        
            }
        }
						
	}
}			

    