CFLAGS := -Wall -Wextra -O2

run: main
	./$< $$(stty size | cut -d' ' -f2) $$(stty size | cut -d' ' -f1)

main: main.c mazegen.c
