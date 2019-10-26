# Use gcc as the C Compiler
CC:=gcc

# Compile with debugging and math library
# Add any extra flags you'd like to this variable (maybe -Wall or -Werror)
CFLAGS:=-g -lm -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
# Include headers from include/
INCLUDE_DIR:=include

# Variables keeping track of objects
OBJS_NO_MAIN:=obj/hw2_p1.o obj/hw2_p2.o obj/hw2_p3.o obj/hw2_p4.o obj/helpers2.o obj/stack.o
OBJS:=$(OBJS_NO_MAIN) obj/53calc_main.o

# Make targets don't have to just be compilation commands, try this one out when you've written 53calc!
math_1: bin/53calc
	bin/53calc < rsrc/math_1_in.txt

# Most of these targets are named as the file they create on purpose to prevent
# make from rebuilding them if no updates are detected
obj/53calc_main.o: src/53calc_main.c
	$(CC) -I $(INCLUDE_DIR) src/53calc_main.c $(CFLAGS) -c -o obj/53calc_main.o

# The percent symbols mean that this is a static rule
# The percents are replacement wildcards; we could do this for all the objects if we really wanted
obj/hw2_p%.o: src/hw2_p%.c
	mkdir -p obj
	$(CC) -I $(INCLUDE_DIR) $^ $(CFLAGS) -c -o $@

obj/stack.o: src/stack.c
	mkdir -p obj
	$(CC) -I $(INCLUDE_DIR) $^ $(CFLAGS) -c -o $@

obj/helpers2.o: src/helpers2.c
	mkdir -p obj
	$(CC) -I $(INCLUDE_DIR) $^ $(CFLAGS) -c -o $@

# Convinience targets for unittest grading; compiles all the non-main c files
objs: clean $(OBJS_NO_MAIN)

clean_compile: clean bin/53calc

bin/p1_testing_main: tests/p1_testing_main.c obj/hw2_p1.o obj/helpers2.o
	mkdir -p bin
	$(CC) -I $(INCLUDE_DIR) $^ $(CFLAGS) -o $@

bin/p2_testing_main: tests/p2_testing_main.c obj/hw2_p2.o obj/hw2_p1.o obj/helpers2.o
	mkdir -p bin
	$(CC) -I $(INCLUDE_DIR) $^ $(CFLAGS) -o $@

bin/stacktest_main: tests/stacktest_main.c obj/hw2_p3.o obj/stack.o obj/helpers2.o
	mkdir -p bin
	$(CC) -I $(INCLUDE_DIR) $^ $(CFLAGS) -o $@

bin/53calc: $(OBJS)
	mkdir -p bin
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm -rf bin obj

.PHONY: stacktest clean math_1

# What is run with no arguments
.DEFAULT_GOAL:=bin/53calc
