#include "IterableDenseMatrix.hpp"
#include <iostream>
#include <cstring>

std::ostream & operator<<(std::ostream & out, const IterableDenseMatrix & c) {
    for (size_t i = 0; i < c.matrix_size; ++i) {
        for (size_t j = 0; j < c.matrix_size; ++j) {
            std::cout << c.get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    return out;
}


size_t IterableDenseMatrix::row_next(size_t row, size_t column) const {
    while (++column < this->matrix_size && !this->buf[row][column]);
    return column;
}


size_t IterableDenseMatrix::column_next(size_t row, size_t column) const {
    while (++row < this->matrix_size && !this->buf[row][column]);
    return row;
}


IterableDenseMatrix::IterableDenseMatrix(size_t size) {
    this->matrix_size = size;
    this->buf = new int*[size];
    for (size_t i = 0; i < size; ++i) {
        this->buf[i] = new int[size];
        for (size_t j = 0; j < size; ++j) {
            this->buf[i][j] = 0;
        }
    }
}


IterableDenseMatrix::IterableDenseMatrix(const IterableDenseMatrix & matr) {
    this->matrix_size = matr.matrix_size;
    this->buf = new int*[matr.matrix_size];
    for (size_t i = 0; i < matr.matrix_size; ++i) {
        this->buf[i] = new int[matr.matrix_size];
        for (size_t j = 0; j < matr.matrix_size; ++j) {
            this->buf[i][j] = matr.buf[i][j];
        }
    }
}


IterableDenseMatrix::~IterableDenseMatrix() {
    for (size_t i = 0; i < this->matrix_size; ++i) {
        delete [] this->buf[i];
    }
    delete [] this->buf;
}


IterableDenseMatrix & IterableDenseMatrix::operator=(const IterableDenseMatrix & matr) {
    if (this == &matr) {
        return *this;
    }
    for (size_t i = 0; i < this->matrix_size; ++i) {
        delete [] this->buf[i];
    }
    delete [] this->buf;
    this->matrix_size = matr.matrix_size;
    this->buf = new int*[matr.matrix_size];
    for (size_t i = 0; i < matr.matrix_size; ++i) {
        this->buf[i] = new int[matr.matrix_size];
        for (size_t j = 0; j < matr.matrix_size; ++j) {
            this->buf[i][j] = matr.buf[i][j];
        }
    }
    return *this;
}


size_t IterableDenseMatrix::size() const {
    return this->matrix_size;
}


int IterableDenseMatrix::get(size_t index1, size_t index2) const {
    return this->buf[index1][index2];
}


void IterableDenseMatrix::set(size_t index1, size_t index2, int value) {
    this->buf[index1][index2] = value;
}


bool IterableDenseMatrix::operator==(const IterableDenseMatrix & matr) const {
    if (this->matrix_size != matr.matrix_size) {
        return false;
    }
    return !memcmp(this->buf, matr.buf, this->matrix_size * this->matrix_size * sizeof(*this->buf));
}


bool IterableDenseMatrix::operator!=(const IterableDenseMatrix & matr) const {
    return !(*this == matr);
}


row_iterator IterableDenseMatrix::iterate_rows(size_t row) const {
    size_t column = -1;
    while (++column < matrix_size && !get(row, column));
    return row_iterator(row, column, *this);
}


row_iterator IterableDenseMatrix::end_row(size_t row) const {
    return row_iterator(row, matrix_size, *this);
}


column_iterator IterableDenseMatrix::iterate_columns(size_t column) const {
    size_t row = -1;
    while (++row < matrix_size && !get(row, column));
    return column_iterator(row, column, *this);
}


column_iterator IterableDenseMatrix::end_column(size_t column) const {
    return column_iterator(matrix_size, column, *this);

}


IterableDenseMatrix::Interm_object IterableDenseMatrix::operator[](size_t index) {
    return Interm_object(this->buf, index);
}


IterableDenseMatrix::Interm_object IterableDenseMatrix::operator*() {
    return Interm_object(this->buf, 0);
}


IterableDenseMatrix::Offset_interm_object IterableDenseMatrix::operator+(size_t index) {
    return Offset_interm_object(this->buf, index);
}


IterableDenseMatrix::Interm_object IterableDenseMatrix::Offset_interm_object::operator*() {
    return Interm_object(this->matrix, this->index);
}


int & IterableDenseMatrix::Interm_object::operator[](size_t j) {
    return matrix[this->index][j]; 
}


int & IterableDenseMatrix::Interm_object::operator*() {
    return matrix[this->index][0];
}


int * IterableDenseMatrix::Interm_object::operator+(size_t i) {
    return &matrix[this->index][i];
}


IterableDenseMatrix::Interm_object IterableDenseMatrix::operator[](size_t i) const {
    return Interm_object(this->buf, i);
}


IterableDenseMatrix::Interm_object IterableDenseMatrix::operator*() const {
    return Interm_object(this->buf, 0);
}


IterableDenseMatrix::Offset_interm_object IterableDenseMatrix::operator+(size_t index) const {
    return Offset_interm_object(this->buf, index);
}


IterableDenseMatrix::Interm_object IterableDenseMatrix::Offset_interm_object::operator*() const {
    return Interm_object(this->matrix, this->index);
}


int IterableDenseMatrix::Interm_object::operator[](size_t j) const {
    return matrix[this->index][j]; 
}


int IterableDenseMatrix::Interm_object::operator*() const {
    return matrix[this->index][0];
}


const int * IterableDenseMatrix::Interm_object::operator+(size_t i) const {
    return &matrix[this->index][i];
}

