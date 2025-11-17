#include "trie.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * @brief Crea un nodo del trie.
 *
 * Reserva memoria para un nodo e inicializa todos los campos a cero.
 *
 * @return Puntero al nodo creado o NULL si la asignacion falla.
 */
Nodo* crear_nodo(void) {
    Nodo* nuevo_nodo = (Nodo*)calloc(1, sizeof(Nodo)); 
    if (nuevo_nodo == NULL) {
        perror("Error de asignacion de memoria para Nodo");
    }
    return nuevo_nodo;
}

/**
 * @brief Obtiene el indice correspondiente a una base de ADN.
 *
 * Convierte los caracteres 'A', 'C', 'G', 'T' en indices 0-3.
 *
 * @param base Caracter de la base.
 * @return Indice 0-3 o -1 si es invalido.
 */
int get_base_index(char base) {
    switch (base) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default: return -1;
    }
}

/**
 * @brief Genera recursivamente el arbol trie completo.
 *
 * @param nivel_actual Nivel en el que se encuentra la recursion.
 * @param profundidad_max Profundidad total del arbol (tamano de los genes).
 * @return Puntero a un nodo valido o NULL si falla la asignacion.
 */
Nodo* generar_arbol_recursivo(int nivel_actual, int profundidad_max) {
    Nodo* nodo = crear_nodo();
    if (nodo == NULL) return NULL;

    if (nivel_actual < profundidad_max) {
        for (int i = 0; i < 4; i++) {
            nodo->hijos[i] = generar_arbol_recursivo(nivel_actual + 1, profundidad_max);
            if (nodo->hijos[i] == NULL && profundidad_max > 0) {
                for (int j = 0; j < i; j++) liberar_arbol(nodo->hijos[j]);
                free(nodo);
                return NULL;
            }
        }
    }
    return nodo;
}

/**
 * @brief Inicializa el trie con profundidad m.
 *
 * Reinicia cualquier estructura previa del programa.
 *
 * @param m Largo de los genes.
 * @return Raiz del trie creado.
 */
Nodo* bio_start(int m) {
    if (m <= 0) {
        printf("%sError: El tamano del gen (m) debe ser mayor a 0.\n%s", ROJO, RESET);
        return NULL;
    }
    if (TRIE_ROOT != NULL) {
        bio_exit();
    }
    GEN_SIZE = m;
    TRIE_ROOT = generar_arbol_recursivo(0, m);
    if (TRIE_ROOT != NULL) {
        printf("%sArbol creado con tamano %d\n%s", AMARILLO, m, RESET);
    } else {
        printf("%sError al crear el arbol.\n%s", ROJO, RESET);
    }
    return TRIE_ROOT;
}

/**
 * @brief Inserta un gen en el trie.
 *
 * @param gen Cadena del gen a insertar.
 * @param posicion Posicion en la secuencia original.
 * @return 0 si se inserto correctamente, -1 si hubo error.
 */
int insertar_gen_en_trie(const char* gen, int posicion) {
    if (TRIE_ROOT == NULL || gen == NULL || (int)strlen(gen) != GEN_SIZE) {
        return -1;
    }

    Nodo* actual = TRIE_ROOT;
    int index;

    for (int i = 0; i < GEN_SIZE; i++) {
        index = get_base_index(gen[i]);
        if (index == -1) {
            return -1;
        }
        actual = actual->hijos[index];
    }
    actual->lista_posiciones = insertar_posicion(actual->lista_posiciones, posicion);
    return 0;
}

/**
 * @brief Lee una secuencia de ADN desde archivo y la carga en el trie.
 *
 * @param nombre_archivo Nombre del archivo a leer.
 * @return 0 si se leyo correctamente, -1 en caso de error.
 */
