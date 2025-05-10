// Protección contra inclusiones múltiples del archivo de cabecera
// Evita errores si este archivo se incluye más de una vez en el proyecto
#ifndef CAUDAL_H   // Si NO se ha definido CAUDAL_H aún
#define CAUDAL_H   // Entonces lo definimos para que no se vuelva a incluir este archivo

#define NUM_ANIOS 10  // Número de años de datos (2012-2021)
#define NUM_MESES 12  // Número de meses de datos (Enero-Diciembre)

// Estructuras para almacenar la información de los embalses y caudales
// Estructura que almacena datos del caudal de los embalses en el rango de años
// dado y por meses.
typedef struct {
    // Mes al que corresponde la medición (ej: "1", "2",...,"12")
    int mes[NUM_MESES];
    // Array de enteros que guarda los años correspondientes a los datos de caudal
    // Por ejemplo: [2012, 2013, 2014, ..., 2021]
    int anios[NUM_ANIOS];
    // Array de números decimales (float) que guarda los caudales en Hm³ para cada año
    // Por ejemplo: [54.2, 60.1, 48.7, ..., 70.5]
    float caudales[NUM_ANIOS];
} DatosEmbalse;

// Definición de una estructura llamada 'Embalse' que agrupa información sobre un embalse
typedef struct {
    // Nombre de la cuenca hidrográfica (ej: "Duero", "Tajo", "Ebro")
    char cuenca[100];
    // Nombre del embalse (ej: "Alcantara", "La Serena", etc.)
    char embalseNombre[100];
    
    // Estructura que contiene los datos anuales de caudal para este embalse y mes
    // A su vez, dentro de esta estructura su nombre va a ser datos.
    DatosEmbalse datos;
} Embalse;



// Declaraciones de funciones relacionadas con el análisis de caudales

// Declaración de otras funciones

void limpiarPantalla();
void reemplazarGuionBajoPorEspacio(char *cadena);
int contarLineas(const char *nombreArchivo);
//no es void porque devuelve algo, puntero
Embalse *leerDatos(const char *nombreArchivo, int totalEmbalses);
void mostrarCuencasYEmbalses(Embalse *embalses, int totalEmbalses);
void guardarCuencasYEmbalses(Embalse *embalses, int totalEmbalses);

//FUNCION PARA CONOCER MEDIA MESUAL DESDE 2012-2021
void mostrarMediaMensualCaudales(Embalse *embalses, int totalEmbalses);

//FUNCION TABLA DATOS MENSUAL
void mostrarTablaDeDatosMes(int anio, float mediacaudales);

//FUNCION AGRICULTURA
void mostrarTablaDeDatosAnio(int anio, float avenaocebada, float mediacaudales);

//void calculoCoefcorrelacion(float *produccion, char *mediaAnual, float mediaTotal)




// Declaracion de funciones del switch

void calcularMediaAnualPorCuenca(Embalse *embalses, int totalEmbalses);
void evolucionCuenca(Embalse *embalses, int totalEmbalses);
void evolucionEmbalse(Embalse *embalses, int totalEmbalses);
void compararCuencas(Embalse *embalses, int totalEmbalses);
void compararEmbalses(Embalse *embalses, int totalEmbalses);
void compararCaudalAgricola(Embalse *embalses, int totalEmbalses);

// Fin de la protección contra inclusiones múltiples
#endif  // CAUDAL_H
