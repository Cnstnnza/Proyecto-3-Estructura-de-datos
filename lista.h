#ifndef LISTA_H
#define LISTA_H

/**
 * @brief Nodo de la lista que guarda una posición.
 */
typedef struct ListaInt {
    int pos;                    /**< Posición almacenada */
    struct ListaInt* siguiente; /**< Puntero al siguiente nodo */
} ListaInt;

/**
 * @brief Crea un nodo que almacena una posición.
 * @param posicion Valor entero a almacenar.
 * @return Puntero al nuevo nodo.
 */
ListaInt* crear_nodo_lista(int posicion);

/**
 * @brief Inserta una posición al final de la lista.
 * @param cabeza Puntero al primer nodo de la lista.
 * @param posicion Valor entero a insertar.
 * @return Puntero a la cabeza de la lista.
 */
ListaInt* insertar_posicion(ListaInt* cabeza, int posicion);

/**
 * @brief Cuenta cuántas posiciones hay en la lista.
 * @param cabeza Puntero a la lista.
 * @return Número de nodos.
 */
int contar_posiciones(ListaInt* cabeza);

/**
 * @brief Imprime todas las posiciones almacenadas.
 * @param cabeza Puntero a la lista.
 */
void imprimir_posiciones(ListaInt* cabeza);

/**
 * @brief Libera toda la memoria usada por la lista.
 * @param cabeza Puntero a la lista.
 */
void liberar_lista(ListaInt* cabeza);

#endif
