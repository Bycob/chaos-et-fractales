//
// Created by louis on 28/11/16.
//

#include <stdexcept>

#include "Matrix.h"

template <typename T> Vector<T>::Vector(int size) : _length(size), _data(new T[size]) {
    for (int i = 0 ; i < size ; i++) {
        _data[i] = 0;
    }
}

template <typename T> Vector<T>::Vector(const Vector<T> &other)
        : _length(other._length), _data(new T[other._length]) {

    for (int i = 0 ; i < other._length ; i++) {
        _data[i] = other._data[i];
    }
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

template <typename T> Vector<T> Vector<T>::operator*(double rhs) const {
    Vector<T> result(this->_length);

    for (int i = 0 ; i < _length ; i++) {
        result._data[i] = _data[i] * rhs;
    }
    return result;
}

template <typename T> T& Vector<T>::operator[](int index) const {
    if (index < 0 || index > _length) {
        throw std::runtime_error("trying access out of bounds");
    }
    return _data[index];
}




template <typename T> Matrix<T>::Matrix(int rows, int cols) :
        _rows(rows), _cols(cols) {

    _data.reserve(cols);
    for (int i = 0 ; i < cols ; i++) {
        _data.push_back(Vector<T>(rows));
    }
}

template <typename T> Matrix<T> Matrix<T>::operator+(const Matrix<T> &rhs) const {
    if (rhs._rows != _rows || rhs._cols != _cols) {
        throw std::runtime_error("addition entre matrices de différentes tailles inderdite !");
    }

    Matrix<T> result(_rows, _cols);
    for (int i = 0 ; i < _cols ; i++) {
        for (int j = 0 ; j < _rows ; j++) {
            result._data[i][j] = _data[i][j] + rhs._data[i][j];
        }
    }

    return result;
}

template <typename T> Matrix<T> Matrix<T>::operator*(double rhs) {
    Matrix<T> result(_rows, _cols);

    for (int i = 0 ; i < _cols ; i++) {
        for (int j = 0 ; j < _rows ; j++) {
            result._data[i][j] = _data[i][j] * rhs;
        }
    }

    return result;
}

template <typename T> const Vector<T> & Matrix<T>::operator[](int colIndex) const {
    return _data[colIndex];
}

template <typename T> Matrix<T>::~Matrix() {

}

template class Vector<double>;
template class Matrix<double>;