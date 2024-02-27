CC = g++
CFLAGS = -g -O2 -ftrapv -fsanitize=address,undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -lm
LFLAGS = -fsanitize=address,undefined

all: graph_coloring max_clique clean_obj

graph_coloring: graph_coloring.o IterableSparseMatrix.o IterableSquareMatrix.o
	$(CC) $(LFLAGS) $^ -o $@
	
graph_coloring.o: graph_coloring.cpp IterableSparseMatrix.hpp
	$(CC) $(CFLAGS) -c $< -o $@
	
max_clique: max_clique.o IterableDenseMatrix.o IterableSquareMatrix.o
	$(CC) $(LFLAGS) $^ -o $@
	
max_clique.o: max_clique.cpp IterableDenseMatrix.hpp
	$(CC) $(CFLAGS) -c $< -o $@
	
%Matrix.o: %Matrix.cpp %Matrix.hpp
	$(CC) $(CFLAGS) -c $< -o $@

pdf:
	dot -Tpdf -o MaxClique.pdf fileMaxClique.dot
	dot -Tpdf -o ColorGraph.pdf fileColorGraph.dot

clean_obj:
	rm *.o
	
clean_all:
	rm graph_coloring max_clique *.dot MaxClique.pdf ColorGraph.pdf
