#include "IterableSparseMatrix.hpp"
#include <iostream>

std::ostream & operator<<(std::ostream & out, const IterableSparseMatrix & c) {
    for (size_t i = 0; i < c.matrix_size; ++i) {
        for (size_t j = 0; j < c.matrix_size; ++j) {
            std::cout << c.get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    return out;
}


size_t IterableSparseMatrix::row_next(size_t row, size_t column) const {
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    size_t min = this->matrix_size;
    for (str = this->matrix; str; str = str->next) {
        if (str->index == row) {
            for (elem = str->first; elem; elem = elem->next) {
                if (elem->index > column && elem->index < min) {
                    min = elem->index;
                }
            }  
        }
    }
    return min;
}


size_t IterableSparseMatrix::column_next(size_t row, size_t column) const {
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    size_t min = this->matrix_size;
    for (str = this->matrix; str; str = str->next) {
        if (str->index > row) {
            for (elem = str->first; elem; elem = elem->next) {
                if (elem->index == column) {
                    min = str->index;
                }
            }
        }
    }
    return min;
}


IterableSparseMatrix::IterableSparseMatrix(const IterableSparseMatrix & sp_matr) {
    this->matrix_size = sp_matr.matrix_size;
    this->matrix = NULL;
    Sp_matr_str *str, *prev_str, *newstr;
    Sp_matr_elem *elem, *prev_elem, *newelem;      
    for (prev_str = NULL, str = sp_matr.matrix; str; prev_str = newstr, str = str->next) {
        newstr = new Sp_matr_str;
        newstr->index = str->index;
        newstr->first = NULL;
        newstr->next = NULL;
        if (prev_str == NULL) {
            this->matrix = newstr;
        }
        else {
            prev_str->next = newstr;
        }
        for (prev_elem = NULL, elem = str->first; elem; prev_elem = newelem, elem = elem->next) {
            newelem = new Sp_matr_elem;
            newelem->index = elem->index;
            newelem->value = elem->value;
            newelem->next = NULL;
            if (prev_elem == NULL) {
                newstr->first = newelem;
            }
            else {
                prev_elem->next = newelem;
            }
        }
    }
}


IterableSparseMatrix & IterableSparseMatrix::operator=(const IterableSparseMatrix & sp_matr) {
    if (this == &sp_matr) {
        return *this;
    }
    while (this->matrix) {
        Sp_matr_str *str = this->matrix;
        while (str->first) {
            Sp_matr_elem *elem = str->first;
            str->first = str->first->next;
            delete elem;
        }
        this->matrix = this->matrix->next;
        delete str;
    }
    this->matrix = NULL;
    this->matrix_size = sp_matr.matrix_size;
    Sp_matr_str *str, *prev_str, *newstr;
    Sp_matr_elem *elem, *prev_elem, *newelem;
    for (prev_str = NULL, str = sp_matr.matrix; str; prev_str = newstr, str = str->next) {
        newstr = new Sp_matr_str;
        newstr->index = str->index;
        newstr->first = NULL;
        newstr->next = NULL;
        if (prev_str == NULL) {
            this->matrix = newstr;
        }
        else {
            prev_str->next = newstr;
        } 
        for (prev_elem = NULL, elem = str->first; elem; prev_elem = newelem, elem = elem->next) {
            newelem = new Sp_matr_elem;
            newelem->index = elem->index;
            newelem->value = elem->value;
            newelem->next = NULL;
            if (prev_elem == NULL) {
                newstr->first = newelem;
            }
            else {
                prev_elem->next = newelem;
            }
        }
    }
    return *this;
}


IterableSparseMatrix::~IterableSparseMatrix() {
    while (this->matrix) {
        Sp_matr_str *str = this->matrix;
        while (str->first) {
            Sp_matr_elem *elem = str->first;
            str->first = str->first->next;
            delete elem;
        }
        this->matrix = this->matrix->next;
        delete str;
    }
}


size_t IterableSparseMatrix::size() const {
    return this->matrix_size;
}


int IterableSparseMatrix::get(size_t index1, size_t index2) const {
    Sp_matr_str *str;
    for (str = this->matrix; str; str = str->next) {
        if (str->index == index1) {
            Sp_matr_elem *elem;
            for (elem = str->first; elem; elem = elem->next) {
                if (elem->index == index2) {
                    return elem->value;
                }
            }
            return 0;
        }
    }
    return 0;
}


void IterableSparseMatrix::set(size_t index1, size_t index2, int value) {
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    for (str = this->matrix; str; str = str->next) {
        if (str->index == index1) {
            for (elem = str->first; elem; elem = elem->next) {
                if (elem->index == index2) {
                    elem->value = value;
                    return;
                }
            }
            elem = new Sp_matr_elem;
            elem->index = index2;
            elem->value = value;
            elem->next = str->first;
            str->first = elem;
            return;   
        }
    }
    str = new Sp_matr_str;
    str->index = index1;
    str->first = new Sp_matr_elem;
    str->first->index = index2;
    str->first->value = value;
    str->first->next = NULL;
    str->next = this->matrix;
    this->matrix = str;
}


void IterableSparseMatrix::add(size_t index1, size_t index2, int value) {
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    for (str = this->matrix; str; str = str->next) {
        if (str->index == index1) {
            for (elem = str->first; elem; elem = elem->next) {
                if (elem->index == index2) {
                    elem->value += value;
                    return;
                }
            }
            elem = new Sp_matr_elem;
            elem->index = index2;
            elem->value = value;
            elem->next = str->first;
            str->first = elem;
            return;   
        }
    }
    str = new Sp_matr_str;
    str->index = index1;
    str->first = new Sp_matr_elem;
    str->first->index = index2;
    str->first->value = value;
    str->first->next = NULL;
    str->next = this->matrix;
    this->matrix = str;
}


bool IterableSparseMatrix::operator==(const IterableSparseMatrix & sp_matr) const {
    if (this->matrix_size != sp_matr.matrix_size) {
        return false;
    }
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    for (str = this->matrix; str; str = str->next) {
        for (elem = str->first; elem; elem = elem->next) {
            if (elem->value != sp_matr.get(str->index, elem->index)) {
                return false;
            }
        }
    }
    return true;
}


bool IterableSparseMatrix::operator!=(const IterableSparseMatrix & sp_matr) const {
    return !(*this == sp_matr);
}


row_iterator IterableSparseMatrix::iterate_rows(size_t row) const {
    size_t column = -1;
    while (++column < matrix_size && !get(row, column));
    return row_iterator(row, column, *this);
}


row_iterator IterableSparseMatrix::end_row(size_t row) const {
    return row_iterator(row, matrix_size, *this);
}


column_iterator IterableSparseMatrix::iterate_columns(size_t column) const {
    size_t row = -1;
    while (++row < matrix_size && !get(row, column));
    return column_iterator(row, column, *this);
}


column_iterator IterableSparseMatrix::end_column(size_t column) const {
    return column_iterator(matrix_size, column, *this);
}


IterableSparseMatrix::Interm_object IterableSparseMatrix::operator[](size_t index) {
    return Interm_object(this, index);
}


const IterableSparseMatrix::Interm_object IterableSparseMatrix::operator[](size_t index) const {
    return Interm_object(this, index);
}


IterableSparseMatrix::Interm_object IterableSparseMatrix::operator*() {
    return Interm_object(this, 0);
}


const IterableSparseMatrix::Interm_object IterableSparseMatrix::operator*() const {
    return Interm_object(this, 0);
}


IterableSparseMatrix::Offset_interm_object IterableSparseMatrix::operator+(int c) {
    return Offset_interm_object(this, c);
}


const IterableSparseMatrix::Offset_interm_object IterableSparseMatrix::operator+(int c) const {
    return Offset_interm_object(this, c);
}


IterableSparseMatrix::Index_object::operator int() {
    return this->matrix_ptr->get(this->index1, this->index2);
}


int IterableSparseMatrix::Index_object::operator=(int c) {
    this->matrix_ptr->set(this->index1, this->index2, c);
    return c;
}


int IterableSparseMatrix::Index_object::operator+=(int c) {
    this->matrix_ptr->add(this->index1, this->index2, c);
    return this->matrix_ptr->get(this->index1, this->index2);
}


int IterableSparseMatrix::Index_object::operator-=(int c) {
    this->matrix_ptr->add(this->index1, this->index2, -c);
    return this->matrix_ptr->get(this->index1, this->index2);
}


int IterableSparseMatrix::Index_object::operator*=(int c) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, c * d);
    return d * c;
}


