CC = gcc
# Flags: -Wall para warnings, -std=c99 para el estandar de C, -g para debug
CFLAGS = -Wall -std=c99 -g
TARGET = bio
SRCS = main.c trie.c lista.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

# Regla principal: compila todo y crea el ejecutable 'bio'
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Regla para compilar archivos .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJS) $(TARGET)