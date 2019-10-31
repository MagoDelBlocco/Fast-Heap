CC		:= g++
CFLAGS  := -std=c++17 -Wall -Wextra -pthread -fstack-protector-all -m64

NAME    := Source

EXE		:= $(NAME).exe
BIN		:= bin
SRC		:= ./
INCLUDE	:= headers
LIB		:= lib

LIBRARIES	:=

build: 
	$(CC) $(CFLAGS) $(SRC)/$(NAME).cpp $(LIBRARIES) -o $(BIN)/$(EXE)

clean:
	$(RM) $(BIN)/$(EXE)

run: build
	./$(BIN)/$(EXE)

rebuild: clean build

debug: clean
	$(CC) $(CFLAGS) -g $(SRC)/$(NAME).cpp $(LIBRARIES) -o $(BIN)/$(EXE)
