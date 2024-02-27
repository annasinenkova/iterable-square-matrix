#include "IterableSparseMatrix.hpp"
#include "IterableDenseMatrix.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>


void creat_color(int index, char *str)
{
    index += 7;
    unsigned color = 0xFFFFFF;
    unsigned rand = (1 & index) * 10000000 + (1 & (index >> 1)) * 2000000 +
                    (1 & (index >> 2)) * 400000 + (1 & (index >> 3)) * 2000 +
                    (1 & (index >> 4)) * 100 + (1 & (index >> 5)) * 10 + (1 & (index >> 6)) * 2;
    color -= rand;
    char m[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    size_t i = 6;
    while (color != 0) {
        size_t j = color % 16;
        color /= 16;
        str[i--] = m[j];
    }
    return;
}


void dot_color_file(const IterableSquareMatrix & graph, std::vector<int> & color)
{
    FILE *f = fopen("fileColorGraph.dot", "w+");
    if (f == NULL) {
        throw "file open err";
    }
    fprintf(f, "digraph\n{\n");
    for (size_t i = 0; i < graph.size(); ++i) {
        char str[8] = "#FFFFFF";
        creat_color(color[i], str);
        fprintf(f, "    %lu [label = \"%lu\"] [style=filled fillcolor=\"%s\"];\n", i, i, str);
    }
    fprintf(f, "//edges\n");
    for (size_t i = 0; i < graph.size(); ++i) {
        row_iterator row_itr = graph.iterate_rows(i);
        for (; row_itr != graph.end_row(i); ++row_itr) {
            if (row_itr.get_column() >= i) {
                fprintf(f, "    %lu -> %lu [label = \"%d\"]  [dir=none];\n", i, row_itr.get_column(), *row_itr);
            }
        }
    }
    fprintf(f,"}");
    fclose(f);
}


bool comp(std::pair<size_t, size_t> a, std::pair<size_t, size_t> b) {
    return a.second > b.second;
}


void vertex_degree(std::vector<std::pair<size_t, size_t> > & degree, const IterableSquareMatrix & graph) {
    degree.resize(graph.size());
    for (size_t i = 0; i < graph.size(); ++i) {
        degree[i] = std::pair<size_t, size_t>(i, 0);
        row_iterator itr = graph.iterate_rows(i);
        for (; itr != graph.end_row(i); ++itr) {
            if (i != itr.get_column()) {
                ++degree[i].second;
            }
        }
    }
    std::sort(degree.begin(), degree.end(), comp);
}
    

bool possible_color(const IterableSquareMatrix & graph, std::vector<int> & color, int v, int c)
{
    row_iterator row_itr = graph.iterate_rows(v);
    for (; row_itr != graph.end_row(v); ++row_itr) {
        if (c == color[row_itr.get_column()]) {
            return false;
        }
    }
    column_iterator column_itr = graph.iterate_columns(v);
    for (; column_itr != graph.end_column(v); ++column_itr) {
        if (c == color[column_itr .get_row()]) {
            return false;
        }
    }
    return true;
}


std::vector<int> color_graph(const IterableSquareMatrix & graph) {
    std::vector<std::pair<size_t, size_t> > degree;
    vertex_degree(degree, graph);
    std::vector<int> color(graph.size(), 0);
    color[degree[0].first] = 1;
    for (size_t i = 1; i < graph.size(); ++i) {
        size_t mincolor = 0;
        while (!possible_color(graph, color, degree[i].first, ++mincolor));
        color[degree[i].first] = mincolor;
    }
    return color;
}


int main(int argc, char *argv[]) {
    std::ifstream fin(argv[1]);
    size_t size;
    fin >> size;
    IterableSparseMatrix graph(size);
    size_t v1, v2;
    int weight;
    while (fin >> v1) {
        fin >> v2;
        fin >> weight;
        graph[v1][v2] = weight;
        graph[v2][v1] = weight;
    }
    fin.close();
    std::vector<int> color = color_graph(graph);
    std::cout << "Colors: ";
    for (size_t i = 0; i < color.size(); ++i) {
        std::cout << color[i] << " ";
    }
    std::cout << std::endl;
    dot_color_file(graph, color);
    return 0;
}

