.PHONY: all clean

APP = prg2baaN3149
CFLAGS = -Wall -Wextra -Werror -g
SRC = $(APP).c matrix.c valid.c
OBJ = $(SRC:.c=.o)

all: $(APP)

# Правило сборки исполняемого файла
$(APP): $(OBJ)
	gcc -o $(APP) $(OBJ) $(CFLAGS)

# Правило сборки объектных файлов из исходных
%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(APP) $(OBJ)
