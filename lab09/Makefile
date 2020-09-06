CC=gcc
FLAGS=-std=c99 -pedantic -Wall -lm -g
BIN=lab09
SOURCE=lab09.c
INPUT_PATH=input
OUTPUT_PATH=output

build:
	$(CC) $(SOURCE) $(FLAGS) -o $(BIN)

run:build
	./$(BIN) < $(INPUT_PATH)/arq01.in > $(OUTPUT_PATH)/arq01.out
	./$(BIN) < $(INPUT_PATH)/arq02.in > $(OUTPUT_PATH)/arq02.out
	./$(BIN) < $(INPUT_PATH)/arq03.in > $(OUTPUT_PATH)/arq03.out
	./$(BIN) < $(INPUT_PATH)/arq04.in > $(OUTPUT_PATH)/arq04.out
	./$(BIN) < $(INPUT_PATH)/arq05.in > $(OUTPUT_PATH)/arq05.out

test:run
	-sdiff -s $(OUTPUT_PATH)/arq01.out $(OUTPUT_PATH)/arq01.res
	-sdiff -s $(OUTPUT_PATH)/arq02.out $(OUTPUT_PATH)/arq02.res
	-sdiff -s $(OUTPUT_PATH)/arq03.out $(OUTPUT_PATH)/arq03.res
	-sdiff -s $(OUTPUT_PATH)/arq04.out $(OUTPUT_PATH)/arq04.res
	-sdiff -s $(OUTPUT_PATH)/arq05.out $(OUTPUT_PATH)/arq05.res

valgrind:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./$(BIN) < $(INPUT_PATH)/arq01.in > $(OUTPUT_PATH)/arq1.out
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./$(BIN) < $(INPUT_PATH)/arq02.in > $(OUTPUT_PATH)/arq2.out
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./$(BIN) < $(INPUT_PATH)/arq03.in > $(OUTPUT_PATH)/arq3.out
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./$(BIN) < $(INPUT_PATH)/arq04.in > $(OUTPUT_PATH)/arq4.out
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./$(BIN) < $(INPUT_PATH)/arq05.in > $(OUTPUT_PATH)/arq5.out
