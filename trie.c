#include "trie.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>



Nodo* crear_nodo(void) {
    // calloc inicializa a 0 (NULL para punteros)
    Nodo* nuevo_nodo = (Nodo*)calloc(1, sizeof(Nodo)); 
    if (nuevo_nodo == NULL) {
        perror("Error de asignacion de memoria para Nodo");
    }
    return nuevo_nodo;
}

int get_base_index(char base) {
    switch (base) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default: return -1; // Invalido
    }
}

Nodo* generar_arbol_recursivo(int nivel_actual, int profundidad_max) {
    Nodo* nodo = crear_nodo();
    if (nodo == NULL) return NULL;

    if (nivel_actual < profundidad_max) {
        for (int i = 0; i < 4; i++) {
            nodo->hijos[i] = generar_arbol_recursivo(nivel_actual + 1, profundidad_max);
            if (nodo->hijos[i] == NULL && profundidad_max > 0) {
                // Manejo de error: liberar lo que ya se asigno
                for (int j = 0; j < i; j++) liberar_arbol(nodo->hijos[j]);
                free(nodo);
                return NULL;
            }
        }
    }
    return nodo;
}

// --- Implementacion de Comandos Principales ---
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

int insertar_gen_en_trie(const char* gen, int posicion) {
    if (TRIE_ROOT == NULL || gen == NULL || (int)strlen(gen) != GEN_SIZE) {
        return -1;
    }

    Nodo* actual = TRIE_ROOT;
    int index;

    for (int i = 0; i < GEN_SIZE; i++) {
        index = get_base_index(gen[i]);
        if (index == -1) {
            return -1; // Caracter invalido
        }
        actual = actual->hijos[index];
    }
    // 'actual' ahora es el nodo Hoja.
    actual->lista_posiciones = insertar_posicion(actual->lista_posiciones, posicion);
    return 0;
}

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
    // Logica para leer toda la secuencia S
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
    
    // --- CORRECCION: LÓGICA DE LIMPIEZA ROBUSTA ---
    // Elimina cualquier caracter no-ADN (saltos de linea, espacios, etc.) del final.
    int n = (int)read_len;
    while (n > 0 && (secuencia_S[n - 1] == '\n' || secuencia_S[n - 1] == '\r' || secuencia_S[n - 1] == ' ' || secuencia_S[n - 1] == '\t')) {
        n--;
    }
    secuencia_S[n] = '\0'; // Asegura el terminador nulo en la posicion correcta
    // ------------------------------------

    int m = GEN_SIZE;

    if (n < m) {
        printf("%sLa secuencia S es mas corta que el tamano del gen m. No se puede cargar.\n%s", AMARILLO, RESET);
        free(secuencia_S);
        return -1;
    }

    // Cargar genes
    char* gen_buffer = (char*)malloc(m + 1);
    if (gen_buffer == NULL) {
        perror("Error de asignacion de memoria para gen_buffer");
        free(secuencia_S);
        return -1;
    }
    gen_buffer[m] = '\0';

    // El bucle va de 0 hasta la posicion de inicio del ultimo gen (n - m)
    for (int i = 0; i <= n - m; i++) {
        strncpy(gen_buffer, secuencia_S + i, m);
        insertar_gen_en_trie(gen_buffer, i);
    }

    // Liberar la memoria
    free(gen_buffer); 
    free(secuencia_S);
    printf("%sSecuencia S leida desde el archivo\n%s", AMARILLO, RESET);
    return 0;
}

int bio_search(const char* gen_G) {
    if (TRIE_ROOT == NULL || GEN_SIZE == 0) {
        printf("%sError: El arbol no esta inicializado.\n%s", ROJO, RESET);
        return -1;
    }
    
    int m = GEN_SIZE;
    if ((int)strlen(gen_G) != m) {
        // Asegúrate de que el formato %d sea el segundo argumento si usas ROJO y RESET.
        printf("%sError: El gen ingresado debe tener el largo %d.\n%s", ROJO, m, RESET);
        return -1;
    }

    Nodo* actual = TRIE_ROOT;
    int index;

    for (int i = 0; i < m; i++) {
        index = get_base_index(gen_G[i]);
        if (index == -1) {
            // Se puede usar %s en lugar del ROJO para colorear todo el mensaje de error.
            printf("%sNo se encontro el gen %s en la secuencia S debido a caracter(es) invalido(s).\n%s", ROJO, gen_G, RESET);
            return -1;
        }
        actual = actual->hijos[index];
    }
    
    int frecuencia = contar_posiciones(actual->lista_posiciones);
    
    if (frecuencia == 0) {
        // Mensaje para cuando el gen no existe en la secuencia, pero es válido.
        printf("El gen %s no se encontro en la secuencia S. Frecuencia: %s-1%s\n", gen_G, ROJO, RESET);
        return -1;
    }
    printf("%sEl gen %s está %d veces en las posiciones:%s",AMARILLO, gen_G, frecuencia, RESET);
    imprimir_posiciones(actual->lista_posiciones);
    printf("\n");
    
    return frecuencia;
}