int IterableSparseMatrix::Index_object::operator/=(int c) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, d / c);
    return d / c;
}


int IterableSparseMatrix::Index_object::operator%=(int c) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, d % c);
    return d % c;
}


int IterableSparseMatrix::Index_object::operator^=(int c) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, d ^ c);
    return d ^ c;
}


int IterableSparseMatrix::Index_object::operator&=(int c) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, d & c);
    return d & c;
}


int IterableSparseMatrix::Index_object::operator|=(int c) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, d | c);
    return d | c;
}


int IterableSparseMatrix::Index_object::operator>>=(int c) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, d >> c);
    return d >> c;
}


int IterableSparseMatrix::Index_object::operator<<=(int c) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, d << c);
    return d << c;
}


int IterableSparseMatrix::Index_object::operator++() {
    this->matrix_ptr->add(this->index1, this->index2, 1);
    return this->matrix_ptr->get(this->index1, this->index2);
}


int IterableSparseMatrix::Index_object::operator++(int) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->add(this->index1, this->index2, 1);
    return d;
}


int IterableSparseMatrix::Index_object::operator--() {
    this->matrix_ptr->add(this->index1, this->index2, -1);
    return this->matrix_ptr->get(this->index1, this->index2);
}


int IterableSparseMatrix::Index_object::operator--(int) {
    int d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->add(this->index1, this->index2, -1);
    return d;
}


