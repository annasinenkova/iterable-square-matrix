#include "IterableSparseMatrix.hpp"
#include "IterableDenseMatrix.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>


void dot_color_file(const IterableSquareMatrix & graph, std::vector<size_t> & res)
{
    FILE *f = fopen("fileMaxClique.dot", "w+");
    if (f == NULL) {
        throw "file open err";
    }
    fprintf(f, "digraph\n{\n");
    for (size_t i = 0; i < graph.size(); ++i) {
        if (std::find(res.begin(), res.end(), i) != res.end()) {
            fprintf(f, "    %lu [label = \"%lu\"] [style=filled fillcolor=\"#6761AF\"];\n", i, i);
        }
        else {
            fprintf(f, "    %lu [label = \"%lu\"]\n", i, i);
        }
    }    
    fprintf(f, "//edges\n");
    for (size_t i = 0; i < graph.size(); ++i) {
        row_iterator row_itr = graph.iterate_rows(i);
        for (; row_itr != graph.end_row(i); ++row_itr) {
            if (row_itr.get_column() >= i) {
                fprintf(f, "    %lu -> %lu [label = \"%d\"] [dir=none];\n", i, row_itr.get_column(), *row_itr);
            }
        }
    }
    fprintf(f,"}");
    fclose(f);
}


bool vertex_to_all(const IterableSquareMatrix & graph, std::vector<size_t> & vertex_set, size_t vertex) {
    size_t size = 0;
    row_iterator row_itr = graph.iterate_rows(vertex);
    for (; row_itr != graph.end_row(vertex); ++row_itr) {
        if (std::find(vertex_set.begin(), vertex_set.end(), row_itr.get_column()) != vertex_set.end()) {
            ++size;
        }
    }
    return (size == vertex_set.size());
}


bool any_vertex_to_all(const IterableSquareMatrix & graph, std::vector<size_t> & vertex_set, std::vector<size_t> & vertex) {
    for (size_t i = 0; i < vertex.size(); ++i) {
        if (vertex_to_all(graph, vertex_set, vertex[i])) {
            return true;
        }
    }
    return false;
}


bool is_edge(const IterableSquareMatrix & graph, size_t vertex1, size_t vertex2) {
    row_iterator row_itr = graph.iterate_rows(vertex1);
    for (; row_itr != graph.end_row(vertex1); ++row_itr) {
        if (row_itr.get_column() == vertex2) {
            return true;
        }
    }
    return false;
}


std::vector<size_t> creat_new_set(const IterableSquareMatrix & graph, std::vector<size_t> & set, size_t vertex) {
    std::vector<size_t> new_set;
    for (size_t i = 0; i < set.size(); ++i) {
        if (is_edge(graph, set[i], vertex)) {
            new_set.push_back(set[i]);
        }
    }
    return new_set;
}


void bron_kerbosch(const IterableSquareMatrix &graph, std::vector<size_t> &impossible, std::vector<size_t> &possible, 
                                               std::vector<size_t> &compsub, std::vector<size_t> &max) {
    while (!possible.empty() && !any_vertex_to_all(graph, possible, impossible)) {
        size_t vertex = possible.back();
        compsub.push_back(vertex);
        std::vector<size_t> new_possible = creat_new_set(graph, possible, vertex);
        std::vector<size_t> new_impossible = creat_new_set(graph, impossible, vertex);
        possible.pop_back();
        impossible.push_back(vertex);
        if (new_impossible.empty() && new_possible.empty()) {
            if (compsub.size() > max.size()) {
                max = compsub;
            }
        }
        else {
            bron_kerbosch(graph, new_impossible, new_possible, compsub, max);
        }
        compsub.pop_back();
    }
}


std::vector<size_t> max_clique(const IterableSquareMatrix & graph) {
    std::vector<size_t> impossible, possible, compsub, max;
    for (size_t i = 0; i < graph.size(); ++i) {
        possible.push_back(i);
    }
    bron_kerbosch(graph, impossible, possible, compsub, max);
    return max;
}


int main(int argc, char *argv[]) {
    std::ifstream fin(argv[1]);
    size_t size;
    fin >> size;
    IterableDenseMatrix graph(size);
    size_t v1, v2;
    int weight;
    while (fin >> v1) {
        fin >> v2;
        fin >> weight;
        graph[v1][v2] = weight;
        graph[v2][v1] = weight;
    }
    fin.close();
    std::vector<size_t> max = max_clique(graph);
    std::cout << "Max clique: ";
    for (size_t i = 0; i < max.size(); ++i) {
        std::cout << max[i] << " ";
    }
    std::cout << std::endl;
    dot_color_file(graph, max);
    return 0;
}

