CFLAGS := -Wall -Wextra -O2
TERMSIZE = $(shell stty size)
ROWS = $(shell echo $(TERMSIZE) | cut -d ' ' -f 2)
COLS = $(shell echo $(TERMSIZE) | cut -d ' ' -f 1)

run: main
	./$< $(ROWS) $(COLS)

main: main.c mazegen.c
