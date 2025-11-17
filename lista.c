#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Crea un nuevo nodo de lista que almacena una posicion.
 *
 * @param posicion Valor entero a guardar en el nodo.
 * @return Puntero al nuevo nodo creado, o NULL si falla la asignacion de memoria.
 */
ListaInt* crear_nodo_lista(int posicion) {
    ListaInt* nuevo_nodo = (ListaInt*)malloc(sizeof(ListaInt));
    if (nuevo_nodo == NULL) {
        perror("Error de asignacion de memoria para ListaInt");
        return NULL;
    }
    nuevo_nodo->pos = posicion;
    nuevo_nodo->siguiente = NULL;
    return nuevo_nodo;
}

/**
 * @brief Inserta una posicion al final de la lista enlazada.
 *
 * Si la lista esta vacia, el nuevo nodo pasa a ser la cabeza.
 *
 * @param cabeza Puntero al primer nodo de la lista.
 * @param posicion Valor entero a insertar.
 * @return Puntero a la cabeza de la lista (puede cambiar si antes era NULL).
 */
ListaInt* insertar_posicion(ListaInt* cabeza, int posicion) {
    ListaInt* nuevo_nodo = crear_nodo_lista(posicion);
    if (nuevo_nodo == NULL) {
        return cabeza;
    }

    if (cabeza == NULL) {
        return nuevo_nodo;
    }

    ListaInt* actual = cabeza;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevo_nodo;
    
    return cabeza;
}

/**
 * @brief Cuenta la cantidad de nodos en la lista.
 *
 * @param cabeza Puntero al primer nodo de la lista.
 * @return Numero total de nodos.
 */
int contar_posiciones(ListaInt* cabeza) {
    int count = 0;
    ListaInt* actual = cabeza;
    while (actual != NULL) {
        count++;
        actual = actual->siguiente;
    }
    return count;
}

/**
 * @brief Imprime todas las posiciones almacenadas en la lista.
 *
 * No imprime saltos de linea; solo valores separados por espacios.
 *
 * @param cabeza Puntero al primer nodo de la lista.
 */
void imprimir_posiciones(ListaInt* cabeza) {
    ListaInt* actual = cabeza;
    while (actual != NULL) {
        printf(" %d", actual->pos);
        actual = actual->siguiente;
    }
}

/**
 * @brief Libera toda la memoria asociada a la lista enlazada.
 *
 * @param cabeza Puntero al primer nodo de la lista a liberar.
 */
void liberar_lista(ListaInt* cabeza) {
    ListaInt* actual = cabeza;
    ListaInt* siguiente;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}