IterableSparseMatrix::Index_object IterableSparseMatrix::Offset_index_object::operator*() {
    return Index_object(this->matrix_ptr, this->index1, this->index2);
}


int IterableSparseMatrix::Offset_index_object::operator*() const {
    return const_matrix_ptr->get(this->index1, this->index2);
}


IterableSparseMatrix::Index_object IterableSparseMatrix::Interm_object::operator[](size_t i) {
    return Index_object(this->matrix_ptr, this->index, i);
}


int IterableSparseMatrix::Interm_object::operator[](size_t i) const {
    return const_matrix_ptr->get(this->index, i);
}


IterableSparseMatrix::Index_object IterableSparseMatrix::Interm_object::operator*() {
    return Index_object(this->matrix_ptr, this->index, 0);
}


int IterableSparseMatrix::Interm_object::operator*() const {
    return const_matrix_ptr->get(this->index, 0);
}


IterableSparseMatrix::Offset_index_object IterableSparseMatrix::Interm_object::operator+(int c) {
    return Offset_index_object(this->matrix_ptr, this->index, c);
}


const IterableSparseMatrix::Offset_index_object IterableSparseMatrix::Interm_object::operator+(int c) const {
    return Offset_index_object(this->const_matrix_ptr, this->index, c);
}


IterableSparseMatrix::Interm_object IterableSparseMatrix::Offset_interm_object::operator*() {
    return Interm_object(this->matrix_ptr, this->index);
}


const IterableSparseMatrix::Interm_object IterableSparseMatrix::Offset_interm_object::operator*() const {
    return Interm_object(this->const_matrix_ptr, this->index);
}