int bio_read(const char* nombre_archivo) {
    if (TRIE_ROOT == NULL) {
        printf("%sError: El arbol no ha sido inicializado. Use 'bio start m' primero.\n%s", ROJO, RESET);
        return -1;
    }
    FILE* file = fopen(nombre_archivo, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long n_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* secuencia_S = (char*)malloc(n_size + 1);
    if (secuencia_S == NULL) {
        perror("Error de asignacion de memoria para S");
        fclose(file);
        return -1;
    }

    size_t read_len = fread(secuencia_S, 1, n_size, file);
    fclose(file);

    int n = (int)read_len;
    while (n > 0 && (secuencia_S[n - 1] == '\n' || secuencia_S[n - 1] == '\r' ||
                     secuencia_S[n - 1] == ' '  || secuencia_S[n - 1] == '\t')) {
        n--;
    }
    secuencia_S[n] = '\0';

    int m = GEN_SIZE;

    if (n < m) {
        printf("%sLa secuencia S es mas corta que el tamano del gen m.\n%s", AMARILLO, RESET);
        free(secuencia_S);
        return -1;
    }

    char* gen_buffer = (char*)malloc(m + 1);
    if (gen_buffer == NULL) {
        perror("Error de asignacion de memoria para gen_buffer");
        free(secuencia_S);
        return -1;
    }
    gen_buffer[m] = '\0';

    for (int i = 0; i <= n - m; i++) {
        strncpy(gen_buffer, secuencia_S + i, m);
        insertar_gen_en_trie(gen_buffer, i);
    }

    free(gen_buffer);
    free(secuencia_S);
    printf("%sSecuencia S leida desde el archivo\n%s", AMARILLO, RESET);
    return 0;
}

/**
 * @brief Busca un gen en el trie y muestra su frecuencia.
 *
 * @param gen_G Gen a buscar.
 * @return Frecuencia encontrada o -1 si no existe.
 */
int bio_search(const char* gen_G) {
    if (TRIE_ROOT == NULL || GEN_SIZE == 0) {
        printf("%sError: El arbol no esta inicializado.\n%s", ROJO, RESET);
        return -1;
    }
    
    int m = GEN_SIZE;
    if ((int)strlen(gen_G) != m) {
        printf("%sError: El gen ingresado debe tener el largo %d.\n%s", ROJO, m, RESET);
        return -1;
    }

    Nodo* actual = TRIE_ROOT;
    int index;

    for (int i = 0; i < m; i++) {
        index = get_base_index(gen_G[i]);
        if (index == -1) {
            printf("%sNo se encontro el gen %s debido a caracter invalido.\n%s", ROJO, gen_G, RESET);
            return -1;
        }
        actual = actual->hijos[index];
    }
    
    int frecuencia = contar_posiciones(actual->lista_posiciones);
    if (frecuencia == 0) {
        printf("El gen %s no se encontro. Frecuencia: %s-1%s\n", gen_G, ROJO, RESET);
        return -1;
    }
    printf("%sEl gen %s esta %d veces en las posiciones:%s", AMARILLO, gen_G, frecuencia, RESET);
    imprimir_posiciones(actual->lista_posiciones);
    printf("\n");
    
    return frecuencia;
}

/**
 * @brief Recorre el arbol para obtener la frecuencia maxima o minima.
 *
 * @param nodo Nodo actual.
 * @param buffer Cadena para construir el gen.
 * @param nivel Nivel actual.
 * @param mode 1 = max, 0 = min.
 * @param extremum Valor a actualizar.
 */
void recorrido_extremos(Nodo* nodo, char* buffer, int nivel, int mode, int* extremum) {
    if (nodo == NULL) return;

    if (nivel == GEN_SIZE) {
        int frecuencia = contar_posiciones(nodo->lista_posiciones);
        if (mode == 1) {
            if (frecuencia > *extremum) *extremum = frecuencia;
        } else {
            if (frecuencia > 0 && frecuencia < *extremum) *extremum = frecuencia;
        }
        return;
    }

    const char bases[] = {'A', 'C', 'G', 'T'};
    for (int i = 0; i < 4; i++) {
        buffer[nivel] = bases[i];
        recorrido_extremos(nodo->hijos[i], buffer, nivel + 1, mode, extremum);
    }
}

/**
 * @brief Imprime los genes cuya frecuencia coincide con un valor objetivo.
 *
 * @param nodo Nodo actual.
 * @param buffer Cadena con el gen construido.
 * @param nivel Nivel actual.
 * @param target_freq Frecuencia objetivo.
 */
void imprimir_recorrido(Nodo* nodo, char* buffer, int nivel, int target_freq) {
    if (nodo == NULL) return;

    if (nivel == GEN_SIZE) {
        int frecuencia = contar_posiciones(nodo->lista_posiciones);
        if (frecuencia == target_freq && frecuencia > 0) {
            buffer[GEN_SIZE] = '\0';
            printf("%s %d", buffer, frecuencia);
            imprimir_posiciones(nodo->lista_posiciones);
            printf("\n");
        }
        return;
    }

    const char bases[] = {'A', 'C', 'G', 'T'};
    for (int i = 0; i < 4; i++) {
        buffer[nivel] = bases[i];
        imprimir_recorrido(nodo->hijos[i], buffer, nivel + 1, target_freq);
    }
}

/**
 * @brief Muestra el gen con mayor frecuencia.
 */
void bio_max() {
    if (TRIE_ROOT == NULL || GEN_SIZE == 0) {
        printf("%sError: Arbol no cargado.\n%s", ROJO, RESET);
        return;
    }

    char* gen_buffer = (char*)malloc(GEN_SIZE + 1);
    if (gen_buffer == NULL) { perror("Error de asignacion"); return; }

    int max_freq = 0;
    recorrido_extremos(TRIE_ROOT, gen_buffer, 0, 1, &max_freq);

    if (max_freq == 0) {
        printf("%sNo hay genes presentes.\n%s", AMARILLO, RESET);
        free(gen_buffer);
        return;
    }

    imprimir_recorrido(TRIE_ROOT, gen_buffer, 0, max_freq);
    free(gen_buffer);
}

/**
 * @brief Muestra el gen con menor frecuencia.
 */
void bio_min() {
    if (TRIE_ROOT == NULL || GEN_SIZE == 0) {
        printf("%sError: Arbol no cargado.\n%s", ROJO, RESET);
        return;
    }

    char* gen_buffer = (char*)malloc(GEN_SIZE + 1);
    if (gen_buffer == NULL) { perror("Error de asignacion"); return; }

    int min_freq = INT_MAX;
    recorrido_extremos(TRIE_ROOT, gen_buffer, 0, 0, &min_freq);

    if (min_freq == INT_MAX) {
        printf("%sNo hay genes presentes.\n%s", AMARILLO, RESET);
        free(gen_buffer);
        return;
    }

    imprimir_recorrido(TRIE_ROOT, gen_buffer, 0, min_freq);
    free(gen_buffer);
}

/**
 * @brief Muestra todos los genes con frecuencia mayor que 0.
 */
void bio_all() {
    if (TRIE_ROOT == NULL || GEN_SIZE == 0) {
        printf("%sError: Arbol no cargado.\n%s", ROJO, RESET);
        return;
    }

    char* gen_buffer = (char*)malloc(GEN_SIZE + 1);
    if (gen_buffer == NULL) { perror("Error de asignacion"); return; }

    void bio_all_recursivo(Nodo* nodo, char* buffer, int nivel) {
        if (nodo == NULL) return;

        if (nivel == GEN_SIZE) {
            int frecuencia = contar_posiciones(nodo->lista_posiciones);
            if (frecuencia > 0) {
                buffer[GEN_SIZE] = '\0';
                printf("%s %d", buffer, frecuencia);
                imprimir_posiciones(nodo->lista_posiciones);
                printf("\n");
            }
            return;
        }

        const char bases[] = {'A', 'C', 'G', 'T'};
        for (int i = 0; i < 4; i++) {
            buffer[nivel] = bases[i];
            bio_all_recursivo(nodo->hijos[i], buffer, nivel + 1);
        }
    }

    bio_all_recursivo(TRIE_ROOT, gen_buffer, 0);
    free(gen_buffer);
}

/**
 * @brief Libera toda la memoria del arbol trie.
 *
 * @param nodo Nodo actual.
 */
void liberar_arbol(Nodo* nodo) {
    if (nodo == NULL) return;

    for (int i = 0; i < 4; i++) {
        liberar_arbol(nodo->hijos[i]);
    }
    liberar_lista(nodo->lista_posiciones);
    free(nodo);
}

/**
 * @brief Libera memoria global y cierra el programa.
 */
void bio_exit() {
    liberar_arbol(TRIE_ROOT);
    TRIE_ROOT = NULL;
    GEN_SIZE = 0;
    printf("%sLimpiando cache y saliendo del programa...\n%s", AMARILLO, RESET);
}

/**
 * @brief Imprime la lista de comandos disponibles.
 */
void help() {
    printf("%sCOMANDOS DISPONIBLES:\n%s", VERDE_CLARO, RESET);
    printf("%sbio start m%s: crea un arbol 4-ario\n", AZUL, RESET);
    printf("%sbio read archivo.txt%s: lee la secuencia S\n", AZUL, RESET);
    printf("%sbio search XX%s: busca un gen\n", AZUL, RESET);
    printf("%sbio max%s: muestra el gen mas repetido\n", AZUL, RESET);
    printf("%sbio min%s: muestra el gen menos repetido\n", AZUL, RESET);
    printf("%sbio all%s: lista todos los genes frecuentes\n", AZUL, RESET);
    printf("%sbio exit%s: libera memoria y termina\n", AZUL, RESET);
}
