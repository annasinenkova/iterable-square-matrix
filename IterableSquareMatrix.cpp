#include "IterableSquareMatrix.hpp"


IterableSquareMatrix::~IterableSquareMatrix() {}


row_iterator & row_iterator::operator++() {
    this->column = this->matrix.row_next(this->row, this->column);
    return *this;
}


row_iterator row_iterator::operator++(int) {
    row_iterator itr = *this;
    this->column = this->matrix.row_next(this->row, this->column);
    return itr;
}


bool row_iterator::operator==(const row_iterator & it) const {
    return this->row == it.row && this->column == it.column && &(this->matrix) == &it.matrix;
}


bool row_iterator::operator!=(const row_iterator & it) const {
    return !(*this == it);
}


int row_iterator::operator*() const {
    return matrix.get(this->row, this->column);
}


size_t row_iterator::get_column() {
    return this->column;
}


column_iterator & column_iterator::operator++() {
    this->row = this->matrix.column_next(this->row, this->column);
    return *this;
}


column_iterator column_iterator::operator++(int) {
    column_iterator itr = *this;
    this->row = this->matrix.column_next(this->row, this->column);
    return itr;
}


bool column_iterator::operator==(const column_iterator & it) const {
    return this->row == it.row && this->column == it.column && &(this->matrix) == &it.matrix;
}


bool column_iterator::operator!=(const column_iterator & it) const {
    return !(*this == it);
}


int column_iterator::operator*() const {
    return matrix.get(this->row, this->column);
}


size_t column_iterator::get_row() {
    return this->row;
}

