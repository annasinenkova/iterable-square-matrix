#ifndef ITERABLEDENSEMATRIX_HPP
#define ITERABLEDENSEMATRIX_HPP

#include "IterableSquareMatrix.hpp"
#include <iostream>

class IterableDenseMatrix : public IterableSquareMatrix {
    int **buf;
    size_t matrix_size;
public:
    IterableDenseMatrix(size_t size);
    IterableDenseMatrix(const IterableDenseMatrix &);
    ~IterableDenseMatrix();
    IterableDenseMatrix & operator=(const IterableDenseMatrix &);
    friend std::ostream & operator<<(std::ostream &, const IterableDenseMatrix &);
    size_t size() const;
    int get(size_t, size_t) const;
    void set(size_t, size_t, int);
    bool operator==(const IterableDenseMatrix &) const;
    bool operator!=(const IterableDenseMatrix &) const;
    row_iterator iterate_rows(size_t) const;
    row_iterator end_row(size_t) const;
    column_iterator iterate_columns(size_t) const;
    column_iterator end_column(size_t) const;
    
    class Interm_object {
        friend class IterableDenseMatrix;
        friend class Offset_interm_object;
        int **matrix;
        size_t index;
        Interm_object(int **m, size_t i) : matrix(m), index(i) {}
    public:
        int & operator[](size_t);
        int & operator*();
        int * operator+(size_t);
        int operator[](size_t) const;
        int operator*() const;
        const int * operator+(size_t) const;
    };
    
    class Offset_interm_object {
        friend class IterableDenseMatrix;
        int **matrix;
        size_t index;
        Offset_interm_object(int **m, size_t i) : matrix(m), index(i) {}
    public:
        Interm_object operator*();
        Interm_object operator*() const;
    };
    
    Interm_object operator[](size_t);
    Interm_object operator*();
    Offset_interm_object operator+(size_t);        
    Interm_object operator[](size_t) const;
    Interm_object operator*() const;
    Offset_interm_object operator+(size_t) const;
    size_t row_next(size_t, size_t) const;
    size_t column_next(size_t, size_t) const;
};

#endif
