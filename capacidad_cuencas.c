#include <stdio.h>
#include <stdlib.h>
#define num_lin 4238

typedef struct {
    char localizacion[30],nombre[30];
    int mes;
    float ano12, ano13, ano14, ano15, ano16;
    float ano17, ano18, ano19, ano20, ano21;
} cuenca;


int main() {
    FILE *pf;
    pf = fopen("dataset.csv", "r");
    int i, j, k;
    char linea[400], meses[30];
    cuenca cuenca[num_lin];
    
    //saltamos la primera línea para evitar errores
    fgets(linea, sizeof(linea), pf);
    
    //leemos el archivo entero y luego dividimos en secciones para evitar problemas con comas y comillas
     for (i = 0; i < num_lin; i++) {
        if (!fgets(linea, sizeof(linea), pf)) {
            printf("Error al leer línea %d\n", i + 1);
            break;
        }
        
        int l = 0;
        // Leer localización
        if (linea[l] == '"') l++;
        j = 0;
        while (linea[l] != '"' && linea[l] != ',' && linea[l] != '\0') {
            cuenca[i].localizacion[j++] = linea[l++];
        }
        cuenca[i].localizacion[j] = '\0';
        while (linea[l] != ',' && linea[l] != '\0') l++;
        l++; // Saltar la coma

        // Leer nombre
        if (linea[l] == '"') l++;
        j = 0;
        while (linea[l] != '"' && linea[l] != ',' && linea[l] != '\0') {
            cuenca[i].nombre[j++] = linea[l++];
        }
        cuenca[i].nombre[j] = '\0';
        while (linea[l] != ',' && linea[l] != '\0') l++;
        l++;

        // Leer mes
        j = 0;
        while (linea[l] != ',' && linea[l] != '\0') {
            meses[j++] = linea[l++];
        }
        meses[j] = '\0';
        cuenca[i].mes = atoi(meses);//atoi permite convertir un vector en un entero
        l++;

        // Leer los valores de todos los años
        float *an[] = {
            &cuenca[i].ano12, &cuenca[i].ano13, &cuenca[i].ano14,
            &cuenca[i].ano15, &cuenca[i].ano16, &cuenca[i].ano17,
            &cuenca[i].ano18, &cuenca[i].ano19, &cuenca[i].ano20,
            &cuenca[i].ano21
        };
        
        //asignamos a cada año el valor que le corresponde
        for (k = 0; k < 10; k++) {
            j = 0;
            while (linea[l] != ',' && linea[l] != '\n' && linea[l] != '\0') {
                meses[j++] = linea[l++];
            }
            meses[j] = '\0';
            *an[k] = atof(meses);// atof es parecido a atoi pero con numeros decimales
            if (linea[l] == ',') l++;
        }
    }
    
    //pruebas
    /*printf("Localizacion: %s\n", cuenca[2].localizacion);
    printf("Nombre: %s\n", cuenca[2].nombre);
    printf("Mes: %d\n", cuenca[2].mes);
    printf("Ano 2012: %.2f\n", cuenca[2].ano21);*/
    fclose(pf);
    return 0;
}
