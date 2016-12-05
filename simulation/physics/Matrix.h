//
// Created by louis on 28/11/16.
//

#ifndef RENDUOPENGL_MATRIX_H
#define RENDUOPENGL_MATRIX_H

#include <vector>

template<typename T> class Vector {
public :
    Vector(int size);
    Vector(const Vector<T> & other);
    virtual ~Vector();

    Vector<T> operator+(const Vector<T> &rhs) const;
    Vector<T> operator*(double rhs) const;
    T & operator[](int index) const;
private :
    int _length;
    T* _data;
};

template<typename T> class Matrix {
public :
    Matrix(int rows, int cols);
    virtual ~Matrix();

    Matrix<T> operator+(const Matrix<T> &rhs) const;
    Matrix<T> operator*(double rhs);
    const Vector<T> & operator[](int colIndex) const;
private :
    int _rows;
    int _cols;
    std::vector<Vector<T>> _data;
};


#endif //RENDUOPENGL_MATRIX_H
