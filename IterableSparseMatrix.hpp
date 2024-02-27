#ifndef ITERABLESPARSEMATRIX_HPP
#define ITERABLESPARSEMATRIX_HPP

#include "IterableSquareMatrix.hpp"
#include <iostream>

class IterableSparseMatrix : public IterableSquareMatrix {
    struct Sp_matr_elem {
        int value;
        size_t index;
        Sp_matr_elem *next;
    };
    struct Sp_matr_str {
        size_t index;
        Sp_matr_str *next;
        Sp_matr_elem *first;
    };
    size_t matrix_size;
    Sp_matr_str *matrix;
    
public:
    IterableSparseMatrix(size_t size) : matrix_size(size), matrix(NULL) {};
    IterableSparseMatrix(const IterableSparseMatrix &);
    ~IterableSparseMatrix();
    IterableSparseMatrix & operator=(const IterableSparseMatrix &);
    friend std::ostream & operator<<(std::ostream &, const IterableSparseMatrix &);
    size_t size() const;
    int get(size_t, size_t) const;
    void set(size_t, size_t, int);
    void add(size_t, size_t, int);
    bool operator==(const IterableSparseMatrix &) const;
    bool operator!=(const IterableSparseMatrix &) const;
    row_iterator iterate_rows(size_t) const;
    row_iterator end_row(size_t) const;
    column_iterator iterate_columns(size_t) const;
    column_iterator end_column(size_t) const;

    class Index_object;
    class Offset_index_object;
    class Interm_object;
    class Offset_interm_object;

    class Index_object {
        friend class Offset_index_object;
        friend class Interm_object;
        size_t index1, index2;
        IterableSparseMatrix *matrix_ptr;
        Index_object(IterableSparseMatrix *mp, size_t ip1, size_t ip2) : index1(ip1), index2(ip2), matrix_ptr(mp) {};
    public:
        operator int();
        int operator=(int);
        int operator+=(int);
        int operator-=(int);
        int operator*=(int);
        int operator/=(int);
        int operator%=(int);
        int operator^=(int);
        int operator&=(int);
        int operator|=(int);
        int operator>>=(int);
        int operator<<=(int);
        int operator++();
        int operator++(int);
        int operator--();
        int operator--(int);
    };

    class Offset_index_object {
        friend class Interm_object;
        size_t index1, index2;
        IterableSparseMatrix *matrix_ptr;
        const IterableSparseMatrix *const_matrix_ptr;
        Offset_index_object(IterableSparseMatrix *mp, size_t ip1, size_t ip2) : index1(ip1), index2(ip2), matrix_ptr(mp) {};
        Offset_index_object(const IterableSparseMatrix *mp, size_t ip1, size_t ip2) : index1(ip1), index2(ip2), const_matrix_ptr(mp) {};
    public:
        Index_object operator*();
        int operator*() const;
    };

    class Interm_object {
        friend class Offset_interm_object;
        friend class IterableSparseMatrix;
        int index;
        IterableSparseMatrix *matrix_ptr;
        const IterableSparseMatrix *const_matrix_ptr;
        Interm_object(IterableSparseMatrix *mp, size_t i) : index(i), matrix_ptr(mp) {};
        Interm_object(const IterableSparseMatrix *mp, size_t i) : index(i), const_matrix_ptr(mp) {};
    public:
        Index_object operator[](size_t);
        Index_object operator*();
        int operator[](size_t) const;
        int operator*() const;
        Offset_index_object operator+(int);
        const Offset_index_object operator+(int) const;
    };

    class Offset_interm_object {
        friend class IterableSparseMatrix;
        size_t index;
        IterableSparseMatrix *matrix_ptr;
        const IterableSparseMatrix *const_matrix_ptr;
        Offset_interm_object(IterableSparseMatrix *mp, size_t i) : index(i), matrix_ptr(mp) {};
        Offset_interm_object(const IterableSparseMatrix *mp, size_t i) : index(i), const_matrix_ptr(mp) {};
    public:
        Interm_object operator*();
        const Interm_object operator*() const;
    };

    Interm_object operator[](size_t);
    const Interm_object operator[](size_t) const;
    Interm_object operator*();
    const Interm_object operator*() const;
    Offset_interm_object operator+(int) ;
    const Offset_interm_object operator+(int) const;
    size_t row_next(size_t, size_t) const;
    size_t column_next(size_t, size_t) const;
};

#endif
