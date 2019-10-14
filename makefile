CC = gcc
OBJS = main.o board.o changeNode.o changesList.o command.o game.o gurobi.o error.o files.o moveNode.o movesList.o stack.o stackNode.o parser.o solver.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56


HEADERS = board.h changeNode.h changesList.h command.h error.h files.h game.h gurobi.h gurobi_c.h moveNode.h movesList.h parser.h solver.h stack.h stackNode.h

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o  $@
main.o: main.c $(HEADERS)
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
board.o: board.c board.h moveNode.h
	$(CC) $(COMP_FLAG) -c $*.c
changeNode.o: changeNode.c changeNode.h
	$(CC) $(COMP_FLAG) -c $*.c
changesList.o: changesList.c changesList.h changeNode.h
	$(CC) $(COMP_FLAG) -c $*.c
command.o: command.c command.h
	$(CC) $(COMP_FLAG) -c $*.c	
game.o: game.c game.h parser.h solver.h command.h board.h error.h files.h movesList.h
	$(CC) $(COMP_FLAG) -c $*.c
gurobi.o: gurobi.c gurobi.h gurobi_c.h board.h error.h
	$(CC) $(COMP_FLAG) -c $*.c
error.o: error.c error.h command.h
	$(CC) $(COMP_FLAG) -c $*.c
files.o: files.c files.h board.h error.h
	$(CC) $(COMP_FLAG) -c $*.c
moveNode.o: moveNode.c moveNode.h changesList.h command.h
	$(CC) $(COMP_FLAG) -c $*.c
movesList.o: movesList.c movesList.h moveNode.h
	$(CC) $(COMP_FLAG) -c $*.c
stack.o: stack.c stack.h stackNode.h
	$(CC) $(COMP_FLAG) -c $*.c
stackNode.o: stackNode.c stackNode.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c parser.h command.h game.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.c solver.h stack.h board.h error.h moveNode.h gurobi.h
	$(CC) $(COMP_FLAG) -c $*.c
# add gurobi.h to solver.o
clean:
	rm -f $(OBJS) $(EXEC)