// --- Recorridos y Extremos ---

void recorrido_extremos(Nodo* nodo, char* buffer, int nivel, int mode, int* extremum) {
    if (nodo == NULL) return;
    if (nivel == GEN_SIZE) {
        int frecuencia = contar_posiciones(nodo->lista_posiciones);
        if (mode == 1) { // MAX
            if (frecuencia > *extremum) *extremum = frecuencia;
        } else { // MIN
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

void bio_max() {
    if (TRIE_ROOT == NULL || GEN_SIZE == 0) { printf("%sError: Arbol no cargado.\n%s",ROJO,RESET); return; } 
    char* gen_buffer = (char*)malloc(GEN_SIZE + 1);
    if (gen_buffer == NULL) { perror("Error de asignacion de memoria"); return; }
    int max_freq = 0;
    recorrido_extremos(TRIE_ROOT, gen_buffer, 0, 1, &max_freq);
    if (max_freq == 0) { printf("%sNo hay genes presentes.\n%s", AMARILLO, RESET); free(gen_buffer); return; }
    imprimir_recorrido(TRIE_ROOT, gen_buffer, 0, max_freq);
    free(gen_buffer);
}

void bio_min() {
    if (TRIE_ROOT == NULL || GEN_SIZE == 0) { printf("%sError: Arbol no cargado.\n%s", ROJO, RESET); return; }
    char* gen_buffer = (char*)malloc(GEN_SIZE + 1);
    if (gen_buffer == NULL) { perror("Error de asignacion de memoria"); return; }
    int min_freq = INT_MAX;
    recorrido_extremos(TRIE_ROOT, gen_buffer, 0, 0, &min_freq);
    if (min_freq == INT_MAX) { printf("%sNo hay genes presentes.\n%s", AMARILLO, RESET); free(gen_buffer); return; }
    imprimir_recorrido(TRIE_ROOT, gen_buffer, 0, min_freq);
    free(gen_buffer);
}

void bio_all() {
    if (TRIE_ROOT == NULL || GEN_SIZE == 0) { printf("%sError: Arbol no cargado.\n%s", ROJO, RESET); return; }
    char* gen_buffer = (char*)malloc(GEN_SIZE + 1);
    if (gen_buffer == NULL) { perror("Error de asignacion de memoria"); return; }
    // Funcion recursiva interna para ALL (imprime todos los que tengan frecuencia > 0)
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

void liberar_arbol(Nodo* nodo) {
    if (nodo == NULL) return;

    // Postorden: liberar hijos, luego lista, luego nodo
    for (int i = 0; i < 4; i++) {
        liberar_arbol(nodo->hijos[i]);
    }
    liberar_lista(nodo->lista_posiciones);
    free(nodo);
}

void bio_exit() {
    liberar_arbol(TRIE_ROOT);
    TRIE_ROOT = NULL;
    GEN_SIZE = 0;
    printf("%sLimpiando cache y salendo del programa...\n%s", AMARILLO, RESET);
}

void help(){
    printf("%sCOMANDOS DISPONIBLES:\n%s",VERDE_CLARO, RESET);
    printf("%sbio start m%s: crea un arbol 4-ario con genes de tamaño m\n",AZUL, RESET);
    printf("%sbio read archivo.txt%s: lee la secuencia de ADN desde el archivo archivo.txt\n",AZUL, RESET);
    printf("%sbio search XX%s: busca el gen XX en la secuencia\n",AZUL, RESET);
    printf("%sbio max%s: muestra el gen mas repetido en la secuencia\n",AZUL, RESET);
    printf("%sbio min%s: muestra el gen menos repetido en la secuencia\n",AZUL, RESET);
    printf("%sbio all%s: muestra todos los gen que tienen frecuencia > 0\n",AZUL, RESET);
    printf("%sbio exit%s: libera memoria y sale del programa\n",AZUL, RESET);
}