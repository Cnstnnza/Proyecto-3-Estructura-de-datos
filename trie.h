#ifndef TRIE_H
#define TRIE_H

#include "lista.h"

#define MORADO "\033[1;35m"
#define RESET "\033[0m"
#define AMARILLO "\033[33m"
#define ROJO "\033[1;31m"
#define VERDE "\033[1;32m"
#define VERDE_CLARO "\033[1;32m"
#define AZUL "\033[1;34m"

/**
 * @brief Nodo del trie 4-ario para almacenar genes.
 *
 * Cada nodo representa una base del gen (A, C, G, T).
 * En los nodos hoja se guarda una lista con las posiciones
 * donde aparece el gen correspondiente.
 */
typedef struct Nodo {
    struct Nodo* hijos[4];        /**< Punteros a los hijos: indices 0=A, 1=C, 2=G, 3=T */
    ListaInt* lista_posiciones;   /**< Lista de posiciones del gen (solo en hojas) */
} Nodo;

/** 
 * @brief Largo del gen utilizado por el trie. 
 *
 * Debe ser definido con bio_start().
 */
extern int GEN_SIZE;

/** 
 * @brief Puntero a la raiz del trie principal. 
 *
 * Se asigna en bio_start().
 */
extern Nodo* TRIE_ROOT;

/**
 * @brief Crea y retorna un nodo del trie.
 * @return Puntero a un nodo inicializado o NULL si falla.
 */
Nodo* crear_nodo(void);

/**
 * @brief Inicializa el trie creando un arbol 4-ario de profundidad m.
 * @param m Largo del gen.
 * @return Puntero a la raiz del trie.
 */
Nodo* bio_start(int m);

/**
 * @brief Imprime los comandos disponibles del programa.
 */
void help();

/**
 * @brief Convierte una base ADN (A,C,G,T) en su indice numerico.
 * @param base Caracter 'A', 'C', 'G' o 'T'.
 * @return Indice entre 0 y 3, o -1 si el caracter es invalido.
 */
int get_base_index(char base);

/**
 * @brief Inserta un gen en el trie y registra la posicion donde aparece.
 * @param gen Secuencia de largo GEN_SIZE.
 * @param posicion Posicion inicial dentro de la secuencia S.
 * @return 0 si se inserta correctamente, -1 si ocurre un error.
 */
int insertar_gen_en_trie(const char* gen, int posicion);

/**
 * @brief Lee un archivo de ADN completo y carga todos los genes en el trie.
 * @param nombre_archivo Nombre del archivo a leer.
 * @return 0 si se carga correctamente, -1 en caso de error.
 */
int bio_read(const char* nombre_archivo);

/**
 * @brief Busca un gen dentro del trie.
 * @param gen_G Secuencia a buscar.
 * @return Frecuencia del gen o -1 si no se encuentra o es invalido.
 */
int bio_search(const char* gen_G);

/**
 * @brief Muestra el gen con mayor frecuencia dentro del trie.
 */
void bio_max();

/**
 * @brief Muestra el gen con menor frecuencia dentro del trie.
 */
void bio_min();

/**
 * @brief Imprime todos los genes con frecuencia mayor a 0.
 */
void bio_all();

/**
 * @brief Libera recursivamente todos los nodos del trie.
 * @param nodo Nodo raiz del subarbol a liberar.
 */
void liberar_arbol(Nodo* nodo);

/**
 * @brief Libera toda la memoria del programa y reinicia el estado.
 */
void bio_exit();

#endif
