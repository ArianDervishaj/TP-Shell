# Nom de l'exécutable
TARGET = shell

# Fichiers sources
SRC = shell.c interface.c commands.c

# Fichiers objets (générés à partir des sources)
OBJ = $(SRC:.c=.o)

# Options du compilateur
CFLAGS = -Wall -Wextra -std=c99 -g

# Commande par défaut : compilation de l'exécutable
all: $(TARGET)

# Règle pour construire l'exécutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Règle pour compiler les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)

# Règle pour reconstruire complètement le projet
.PHONY: rebuild
rebuild: clean all
