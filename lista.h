#ifndef LISTA_H
#define LISTA_H

// Estructura para guardar una posicion
typedef struct ListaInt {
    int pos;
    struct ListaInt* siguiente;
} ListaInt;

// Funciones basicas de la lista
ListaInt* crear_nodo_lista(int posicion);
ListaInt* insertar_posicion(ListaInt* cabeza, int posicion);
int contar_posiciones(ListaInt* cabeza);
void imprimir_posiciones(ListaInt* cabeza);
void liberar_lista(ListaInt* cabeza);

#endif