CC=gcc
CFLAGS=-g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic 
BASE_NAME=huffman
SRC_C=huffman.c priority_queue.c frequencies.c bit_writer.c
TEST_C=test_huffman.c
ASG_NICKNAME=hw21
SRC_H=huffman.h priority_queue.h frequencies.h bit_writer.h clog.h miniunit.h
SUBMIT_FILES=$(SRC_C) $(TEST_C) clog.h miniunit.h Makefile 
EXECUTABLE=test_huffman
EXECUTABLE_GCOV=$(EXECUTABLE)_gcov
CFLAGS_GCOV=$(CFLAGS) -fprofile-arcs -ftest-coverage
SHELL=/bin/bash

submit: $(SUBMIT_FILES)
	264submit $(ASG_NICKNAME) $(SRC_C) $(SRC_H) $(TEST_C) Makefile

test:  $(EXECUTABLE) 
	./$(EXECUTABLE)
	 valgrind ./$(EXECUTABLE)

pretest: submit
	264test $(ASG_NICKNAME) 

clean: 
	rm -f $(EXECUTABLE) *.c.gcov *.gcda *.gcno

$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) -o test_$(BASE_NAME) $(SRC_C) $(TEST_C) $(CFLAGS)

coverage: $(SRC_C) $(TEST_C)
	$(CC) $(CFLAGS_GCOV) $(SRC_C) $(TEST_C) -o $(EXECUTABLE_GCOV)
	./$(EXECUTABLE_GCOV)
	gcov -f $(SRC_C)

.PHONY: submit test pretest clean coverage
