.PHONY: all clean debug

APP=inf2aabN3149
CFLAGS=-Wall -Wextra -Werror -g
NASMFLAGS=-felf64 -g

all: $(APP)_c $(APP)_n

$(APP)_c: $(APP).c
	gcc -o $@ $(CFLAGS) $^

$(APP)_n: $(APP).o
	gcc -no-pie -o $@ $^

$(APP).o: $(APP).S
	nasm $(NASMFLAGS) $^ -o $@

clean:
	rm -rf $(APP)_c $(APP)_n $(APP).o

debug: $(APP)_n
	gdb $<