#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Tamano del gen cargado
int GEN_SIZE = 0;

/// Nodo raiz del trie
Nodo* TRIE_ROOT = NULL;

/**
 * @brief Funcion principal del programa BioSearch.
 *
 * Se encarga de recibir e interpretar comandos ingresados por el usuario.
 * Los comandos disponibles permiten inicializar el trie, leer la secuencia
 * de ADN, buscar genes, mostrar estadisticas y finalizar el programa.
 *
 * Comandos soportados:
 * - start m
 * - read archivo.txt
 * - search GEN
 * - max
 * - min
 * - all
 * - help
 * - exit
 *
 * @return 0 al finalizar la ejecucion.
 */
int main(void) {
    char linea[256];
    char comando[10], arg1[100];
    
    printf("%sBienvenido a BioSearch!%s\n", VERDE, RESET);
    printf("%sPara obtener ayuda, escriba 'help'%s\n", AMARILLO, RESET);
    while (1) {
        printf("%s>bio %s", MORADO, RESET);

        if (fgets(linea, sizeof(linea), stdin) == NULL) {
            bio_exit();
            break;
        }

        linea[strcspn(linea, "\n")] = 0;  // Elimina el salto de linea
        int num_args = sscanf(linea, "%s %s", comando, arg1);

        if (strcmp(comando, "start") == 0) {
            if (num_args == 2) bio_start(atoi(arg1));
            else printf("%sUso: bio start m%s\n", AMARILLO, RESET);

        } else if (strcmp(comando, "help") == 0) {
            help();

        } else if (strcmp(comando, "read") == 0) {
            if (num_args == 2) bio_read(arg1);
            else printf("%sUso: bio read adn.txt%s\n", AMARILLO, RESET);

        } else if (strcmp(comando, "search") == 0) {
            if (num_args == 2) bio_search(arg1);
            else printf("%sUso: bio search GEN%s\n", AMARILLO, RESET);

        } else if (strcmp(comando, "max") == 0 && num_args == 1) {
            bio_max();

        } else if (strcmp(comando, "min") == 0 && num_args == 1) {
            bio_min();

        } else if (strcmp(comando, "all") == 0 && num_args == 1) {
            bio_all();

        } else if (strcmp(comando, "exit") == 0 && num_args == 1) {
            bio_exit();
            break;

        } else {
            printf("%sComando o argumento invalido. Revise la sintaxis.%s\n", ROJO, RESET);
        }
    }

    return 0;
}
