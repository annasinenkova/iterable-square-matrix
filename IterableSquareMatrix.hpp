#ifndef ITERABLESQUAREMATRIX_HPP
#define ITERABLESQUAREMATRIX_HPP

#include <iostream>

class IterableSquareMatrix;

class row_iterator {
    size_t row;
    size_t column;
    const IterableSquareMatrix & matrix;
public:
    row_iterator(size_t r, size_t c, const IterableSquareMatrix & m) : row(r), column(c), matrix(m) {};
    row_iterator & operator++();
    row_iterator operator++(int);
    bool operator==(const row_iterator &) const;
    bool operator!=(const row_iterator &) const;
    int operator*() const;
    size_t get_column();
};

class column_iterator {
    size_t row;
    size_t column;
    const IterableSquareMatrix & matrix;
public:
    column_iterator(size_t r, size_t c, const IterableSquareMatrix & m) : row(r), column(c), matrix(m) {};
    column_iterator & operator++();
    column_iterator operator++(int);
    bool operator==(const column_iterator &) const;
    bool operator!=(const column_iterator &) const;
    int operator*() const;
    size_t get_row();
};

class IterableSquareMatrix {
public:
    friend class row_iterator;
    friend class column_iterator;
    virtual row_iterator iterate_rows(size_t) const = 0;
    virtual row_iterator end_row(size_t) const = 0;
    virtual column_iterator iterate_columns(size_t) const = 0;
    virtual column_iterator end_column(size_t) const = 0;
    virtual size_t size() const = 0;
    virtual int get(size_t, size_t) const = 0;
    virtual void set(size_t, size_t, int) = 0;
    virtual size_t row_next(size_t, size_t) const = 0;
    virtual size_t column_next(size_t, size_t) const = 0;
    virtual ~IterableSquareMatrix() = 0;
};

#endif
