#ifndef TRIE_H
#define TRIE_H

#include "lista.h"

// Estructura del Nodo 4-ario (Trie)
typedef struct Nodo {
    // Punteros a los 4 posibles hijos ('A', 'C', 'G', 'T')
    struct Nodo* hijos[4];
    
    // Lista de posiciones (Solo se usa en las hojas)
    ListaInt* lista_posiciones;
} Nodo;

// Variables globales para el estado del programa
extern int GEN_SIZE;
extern Nodo* TRIE_ROOT;

// Funciones del Trie
Nodo* crear_nodo(void);
Nodo* bio_start(int m);
void help();
int get_base_index(char base);
int insertar_gen_en_trie(const char* gen, int posicion);
int bio_read(const char* nombre_archivo);
int bio_search(const char* gen_G);
void bio_max();
void bio_min();
void bio_all();
void liberar_arbol(Nodo* nodo);
void bio_exit();

#endif