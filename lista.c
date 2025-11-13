#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

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

int contar_posiciones(ListaInt* cabeza) {
    int count = 0;
    ListaInt* actual = cabeza;
    while (actual != NULL) {
        count++;
        actual = actual->siguiente;
    }
    return count;
}

void imprimir_posiciones(ListaInt* cabeza) {
    ListaInt* actual = cabeza;
    while (actual != NULL) {
        printf(" %d", actual->pos);
        actual = actual->siguiente;
    }
}

void liberar_lista(ListaInt* cabeza) {
    ListaInt* actual = cabeza;
    ListaInt* siguiente;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}