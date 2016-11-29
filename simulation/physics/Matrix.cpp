//
// Created by louis on 28/11/16.
//

#include <stdexcept>

#include "Matrix.h"

template <typename T> Vector<T>::Vector(int size) : _length(size), _data(new T[size]) {

}

template <typename T> Vector<T>::~Vector() {
    delete[] _data;
}

template <typename T> Vector<T> Vector<T>::operator+(const Vector<T> &rhs) const {
    if (rhs._length != this->_length) {
        throw std::runtime_error("Trying to add 2 vectors of different size");
    }

    Vector<T> result(this->_length);

    for (int i = 0 ; i < _length ; i++) {
        result._data[i] = _data[i] + rhs._data[i];
    }
    return result;
}



template <typename T> Matrix<T>::Matrix(int rows, int cols) :
        _rows(rows), _cols(cols) {

}

template <typename T> Matrix<T>::~Matrix() {
    delete[] _data;
}